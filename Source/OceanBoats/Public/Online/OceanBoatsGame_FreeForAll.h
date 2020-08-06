// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "OceanBoatsGame_FreeForAll.generated.h"

class ASoldierPlayerState;

UCLASS()
class AOceanBoatsGame_FreeForAll : public AOceanBoatsGameMode
{
	GENERATED_BODY( )
public:
		AOceanBoatsGame_FreeForAll( );
protected:

	/** best player */
	UPROPERTY(transient)
	ASoldierPlayerState* WinnerPlayerState;

	/** check who won */
	virtual void DetermineMatchWinner() override;

	/** check if PlayerState is a winner */
	virtual bool IsWinner(ASoldierPlayerState* PlayerState) const override;
};
