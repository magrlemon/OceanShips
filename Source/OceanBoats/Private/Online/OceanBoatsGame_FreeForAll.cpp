// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "OceanBoatsGame_FreeForAll.h"
#include "OceanBoats.h"
#include "SoldierPlayerState.h"

AOceanBoatsGame_FreeForAll::AOceanBoatsGame_FreeForAll() 
{
	bDelayedStart = true;
}

void AOceanBoatsGame_FreeForAll::DetermineMatchWinner()
{
	ASoldierGameState const* const MyGameState = CastChecked<ASoldierGameState>(GameState);
	float BestScore = MIN_flt;
	int32 BestPlayer = -1;
	int32 NumBestPlayers = 0;

	for (int32 i = 0; i < MyGameState->PlayerArray.Num(); i++)
	{
		const float PlayerScore = MyGameState->PlayerArray[i]->Score;
		if (BestScore < PlayerScore)
		{
			BestScore = PlayerScore;
			BestPlayer = i;
			NumBestPlayers = 1;
		}
		else if (BestScore == PlayerScore)
		{
			NumBestPlayers++;
		}
	}

	WinnerPlayerState = (NumBestPlayers == 1) ? Cast<ASoldierPlayerState>(MyGameState->PlayerArray[BestPlayer]) : NULL;
}

bool AOceanBoatsGame_FreeForAll::IsWinner(ASoldierPlayerState* PlayerState) const
{
	return PlayerState && !PlayerState->IsQuitter() && PlayerState == WinnerPlayerState;
}
