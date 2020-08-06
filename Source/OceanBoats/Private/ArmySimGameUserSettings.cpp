// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "ArmySimGameUserSettings.h"

UArmySimGameUserSettings::UArmySimGameUserSettings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetToDefaults();
}

void UArmySimGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	GraphicsQuality = 1;	
	bIsLanMatch = true;
	bIsDedicatedServer = false;
}

void UArmySimGameUserSettings::ApplySettings(bool bCheckForCommandLineOverrides)
{
	if (GraphicsQuality == 0)
	{
		ScalabilityQuality.SetFromSingleQualityLevel(1);
	}
	else
	{
		ScalabilityQuality.SetFromSingleQualityLevel(3);
	}

	Super::ApplySettings(bCheckForCommandLineOverrides);
}