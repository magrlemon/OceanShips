// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Styling/SlateWidgetStyleContainerBase.h"
#include "ArmySimMenuItemWidgetStyle.generated.h"

/**
 * Represents the appearance of an FSimMenuItem
 */
USTRUCT()
struct FArmySimMenuItemStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FArmySimMenuItemStyle();
	virtual ~FArmySimMenuItemStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FArmySimMenuItemStyle& GetDefault();

	/**
	 * The brush used for the item background
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush BackgroundBrush;
	FArmySimMenuItemStyle& SetBackgroundBrush(const FSlateBrush& InBackgroundBrush) { BackgroundBrush = InBackgroundBrush; return *this; }

	/**
	 * The image used for the left arrow
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush LeftArrowImage;
	FArmySimMenuItemStyle& SetLeftArrowImage(const FSlateBrush& InLeftArrowImage) { LeftArrowImage = InLeftArrowImage; return *this; }

	/**
	 * The image used for the right arrow
	 */	
	UPROPERTY(EditAnywhere, Category=Appearance)
	FSlateBrush RightArrowImage;
	FArmySimMenuItemStyle& SetRightArrowImage(const FSlateBrush& InRightArrowImage) { RightArrowImage = InRightArrowImage; return *this; }
};


/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class USimMenuItemWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_UCLASS_BODY()

public:
	/** The actual data describing the menu's appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FArmySimMenuItemStyle MenuItemStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast< const struct FSlateWidgetStyle* >( &MenuItemStyle );
	}
};
