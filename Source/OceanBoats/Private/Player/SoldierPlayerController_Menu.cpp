// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "Player/SoldierPlayerController_Menu.h"
#include "ArmySimStyle.h"


ASoldierPlayerController_Menu::ASoldierPlayerController_Menu(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void ASoldierPlayerController_Menu::PostInitializeComponents() 
{
	Super::PostInitializeComponents();

	FArmySimStyle::Initialize();
}
