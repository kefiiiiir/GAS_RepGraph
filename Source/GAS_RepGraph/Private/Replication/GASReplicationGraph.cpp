// Fill out your copyright notice in the Description page of Project Settings.


#include "Replication/GASReplicationGraph.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

UGASReplicationGraph::UGASReplicationGraph()
{
	ReplicationConnectionManagerClass =	UGASReplicationGraphConnection::StaticClass();
}

void UGASReplicationGraph::InitGlobalGraphNodes()
{
	Super::InitGlobalGraphNodes();
	
	// Создаем ноду
	AlwaysRelevantNode = CreateNewNode<UReplicationGraphNode_AlwaysRelevant>();
	
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
	}
	
	if (UGASReplicationGraphConnection* Connection = GetConnectionForActor(ActorInfo.Actor); ActorInfo.Actor->bOnlyRelevantToOwner && Connection)
	{
		Connection->AlwaysRelevantForConnectionNode->NotifyAddNetworkActor(ActorInfo);
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
