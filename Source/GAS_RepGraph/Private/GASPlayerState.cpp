// Fill out your copyright notice in the Description page of Project Settings.


#include "GASPlayerState.h"
#include "GAS/GASAbilitySystemComponent.h"

AGASPlayerState::AGASPlayerState()
{
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 66.f;
	
	AbilitySystemComp = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComp->SetIsReplicated(true);
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	AttributeSet = CreateDefaultSubobject<UGASAttributeSet>("AttributeSet");
}

UAbilitySystemComponent* AGASPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

UGASAbilitySystemComponent* AGASPlayerState::GetGASAbilitySystemComponent() const
{
	return AbilitySystemComp;
}

UGASAttributeSet* AGASPlayerState::GetAttributeSet() const
{
	return AttributeSet;
}

