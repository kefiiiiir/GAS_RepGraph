// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "GAS/GASAttributeSet.h"
#include "GASPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAS_REPGRAPH_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	AGASPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	UGASAbilitySystemComponent* GetGASAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintCallable)
	UGASAttributeSet* GetAttributeSet() const;
	
private:
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComp;
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAttributeSet> AttributeSet;
};
