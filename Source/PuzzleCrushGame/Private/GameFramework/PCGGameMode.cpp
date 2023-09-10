// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PCGGameMode.h"

#include "GameFramework/PCGPlayerController.h"

//----------------------------------------------------------------------------------------------------------------------

APCGGameMode::APCGGameMode()
{

}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameMode::CallEndMatch()
{
	EndMatch();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGGameMode::CallRestartMatch()
{
	RestartGame();
}

//----------------------------------------------------------------------------------------------------------------------
