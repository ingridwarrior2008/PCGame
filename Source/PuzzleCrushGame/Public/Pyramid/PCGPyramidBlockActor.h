// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PCGPyramidGridInterface.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "General/PCGActorSizeInterface.h"
#include "General/PCGDestructibleInterface.h"
#include "PCGPyramidBlockActor.generated.h"

class APCGPyramidSystemActor;
class UTextRenderComponent;
class UDataTable;

/**
 * Native base class for pyramid blocks, is only be created by the pyramid system.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGPyramidBlockActor : public AActor, public IPCGDestructibleInterface, public IPCGActorSizeInterface, public IPCGPyramidGridInterface
{
	GENERATED_BODY()
	
public:	
	APCGPyramidBlockActor();

#pragma region DestructibleInterface
	virtual void BP_OnDestroyActor_Implementation(AActor* ActorInstigator) override;
#pragma endregion DestructibleInterface

#pragma region ActorSizeInterface
	virtual FVector BP_GetBoxExtent_Implementation() const override;
#pragma endregion ActorSizeInterface

#pragma region PyramidGridInterface
	virtual void SetGridLocation(const int32 InColumLocation, const int32 InRowLocation) override;
	virtual const FVector2D& GetGridLocation() const override;
	virtual void SetGridColor(const FDataTableRowHandle& InColorDefinition) override;
	virtual uint8 GetGridColorID() const override;
	virtual void ReLocateGridWorldLocation(const FVector& InNewWorldLocation, const FVector2D& InPositionInGrid) override;
	virtual bool IsAdjacentLocation(const FVector2D& InPositionInGrid, const uint8 InColorID) const override;
	virtual void GetAdjacentActors(TArray<AActor*>& OutAdjacentPyramidActors) const override;
	virtual void ForEachAdjacentActors(TFunctionRef<void(AActor* PyramidBlockActor)> Predicate) const override;
	virtual void CallToDestroyGrid() override;
	virtual bool IsBeingRelocated() const override;
	virtual void SetIsBeingRelocated(const bool bNewIsBeingRelocated) override;
	virtual bool IsPendingDelete() const override;
	virtual void SetIsPendingDelete(const bool bNewIsPendingDelete) override;
	virtual FVector GetGridWorldLocation() const override;
	virtual AActor* GetGridRawActor() const override;
#pragma endregion PyramidGridInterface

protected:
	virtual void BeginPlay() override;

	/** Pyramid Colors data table. */
	UPROPERTY(EditDefaultsOnly, Category = "Pyramid")
	TObjectPtr<UDataTable> ColorTable;
	
private:

	UFUNCTION()
	void OnRep_ColorIndex();

	UFUNCTION()
	void OnRep_GridLocation();

	/** Update the block mesh material based in the Color ID. */
	void UpdateBlockMaterialColor();

	/** Update Grid 2D location text. */
	void UpdateGridLocationText();

	/** Pyramid Block Mesh. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pyramid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> PyramidBlockMesh;

	/** PyramidGrid TextRender. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pyramid", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextRenderComponent> PyramidGridTextRender;

	/** A reference of PyramidSystem. */
	UPROPERTY(Transient)
	APCGPyramidSystemActor* PyramidSystemActor;

	/** True, if the pyramid block is being relocated to a new 2D position. */
	UPROPERTY(Transient)
	uint8 bIsBeingRelocated : 1;

	/** True, if the pyramid block was marked to be destroyed. */
	UPROPERTY(Transient)
	uint8 bIsPendingDelete : 1;

	/** The grid location in a 2D space. */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_GridLocation)
	FVector2D GridLocation;

	/** The color ID. */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_ColorIndex)
	uint8 ColorID;

	/** The PyramidGridInterface reference of this actor. */
	UPROPERTY(Transient)
	TScriptInterface<IPCGPyramidGridInterface> ThisGridInterface;
};
