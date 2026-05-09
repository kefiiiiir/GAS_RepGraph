// Copyright Epic Games, Inc. All Rights Reserved.


#include "GAS_RepGraphPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Replication/GASReplicationGraph.h"

void AGAS_RepGraphPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
	
	if (UNetDriver* NetDriver = GetWorld()->GetNetDriver())
	{
		if (UGASReplicationGraph*  ReplicationGraph = NetDriver->GetReplicationDriver<UGASReplicationGraph>())
		{
			ReplicationGraph->AddPlayerController(this);
		}
	}
}