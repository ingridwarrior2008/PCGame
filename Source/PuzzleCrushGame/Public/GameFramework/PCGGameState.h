// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PCGGameState.generated.h"


class APCGPyramidSystemActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPCGOnMatchEndedSignature);


/**
 * APCGGameState
 *
 *	The base game state.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGGameState : public AGameState
{
	GENERATED_BODY()

public:
	APCGGameState();

	/** Gets the remaining time to restart the match. */
	int32 GetRemainingTimeToRestartMatch() const { return RemainingTimeToRestartMatch; }

	/** Triggers when the game state receive MatchHasEnded. */
	UPROPERTY(BlueprintAssignable)
	FPCGOnMatchEndedSignature OnMatchEndedDelegate;
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void HandleMatchHasEnded() override;

	/** The initial value of the Remaining time to restart a match. */
	UPROPERTY(EditDefaultsOnly)
	int32 InitialRemainingTime;
	
private:

	/** Stores all the pyramid systems that are in the game world. */
	void InitializePyramidSystems();

	/** Starts the countdown to restart the match. */
	void InitializeRestartMatchCountdown();

	/** Tells the game mode to restart the match. */
	void CallRestartMatch();

	/** Callback function that is called when all the grids of a pyramid system have been destroyed. */
	UFUNCTION()
	void OnGridsDestroyedCallback(APCGPyramidSystemActor* FinishedPyramidSystemActor);

	/** Callback function that is called on RemainingTimeTimer tick. */
	UFUNCTION()
	void RemainingTimeCallback();

	/** Array that stores all the pyramid systems that are in the game world. */
	UPROPERTY(Transient)
	TArray<APCGPyramidSystemActor*> PyramidSystemActorArray;

	/** the remaining time timer handle . */
	UPROPERTY(Transient)
	FTimerHandle RemainingTimeTimerHandle;

	/** A integer value to display in runtime the remaining time to restart a match. */
	UPROPERTY(Transient, Replicated)
	int32 RemainingTimeToRestartMatch;
};
