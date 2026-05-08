// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_RepGraphGameMode.h"
#include "GAS_RepGraphCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGAS_RepGraphGameMode::AGAS_RepGraphGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

UCharacterClassInfo* AGAS_RepGraphGameMode::GetCharacterClassDefaultInfo() const
{
	return ClassDefaults;
}

