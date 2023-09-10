// Fill out your copyright notice in the Description page of Project Settings.


#include "Pyramid/PCGPyramidBlockActor.h"

#include "Character/PCGCharacter.h"
#include "Components/TextRenderComponent.h"
#include "Engine/DataTable.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"
#include "Pyramid/PCGPyramidDefinition.h"
#include "Pyramid/PCGPyramidSystemActor.h"

constexpr static uint8 PyramidInvalidColorId = 255;
constexpr static float PyramidInvalidGirdLocation = 255.0f;

//----------------------------------------------------------------------------------------------------------------------

APCGPyramidBlockActor::APCGPyramidBlockActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	bIsBeingRelocated = false;
	bIsPendingDelete = false;
	PyramidBlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PyramidBlockMesh"));
	SetRootComponent(PyramidBlockMesh);

	PyramidGridTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PyramidGridTextRender"));
	PyramidGridTextRender->SetupAttachment(PyramidBlockMesh);
	
	ColorID = PyramidInvalidColorId;
	GridLocation = FVector2D(PyramidInvalidGirdLocation, PyramidInvalidGirdLocation);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::BeginPlay()
{
	Super::BeginPlay();
	ThisGridInterface = TScriptInterface<IPCGPyramidGridInterface>(this);
	PyramidSystemActor = Cast<APCGPyramidSystemActor>(GetOwner());
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(APCGPyramidBlockActor, ColorID, COND_InitialOnly)
	DOREPLIFETIME(APCGPyramidBlockActor, GridLocation)
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::OnRep_ColorIndex()
{
	UpdateBlockMaterialColor();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::OnRep_GridLocation()
{
	UpdateGridLocationText();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::UpdateBlockMaterialColor()
{
	if (!ColorTable || !ColorTable->GetRowNames().IsValidIndex(ColorID))
	{
		ensureMsgf(false, TEXT("ColorTable is null or ColorID is an invalid index"));
		return;
	}
	
	const FName ColorName = ColorTable->GetRowNames()[ColorID];
	if (const FPCGPyramidColorDefinitionRow* PyramidColorDefinitionRowPtr = ColorTable->FindRow<FPCGPyramidColorDefinitionRow>(ColorName, TEXT("")))
	{
		if (IsValid(PyramidBlockMesh))
		{
			PyramidBlockMesh->SetMaterial(0, PyramidColorDefinitionRowPtr->PyramidMaterial);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::UpdateGridLocationText()
{
	if (IsValid(PyramidGridTextRender))
	{
		PyramidGridTextRender->SetText(FText::FromString(FString::Printf(TEXT("( %.1f, %.1f )"), GridLocation.X, GridLocation.Y)));
	}
}

//----------------------------------------------------------------------------------------------------------------------

#pragma region DestructibleInterface

void APCGPyramidBlockActor::BP_OnDestroyActor_Implementation(AActor* ActorInstigator)
{
	if (HasAuthority() && IsValid(PyramidSystemActor))
	{
		const int32 NumOfDestroyedGrids =  PyramidSystemActor->DestroyAllAdjacentActorsInLocation(GetGridLocation());
		APCGCharacter* PCGCharacter = Cast<APCGCharacter>(ActorInstigator);
		if (IsValid(PCGCharacter))
		{
			PCGCharacter->SendPlayerScore(NumOfDestroyedGrids);
		}
	}
}

#pragma endregion DestructibleInterface

//----------------------------------------------------------------------------------------------------------------------

#pragma region ActorSizeInterface

FVector APCGPyramidBlockActor::BP_GetBoxExtent_Implementation() const
{
	return IsValid(PyramidBlockMesh) ? PyramidBlockMesh->Bounds.BoxExtent : FVector::ZeroVector;
}

#pragma endregion ActorSizeInterface

//----------------------------------------------------------------------------------------------------------------------

#pragma region PyramidGridInterface

void APCGPyramidBlockActor::SetGridLocation(const int32 InColumLocation, const int32 InRowLocation)
{
	if (HasAuthority())
	{
		GridLocation = FVector2D(InColumLocation, InRowLocation);
		UpdateGridLocationText();
	}
}

//----------------------------------------------------------------------------------------------------------------------

const FVector2D& APCGPyramidBlockActor::GetGridLocation() const
{
	return GridLocation;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::SetGridColor(const FDataTableRowHandle& InColorDefinition)
{
	if (HasAuthority())
	{
		if (!ColorTable)
		{
			ensureMsgf(false, TEXT("ColorTable should not be null"));
			return;
		}
		
		ColorID = ColorTable->GetRowNames().IndexOfByKey(InColorDefinition.RowName);
		ensureMsgf(ColorID != INDEX_NONE, TEXT("ColorIndexFromRow should not be IndexNone"));
		UpdateBlockMaterialColor();
	}
}

//----------------------------------------------------------------------------------------------------------------------

uint8 APCGPyramidBlockActor::GetGridColorID() const
{
	return ColorID;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::ReLocateGridWorldLocation(const FVector& InNewWorldLocation, const FVector2D& InPositionInGrid)
{
	if (HasAuthority())
	{
		SetActorLocation(InNewWorldLocation);
		SetGridLocation(InPositionInGrid.X, InPositionInGrid.Y);
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool APCGPyramidBlockActor::IsAdjacentLocation(const FVector2D& InPositionInGrid, const uint8 InColorID) const
{
	if (InColorID != ColorID)
	{
		return false;
	}
	
	return  InPositionInGrid == GetGridLocation() + FVector2D(0, 1)   ||
			InPositionInGrid == GetGridLocation() + FVector2D(0, -1)  ||
			InPositionInGrid == GetGridLocation() + FVector2D(1, 0)   ||
			InPositionInGrid == GetGridLocation() + FVector2D(1, 1)   ||
			InPositionInGrid == GetGridLocation() + FVector2D(-1, -1) ||
			InPositionInGrid == GetGridLocation() + FVector2D(-1, 0);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::GetAdjacentActors(TArray<AActor*>& OutAdjacentPyramidActors) const
{
	if (IsValid(PyramidSystemActor))
	{
		PyramidSystemActor->GetAdjacentActorsOfPyramidActor(ThisGridInterface, OutAdjacentPyramidActors);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::ForEachAdjacentActors(const TFunctionRef<void(AActor* PyramidBlockActor)> Predicate) const
{
	TArray<AActor*> AdjacentPyramidActors;
	GetAdjacentActors(AdjacentPyramidActors);
	for (AActor* AdjacentPyramidActor : AdjacentPyramidActors)
	{
		Predicate(AdjacentPyramidActor);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::CallToDestroyGrid()
{
	if ( HasAuthority() && IsValid(PyramidSystemActor))
	{
		const TScriptInterface<IPCGPyramidGridInterface>& PyramidBlockInterface = PyramidSystemActor->FindNearestUpperBlockFromLocation(GetGridLocation());
		FVector New3DLocation;
		FVector2D New2DLocation;
		
		if (PyramidBlockInterface.GetObject() && !PyramidBlockInterface->IsBeingRelocated())
		{
			New3DLocation = PyramidBlockInterface->GetGridWorldLocation();
			New2DLocation = PyramidBlockInterface->GetGridLocation();
			PyramidBlockInterface->SetIsBeingRelocated(true);
			PyramidBlockInterface->ReLocateGridWorldLocation(GetActorLocation(), GetGridLocation());
		}
		
		TScriptInterface<IPCGPyramidGridInterface> PyramidBlockActorIT = PyramidBlockInterface;
		while (PyramidBlockActorIT.GetObject() != nullptr)
		{
			const TScriptInterface<IPCGPyramidGridInterface>& SubPyramidBlockInterface = PyramidSystemActor->FindNearestUpperBlockFromLocation(PyramidBlockActorIT->GetGridLocation());
			FVector BeforeNew3DLocation;
			FVector2D BeforeNew2DLocation;
			if (IsValid(SubPyramidBlockInterface.GetObject()) && !SubPyramidBlockInterface->IsBeingRelocated())
			{
				BeforeNew3DLocation = SubPyramidBlockInterface->GetGridWorldLocation();
				BeforeNew2DLocation = SubPyramidBlockInterface->GetGridLocation();
				SubPyramidBlockInterface->SetIsBeingRelocated(true);
				SubPyramidBlockInterface->ReLocateGridWorldLocation(New3DLocation, New2DLocation);
			}
			New3DLocation = BeforeNew3DLocation;
			New2DLocation = BeforeNew2DLocation;
			PyramidBlockActorIT = SubPyramidBlockInterface;
		}
		
		Destroy();
	}
}

//----------------------------------------------------------------------------------------------------------------------

bool APCGPyramidBlockActor::IsBeingRelocated() const
{
	return bIsBeingRelocated;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::SetIsBeingRelocated(const bool bNewIsBeingRelocated)
{
	bIsBeingRelocated = bNewIsBeingRelocated;
}

//----------------------------------------------------------------------------------------------------------------------

bool APCGPyramidBlockActor::IsPendingDelete() const
{
	return bIsPendingDelete;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidBlockActor::SetIsPendingDelete(const bool bNewIsPendingDelete)
{
	bIsPendingDelete = bNewIsPendingDelete;
}

//----------------------------------------------------------------------------------------------------------------------

FVector APCGPyramidBlockActor::GetGridWorldLocation() const
{
	return GetActorLocation();
}

//----------------------------------------------------------------------------------------------------------------------

AActor* APCGPyramidBlockActor::GetGridRawActor() const
{
	return RootComponent->GetOwner();
}

#pragma endregion PyramidGridInterface

//----------------------------------------------------------------------------------------------------------------------



