// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "GAS/GASAttributeSet.h"
#include "GASPlayerState.generated.h"

/**

* Реализация PlayerState, отвечающая за владение и предоставление доступа к
* компонентам системы игровых способностей (GAS) для игрока.

* * Компоненты GAS намеренно хранятся в PlayerState, а не в Character,

* чтобы:

* - Атрибуты сохранялись при возрождении игрока
* - Активные игровые эффекты сохранялись при смене владельца
* - Данные о способностях оставались стабильными во время беспрепятственного перемещения
* - Сервер поддерживал авторитетный прогресс/состояние игрока

*

* Этот класс выступает в качестве центрального постоянного контейнера игровых данных

* для всех систем игрока, связанных с GAS.

*/

UCLASS()
class GAS_REPGRAPH_API AGASPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	
	AGASPlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable)
	UGASAbilitySystemComponent* GetGASAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintCallable)
	UGASAttributeSet* GetAttributeSet() const;
	
private:
	
	/**

	* Основной компонент системы способностей в игровом процессе.

	*
	* Отвечает за:
	* - Активацию способностей

	* - Обработку игровых эффектов

	* - Репликацию атрибутов

	* - Игровые теги

	* - Игровые подсказки
	* - Обработку предсказаний
	*

	* Режим репликации настроен на смешанную репликацию для

	* балансировки пропускной способности сети и отзывчивости игрового процесса.

	*/
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAbilitySystemComponent> AbilitySystemComp;
	
	/**

	* Контейнер для всех игровых атрибутов, принадлежащих этому игроку.

	*

	* Наборы атрибутов — это подобъекты, управляемые системой способностей,

	* и автоматически реплицируемые с помощью логики репликации GAS.

	*/
	
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UGASAttributeSet> AttributeSet;
};
