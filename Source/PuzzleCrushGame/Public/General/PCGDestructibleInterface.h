// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PCGDestructibleInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPCGDestructibleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can handle when receive a damage call.
 */
class PUZZLECRUSHGAME_API IPCGDestructibleInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void BP_OnDestroyActor(AActor* ActorInstigator);
};
