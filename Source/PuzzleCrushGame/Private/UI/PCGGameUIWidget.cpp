// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PCGGameUIWidget.h"

#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PCGGameState.h"
#include "GameFramework/PCGPlayerController.h"
#include "GameFramework/PCGPlayerState.h"
#include "GameFramework/PlayerState.h"
#include "UI/PCGHUD.h"

#define LOCTEXT_NAMESPACE "GameUIWidget"

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PCGPlayerController = Cast<APCGPlayerController>(GetOwningPlayer());

	if (IsValid(PCGPlayerController))
	{
		PCGPlayerController->OnPlayerStateWasSetDelegate.AddUniqueDynamic(this, &UPCGGameUIWidget::OnPlayerStateWasSetCallback);
	}

	InitializeGameFrameworkClasses();
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::NativeDestruct()
{
	Super::NativeDestruct();
	GetWorld()->GetTimerManager().ClearTimer(RemainingTimeTimerHandle);
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::OnScoreChangedCallback(const float NewScore)
{
	PlayerScoreText->SetText(FText::AsNumber(NewScore));
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::OnMatchEndedCallback()
{
	ShowUIIndex((uint8)EPCGHudState::EndGame);
	ShowPlayersScore();
	GetWorld()->GetTimerManager().SetTimer(RemainingTimeTimerHandle, this, &UPCGGameUIWidget::OnRemainingTimeChangedCallback, 0.1f, true);
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::OnPlayerStateWasSetCallback()
{
	InitializeGameFrameworkClasses();
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::ShowPlayersScore()
{
	if (IsValid(PCGGameState))
	{
		PlayersListScrollBox->ClearChildren();
		
		TArray<APlayerState*> PlayerArray = PCGGameState->PlayerArray;
		
		PlayerArray.Sort([](const APlayerState& PlayerStateRHS, const APlayerState& PlayerStateLHS)
		{
			return PlayerStateRHS.GetScore() > PlayerStateLHS.GetScore();
		});
		
		
		for (const APlayerState* PlayerState : PlayerArray)
		{
			if (!IsValid(PlayerState))
			{
				continue;
			}
			
			UTextBlock* PlayerInfoText = NewObject<UTextBlock>(PlayersListScrollBox, UTextBlock::StaticClass());

			FFormatNamedArguments Args;
			Args.Add(TEXT("Name"),FText::FromString(PlayerState->GetPlayerName()));
			Args.Add(TEXT("Score"), FText::AsNumber(PlayerState->GetScore()));
			Args.Add(TEXT("CurrentPlayer"), PlayerState->GetPlayerId() == PCGPlayerState->GetPlayerId()
												? FText::FromString(TEXT("(YOU)"))
												: FText::FromString(TEXT("")));
			
			PlayerInfoText->SetText(FText::Format(LOCTEXT("PlayerMath", "{Name} score: {Score} {CurrentPlayer}"), Args));
			PlayerInfoText->SetJustification(ETextJustify::Center);
			PlayersListScrollBox->AddChild(PlayerInfoText);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::OnRemainingTimeChangedCallback()
{
	if (IsValid(PCGGameState))
	{
		RemainTimeText->SetText(FText::AsNumber(PCGGameState->GetRemainingTimeToRestartMatch()));
		if (PCGGameState->GetRemainingTimeToRestartMatch() <= 0)
		{
			GetWorld()->GetTimerManager().ClearTimer(RemainingTimeTimerHandle);
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::InitializeGameFrameworkClasses()
{
	PCGPlayerState = IsValid(PCGPlayerController) ? PCGPlayerController->GetPlayerState<APCGPlayerState>() : nullptr;
	if (IsValid(PCGPlayerState))
	{
		PCGPlayerState->OnScoreChangedDelegate.AddUniqueDynamic(this, &UPCGGameUIWidget::OnScoreChangedCallback);
	}

	PCGGameState = GetWorld()->GetGameState<APCGGameState>();
	if (IsValid(PCGGameState))
	{
		PCGGameState->OnMatchEndedDelegate.AddUniqueDynamic(this, &UPCGGameUIWidget::OnMatchEndedCallback);
	}

	InitializePlayerName();
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::InitializePlayerName()
{
	if (IsValid(PCGPlayerState))
	{
		PlayerNameText->SetText(FText::FromString((PCGPlayerState->GetPlayerName())));
	}
}

//----------------------------------------------------------------------------------------------------------------------

void UPCGGameUIWidget::ShowUIIndex(const int32 SwitcherIndex)
{
	GameUISwitcher->SetActiveWidgetIndex(SwitcherIndex);
}

//----------------------------------------------------------------------------------------------------------------------

#undef LOCTEXT_NAMESPACE