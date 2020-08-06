// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "SoldierGame_Menu.h"
#include "OceanBoats.h"

#include "SoldierMainMenu.h"
#include "SoldierWelcomeMenu.h"
#include "SoldierMessageMenu.h"
#include "SoldierPlayerController_Menu.h"

ASoldierGame_Menu::ASoldierGame_Menu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PlayerControllerClass = ASoldierPlayerController_Menu::StaticClass();
}

void ASoldierGame_Menu::RestartPlayer(class AController* NewPlayer)
{
	// don't restart
}

