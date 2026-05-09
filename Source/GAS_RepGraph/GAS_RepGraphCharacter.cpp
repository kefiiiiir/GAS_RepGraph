// Copyright Epic Games, Inc. All Rights Reserved.

#include "GAS_RepGraphCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GASPlayerState.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GAS/GASAbilitySystemComponent.h"
#include "Iris/Core/IrisDebugging.h"
#include "Libraries/GASAbilitySystemLibrary.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGAS_RepGraphCharacter

AGAS_RepGraphCharacter::AGAS_RepGraphCharacter()
{
	// В начале у персонажа нет винтовки.
	bHasRifle = false;
	
	// Настраиваем размер капсулы коллизии.
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Создаем компонент камеры.
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Позиция камеры относительно капсулы.
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Создаем меш рук для вида от первого лица (виден только владельцу).
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AGAS_RepGraphCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Информация об акторе GAS должна быть инициализирована на сервере после запоссесивания
	if (HasAuthority())
	{
		InitAbilityActorInfo();
	}
}

void AGAS_RepGraphCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Клиенты инициализируют информацию об акторе GAS после завершения репликации PlayerState.
	InitAbilityActorInfo();
}

UAbilitySystemComponent* AGAS_RepGraphCharacter::GetAbilitySystemComponent() const
{
	return GASAbilitySystemComp;
}

void AGAS_RepGraphCharacter::InitAbilityActorInfo()
{
	if (AGASPlayerState* GASPlayerState = GetPlayerState<AGASPlayerState>())
	{
		// Получение референса на GAS из постоянно хранимого состояния игрока.
		GASAbilitySystemComp = GASPlayerState->GetGASAbilitySystemComponent();
		GASAttributeSet = GASPlayerState->GetAttributeSet();
		
		if (IsValid(GASAbilitySystemComp))
		{
			// Привзяка OwnerActor (PlayerState) к AvatarActor (Character).
			GASAbilitySystemComp->InitAbilityActorInfo(GASPlayerState, this);
			
			BindCallbacksToDependencies();
			
			if (HasAuthority())
			{
				InitClassDefaults();
			}
		}
	}
}

// Инициализация параметров по умолчанию
void AGAS_RepGraphCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No character tag selected in this character %s"), *GetNameSafe(this));
	}
	
	else if (UCharacterClassInfo* ClassInfo = UGASAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (const FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			// Добавление способностей к персонажу
			if (IsValid(GASAbilitySystemComp))
			{
				GASAbilitySystemComp->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				GASAbilitySystemComp->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassives);
				GASAbilitySystemComp->InitializeDefaultAttributes(SelectedClassInfo->DefaultAttributes);
			}
		}
	}
}

void AGAS_RepGraphCharacter::BindCallbacksToDependencies()
{
	// Привязка ивентов к изменению атрибутов
	if (IsValid(GASAbilitySystemComp) && IsValid(GASAttributeSet))
	{
		GASAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetHealthAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnHealthChanged(Data.NewValue, GASAttributeSet->GetMaxHealth(), Data.OldValue);
			});
		
		GASAbilitySystemComp->GetGameplayAttributeValueChangeDelegate(GASAttributeSet->GetManaAttribute()).AddLambda(
			[this] (const FOnAttributeChangeData& Data)
			{
				OnManaChanged(Data.NewValue, GASAttributeSet->GetMaxMana(), Data.OldValue);
			});
	}
}

void AGAS_RepGraphCharacter::BroadcastInitialValues()
{
	if (IsValid(GASAttributeSet))
	{
		OnHealthChanged(GASAttributeSet->GetHealth(), GASAttributeSet->GetMaxHealth(), 0.f);
		OnManaChanged(GASAttributeSet->GetMana(), GASAttributeSet->GetMaxMana(), 0.f);
	}
}

void AGAS_RepGraphCharacter::BeginPlay()
{
	// Вызываем базовую реализацию.
	Super::BeginPlay();

	// Добавляем базовый контекст ввода.
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AGAS_RepGraphCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Настраиваем бинды действий.
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Прыжок.
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Передвижение.
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_RepGraphCharacter::Move);

		// Взгляд (камера).
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_RepGraphCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGAS_RepGraphCharacter::Move(const FInputActionValue& Value)
{
	// Входное значение осей движения.
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Добавляем движение по локальным осям.
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGAS_RepGraphCharacter::Look(const FInputActionValue& Value)
{
	// Входное значение осей обзора.
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Передаем поворот контроллеру (yaw/pitch).
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGAS_RepGraphCharacter::ActivateTaggedAbility(const FInputActionValue& Value)
{
	(void)Value;

	if (!AbilityTriggerTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("AbilityTriggerTag is not set on character %s"), *GetNameSafe(this));
		return;
	}

	if (!IsValid(GASAbilitySystemComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("GASAbilitySystemComp is invalid on character %s"), *GetNameSafe(this));
		return;
	}

	if (!GASAbilitySystemComp->TryActivateAbilityByTag(AbilityTriggerTag))
	{
		UE_LOG(LogTemp, Warning, TEXT("No activatable ability found for tag %s"), *AbilityTriggerTag.ToString());
	}
}

void AGAS_RepGraphCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGAS_RepGraphCharacter::GetHasRifle()
{
	return bHasRifle;
}