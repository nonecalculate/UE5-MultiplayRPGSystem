// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Game/PlayerState/RPGPlayerState.h"
#include "Data/CharacterClassInfo.h"
#include "Libraries/RPGAbilitySystemLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"



DEFINE_LOG_CATEGORY(LogTemplateCharacter);



AMultiPlayerCharacter::AMultiPlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); 

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMultiPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (HasAuthority()) // ������ �ִ� ��� �������� AbilityActorInfo �� �ʱ�ȭ
	{
		InitAbilityActorInfo();
	}
}

void AMultiPlayerCharacter::OnRep_PlayerState()// rep �˸��� Ŭ���̾�Ʈ���� �ùٸ��� ���޵ǰ� �÷��̾� ���°� ��ȿ�� ���� ���� ������ �� �� ������ �ʱ�ȭ
{
	Super::OnRep_PlayerState(); 

	InitAbilityActorInfo(); // Ŭ���̾�Ʈ�� ���� �ɶ����� OnRep �� ����Ǳ⿡ ���� Valid üũ�� �ʿ����.
}


void AMultiPlayerCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMultiPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMultiPlayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMultiPlayerCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMultiPlayerCharacter::InitAbilityActorInfo()
{
	if (ARPGPlayerState* RPGPlayerState = GetPlayerState<ARPGPlayerState>())
	{
		RPGAbilitySystemComp = RPGPlayerState->GetRPGAbilitySystemComponent();
		RPGAttributes = RPGPlayerState->GetRPGAttributes();

		if (IsValid(RPGAbilitySystemComp))
		{
			RPGAbilitySystemComp->InitAbilityActorInfo(RPGPlayerState, this); // �÷��̾� ������Ʈ�� �����Ƽ �ý����� ������ �� �������� ������ ȣ�� �Ű������� OwnerActor, AvatarActor
			

			if (HasAuthority()) // ���������� ����Ǿ����
			{
				InitClassDefaults();
			}
		}
	}
}

void AMultiPlayerCharacter::InitClassDefaults()
{
	if (!CharacterTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("NO CHARACTER TAG SELECTED IN THIS CHARACTER %s"), *GetNameSafe(this));
	}
	else if (UCharacterClassInfo* ClassInfo = URPGAbilitySystemLibrary::GetCharacterClassDefaultInfo(this))
	{
		if (const FCharacterClassDefaultInfo* SelectedClassInfo = ClassInfo->ClassDefaultInfoMap.Find(CharacterTag))
		{
			if (IsValid(RPGAbilitySystemComp))
			{
				RPGAbilitySystemComp->AddCharacterAbilities(SelectedClassInfo->StartingAbilities);
				RPGAbilitySystemComp->AddCharacterPassiveAbilities(SelectedClassInfo->StartingPassive);
				RPGAbilitySystemComp->InitializeDefaultAttribute(SelectedClassInfo->DefaultAttribute);
			}
		}
	}
}

void AMultiPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMultiPlayerCharacter::Look(const FInputActionValue& Value)
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
