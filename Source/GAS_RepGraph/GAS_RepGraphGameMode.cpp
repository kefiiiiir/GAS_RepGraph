// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_RepGraphGameMode.h"
#include "Replication/GASReplicationGraph.h"
#include "ReplicationGraph.h"
#include "UObject/ConstructorHelpers.h"

AGAS_RepGraphGameMode::AGAS_RepGraphGameMode()
	: Super()
{
	// Назначаем класс персонажа по умолчанию из Blueprint.
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

UCharacterClassInfo* AGAS_RepGraphGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}

