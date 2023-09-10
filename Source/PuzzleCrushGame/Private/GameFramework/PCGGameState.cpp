// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PCGGameState.h"

#include "EngineUtils.h"
#include "GameFramework/PCGGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Pyramid/PCGPyramidSystemActor.h"

//----------------------------------------------------------------------------------------------------------------------

APCGGameState::APCGGameState()
{
	InitialRemainingTime = 10;
	RemainingTimeToRestartMatch = InitialRemainingTime;
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	InitializePyramidSystems();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APCGGameState, RemainingTimeToRestartMatch);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::HandleMatchHasEnded()
{
	Super::HandleMatchHasEnded();
	
	OnMatchEndedDelegate.Broadcast();

	if (HasAuthority())
	{
		InitializeRestartMatchCountdown();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::InitializePyramidSystems()
{
	UWorld* World = GetWorld();
	if (HasAuthority() && World)
	{
		for (TActorIterator<APCGPyramidSystemActor> It(World); It; ++It)
		{
			APCGPyramidSystemActor* PyramidSystemActor = *It;
			if (IsValid(PyramidSystemActor))
			{
				PyramidSystemActorArray.AddUnique(PyramidSystemActor);
				PyramidSystemActor->OnGridsDestroyedDelegate.AddDynamic(this, &APCGGameState::OnGridsDestroyedCallback);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::InitializeRestartMatchCountdown()
{
	GetWorldTimerManager().ClearTimer(RemainingTimeTimerHandle);
	GetWorldTimerManager().SetTimer(RemainingTimeTimerHandle, this, &APCGGameState::RemainingTimeCallback, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::CallRestartMatch()
{
	GetWorldTimerManager().ClearTimer(RemainingTimeTimerHandle);
	APCGGameMode* GameMode = Cast<APCGGameMode>(AuthorityGameMode);
	if (IsValid(GameMode))
	{
		GameMode->CallRestartMatch();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::OnGridsDestroyedCallback(APCGPyramidSystemActor* FinishedPyramidSystemActor)
{
	PyramidSystemActorArray.Remove(FinishedPyramidSystemActor);
	if (PyramidSystemActorArray.IsEmpty())
	{
		APCGGameMode* GameMode = Cast<APCGGameMode>(AuthorityGameMode);
		if (IsValid(GameMode))
		{
			GameMode->CallEndMatch();
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameState::RemainingTimeCallback()
{
	RemainingTimeToRestartMatch--;
	if (RemainingTimeToRestartMatch <= 0)
	{
		CallRestartMatch();
	}
}

//----------------------------------------------------------------------------------------------------------------------
