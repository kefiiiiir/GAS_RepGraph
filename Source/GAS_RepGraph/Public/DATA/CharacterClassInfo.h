// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

/**

* Конфигурация системы игровых способностей по умолчанию для конкретного класса персонажей.

* Эта структура определяет:

* - Начальную настройку атрибутов с помощью игровых эффектов

* - Начальные активные способности, предоставляемые при появлении/инициализации

* - Пассивные способности, предоставляемые классу навсегда

* Используется в рамках подхода к проектированию, основанного на данных, чтобы классы персонажей

* могли быть настроены без жесткого кодирования игровой логики в C++.

*/

USTRUCT()
struct FCharacterClassDefaultInfo
{
	/**

	* Игровой эффект, применяемый при инициализации персонажа
	* для установки базовых значений атрибутов (здоровье, мана и т. д.).

	*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> DefaultAttributes;
	
	/**

	* Активные способности, предоставляемые классу персонажа при появлении.

	* Эти способности обычно включают:
	* - Навыки, активируемые игроком
	* - Боевые действия
	* - Вспомогательные способности

	*/
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
	
	/**

	* Пассивные способности, навсегда предоставляемые классу персонажа.
	* 
	* Эти способности:
	* - Всегда активны
	* - Не требуют активации ввода
	* - Часто изменяют характеристики или предоставляют скрытые эффекты

	*/
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayAbility>> StartingPassives;
};

/**
 * 
 */
UCLASS()
class GAS_REPGRAPH_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:
	
	/**

	* Сопоставляет тег класса персонажа с его стандартной конфигурацией игрового процесса.

	* Пример:
	* - Воин -> Высокое здоровье, способности ближнего боя
	* - Маг -> Способности, основанные на мане, низкая защита
	* - Разбойник -> Способности скрытности, навыки мобильности

	* Это позволяет полностью балансировать классы на основе данных.

	*/
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FCharacterClassDefaultInfo> ClassDefaultInfoMap;
	
};

