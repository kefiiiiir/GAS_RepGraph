// Copyright Epic Games, Inc. All Rights Reserved.


#include "GAS_RepGraphPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Replication/GASReplicationGraph.h"

void AGAS_RepGraphPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Получаем подсистему Enhanced Input.
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Добавляем контекст сопоставления, чтобы включить управление.
		Subsystem->AddMappingContext(InputMappingContext, 0);

		UE_LOG(LogTemp, Warning, TEXT("BeginPlay"));
	}
	
	if (UNetDriver* NetDriver = GetWorld()->GetNetDriver())
	{
		// Регистрируем контроллер в графе репликации после инициализации сети.
		if (UGASReplicationGraph*  ReplicationGraph = NetDriver->GetReplicationDriver<UGASReplicationGraph>())
		{
			ReplicationGraph->AddPlayerController(this);
		}
	}
}