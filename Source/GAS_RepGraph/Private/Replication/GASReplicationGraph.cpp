// Fill out your copyright notice in the Description page of Project Settings.


#include "Replication/GASReplicationGraph.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

UReplicationGraphNode_AlwaysRelevant_WithPending::UReplicationGraphNode_AlwaysRelevant_WithPending()
{
	bRequiresPrepareForReplicationCall = true;
}

void UReplicationGraphNode_AlwaysRelevant_WithPending::PrepareForReplication()
{
	Super::PrepareForReplication();
	
	if (UGASReplicationGraph* RepGraph = Cast<UGASReplicationGraph>(GetOuter()))
	{
		RepGraph->HandlePendingActors();
	}
}

void UGASReplicationGraph::HandlePendingActors()
{
	if (PendingActors.IsEmpty())
		return;
	
	TArray<AActor*, TInlineAllocator<16>> NewPendingActors = MoveTemp(PendingActors);
	
	for (AActor* Actor : NewPendingActors)
	{
		if (Actor)
		{
			FGlobalActorReplicationInfo& GlobalInfo = GlobalActorReplicationInfoMap.Get(Actor);
			
			FNewReplicatedActorInfo ActorInfo(Actor);

			Super::RouteAddNetworkActorToNodes(ActorInfo, GlobalInfo);
		}
	}
}

void UGASReplicationGraph::HandlePendingPlayerControllers()
{
	if (PlayerControllers.IsEmpty())
		return;
	
	TArray<const APlayerController*, TInlineAllocator<16>> NewPendingControllers = MoveTemp(PlayerControllers);
	
	for (const APlayerController* Controller : NewPendingControllers)
	{
		if (Controller)
		{
			AddPlayerController(Controller);
		}
	}
}

void UGASReplicationGraph::AddPlayerController(const APlayerController* NewPlayerController)
{
	UGASReplicationGraphConnection* GraphConnection = GetConnectionForActor(NewPlayerController);
	if (GraphConnection && !PlayerConnections.Contains(GraphConnection))
	{
		PlayerConnections.AddUnique(GraphConnection);
	}
	else if (!GraphConnection)
	{
		PlayerControllers.Add(NewPlayerController);
	}
}


UGASReplicationGraph::UGASReplicationGraph()
{
	ReplicationConnectionManagerClass =	UGASReplicationGraphConnection::StaticClass();
}

void UGASReplicationGraph::InitGlobalGraphNodes()
{
	Super::InitGlobalGraphNodes();
	
	// Создаем ноду
	AlwaysRelevantNode = CreateNewNode<UReplicationGraphNode_AlwaysRelevant_WithPending>();
	
	// Добавляем ее в глобальный скоуп
	AddGlobalGraphNode(AlwaysRelevantNode);
	
	UE_LOG(LogTemp, Warning, TEXT(">>> GAS REPLICATION GRAPH ACTIVE <<<"));
}

void UGASReplicationGraph::RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo,
	FGlobalActorReplicationInfo& GlobalInfo)
{
	Super::RouteAddNetworkActorToNodes(ActorInfo, GlobalInfo);
	
	UE_LOG(LogTemp, Warning, TEXT("Routing Actor: %s"), *ActorInfo.Actor->GetName());
	
	//if (ActorInfo.Actor->IsA<APawn>())
	//{
		//if (UGASReplicationGraphConnection* Connection = GetConnectionForActor(ActorInfo.Actor))
		//{
			//Connection->ActorListNode->NotifyAddNetworkActor(ActorInfo);
			//UE_LOG(LogTemp, Warning, TEXT("Routing Actor: %s"), *ActorInfo.Actor->GetName());
		//}
		//return;
	//}
	
	if (ActorInfo.Class->IsChildOf(AGameStateBase::StaticClass()) || ActorInfo.Class->IsChildOf(APlayerState::StaticClass()))
	{
		AlwaysRelevantNode->AddAlwaysRelevantClass(ActorInfo.Class);
		return;
	}
	
	if (UGASReplicationGraphConnection* Connection = GetConnectionForActor(ActorInfo.Actor))
	{
		if (ActorInfo.Actor->bOnlyRelevantToOwner)
		{
			Super::RouteAddNetworkActorToNodes(ActorInfo, GlobalInfo);
		}
		else
		{
			Connection->ActorListNode->NotifyAddNetworkActor(ActorInfo);
		}
	} 
	else
	{
		if (ActorInfo.Actor->IsNetStartupActor())
		{
			return;
		}

		PendingActors.Add(ActorInfo.Actor);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Actor routed: %s | HasConnection: %s"), *ActorInfo.Actor->GetName(), GetConnectionForActor(ActorInfo.Actor) ? TEXT("YES") : TEXT("NO"));
}

void UGASReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager)
{
	Super::InitConnectionGraphNodes(ConnectionManager);
	
	UGASReplicationGraphConnection* GraphConnection = Cast<UGASReplicationGraphConnection>(ConnectionManager);
	
	if (GraphConnection)
	{
		GraphConnection->AlwaysRelevantForConnectionNode = CreateNewNode<UReplicationGraphNode_AlwaysRelevant_ForConnection>();
		
		AddConnectionGraphNode(GraphConnection->AlwaysRelevantForConnectionNode, GraphConnection);
		
		GraphConnection->CubeRelevancyNode = CreateNewNode<UReplicationGraphNode_ConnectionActors>();
		
		AddConnectionGraphNode(GraphConnection->CubeRelevancyNode, GraphConnection);
		
		GraphConnection->ActorListNode = CreateNewNode<UReplicationGraphNode_ConnectionActors>();
		
		AddConnectionGraphNode(GraphConnection->ActorListNode, GraphConnection);
	}
}

UGASReplicationGraphConnection* UGASReplicationGraph::GetConnectionForActor(const AActor* Actor)
{
	if (!Actor)
		return nullptr;

	// STEP 1: direct connection (rare but valid)
	if (UNetConnection* Conn = Actor->GetNetConnection())
	{
		return Cast<UGASReplicationGraphConnection>(FindOrAddConnectionManager(Conn));
	}

	// STEP 2: Pawn → Controller → PlayerState → Connection (correct chain)
	if (const APawn* Pawn = Cast<APawn>(Actor))
	{
		if (const AController* Controller = Pawn->GetController())
		{
			if (UNetConnection* Conn = Controller->GetNetConnection())
			{
				return Cast<UGASReplicationGraphConnection>(FindOrAddConnectionManager(Conn));
			}
		}

		if (const APlayerState* PS = Pawn->GetPlayerState())
		{
			if (UNetConnection* Conn = PS->GetNetConnection())
			{
				return Cast<UGASReplicationGraphConnection>(FindOrAddConnectionManager(Conn));
			}
		}
	}

	// STEP 3: PlayerController fallback
	if (const APlayerController* PC = Cast<APlayerController>(Actor->GetOwner()))
	{
		if (UNetConnection* Conn = PC->GetNetConnection())
		{
			return Cast<UGASReplicationGraphConnection>(FindOrAddConnectionManager(Conn));
		}
	}

	return nullptr;
}

UReplicationGraphNode_ConnectionActors::UReplicationGraphNode_ConnectionActors()
{
	bRequiresPrepareForReplicationCall = true;
}

void UReplicationGraphNode_ConnectionActors::GatherActorListsForConnection(
	const FConnectionGatherActorListParameters& Params)
{
	Super::GatherActorListsForConnection(Params);
	
	UE_LOG(LogTemp, Warning, TEXT("Gathering for connection"));
	
	if (UGASReplicationGraph* ReplicationGraph = Cast<UGASReplicationGraph>(GetOuter()))
	{
		const TArray<UGASReplicationGraphConnection*, TInlineAllocator<16>> Connections = ReplicationGraph->GetPlayerConnections();
		
		for (UGASReplicationGraphConnection* Connection : Connections)
		{
			Connection->GetConnectionActorListNode()->GatherActorListsForConnectionDefault(Params);
		}
	}
}

void UReplicationGraphNode_ConnectionActors::GatherActorListsForConnectionDefault(const FConnectionGatherActorListParameters& Params)
{
	Super::GatherActorListsForConnection(Params);
}

void UReplicationGraphNode_ConnectionActors::PrepareForReplication()
{
	Super::PrepareForReplication();
	
	if (UGASReplicationGraph* ReplicationGraph = Cast<UGASReplicationGraph>(GetOuter()))
	{
		ReplicationGraph->HandlePendingPlayerControllers();
	}
}

