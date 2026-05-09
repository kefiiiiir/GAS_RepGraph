// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/EffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Libraries/GASAbilitySystemLibrary.h"

// Устанавливает значения по умолчанию.
AEffectActor::AEffectActor()
{
 	// Tick отключен, так как логика работает через оверлап и события.
	PrimaryActorTick.bCanEverTick = false;
	
	bReplicates = true;
	
	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(ActorMesh);
	
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(GetRootComponent());
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}

// Вызывается при старте игры или при спавне.
void AEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AEffectActor::OnBoxBeginOverlap);
	}
	
}

void AEffectActor::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Добавляем выбранный в редакторе эффект к игроку
	if (UAbilitySystemComponent* OtherASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		const FGameplayEffectContextHandle ContextHandle = OtherASC->MakeEffectContext();
		const FGameplayEffectSpecHandle SpecHandle = OtherASC->MakeOutgoingSpec(Effect, 1.f, ContextHandle);
		
		OtherASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		
		Destroy();
	}
}

