// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PCGCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PCGPlayerController.h"
#include "GameFramework/PCGPlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/PCGWeaponActor.h"

const static FName WeaponSocket(TEXT("WeaponSocket"));

//----------------------------------------------------------------------------------------------------------------------

APCGCharacter::APCGCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = false;	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	PlayerNameTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerNameTextRender"));
	PlayerNameTextRender->SetupAttachment(GetMesh());
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::BeginDestroy()
{
	Super::BeginDestroy();
	
	if (IsValid(WeaponActor))
	{
		WeaponActor->Destroy();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APCGCharacter, WeaponActor)
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::InitializeInputMapping()
{
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::SendPlayerScore(const int32 Sequence)
{
	if (HasAuthority())
	{
		APCGPlayerState* PCGPlayerState = GetPlayerState<APCGPlayerState>();
		if (IsValid(PCGPlayerState))
		{
			PCGPlayerState->SetPlayerScore(Sequence);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APCGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APCGCharacter::Look);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APCGCharacter::Shoot);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitializeInputMapping();
	InitializeWeapon();
	ShowPlayerName();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ShowPlayerName();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D& MovementVector = Value.Get<FVector2D>();
		const FRotator& Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
		const FVector& ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector& RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D& LookAxisVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::Shoot()
{
	if (IsValid(WeaponActor))
	{
		const APCGPlayerController* PCGPlayerController = Cast<APCGPlayerController>(GetController());
		if (IsValid(PCGPlayerController) && IsValid(PCGPlayerController->PlayerCameraManager))
		{
			WeaponActor->SpawnProjectile(PCGPlayerController->PlayerCameraManager->GetCameraRotation());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::InitializeWeapon()
{
	if (HasAuthority())
	{
		if (ensureMsgf(WeaponActorClass, TEXT("WeaponActorClass should not be null")))
		{
			WeaponActor = GetWorld()->SpawnActorDeferred<APCGWeaponActor>(WeaponActorClass, FTransform::Identity, this, this);
			WeaponActor->FinishSpawning(FTransform::Identity, true);
			WeaponActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, WeaponSocket);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGCharacter::ShowPlayerName()
{
	if (IsValid(PlayerNameTextRender) && IsValid(GetPlayerState()))
	{
		PlayerNameTextRender->SetText(FText::FromString(GetPlayerState()->GetPlayerName()));
	}
}

//----------------------------------------------------------------------------------------------------------------------
