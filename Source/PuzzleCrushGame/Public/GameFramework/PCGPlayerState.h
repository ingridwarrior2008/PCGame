// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PCGPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPCGOnScoreChangedSignature, const float, NewScore);

/**
 * APCGPlayerState
 *
 *	Base player state class.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	
	/**
	 * Set the new player score based on the number of grids that were destroyed.
	 * @param NumScoreSequence Number of grids destroyed.
	 * @note Only valid on Server.
	 */
	void SetPlayerScore(const int32 NumScoreSequence);

	/** Triggers when PlayerState score changed. */
	UPROPERTY(BlueprintAssignable)
	FPCGOnScoreChangedSignature OnScoreChangedDelegate;

protected:
	
	virtual void OnRep_Score() override;
};
