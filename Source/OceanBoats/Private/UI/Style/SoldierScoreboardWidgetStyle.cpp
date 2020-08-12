// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SoldierScoreboardWidgetStyle.h"

FSoldierScoreboardStyle::FSoldierScoreboardStyle()
{
}

FSoldierScoreboardStyle::~FSoldierScoreboardStyle()
{
}

const FName FSoldierScoreboardStyle::TypeName(TEXT("FSoldierScoreboardStyle"));

const FSoldierScoreboardStyle& FSoldierScoreboardStyle::GetDefault()
{
	static FSoldierScoreboardStyle Default;
	return Default;
}

void FSoldierScoreboardStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	OutBrushes.Add(&ItemBorderBrush);
	OutBrushes.Add(&ItemButton);
	OutBrushes.Add(&ItemBackGround);
	OutBrushes.Add(&ItemBeginButton);
	OutBrushes.Add(&ItemPauseButton);
	OutBrushes.Add(&ItemEndButton);
}


USoldierScoreboardWidgetStyle::USoldierScoreboardWidgetStyle( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
	
}
