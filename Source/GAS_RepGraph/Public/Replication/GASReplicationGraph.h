// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicationGraph.h"
#include "GASReplicationGraph.generated.h"

/**
 * 
 */

UCLASS()
class UReplicationGraphNode_AlwaysRelevant_WithPending : public UReplicationGraphNode_AlwaysRelevant
{
	GENERATED_BODY()
	
public:
	UReplicationGraphNode_AlwaysRelevant_WithPending();
	
private:
	
	virtual void PrepareForReplication() override;
};

UCLASS()
class GAS_REPGRAPH_API UGASReplicationGraph : public UReplicationGraph
{
	GENERATED_BODY()
	
public:
	
	UGASReplicationGraph();
	
	void HandlePendingActors();
	
protected:
	
	virtual void InitGlobalGraphNodes() override;
	
	virtual void RouteAddNetworkActorToNodes(const FNewReplicatedActorInfo& ActorInfo, FGlobalActorReplicationInfo& GlobalInfo) override;
	
	virtual void InitConnectionGraphNodes(UNetReplicationGraphConnection* ConnectionManager) override;

private:
	
	UGASReplicationGraphConnection* GetConnectionForActor(const AActor* Actor);
	
private:
	
	UPROPERTY()
	UReplicationGraphNode_AlwaysRelevant_WithPending* AlwaysRelevantNode;
	
	TArray<AActor*, TInlineAllocator<16>> PendingActors;
	
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
	
	UPROPERTY()
	UReplicationGraphNode_CubeRelevancy* CubeRelevancyNode;
	
	UPROPERTY()
	UReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantForConnectionNode;
	
	UPROPERTY()
	UReplicationGraphNode_ActorList* ActorListNode;
	
	uint8 bCubeRelevant;
};

