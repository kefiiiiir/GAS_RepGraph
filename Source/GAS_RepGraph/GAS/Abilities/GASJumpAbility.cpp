// Fill out your copyright notice in the Description page of Project Settings.


#include "GASJumpAbility.h"
#include "GameFramework/Character.h"
#include "AbilitySystemComponent.h"

void UGASJumpAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	// Check character reference
	if (ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get()))
	{
		// Jump
		Character->Jump();
	}
	
	// End ability
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}