// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PCGGameMode.generated.h"

/**
 * APCGGameMode
 *
 *	The base game mode class.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	APCGGameMode();
	
	virtual void HandleMatchHasStarted() override;

	/** Wrapper function to call EndMatch(). */
	void CallEndMatch();

	/** Wrapper function to call RestartGame(). */
	void CallRestartMatch();
};
