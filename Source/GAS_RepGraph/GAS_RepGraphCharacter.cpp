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
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
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
		}
	}
}

void AGAS_RepGraphCharacter::InitClassDefaults()
{
	// Перед поиском проверяем тег класса.
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("No character tag selected in this character %s"), *GetNameSafe(this));
	}
	
	// Получение данных класса из глобальной библиотеки.
	else if (UCharacterClassInfo* ClassInfo = UGASAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		
		// Найти конфигурацию для этого класса символов.
		if (const FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			
		}
	}
}

void AGAS_RepGraphCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
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
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGAS_RepGraphCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGAS_RepGraphCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AGAS_RepGraphCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGAS_RepGraphCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
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