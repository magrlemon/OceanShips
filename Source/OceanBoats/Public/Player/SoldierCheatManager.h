// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SoldierCheatManager.generated.h"

UCLASS(Within=SoldierPlayerController)
class USoldierCheatManager : public UCheatManager
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(exec)
	void ToggleInfiniteAmmo();

	UFUNCTION(exec)
	void ToggleInfiniteClip();

	UFUNCTION(exec)
	void ToggleMatchTimer();

	UFUNCTION(exec)
	void ForceMatchStart();

	UFUNCTION(exec)
	void SpawnBot();
};
