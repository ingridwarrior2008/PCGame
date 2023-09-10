// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CheatManager.h"
#include "PCGCheatManager.generated.h"

/**
 * CheatManager for testing functionality in debug builds.
 */
UCLASS()
class PUZZLECRUSHGAME_API UPCGCheatManager : public UCheatManager
{
	GENERATED_BODY()

public:

	UFUNCTION(exec, BlueprintAuthorityOnly)
	void Server_PrintPyramidAdjacentBlockInLocation(const float InX, const float InY);

	UFUNCTION(exec)
	void PrintPyramidAdjacentBlockInLocation(const float InX, const float InY);

	UFUNCTION(exec)
	void PrintFibonacciScoreSequence(const float Sequence);

	UFUNCTION(exec, BlueprintAuthorityOnly)
	void Server_DestroyBlockInLocation(const float InX, const float InY);

	UFUNCTION(exec, BlueprintAuthorityOnly)
	void Server_FindNearestUpperBlockFromLocation(const float InX, const float InY);

private:
	void PrintPyramidAdjacentBlockInLocationInternal(const float InX, const float InY);
};
