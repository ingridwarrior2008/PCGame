// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/PCGPlayerState.h"

#include "General/PCGStatics.h"

//----------------------------------------------------------------------------------------------------------------------

void APCGPlayerState::SetPlayerScore(const int32 NumScoreSequence)
{
	if (HasAuthority())
	{
		const float FibonacciSequence = UPCGStatics::CalculateFibonacciSequence(NumScoreSequence);
		const int32 CurrentScore = GetScore() + FibonacciSequence;
		SetScore(CurrentScore);
		OnRep_Score();
	}
}

//----------------------------------------------------------------------------------------------------------------------

void APCGPlayerState::OnRep_Score()
{
	Super::OnRep_Score();
	
	OnScoreChangedDelegate.Broadcast(GetScore());
}
