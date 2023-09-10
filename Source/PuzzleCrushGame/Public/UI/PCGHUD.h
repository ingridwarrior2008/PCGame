// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PCGHUD.generated.h"

class APCGPlayerController;
class UPCGGameUIWidget;

/** Types of the HUD state */
UENUM()
enum class EPCGHudState : uint8
{
	InGame,
	EndGame,
};


/**
 * Base class of the Player HUD.
 */
UCLASS()
class PUZZLECRUSHGAME_API APCGHUD : public AHUD
{
	GENERATED_BODY()

public:

	/** Creates the user HUD */
	void CreateUserHUD();

	/** Show the UI based on the HUD state */
	void ShowUIState(const EPCGHudState HudState);

protected:
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Widget)
	TSubclassOf<UPCGGameUIWidget> GameUIWidgetClass;
	
private:
	UPROPERTY(Transient)
	UPCGGameUIWidget* GameUIWidget;
	
	UPROPERTY(Transient)
	APCGPlayerController* PCGPlayerController;
};
