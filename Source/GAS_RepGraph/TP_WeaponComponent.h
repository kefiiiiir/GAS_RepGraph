// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "TP_WeaponComponent.generated.h"

class AGAS_RepGraphCharacter;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAS_REPGRAPH_API UTP_WeaponComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Класс снаряда для спавна */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AGAS_RepGraphProjectile> ProjectileClass;

	/** Звук, который проигрывается при каждом выстреле */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;
	
	/** Анимационный монтаж, проигрываемый при каждом выстреле */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Смещение дула оружия относительно персонажа */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	FVector MuzzleOffset;

	/** Контекст сопоставления ввода */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Действие ввода для стрельбы */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	/** Устанавливает значения по умолчанию для свойств компонента */
	UTP_WeaponComponent();

	/** Прикрепляет оружие к персонажу от первого лица */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void AttachWeapon(AGAS_RepGraphCharacter* TargetCharacter);

	/** Выполняет выстрел снарядом */
	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

protected:
	/** Завершает работу компонента при окончании игры/жизни актора. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	/** Персонаж, который держит это оружие */
	AGAS_RepGraphCharacter* Character;
};
