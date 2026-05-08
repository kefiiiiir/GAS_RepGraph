// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "DATA/CharacterClassInfo.h"
#include "GameFramework/GameModeBase.h"
#include "GAS_RepGraphGameMode.generated.h"

/**
* Пользовательский режим игры, отвечающий за определение основных правил игрового процесса,
* и предоставляющий доступ к глобальной конфигурации классов персонажей.
* 
* Режим игры существует только на сервере и не реплицируется на клиенты,
* что делает его авторитетным источником данных для настройки игрового процесса.
*/
UCLASS(minimalapi)
class AGAS_RepGraphGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGAS_RepGraphGameMode();
	
	/**
	* Возвращает глобальные данные конфигурации класса персонажа.

	* Используется для:
	* - Присвоения начальных атрибутов
	* - Предоставления способностей по умолчанию
	* - Определения настроек игрового процесса на основе класса
	*/
	UCharacterClassInfo* GetCharacterClassDefaultInfo() const;
	
private:
	
	/**
	* Ресурс данных, содержащий конфигурацию по умолчанию для всех классов персонажей.

	* Включает в себя:
	* - Базовые настройки атрибутов
	* - Начальные способности
	* - Определения пассивных способностей
	
	* Разработано для балансировки игрового процесса на основе данных.
	*/
	UPROPERTY(EditDefaultsOnly, Category="Custom Values | Class Defaults")
	TObjectPtr<UCharacterClassInfo> ClassDefaults;
};



