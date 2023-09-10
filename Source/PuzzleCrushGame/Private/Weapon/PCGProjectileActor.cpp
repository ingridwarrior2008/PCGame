// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PCGProjectileActor.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "General/PCGDestructibleInterface.h"

//----------------------------------------------------------------------------------------------------------------------

APCGProjectileActor::APCGProjectileActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicatingMovement(true);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(ProjectileMesh);

	CollisionVolume = CreateDefaultSubobject<USphereComponent>("CollisionVolume");
	CollisionVolume->SetupAttachment(ProjectileMesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	
	bDidProjectileHit = false;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &APCGProjectileActor::OnCollisionVolumeOverlap);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGProjectileActor::OnCollisionVolumeOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bDidProjectileHit || !IsValid(OtherActor) || OtherActor == this || GetInstigator() == OtherActor)
	{
		return;
	}

	bDidProjectileHit = true;
	ProjectileMovement->StopMovementImmediately();

	if (OtherActor->Implements<UPCGDestructibleInterface>())
	{
		IPCGDestructibleInterface::Execute_BP_OnDestroyActor(OtherActor, GetInstigator());
	}

	Destroy();
}

//----------------------------------------------------------------------------------------------------------------------
