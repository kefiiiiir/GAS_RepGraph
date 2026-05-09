// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Настраиваем сферическую коллизию.
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Регистрируем обработчик события перекрытия.
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Проверяем, что перекрытие произошло с персонажем от первого лица.
	AGAS_RepGraphCharacter* Character = Cast<AGAS_RepGraphCharacter>(OtherActor);
	if(Character != nullptr)
	{
		// Уведомляем подписчиков, что объект был подобран.
		OnPickUp.Broadcast(Character);

		// Отписываемся от события, чтобы больше не срабатывать.
		OnComponentBeginOverlap.RemoveAll(this);
	}
}
