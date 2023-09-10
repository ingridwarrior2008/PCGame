// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine/DataTable.h"
#include "PCGPyramidDefinition.generated.h"

class APCGPyramidBlockActor;

/**
 * Struct for the pyramid color definitions.
 */
USTRUCT(BlueprintType)
struct FPCGPyramidColorDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInterface> PyramidMaterial;

	FPCGPyramidColorDefinitionRow()
	{
		PyramidMaterial = nullptr;
	}
};

/**
 * Struct for the pyramid system definitions.
 */
USTRUCT(BlueprintType)
struct FPCGPyramidDefinitionRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RowType = "PCGPyramidColorDefinitionRow"))
	TArray<FDataTableRowHandle> PyramidColorDefinitionArray;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<APCGPyramidBlockActor> PyramidBlockActorClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 MaxPyramidRow;

	FPCGPyramidDefinitionRow()
	{
		PyramidBlockActorClass = nullptr;
		MaxPyramidRow = 7;
	}
};