// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SoldierPlayerState.h"
#include "SoldierGameState.generated.h"

class ASoldierPlayerState;
/** ranked PlayerState map, created from the GameState */
typedef TMap<int32, TWeakObjectPtr<ASoldierPlayerState> > RankedPlayerMap;


UCLASS()
class ASoldierGameState : public AGameState
{
	GENERATED_UCLASS_BODY()

public:

	/** number of teams in current game (doesn't deprecate when no players are left in a team) */
	UPROPERTY(Transient, Replicated)
	int32 NumTeams;

	/** accumulated score per team */
	UPROPERTY(Transient, Replicated)
	TArray<int32> TeamScores;

	/** time left for warmup / match */
	UPROPERTY(Transient, Replicated)
	int32 RemainingTime;

	/** is timer paused? */
	UPROPERTY(Transient, Replicated)
	bool bTimerPaused;

	/** gets ranked PlayerState map for specific team */
	void GetRankedMap(int32 TeamIndex, RankedPlayerMap& OutRankedMap) const;	

	void RequestFinishAndExitToMainMenu();
};
