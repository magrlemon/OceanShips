// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "ArmySimMenuSoundsWidgetStyle.h"
#include "OceanBoats.h"

FSimMenuSoundsStyle::FSimMenuSoundsStyle()
{
}

FSimMenuSoundsStyle::~FSimMenuSoundsStyle()
{
}

const FName FSimMenuSoundsStyle::TypeName(TEXT("FSimMenuSoundsStyle"));

const FSimMenuSoundsStyle& FSimMenuSoundsStyle::GetDefault()
{
	static FSimMenuSoundsStyle Default;
	return Default;
}

void FSimMenuSoundsStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
}


USimMenuSoundsWidgetStyle::USimMenuSoundsWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
