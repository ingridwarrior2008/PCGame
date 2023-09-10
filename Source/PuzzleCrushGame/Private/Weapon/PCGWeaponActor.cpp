// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PCGWeaponActor.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/PCGProjectileActor.h"

const static FName ProjectileSocket(TEXT("Ammo"));

//----------------------------------------------------------------------------------------------------------------------

APCGWeaponActor::APCGWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	WeaponMesh = CreateOptionalDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetGenerateOverlapEvents(false);
	WeaponMesh->SetCanEverAffectNavigation(false);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGWeaponActor::BeginPlay()
{
	Super::BeginPlay();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGWeaponActor::SpawnProjectile(const FRotator& PlayerCameraRotation)
{
	if (HasAuthority())
	{
		SpawnProjectileInternal(PlayerCameraRotation);
	}
	else
	{
		Server_SpawnProjectile(PlayerCameraRotation);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGWeaponActor::Server_SpawnProjectile_Implementation(const FRotator& PlayerCameraRotation)
{
	SpawnProjectileInternal(PlayerCameraRotation);
}

//----------------------------------------------------------------------------------------------------------------------

bool APCGWeaponActor::Server_SpawnProjectile_Validate(const FRotator& PlayerCameraRotation)
{
	return true;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGWeaponActor::Multicast_PlayShootSFX_Implementation()
{
	if (IsValid(FireSound))
	{
		const FVector& ProjectileSocketLocation = WeaponMesh->GetSocketLocation(ProjectileSocket);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, ProjectileSocketLocation);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGWeaponActor::SpawnProjectileInternal(const FRotator& PlayerCameraRotation)
{
	if (!IsValid(ProjectileClass))
	{
		ensureMsgf(false, TEXT("ProjectileClass is null"));
		return;
	}
	const FVector& ProjectileSocketLocation = WeaponMesh->GetSocketLocation(ProjectileSocket);
	const FVector& RotateVector = PlayerCameraRotation.RotateVector(ProjectileOffset);
	const FTransform& ProjectileTransform = FTransform(PlayerCameraRotation, ProjectileSocketLocation + RotateVector, ProjectileScale);

	AActor* ProjectileActor = GetWorld()->SpawnActorDeferred<AActor>(ProjectileClass, ProjectileTransform, GetOwner(), GetInstigator());
	if (IsValid(ProjectileActor))
	{
		ProjectileActor->FinishSpawning(ProjectileTransform, true);
	}

	Multicast_PlayShootSFX();
}

//----------------------------------------------------------------------------------------------------------------------
