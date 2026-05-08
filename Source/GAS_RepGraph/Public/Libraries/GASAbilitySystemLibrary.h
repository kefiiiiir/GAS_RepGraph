// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DATA/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GASAbilitySystemLibrary.generated.h"

/**
* Глобальная библиотека вспомогательных функций для системы способностей игрового процесса.

* Этот класс предоставляет статические вспомогательные функции, доступные из:
* - C++
* - Blueprints

* Он выступает в качестве центрального моста между игровыми системами и ресурсами данных,
* уменьшая прямую связь между классами игрового процесса и логикой игрового режима.
*/
UCLASS()
class GAS_REPGRAPH_API UGASAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	
	/**
	* Извлекает глобальные данные конфигурации класса персонажа
	* из текущего игрового gamemode.

	* ПРИМЕЧАНИЕ:
	* - Действительно только на сервере (игровой режим существует только на сервере)
	* - Возвращает nullptr на клиентах

	* Используется для доступа к:
	* - Настройкам атрибутов по умолчанию
	* - Начальным способностям
	* - Конфигурациям пассивных способностей
	*/
	UFUNCTION(BlueprintPure)
	static UCharacterClassInfo* GetCharacterClassDefaultInfo(const UObject* WorldContextObject);
	
};

