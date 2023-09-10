// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PCGPyramidGridInterface.generated.h"

UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UPCGPyramidGridInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for actors that will have the functionality of a Pyramid Grid. Cannot be implemented in BPs
 */
class PUZZLECRUSHGAME_API IPCGPyramidGridInterface
{
	GENERATED_BODY()

public:

	/**
	 * Sets a new Grid 2D location.
	 * @param InColumLocation Column Location.
	 * @param InRowLocation Row Location.
	 * @note Only valid on Server
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void SetGridLocation(const int32 InColumLocation, const int32 InRowLocation) = 0;

	/**
	 * Gets the Grid 2D location.
	 */
	UFUNCTION(BlueprintCallable)
	virtual const FVector2D& GetGridLocation() const = 0;

	/**
	 * Sets the new grid color.
	 * @param InColorDefinition Color Definition Row from PyramidColorDefinitionRow struct.
	 * @note Only valid on Server
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void SetGridColor(const FDataTableRowHandle& InColorDefinition) = 0;

	/**
	 * Gets the Grid Color ID.
	 */
	UFUNCTION(BlueprintCallable)
	virtual uint8 GetGridColorID() const = 0;

	/**
	 *  Relocate the grid to a new position.
	 * @param InNewWorldLocation New Grid World location.
	 * @param InPositionInGrid New Grid 2D location.
	 * @note Only valid on Server
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void ReLocateGridWorldLocation(const FVector& InNewWorldLocation, const FVector2D& InPositionInGrid) = 0;

	/**
	 * Gets if a Location its adjacent.
	 */
	UFUNCTION(BlueprintCallable)
	virtual bool IsAdjacentLocation(const FVector2D& InPositionInGrid, const uint8 InColorID) const = 0;

	/**
	 * Gets all adjacent actors from the current grid.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void GetAdjacentActors(TArray<AActor*>& OutAdjacentPyramidActors) const = 0;

	/**
	 * Call to destroy the grid.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	virtual void CallToDestroyGrid() = 0;

	/**
	 * Gets the grid raw actor.
	 */
	UFUNCTION(BlueprintCallable)
	virtual AActor* GetGridRawActor() const = 0;

	/**
	 * Gets if the grid is being relocated.
	 */
	UFUNCTION(BlueprintCallable)
	virtual bool IsBeingRelocated() const = 0;

	/**
	 * Gets the grid world location.
	 */
	UFUNCTION(BlueprintCallable)
	virtual FVector GetGridWorldLocation() const = 0;

	/**
	 * Gets if the grid mark for delete.
	 */
	UFUNCTION(BlueprintCallable)
	virtual bool IsPendingDelete() const = 0;

	/**
	 * Set new status for IsBeingRelocated.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetIsBeingRelocated(const bool bNewIsBeingRelocated) = 0;

	/**
	 * Sets new status for IsPendingDelete.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SetIsPendingDelete(const bool bNewIsPendingDelete) = 0;

	/**
	 * Loop the adjacent actors for the current grid.
	 */
	virtual void ForEachAdjacentActors(TFunctionRef<void(AActor* PyramidBlockActor)> Predicate) const = 0;
};
