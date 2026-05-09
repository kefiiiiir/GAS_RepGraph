// Fill out your copyright notice in the Description page of Project Settings.


#include "Replication/GASReplicationGraph.h"

#include "GameFramework/GameStateBase.h"
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
			
			RouteAddNetworkActorToNodes(FNewReplicatedActorInfo(Actor), GlobalInfo);
		}
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
	
	if (ActorInfo.Class->IsChildOf(AGameStateBase::StaticClass()) || ActorInfo.Class->IsChildOf(APlayerState::StaticClass()))
	{
		AlwaysRelevantNode->AddAlwaysRelevantClass(ActorInfo.Class);
		return;
	}
	
	if (UGASReplicationGraphConnection* Connection = GetConnectionForActor(ActorInfo.Actor))
	{
		if (ActorInfo.Actor->bOnlyRelevantToOwner)
		{
			Connection->AlwaysRelevantForConnectionNode->NotifyAddNetworkActor(ActorInfo);
		}
		else
		{
			Connection->ActorListNode->NotifyAddNetworkActor(ActorInfo);
		}
	} 
	else if (ActorInfo.Actor->HasNetOwner())
	{
		PendingActors.Add(ActorInfo.Actor);
	}
}

void UGASReplicationGraph::InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager)
{
	Super::InitConnectionGraphNodes(ConnectionManager);
	
	UGASReplicationGraphConnection* GraphConnection = Cast<UGASReplicationGraphConnection>(ConnectionManager);
	
	if (GraphConnection)
	{
		GraphConnection->AlwaysRelevantForConnectionNode = CreateNewNode<UReplicationGraphNode_AlwaysRelevant_ForConnection>();
		
		AddConnectionGraphNode(GraphConnection->AlwaysRelevantForConnectionNode, GraphConnection);
		
		GraphConnection->CubeRelevancyNode = CreateNewNode<UReplicationGraphNode_CubeRelevancy>();
		
		AddConnectionGraphNode(GraphConnection->CubeRelevancyNode, GraphConnection);
		
		GraphConnection->ActorListNode = CreateNewNode<UReplicationGraphNode_ActorList>();
		
		AddConnectionGraphNode(GraphConnection->ActorListNode, GraphConnection);
	}
}

UGASReplicationGraphConnection* UGASReplicationGraph::GetConnectionForActor(const AActor* Actor)
{
	if (Actor)
	{
		if (UNetConnection* Connection = Actor->GetNetConnection())
		{
			if (UGASReplicationGraphConnection* GraphConnection = Cast<UGASReplicationGraphConnection>(FindOrAddConnectionManager(Connection)))
			{
				return GraphConnection;
			}
		}
	}
	
	return nullptr;
}
