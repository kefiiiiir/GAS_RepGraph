// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "GASReplicationGraph.generated.h"

/**
 * 
 */
UCLASS()
class GAS_REPGRAPH_API UGASReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()
	
public:
	
	UGASReplicationGraph();
	
protected:
	
	virtual void InitGlobalGraphNodes() override;
	
	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo) override;
	
	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager) override;

private:
	
	UGASReplicationGraphConnection* GetConnectionForActor(const AActor* Actor);
	
private:
	
	UReplicationGraphNode_AlwaysRelevant* AlwaysRelevantNode;
	
};

UCLASS()
class GAS_REPGRAPH_API UReplicationGraphNode_CubeRelevancy : public UReplicationGraphNode_ActorList
{
	GENERATED_BODY()
	
	
};

UCLASS()
class GAS_REPGRAPH_API UGASReplicationGraphConnection : public UNetReplicationGraphConnection
{
	GENERATED_BODY()
	
	friend UGASReplicationGraph;
	
private:
	
	UReplicationGraphNode_CubeRelevancy* CubeRelevancyNode;
	
	UReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantForConnectionNode;
	
	uint8 bCubeRelevant;
};

