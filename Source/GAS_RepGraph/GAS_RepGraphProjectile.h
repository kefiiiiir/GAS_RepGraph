// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_RepGraphProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class AGAS_RepGraphProjectile : public AActor
{
	GENERATED_BODY()

	/** Сферический компонент коллизии */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	USphereComponent* CollisionComp;

	/** Компонент движения снаряда */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

public:
	AGAS_RepGraphProjectile();

	/** Вызывается при столкновении снаряда */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Возвращает подобъект `CollisionComp` **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Возвращает подобъект `ProjectileMovement` **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};

