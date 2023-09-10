// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCGHUD.h"

#include "Blueprint/UserWidget.h"
#include "GameFramework/PCGPlayerController.h"
#include "UI/PCGGameUIWidget.h"

//----------------------------------------------------------------------------------------------------------------------

void APCGHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PCGPlayerController = Cast<APCGPlayerController>(GetOwner());
}

//----------------------------------------------------------------------------------------------------------------------

void APCGHUD::CreateUserHUD()
{
	if (ensureMsgf(GameUIWidgetClass, TEXT("GameUIWidgetClass should not be null")))
	{
		GameUIWidget = CreateWidget<UPCGGameUIWidget>(PCGPlayerController, GameUIWidgetClass);
		if (IsValid(GameUIWidget))
		{
			GameUIWidget->AddToViewport();
		}
	}
	
	ShowUIState(EPCGHudState::InGame);
}

//----------------------------------------------------------------------------------------------------------------------

void APCGHUD::ShowUIState(const EPCGHudState HudState)
{
	if (IsValid(GameUIWidget))
	{
		GameUIWidget->ShowUIIndex((uint8)HudState);
	}
}
//----------------------------------------------------------------------------------------------------------------------
