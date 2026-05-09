// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "EffectActor.generated.h"

UCLASS()
class GAS_REPGRAPH_API AEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Устанавливает значения по умолчанию для свойств актора.
	AEffectActor();

protected:
	// Вызывается при старте игры или при спавне.
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private:
	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> ActorMesh;

	UPROPERTY(VisibleAnywhere, meta=(AllowPrivateAccess = true))
	TObjectPtr<UBoxComponent> OverlapBox;
	
	UPROPERTY(EditAnywhere, Category="Custom Values | Effects")
	TSubclassOf<UGameplayEffect> Effect;
};

