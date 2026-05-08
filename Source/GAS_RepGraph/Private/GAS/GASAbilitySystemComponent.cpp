// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GASAbilitySystemComponent.h"

void UGASAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant)
{
	// Проходим по всем классам способностей и предоставляем их этому классу способностей.
	for (const TSubclassOf<UGameplayAbility>& Ability : AbilitiesToGrant)
	{
		// Создайте базовую специализацию способностей с 1-м уровнем.
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		
		// Предоставляем возможность этому компоненту системы способностей.
		GiveAbility(AbilitySpec);
	}
}

void UGASAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant)
{
	// Пассивные способности предоставляются и активируются немедленно.
	for (const TSubclassOf<UGameplayAbility>& Ability : PassivesToGrant)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability, 1.f);
		
		// Предоставляет способность и активирует её один раз сразу после назначения.
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UGASAbilitySystemComponent::InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect)
{
	// проверяем, существует ли допустимый эффект инициализации атрибута.
	checkf(AttributeEffect, TEXT("No Valid defualt attributes for this character %s"), *GetAvatarActor()->GetName());
	
	// Создание контекста игрового эффекта для его самостоятельного применения
	const FGameplayEffectContextHandle ContextHandle = MakeEffectContext();
	
	// Создание спецификации для эффекта инициализации атрибута.
	const FGameplayEffectSpecHandle SpecHandle = MakeOutgoingSpec(AttributeEffect, 1.f, ContextHandle);
	
	// Применить эффект инициализации атрибута к самому себе.
	ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

bool UGASAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag& AbilityTag)
{
	if (!AbilityTag.IsValid())
	{
		return false;
	}

	FGameplayTagContainer AbilityTagContainer;
	AbilityTagContainer.AddTag(AbilityTag);

	TArray<FGameplayAbilitySpec*> MatchingSpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagContainer, MatchingSpecs, false);

	for (const FGameplayAbilitySpec* AbilitySpec : MatchingSpecs)
	{
		if (AbilitySpec && TryActivateAbility(AbilitySpec->Handle))
		{
			return true;
		}
	}

	return false;
}

