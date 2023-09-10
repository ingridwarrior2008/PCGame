// Fill out your copyright notice in the Description page of Project Settings.


#include "Pyramid/PCGPyramidSystemActor.h"

#include "Debug/PCGLogChannels.h"
#include "Pyramid/PCGPyramidBlockActor.h"
#include "Pyramid/PCGPyramidDefinition.h"

//----------------------------------------------------------------------------------------------------------------------

APCGPyramidSystemActor::APCGPyramidSystemActor()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAllAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<TScriptInterface<IPCGPyramidGridInterface>>& OutAdjacentPyramidGrids) const
{
	TArray<IPCGPyramidGridInterface*>InActors;
	GetAllAdjacentActorsOfPyramidActor(PyramidGridInterface, InActors);
	for (const IPCGPyramidGridInterface* AdjacentPyramidInterface : InActors)
	{
		const TScriptInterface<IPCGPyramidGridInterface>& AdjacentActorOfGridLocation (AdjacentPyramidInterface->GetGridRawActor());
		OutAdjacentPyramidGrids.Add(AdjacentActorOfGridLocation);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAllAdjacentActorsOfGridLocation(const FVector2D& GridLocation, TArray<TScriptInterface<IPCGPyramidGridInterface>>& OutAdjacentPyramidGrids) const
{
	TArray<IPCGPyramidGridInterface*>InActors;
	GetAllAdjacentActorsOfGridLocation(GridLocation, InActors);
	for (const IPCGPyramidGridInterface* AdjacentPyramidInterface : InActors)
	{
		TScriptInterface<IPCGPyramidGridInterface> AdjacentActorOfGridLocation (AdjacentPyramidInterface->GetGridRawActor());
		OutAdjacentPyramidGrids.Add(AdjacentActorOfGridLocation);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAllAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const
{
	if (IsValid(PyramidGridInterface.GetObject()))
	{
		GetAllAdjacentActorsInternal(PyramidGridInterface->GetGridLocation(), PyramidGridInterface->GetGridColorID(), OutAdjacentPyramidGrids);	
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAllAdjacentActorsOfGridLocation(const FVector2D& GridLocation, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const
{
	const TScriptInterface<IPCGPyramidGridInterface> PyramidGridInterface = GetPyramidBlockActorFromLocation(GridLocation);
	if (PyramidGridInterface.GetInterface())
	{
		GetAllAdjacentActorsInternal(GridLocation, PyramidGridInterface->GetGridColorID(), OutAdjacentPyramidGrids);	
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAdjacentActorsOfPyramidActor(const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface, TArray<AActor*>& OutAdjacentPyramidActors) const
{
	if (IsValid(PyramidGridInterface.GetObject()))
	{
		GetAdjacentActorsOfGridLocationInternal(PyramidGridInterface->GetGridLocation(), PyramidGridInterface->GetGridColorID(), OutAdjacentPyramidActors);	
	}
}

//----------------------------------------------------------------------------------------------------------------------

TScriptInterface<IPCGPyramidGridInterface> APCGPyramidSystemActor::GetPyramidBlockActorFromLocation(const FVector2D& GridLocation) const
{
	const TObjectPtr<AActor>* FoundActor = Children.FindByPredicate([&GridLocation](const AActor* OtherActor)
	{
		const IPCGPyramidGridInterface* PyramidGridInterface = Cast<IPCGPyramidGridInterface>(OtherActor);
		return PyramidGridInterface ? PyramidGridInterface->GetGridLocation() == GridLocation : false;
	});

	TScriptInterface<IPCGPyramidGridInterface> PyramidGridInterface;
	if (FoundActor && *FoundActor)
	{
		PyramidGridInterface = TScriptInterface<IPCGPyramidGridInterface>(*FoundActor);
	}
	return PyramidGridInterface;
}

//----------------------------------------------------------------------------------------------------------------------

int32 APCGPyramidSystemActor::DestroyAllAdjacentActorsInLocation(const FVector2D& GridLocation)
{
	int32 TotalOfDestroyedGrids = 0;
	const TScriptInterface<IPCGPyramidGridInterface>& PyramidGridInterface = GetPyramidBlockActorFromLocation(GridLocation);
	if (IsValid(PyramidGridInterface.GetObject()))
	{
		TArray<IPCGPyramidGridInterface*> AdjacentPyramidActors;
		GetAllAdjacentActorsInternal(GridLocation, PyramidGridInterface->GetGridColorID(), AdjacentPyramidActors);
		if (AdjacentPyramidActors.IsEmpty())
		{
			AdjacentPyramidActors.AddUnique(PyramidGridInterface.GetInterface());
		}
		
		TotalOfDestroyedGrids = AdjacentPyramidActors.Num();
		
		for (IPCGPyramidGridInterface* AdjacentPyramidInterface : AdjacentPyramidActors)
		{
			if (AdjacentPyramidInterface)
			{
				AdjacentPyramidInterface->SetIsPendingDelete(true);
			}
		}
		
		for (IPCGPyramidGridInterface* AdjacentPyramidInterface : AdjacentPyramidActors)
		{
			if (AdjacentPyramidInterface)
			{
				AdjacentPyramidInterface->CallToDestroyGrid();
			}
		}
		
		FinishGridRelocations();
	}

	return TotalOfDestroyedGrids;
}

TScriptInterface<IPCGPyramidGridInterface> APCGPyramidSystemActor::FindNearestUpperBlockFromLocation(const FVector2D& GridLocation) const
{
	const int32 UpperGridRow = GridLocation.Y - 1;
	TScriptInterface<IPCGPyramidGridInterface> BestGridInterface;
	if (UpperGridRow > 0)
	{
		double BestDistance = MAX_dbl;
		for (int32  Row = UpperGridRow;  Row>= 0; --Row)
		{
			for (int32  Column = 0;  Column <= UpperGridRow; Column++)
			{
				TScriptInterface<IPCGPyramidGridInterface> PyramidGridInterface = GetPyramidBlockActorFromLocation(FVector2D(Column, Row));
				if (IsValid(PyramidGridInterface.GetObject()))
				{
					if (PyramidGridInterface->IsPendingDelete() || PyramidGridInterface->IsBeingRelocated())
					{
						continue;
					}
					
					if (Row == UpperGridRow)
					{
						if (!BestGridInterface.GetObject())
						{
							BestGridInterface = PyramidGridInterface;
						}
						
						const float CurrentDist = FMath::Abs(GridLocation.X - Column);
						if (CurrentDist < BestDistance && GetPyramidBlockActorFromLocation(FVector2D(Column, UpperGridRow)).GetObject())
						{
							BestDistance = CurrentDist;
							BestGridInterface = GetPyramidBlockActorFromLocation(FVector2D(Column, UpperGridRow));
						}
					}
					else if (BestGridInterface.GetObject())
					{
						break;
					}
					else
					{
						if (!IsValid(BestGridInterface.GetObject()))
						{
							BestGridInterface = PyramidGridInterface;
						}
						const float CurrentDist = FMath::Abs(GridLocation.X - Column);
						if (CurrentDist < BestDistance && GetPyramidBlockActorFromLocation(FVector2D(Column, Row)).GetObject())
						{
							BestDistance = CurrentDist;
							BestGridInterface = GetPyramidBlockActorFromLocation(FVector2D(Column, Row));
						}
					}
				}
			}
		}
	}
	
	return BestGridInterface;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		GeneratePyramidGird();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GeneratePyramidGird()
{
	if (!IsValid(GetWorld()))
	{
		return;
	}
	
	if (PyramidDefinition.IsNull())
	{
		ensureMsgf(false, TEXT("PyramidDefinition is not been selected in BP"));
		return;
	}

	const FPCGPyramidDefinitionRow* PyramidDefinitionRowPtr = PyramidDefinition.GetRow<FPCGPyramidDefinitionRow>(TEXT(""));
	if (!PyramidDefinitionRowPtr || !PyramidDefinitionRowPtr->PyramidBlockActorClass)
	{
		UE_LOG(LogPCG, Warning, TEXT("PyramidDefinitionRowPtr or PyramidBlockActorClass are null"));
		return;
	}

	const TArray<FDataTableRowHandle>& GridColors = PyramidDefinitionRowPtr->PyramidColorDefinitionArray;
	float PyramidGridRowSize = 0.0f;
	float PyramidGridColumnSize = 0.0f;
	float PyramidGridInitialColumnPosition = 0.0f;
	FVector LastGridBoxExtent;
	int32 CurrentPyramidColumn = PyramidDefinitionRowPtr->MaxPyramidRow - 1;
	
	for (int GridRowIndex = PyramidDefinitionRowPtr->MaxPyramidRow - 1; GridRowIndex >= 0; GridRowIndex--)
	{
		for (int GridColumIndex = 0; GridColumIndex <= CurrentPyramidColumn; GridColumIndex++)
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnInfo.Owner = this;
			
			const FVector& SystemLocation = GetActorLocation();
			const FVector& PyramidGridLocation = FVector(SystemLocation.X, SystemLocation.Y + PyramidGridColumnSize, SystemLocation.Z + PyramidGridRowSize);
			AActor* PyramidActor = GetWorld()->SpawnActor<AActor>(PyramidDefinitionRowPtr->PyramidBlockActorClass, PyramidGridLocation, FRotator::ZeroRotator, SpawnInfo);
			IPCGPyramidGridInterface* PyramidActorInterface = Cast<IPCGPyramidGridInterface>(PyramidActor);
			
			if (!IsValid(PyramidActor) || !PyramidActorInterface)
			{
				continue;
			}

			if (!PyramidActor->Implements<UPCGActorSizeInterface>())
			{
				ensureMsgf(false, TEXT("PyramidBlockActor should implements UPCGPyramidGridInterface and UPCGActorSizeInterface."));
				continue;
			}
			
			if (GridColors.Num() > 0)
			{
				const int32 RandomColorIndex = FMath::RandRange(0, GridColors.Num() - 1);
				PyramidActorInterface->SetGridColor(GridColors[RandomColorIndex]);
			}
			
			PyramidActorInterface->SetGridLocation(GridColumIndex, GridRowIndex);
			PyramidActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			const FVector& PyramidBlockBoxExtent = IPCGActorSizeInterface::Execute_BP_GetBoxExtent(PyramidActor);
			
			if (GridColumIndex == 0)
			{
				PyramidGridInitialColumnPosition = PyramidGridInitialColumnPosition + PyramidBlockBoxExtent.Y;
			}

			PyramidGridColumnSize = PyramidGridColumnSize + (PyramidBlockBoxExtent.Y * 2);

			if (GridColumIndex == CurrentPyramidColumn - 1)
			{
				LastGridBoxExtent = PyramidBlockBoxExtent;
			}
		}

		PyramidGridColumnSize = PyramidGridInitialColumnPosition;
		PyramidGridRowSize = PyramidGridRowSize + (LastGridBoxExtent.Z * 2);
		CurrentPyramidColumn = --CurrentPyramidColumn;
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAllAdjacentActorsInternal(const FVector2D& GridLocation, const uint8 ColorID, TArray<IPCGPyramidGridInterface*>& OutAdjacentPyramidGrids) const
{
	TSet<IPCGPyramidGridInterface*> AdjacentPyramidActors;
	TMap<FVector2D, bool> GridLocationsViewed = { {GridLocation, false} };
	while (GridLocationsViewed.FindKey(false))
	{
		if (const FVector2D* GridLocationLast = GridLocationsViewed.FindKey(false))
		{
			GridLocationsViewed[*GridLocationLast] = true;

			ForEachGridPoints(*GridLocationLast, ColorID, [&AdjacentPyramidActors, &GridLocationsViewed](IPCGPyramidGridInterface* GridActor)
			{
				if (GridActor)
				{
					if(!GridLocationsViewed.Contains(GridActor->GetGridLocation()))
					{
						GridLocationsViewed.Add(GridActor->GetGridLocation(), false);
					}
					AdjacentPyramidActors.Add(GridActor);
				}
			});
		}
	}
	OutAdjacentPyramidGrids = AdjacentPyramidActors.Array();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::ForEachGridPoints(const FVector2D& GridLocation, const uint8 ColorID, const TFunctionRef<void(IPCGPyramidGridInterface* PyramidGrid)> Predicate) const
{
	TArray<AActor*> AdjacentPyramidActors;
	GetAdjacentActorsOfGridLocationInternal(GridLocation, ColorID, AdjacentPyramidActors);
	for (AActor* AdjacentPyramidActor : AdjacentPyramidActors)
	{
		IPCGPyramidGridInterface* PyramidGridInterface = Cast<IPCGPyramidGridInterface>(AdjacentPyramidActor);
		if (!PyramidGridInterface)
		{
			continue;	
		}

		Predicate(PyramidGridInterface);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::GetAdjacentActorsOfGridLocationInternal(const FVector2D& GridLocation, const uint8 ColorID, TArray<AActor*>& OutAdjacentPyramidActors) const
{
	OutAdjacentPyramidActors= Children.FilterByPredicate([&GridLocation, &ColorID](const AActor* OtherChildren)
	{
		const IPCGPyramidGridInterface* PyramidGridInterface = Cast<IPCGPyramidGridInterface>(OtherChildren);
		return PyramidGridInterface ? PyramidGridInterface->IsAdjacentLocation(GridLocation, ColorID) : false;
	});
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPyramidSystemActor::FinishGridRelocations()
{
	TArray<AActor*> BeingRelocatedActors = Children.FilterByPredicate([](const AActor* OtherChildren)
	{
		const IPCGPyramidGridInterface* PyramidGridInterface = Cast<IPCGPyramidGridInterface>(OtherChildren);
		return PyramidGridInterface ? PyramidGridInterface->IsBeingRelocated() : false;
	});
	
	for (AActor* RelocatedActor : BeingRelocatedActors)
	{
		if (IPCGPyramidGridInterface* PyramidGridInterface = Cast<IPCGPyramidGridInterface>(RelocatedActor))
		{
			PyramidGridInterface->SetIsBeingRelocated(false);
		}
	}

	if (Children.Num() == 0)
	{
		OnGridsDestroyedDelegate.Broadcast(this);
	}
}

//----------------------------------------------------------------------------------------------------------------------
