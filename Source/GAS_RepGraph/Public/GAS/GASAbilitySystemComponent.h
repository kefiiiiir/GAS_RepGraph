// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "GASAbilitySystemComponent.generated.h"

/**
* Пользовательский компонент системы способностей игрового процесса.
* Расширяет базовый компонент UAbilitySystemComponent вспомогательными функциями
* для инициализации персонажа и настройки игрового процесса.

* Обязанности:
* - Предоставление активных способностей
* - Предоставление пассивных способностей
* - Применение атрибута по умолчанию GameplayEffects

* Разработан для централизации логики настройки GAS и предотвращения повторений
* для разных персонажей и классов игрового процесса.
*/
UCLASS()
class GAS_REPGRAPH_API UGASAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	/**
	* Предоставляет персонажу активные игровые способности.
	
	* Эти способности:
	* - Активируются игроком
	* - Сохраняются после предоставления
	* - Привязываются к вводу или игровым событиям
	*/
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& AbilitiesToGrant);
	
	/**
	* Предоставляет пассивные игровые способности и активирует их немедленно.

	* Эти способности:
	* - Не требуют участия игрока
	* - Активируются один раз после предоставления
	* - Обычно обрабатывают баффы или постоянные модификаторы
	*/
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& PassivesToGrant);
	
	/**
	* Применяет атрибут GameplayEffect по умолчанию для инициализации характеристик.

	* Примеры атрибутов:
	* - Здоровье
	* - Мана
	* - Выносливость
	* - Броня
	*/
	void InitializeDefaultAttributes(const TSubclassOf<UGameplayEffect>& AttributeEffect);

	/** Activates the first granted ability that contains the provided ability tag. */
	UFUNCTION(BlueprintCallable, Category="Abilities")
	bool TryActivateAbilityByTag(const FGameplayTag& AbilityTag);
	
};

