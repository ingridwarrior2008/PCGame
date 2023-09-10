// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PCGPlayerController.h"

#include "Character/PCGCharacter.h"
#include "Debug/PCGCheatManager.h"
#include "UI/PCGHUD.h"

//----------------------------------------------------------------------------------------------------------------------

APCGPlayerController::APCGPlayerController()
{
	CheatClass = UPCGCheatManager::StaticClass();
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPlayerController::AcknowledgePossession(APawn* InPawn)
{
	Super::AcknowledgePossession(InPawn);

	APCGCharacter* PCGCharacter = Cast<APCGCharacter>(InPawn);
	if (IsValid(PCGCharacter))
	{
		PCGCharacter->InitializeInputMapping();
	}

	GameHUD = Cast<APCGHUD>(GetHUD());
	if (IsValid(GameHUD))
	{
		GameHUD->CreateUserHUD();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	OnPlayerStateWasSetDelegate.Broadcast();
}

//----------------------------------------------------------------------------------------------------------------------
