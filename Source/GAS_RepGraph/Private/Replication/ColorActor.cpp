// Fill out your copyright notice in the Description page of Project Settings.


#include "Replication/ColorActor.h"

#include "Net/UnrealNetwork.h"

// Устанавливает значения по умолчанию.
AColorActor::AColorActor()
{
 	// Tick отключен, так как он не нужен для логики актора.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	RootComponent = StaticMeshComponent;

}

// Вызывается при старте игры или при спавне.
void AColorActor::BeginPlay()
{
	Super::BeginPlay();
	
	DynamicMaterial = StaticMeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	
	ApplyColor();
	
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			ColorTimerHandle,
			this,
			&AColorActor::ChangeColor,
			ChangeInterval,
			true
		);
	}
}

void AColorActor::ChangeColor()
{
	CurrentColor = FLinearColor(
		FMath::FRand(),
		FMath::FRand(),
		FMath::FRand(),
		1.f
	);
	
	ApplyColor();
}

void AColorActor::OnRep_Color()
{
	ApplyColor();
}

void AColorActor::ApplyColor()
{
	if (DynamicMaterial)
	{
		DynamicMaterial->SetVectorParameterValue(TEXT("Color"), CurrentColor);
	}
}

void AColorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AColorActor, CurrentColor);
}
