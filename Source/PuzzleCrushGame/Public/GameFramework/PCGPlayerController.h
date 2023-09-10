// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCGPlayerController.generated.h"

class APCGHUD;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPCGOnPlayerStateWasSetSignature);

/**
 * APCGPlayerController
 *
 *	The base PlayerController class.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APCGPlayerController();

	virtual void AcknowledgePossession(APawn* InPawn) override;

	/** Triggers when Player state was replicated to the clients. */
	UPROPERTY(BlueprintAssignable)
	FPCGOnPlayerStateWasSetSignature OnPlayerStateWasSetDelegate;

protected:

	virtual void OnRep_PlayerState() override;

private:

	/** A reference of the game HUD. */
	UPROPERTY(Transient)
	APCGHUD* GameHUD;
};
