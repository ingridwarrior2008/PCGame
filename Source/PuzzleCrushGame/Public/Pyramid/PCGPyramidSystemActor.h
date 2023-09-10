// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "PCGPyramidSystemActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPCGOnPyramidSystemSignature, APCGPyramidSystemActor*, PyramidSystemActorRef);

class APCGPyramidBlockActor;

/**
 * Native base class for the pyramid system, This class should be in blueprint and placed in the game map.
 * It will create a grid of PyramidBlock depending how was defined in PyramidDefinitionRow.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGPyramidSystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APCGPyramidSystemActor();

	/**
	 * Gets all the adjacent actors of PyramidActor.
	 * @param PyramidGridInterface the pyramid grid to get the adjacents.
	 * @param OutAdjacentPyramidGrids return an array of adjacent grids.
	 */
	UFUNCTION(BlueprintCallable)
	void GetAllAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<TScriptInterface<IPCGPyramidGridInterface>>& OutAdjacentPyramidGrids) const;

	/**
	 * Gets all the adjacent actors of a grid location.
	 * @param GridLocation the grid location to get the adjacents.
	 * @param OutAdjacentPyramidGrids return an array of adjacent grids.
	 */
	UFUNCTION(BlueprintCallable)
	void GetAllAdjacentActorsOfGridLocation(const FVector2D& GridLocation, TArray<TScriptInterface<IPCGPyramidGridInterface>>& OutAdjacentPyramidGrids) const;

	/**
	 * Gets a single adjacent actors of a grid location.
	 * @param PyramidGridInterface the pyramid grid to get the adjacents.
	 * @param OutAdjacentPyramidActors return an array of adjacent grids.
	 */
	UFUNCTION(BlueprintCallable)
	void GetAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<AActor*>& OutAdjacentPyramidActors) const;

	/**
	 * Gets a pyramid block interface from a location.
	 * @param GridLocation the grid location to get the pyramid interface.
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IPCGPyramidGridInterface> GetPyramidBlockActorFromLocation(const FVector2D& GridLocation) const;

	/**
	 * Destroy all adjacent actors in a location.
	 * @param GridLocation the grid location to found and destroy the adjacent actors.
	 * @note Only valid on Server.
	 * @return The number of actors that was destroyed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	int32 DestroyAllAdjacentActorsInLocation(const FVector2D& GridLocation);

	/**
	 * Find nearest upper block from a location.
	 * @param GridLocation the grid location to found the upper block.
	 * @return the nearest upper block.
	 */
	UFUNCTION(BlueprintCallable)
	TScriptInterface<IPCGPyramidGridInterface> FindNearestUpperBlockFromLocation(const FVector2D& GridLocation) const;

	void GetAllAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const;
	void GetAllAdjacentActorsOfGridLocation(const FVector2D& GridLocation, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const;

	/** Triggers when all the grids was destroyed. */
	UPROPERTY(BlueprintAssignable)
	FPCGOnPyramidSystemSignature OnGridsDestroyedDelegate;
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (RowType = "PCGPyramidDefinitionRow"))
	FDataTableRowHandle PyramidDefinition;
private:
	void GeneratePyramidGird();

	void GetAllAdjacentActorsInternal(const FVector2D& GridLocation, const uint8 ColorID, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const;
	void ForEachGridPoints(const FVector2D& GridLocation, const uint8 ColorID, TFunctionRef<void(IPCGPyramidGridInterface* PyramidGrid)> Predicate) const;
	void GetAdjacentActorsOfGridLocationInternal(const FVector2D& GridLocation, const uint8 ColorID, TArray<AActor*>& OutAdjacentPyramidActors) const;
	void FinishGridRelocations();
};
