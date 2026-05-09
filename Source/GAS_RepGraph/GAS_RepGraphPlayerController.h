// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GAS_RepGraphPlayerController.generated.h"

class UInputMappingContext;

/** Контроллер игрока, который настраивает локальный ввод при старте. */
UCLASS()
class GAS_REPGRAPH_API AGAS_RepGraphPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/** Контекст ввода, применяемый для управления игроком */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;

	// Начало интерфейса Actor
protected:

	virtual void BeginPlay() override;

	// Конец интерфейса Actor
};
