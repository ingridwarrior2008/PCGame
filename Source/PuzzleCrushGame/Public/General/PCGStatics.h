// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PCGStatics.generated.h"

/**
 * Helper functions to be accessed in the project.
 */
UCLASS()
class PUZZLECRUSHGAME_API UPCGStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * Calculate Fibonacci Sequence. 
	 * @param Sequence The sequence to get in the fibonacci.
	 * @return The result of fibonacci in the sequence.
	 */
	UFUNCTION(BlueprintPure, Category="Player | Score")
	static float CalculateFibonacciSequence(const int32 Sequence);
};
