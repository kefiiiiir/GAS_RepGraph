#include "GAS/Abilities/GA_PlaySoundAndEffect.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

UGA_PlaySoundAndEffect::UGA_PlaySoundAndEffect()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UGA_PlaySoundAndEffect::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();
	UAbilitySystemComponent* SourceASC = GetAbilitySystemComponentFromActorInfo();

	if (ActivationSound && AvatarActor)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ActivationSound, AvatarActor->GetActorLocation());
	}

	if (EffectToApply && SourceASC)
	{
		const FGameplayEffectContextHandle EffectContext = SourceASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(EffectToApply, EffectLevel, EffectContext);
		if (SpecHandle.IsValid())
		{
			SourceASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
