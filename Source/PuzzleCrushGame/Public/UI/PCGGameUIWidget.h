// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PCGGameUIWidget.generated.h"

class UScrollBox;
class APCGPlayerController;
class APCGGameState;
class APCGPlayerState;
class UTextBlock;
class UWidgetSwitcher;

/**
 * Base class of the Game UI.
 */
UCLASS(meta = (DisableNativeTick))
class PUZZLECRUSHGAME_API UPCGGameUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void ShowUIIndex(const int32 SwitcherIndex);

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
private:

	UFUNCTION()
	void OnScoreChangedCallback(const float NewScore);
	
	UFUNCTION()
	void OnMatchEndedCallback();

	UFUNCTION()
	void OnPlayerStateWasSetCallback();
	
	UFUNCTION()
	void OnRemainingTimeChangedCallback();

	void InitializeGameFrameworkClasses();

	void InitializePlayerName();

	void ShowPlayersScore();
	
	UPROPERTY(Transient)
	APCGPlayerController* PCGPlayerController;

	UPROPERTY(Transient)
	APCGPlayerState* PCGPlayerState;

	UPROPERTY(Transient)
	APCGGameState* PCGGameState;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> GameUISwitcher;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerScoreText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RemainTimeText;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> PlayersListScrollBox;

	UPROPERTY(Transient)
	FTimerHandle RemainingTimeTimerHandle;
};
