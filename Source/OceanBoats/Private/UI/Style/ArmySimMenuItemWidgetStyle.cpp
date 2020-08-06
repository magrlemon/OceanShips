// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArmySimMenuItemWidgetStyle.h"
#include "OceanBoats.h"

FArmySimMenuItemStyle::FArmySimMenuItemStyle()
{
}

FArmySimMenuItemStyle::~FArmySimMenuItemStyle()
{
}

const FName FArmySimMenuItemStyle::TypeName(TEXT("FArmySimMenuItemStyle"));

const FArmySimMenuItemStyle& FArmySimMenuItemStyle::GetDefault()
{
	static FArmySimMenuItemStyle Default;
	return Default;
}

void FArmySimMenuItemStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&BackgroundBrush);
	OutBrushes.Add(&LeftArrowImage);
	OutBrushes.Add(&RightArrowImage);
}


USimMenuItemWidgetStyle::USimMenuItemWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
