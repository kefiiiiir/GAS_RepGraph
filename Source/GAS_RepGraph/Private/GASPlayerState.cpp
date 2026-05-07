// Fill out your copyright notice in the Description page of Project Settings.


#include "GASPlayerState.h"
#include "GAS/GASAbilitySystemComponent.h"

AGASPlayerState::AGASPlayerState()
{
	// Увеличваем частоту репликации для повышения скорости обновления атрибутов
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 66.f;
	
	// Создание и настройка основного компонента системы способностей
	AbilitySystemComp = CreateDefaultSubobject<UGASAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComp->SetIsReplicated(true);
	
	// Смешанная репликация отправляет минимальный объем данных невладельцам, сохраняя при этом
	// полную информацию об игровом процессе для клиента-владельца.
	AbilitySystemComp->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	// Создаем набор атрибутов, содержащий повторяющиеся игровые атрибуты.
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

