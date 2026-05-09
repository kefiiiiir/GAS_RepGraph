// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ColorActor.generated.h"

UCLASS()
class GAS_REPGRAPH_API AColorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Устанавливает значения по умолчанию для свойств актора.
	AColorActor();

protected:
	// Вызывается при старте игры или при спавне.
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;
	
	UPROPERTY(ReplicatedUsing=OnRep_Color)
	FLinearColor CurrentColor;
	
	UPROPERTY(EditAnywhere, Category = Color)
	float ChangeInterval = 2.f;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterial;
	
	FTimerHandle ColorTimerHandle;
	
	UFUNCTION()
	void OnRep_Color();
	
	void ChangeColor();
	
	void ApplyColor();

public:	
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
