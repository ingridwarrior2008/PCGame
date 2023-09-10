// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PCGActorSizeInterface.generated.h"

UINTERFACE(MinimalAPI)
class UPCGActorSizeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that can obtain the root mesh size like BoxExtent.
 */
class PUZZLECRUSHGAME_API IPCGActorSizeInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector BP_GetBoxExtent() const;
};
