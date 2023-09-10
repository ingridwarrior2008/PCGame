// Fill out your copyright notice in the Description page of Project Settings.


#include "General/PCGStatics.h"

//----------------------------------------------------------------------------------------------------------------------

float UPCGStatics::CalculateFibonacciSequence(const int32 Sequence)
{
	int32 FirstSequence = 0;
	int32 SecondSequence = 1;
	int32 ResultInSequence = 0;

	for (int32 SequenceIndex = 1; SequenceIndex <= Sequence; ++SequenceIndex)
	{
		if (SequenceIndex == 1)
		{
			ResultInSequence = 1;
		}
		else
		{
			ResultInSequence = FirstSequence + SecondSequence;
			FirstSequence = SecondSequence;
			SecondSequence = ResultInSequence;
		}
	}
	return ResultInSequence;
}
//----------------------------------------------------------------------------------------------------------------------
