// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "GAS/GASAttributeSet.h"
#include "Logging/LogMacros.h"
#include "GAS_RepGraphCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AGAS_RepGraphCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

	/** Меш рук для вида от первого лица (виден только владельцу) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Камера от первого лица */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Контекст сопоставления ввода */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Действие ввода прыжка */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Действие ввода движения */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Действие ввода для активации способности */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* AbilityAction;
	
public:
	AGAS_RepGraphCharacter();
	
	/**

	* Вызывается на сервере, когда персонаж запоссесывается.

	* Инициализация информации об акторе GAS происходит здесь на стороне authority,

	* потому что гарантируется существование PlayerState и Controller.

	*/
	
	virtual void PossessedBy(AController* NewController) override;

	/**

	* Вызывается на клиентах после завершения репликации PlayerState.

	* Информация об акторе GAS также должна быть инициализирована на стороне клиента, поскольку
	* реплицированные ссылки на PlayerState недоступны сразу во время создания персонажа.

	*/
	
	virtual void OnRep_PlayerState();
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float CurrentHealth, float MaxHealth, float OldHealth);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float CurrentMana, float MaxMana, float OldMana);
	
protected:
	virtual void BeginPlay();

public:
		
	/** Действие ввода обзора */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Флаг для AnimBP: переключение на другой набор анимаций */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Сеттер для флага оружия */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Геттер флага оружия */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Обработчик ввода движения */
	void Move(const FInputActionValue& Value);

	/** Обработчик ввода обзора */
	void Look(const FInputActionValue& Value);

	/** Активирует способность, содержащую `AbilityTriggerTag`. */
	void ActivateTaggedAbility(const FInputActionValue& Value);

protected:
	// Интерфейс APawn
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// Конец интерфейса APawn
	
private:
	/**
	* Кэшированная ссылка на компонент системы способностей игрока.
	* 
	* Извлекается из состояния игрока во время инициализации GAS.

	* ПРИМЕЧАНИЕ:

	* Компонент системы способностей намеренно принадлежит состоянию игрока, чтобы обеспечить
	* сохранение состояния при возрождении и замене персонажей.

	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASAbilitySystemComponent> GASAbilitySystemComp;
	
	/**

	* Кэшированная ссылка на набор атрибутов игрока.

	* Содержит реплицированные игровые атрибуты, управляемые GAS.

	*/
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UGASAttributeSet> GASAttributeSet;
	
	/**
	* Идентификатор класса игрового процесса, используемый для настройки на основе данных.
	*/
	UPROPERTY(EditAnywhere, Category="Custom Values | Character Info")
	FGameplayTag CharacterTag;

	/** Тег способности, используемый при нажатии `AbilityAction`. */
	UPROPERTY(EditAnywhere, Category="Custom Values | Abilities")
	FGameplayTag AbilityTriggerTag;
	
	/** Инициализирует связь владельца GAS с аватаром */
	void InitAbilityActorInfo();
	
	/** Применяет атрибуты и способности по умолчанию, основанные на классе */
	void InitClassDefaults();
	
	void BindCallbacksToDependencies();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();

public:
	/** Возвращает подобъект `Mesh1P` **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Возвращает подобъект `FirstPersonCameraComponent` **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

