// Fill out your copyright notice in the Description page of Project Settings.


#include "Debug/PCGCheatManager.h"

#include "General/PCGStatics.h"
#include "Kismet/GameplayStatics.h"
#include "Pyramid/PCGPyramidBlockActor.h"
#include "Pyramid/PCGPyramidSystemActor.h"

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::Server_PrintPyramidAdjacentBlockInLocation(const float InX, const float InY)
{
	const APlayerController* PlayerController = GetOuterAPlayerController();
	if (IsValid(PlayerController) && PlayerController->HasAuthority())
	{
		PrintPyramidAdjacentBlockInLocationInternal(InX, InY);
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::PrintPyramidAdjacentBlockInLocation(const float InX, const float InY)
{
	PrintPyramidAdjacentBlockInLocationInternal(InX, InY);
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::PrintFibonacciScoreSequence(const float Sequence)
{
	const float FibonacciSequence = UPCGStatics::CalculateFibonacciSequence(Sequence);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("FibonacciSequence %f "), FibonacciSequence));
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::Server_DestroyBlockInLocation(const float InX, const float InY)
{
	const APlayerController* PlayerController = GetOuterAPlayerController();
	if (IsValid(PlayerController) && PlayerController->HasAuthority())
	{
		AActor* Actor = UGameplayStatics::GetActorOfClass(this, APCGPyramidSystemActor::StaticClass());
		APCGPyramidSystemActor* PyramidSystemActor = Cast<APCGPyramidSystemActor>(Actor);
		if (IsValid(PyramidSystemActor))
		{
			const FVector2D& GridLocation = FVector2D(InX, InY);
			PyramidSystemActor->DestroyAllAdjacentActorsInLocation(GridLocation);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::Server_FindNearestUpperBlockFromLocation(const float InX, const float InY)
{
	const APlayerController* PlayerController = GetOuterAPlayerController();
	if (IsValid(PlayerController) && PlayerController->HasAuthority())
	{
		AActor* Actor = UGameplayStatics::GetActorOfClass(this, APCGPyramidSystemActor::StaticClass());
		const APCGPyramidSystemActor* PyramidSystemActor = Cast<APCGPyramidSystemActor>(Actor);
		if (IsValid(PyramidSystemActor))
		{
			const TScriptInterface<IPCGPyramidGridInterface> PyramidBlockActor = PyramidSystemActor->FindNearestUpperBlockFromLocation(FVector2D(InX, InY));
			if (IsValid(PyramidBlockActor.GetObject()))
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Best Nearst Upper location: \'%s\'"), *PyramidBlockActor->GetGridLocation().ToString()));
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGCheatManager::PrintPyramidAdjacentBlockInLocationInternal(const float InX, const float InY)
{
	const FVector2D& GridLocation = FVector2D(InX, InY);
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Find Adjacent sequence for Location: %s "), *GridLocation.ToString()));
		
	AActor* Actor = UGameplayStatics::GetActorOfClass(this, APCGPyramidSystemActor::StaticClass());
	const APCGPyramidSystemActor* PyramidSystemActor = Cast<APCGPyramidSystemActor>(Actor);
	if (IsValid(PyramidSystemActor))
	{
		TArray<IPCGPyramidGridInterface*>InActors;
		PyramidSystemActor->GetAllAdjacentActorsOfGridLocation(GridLocation, InActors);
		for (const IPCGPyramidGridInterface* PyramidBlockActor : InActors)
		{
			if (PyramidBlockActor)
			{
				UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("Adjance location: \'%s\'"), *PyramidBlockActor->GetGridLocation().ToString()));
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
