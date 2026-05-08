#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GA_PlaySoundAndEffect.generated.h"

class UGameplayEffect;
class USoundBase;

UCLASS()
class GAS_REPGRAPH_API UGA_PlaySoundAndEffect : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGA_PlaySoundAndEffect();

protected:
	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Audio")
	TObjectPtr<USoundBase> ActivationSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Effect")
	TSubclassOf<UGameplayEffect> EffectToApply;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability|Effect", meta=(ClampMin="0.0"))
	float EffectLevel = 1.f;
};
