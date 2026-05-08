// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
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
class AGAS_RepGraphCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	
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
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(float CurrentHealth, float MaxHealth);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnManaChanged(float CurrentMana, float MaxMana);
	
protected:
	virtual void BeginPlay();

public:
		
	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	
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
	
	/** Инициализирует связь владельца GAS с аватаром */
	void InitAbilityActorInfo();
	
	/** Применяет атрибуты и способности по умолчанию, основанные на классе */
	void InitClassDefaults();
	
	void BindCallbacksToDependencies();
	
	UFUNCTION(BlueprintCallable)
	void BroadcastInitialValues();

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

