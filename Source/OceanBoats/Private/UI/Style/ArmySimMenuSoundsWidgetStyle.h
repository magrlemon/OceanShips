// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling\SlateWidgetStyleContainerBase.h"
#include "Styling\SlateWidgetStyle.h"
#include "..\..\Engine\Source\Runtime\SlateCore\Public\Styling\SlateWidgetStyleContainerBase.h"
#include "ArmySimMenuSoundsWidgetStyle.generated.h"

/**
 * Represents the common menu sounds used in the shooter game
 */
USTRUCT()
struct FSimMenuSoundsStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FSimMenuSoundsStyle();
	virtual ~FSimMenuSoundsStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FSimMenuSoundsStyle& GetDefault();

	/**
	 * The sound that should play when starting the game
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound StartGameSound;
	FSimMenuSoundsStyle& SetStartGameSound(const FSlateSound& InStartGameSound) { StartGameSound = InStartGameSound; return *this; }

	/**
	 * The sound that should play when exiting the game
	 */	
	UPROPERTY(EditAnywhere, Category=Sound)
	FSlateSound ExitGameSound;
	FSimMenuSoundsStyle& SetExitGameSound(const FSlateSound& InExitGameSound) { ExitGameSound = InExitGameSound; return *this; }
};


/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USimMenuSoundsWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the sounds */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FSimMenuSoundsStyle SoundsStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &SoundsStyle );
	}
};
