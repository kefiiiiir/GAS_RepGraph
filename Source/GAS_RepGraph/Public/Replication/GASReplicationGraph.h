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
	
	void HandlePendingPlayerControllers();
	
	void AddPlayerController(const APlayerController* NewPlayerController);
	
	const TArray<UGASReplicationGraphConnection*, TInlineAllocator<16>>& GetPlayerConnections() const
	{
		return PlayerConnections;
	}
	
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
	
	TArray<const APlayerController*, TInlineAllocator<16>> PlayerControllers;
	
	TArray<UGASReplicationGraphConnection*, TInlineAllocator<16>> PlayerConnections;
	
};

UCLASS()
class GAS_REPGRAPH_API UReplicationGraphNode_ConnectionActors : public UReplicationGraphNode_ActorList
{
	GENERATED_BODY()
	
public:
	UReplicationGraphNode_ConnectionActors();
	
protected:
	
	virtual void GatherActorListsForConnection(const FConnectionGatherActorListParameters& Params) override;
	
	void GatherActorListsForConnectionDefault(const FConnectionGatherActorListParameters& Params);
	
	virtual void PrepareForReplication() override;
};

UCLASS()
class GAS_REPGRAPH_API UGASReplicationGraphConnection : public UNetReplicationGraphConnection
{
	GENERATED_BODY()
	
	friend UGASReplicationGraph;
	
public:
	
	UReplicationGraphNode_ConnectionActors* GetConnectionActorListNode()
	{
		return ActorListNode;
	}
	
private:
	
	UPROPERTY()
	UReplicationGraphNode_ConnectionActors* CubeRelevancyNode;
	
	UPROPERTY()
	UReplicationGraphNode_AlwaysRelevant_ForConnection* AlwaysRelevantForConnectionNode;
	
	UPROPERTY()
	UReplicationGraphNode_ConnectionActors* ActorListNode;
	
	uint8 bCubeRelevant;
};

