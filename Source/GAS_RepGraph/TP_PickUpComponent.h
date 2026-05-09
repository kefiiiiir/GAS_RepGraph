// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GAS_RepGraphCharacter.h"
#include "TP_PickUpComponent.generated.h"

// Делегат, вызываемый когда кто-то подбирает объект.
// В параметре передается персонаж, который выполнил подбор.
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPickUp, AGAS_RepGraphCharacter*, PickUpCharacter);

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAS_REPGRAPH_API UTP_PickUpComponent : public USphereComponent
{
	GENERATED_BODY()

public:
	
	/** Делегат события подбора, на который могут подписаться другие системы */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnPickUp OnPickUp;

	UTP_PickUpComponent();
protected:

	/** Вызывается при старте игры */
	virtual void BeginPlay() override;

	/** Обработчик перекрытия этого компонента */
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};