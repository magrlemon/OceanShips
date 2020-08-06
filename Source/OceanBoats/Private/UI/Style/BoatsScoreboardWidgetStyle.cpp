// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "BoatsScoreboardWidgetStyle.h"

FBoatsScoreboardStyle::FBoatsScoreboardStyle()
{
}

FBoatsScoreboardStyle::~FBoatsScoreboardStyle()
{
}

const FName FBoatsScoreboardStyle::TypeName(TEXT("FBoatsScoreboardStyle"));

const FBoatsScoreboardStyle& FBoatsScoreboardStyle::GetDefault()
{
	static FBoatsScoreboardStyle Default;
	return Default;
}

void FBoatsScoreboardStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&ItemBorderBrush);
}


UBoatsScoreboardWidgetStyle::UBoatsScoreboardWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
