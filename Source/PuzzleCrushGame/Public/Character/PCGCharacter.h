// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PCGCharacter.generated.h"

class APCGWeaponActor;
class UCameraComponent;
class UInputAction;
class UInputComponent;
class UInputMappingContext;
class USpringArmComponent;
class UTextRenderComponent;

/**
 * APCGCharacter
 *
 *	The base character pawn class.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	APCGCharacter();

	/** Initialize the input mapping. */
	void InitializeInputMapping();

	/** 
	 * Sends score based of the sequence to the player state.
	 * @note: Only valid on Sever
	 */
	void SendPlayerScore(const int32 Sequence);

protected:

	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Shoot();

	/** The class of the weapon to spawn. */
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<APCGWeaponActor> WeaponActorClass;

private:

	/** Initialize Weapon Actor. */
	void InitializeWeapon();

	/** Shows the player name in the TextRenderComponent. */
	void ShowPlayerName();

	/** A reference of the weapon actor. */
	UPROPERTY(Transient, Replicated)
	APCGWeaponActor* WeaponActor;
	
	/** MappingContext. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	/** Jump Input Action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	/** Move Input Action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	/** Look Input Action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	/** Shoot Input Action. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> ShootAction;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	/** Player Name Text Render. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> PlayerNameTextRender;
};
