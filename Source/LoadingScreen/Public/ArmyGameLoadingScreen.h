// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#ifndef __ARMYGAMELOADINGSCREEN_H__
#define __ARMYGAMELOADINGSCREEN_H__

#include "ModuleInterface.h"


/** Module interface for this game's loading screens */
class IArmyGameLoadingScreenModule : public IModuleInterface
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartInGameLoadingScreen() = 0;
};

#endif // __ARMYGAMELOADINGSCREEN_H__
