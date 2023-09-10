// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PCGWeaponActor.generated.h"

class APCGProjectileActor;
class USkeletalMeshComponent;

/**
 * Native base class for the game weapons.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCGWeaponActor();

	void SpawnProjectile(const FRotator& PlayerCameraRotation);

protected:
	
	virtual void BeginPlay() override;

	/**
	 * Call on Server to spawn the projectile.
	 * @param PlayerCameraRotation the player camera rotation to take the direct to spawn the projectile.
	 */
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SpawnProjectile(const FRotator& PlayerCameraRotation);

	/**
	 * Multicast function to play shoot sfx.
	 */
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayShootSFX();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Weapon)
	TSubclassOf<APCGProjectileActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FVector ProjectileOffset;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FVector ProjectileScale;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TObjectPtr<USoundBase> FireSound;

private:
	void SpawnProjectileInternal(const FRotator& PlayerCameraRotation);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> WeaponMesh;
};
