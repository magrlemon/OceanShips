// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#include "ArmySimStyle.h"
#include "OceanBoats.h"
#include "Slate\SlateGameResources.h"

TSharedPtr< FSlateStyleSet > FArmySimStyle::SimStyleInstance = NULL;

void FArmySimStyle::Initialize()
{
	if ( !SimStyleInstance.IsValid() )
	{
		SimStyleInstance = Create();
		FSlateStyleRegistry::RegisterSlateStyle( *SimStyleInstance );
	}
}

void FArmySimStyle::Shutdown()
{
	FSlateStyleRegistry::UnRegisterSlateStyle( *SimStyleInstance );
	ensure( SimStyleInstance.IsUnique() );
	SimStyleInstance.Reset();
}

FName FArmySimStyle::GetStyleSetName()
{
	static FName StyleSetName(TEXT("ArmySimStyle"));
	return StyleSetName;
}

#define IMAGE_BRUSH( RelativePath, ... ) FSlateImageBrush( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define BOX_BRUSH( RelativePath, ... ) FSlateBoxBrush( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define BORDER_BRUSH( RelativePath, ... ) FSlateBorderBrush( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".png"), __VA_ARGS__ )
#define TTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".ttf"), __VA_ARGS__ )
#define OTF_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".otf"), __VA_ARGS__ )

PRAGMA_DISABLE_OPTIMIZATION
TSharedRef< FSlateStyleSet > FArmySimStyle::Create()
{	
	TSharedRef<FSlateStyleSet> StyleRef = FSlateGameResources::New(FArmySimStyle::GetStyleSetName(), "/Game/UI/Styles", "/Game/UI/Styles");
	FSlateStyleSet& Style = StyleRef.Get();

	// Load the speaker icon to be used for displaying when a user is talking
	Style.Set("OceanBoats.Speaker", new IMAGE_BRUSH("Images/SoundCue_SpeakerIcon", FVector2D(32, 32)));

	// The border image used to draw the replay timeline bar
	Style.Set("OceanBoats.ReplayTimelineBorder", new BOX_BRUSH("Images/ReplayTimeline", FMargin(3.0f / 8.0f)));

	// The border image used to draw the replay timeline bar
	Style.Set("OceanBoats.ReplayTimelineIndicator", new IMAGE_BRUSH("Images/ReplayTimelineIndicator", FVector2D(4.0f, 26.0f)));

	// The image used to draw the replay pause button
	Style.Set("OceanBoats.ReplayPauseIcon", new IMAGE_BRUSH("Images/ReplayPause", FVector2D(32.0f, 32.0f)));

	const FTableColumnHeaderStyle TableColumnHeaderStyle = FTableColumnHeaderStyle()
		.SetSortPrimaryAscendingImage(IMAGE_BRUSH("Common/SortUpArrow", FVector2D(8.0f, 4.0f)))
		.SetSortPrimaryDescendingImage(IMAGE_BRUSH("Common/SortDownArrow", FVector2D(8.0f, 4.0f)))
		.SetSortSecondaryAscendingImage(IMAGE_BRUSH("Common/SortUpArrows", FVector2D(16.0f, 4.0f)))
		.SetSortSecondaryDescendingImage(IMAGE_BRUSH("Common/SortDownArrows", FVector2D(16.0f, 4.0f)))
		.SetNormalBrush(BOX_BRUSH("Common/ColumnHeader", 8.f / 32.f))
		.SetHoveredBrush(BOX_BRUSH("Common/ColumnHeader_Hovered", 4.f / 32.f))
		.SetMenuDropdownImage(IMAGE_BRUSH("Common/ColumnHeader_Arrow", FVector2D(8.0f, 8.0f)))
		.SetMenuDropdownNormalBorderBrush(BOX_BRUSH("Common/ColumnHeaderMenuButton_Normal", 4.f / 32.f))
		.SetMenuDropdownHoveredBorderBrush(BOX_BRUSH("Common/ColumnHeaderMenuButton_Hovered", 4.f / 32.f));
	const FTableColumnHeaderStyle TableLastColumnHeaderStyle = FTableColumnHeaderStyle()
		.SetSortPrimaryAscendingImage(IMAGE_BRUSH("Common/SortUpArrow", FVector2D(8.0f, 4.0f)))
		.SetSortPrimaryDescendingImage(IMAGE_BRUSH("Common/SortDownArrow", FVector2D(8.0f, 4.0f)))
		.SetSortSecondaryAscendingImage(IMAGE_BRUSH("Common/SortUpArrows", FVector2D(8.0f, 4.0f)))
		.SetSortSecondaryDescendingImage(IMAGE_BRUSH("Common/SortDownArrows", FVector2D(16.0f, 4.0f)))
		.SetNormalBrush(FSlateNoResource())
		.SetHoveredBrush(BOX_BRUSH("Common/LastColumnHeader_Hovered", 4.f / 32.f))
		.SetMenuDropdownImage(IMAGE_BRUSH("Common/ColumnHeader_Arrow", FVector2D(8.0f, 8.0f)))
		.SetMenuDropdownNormalBorderBrush(BOX_BRUSH("Common/ColumnHeaderMenuButton_Normal", 4.f / 32.f))
		.SetMenuDropdownHoveredBorderBrush(BOX_BRUSH("Common/ColumnHeaderMenuButton_Hovered", 4.f / 32.f));
	const FSplitterStyle TableHeaderSplitterStyle = FSplitterStyle()
		.SetHandleNormalBrush(FSlateNoResource())
		.SetHandleHighlightBrush(IMAGE_BRUSH("Common/HeaderSplitterGrip", FVector2D(8.0f, 8.0f)));
	Style.Set("OceanBoats.Row.HeaderRowStyle", FHeaderRowStyle()
		.SetColumnStyle(TableColumnHeaderStyle)
		.SetLastColumnStyle(TableLastColumnHeaderStyle)
		.SetColumnSplitterStyle(TableHeaderSplitterStyle)
		.SetBackgroundBrush(BOX_BRUSH("Common/TableViewHeader", 8.f / 32.f))
		.SetForegroundColor(FSlateColor(MakeShareable(new FLinearColor(0.72f, 0.72f, 0.72f, 1.f))))
	);

	// Fonts still need to be specified in code for now
	Style.Set("OceanBoats.MenuServerListTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 14))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.MenuStoreListTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 14))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1, 1))
	);

	Style.Set("OceanBoats.ScoreboardListTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 18))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.MenuProfileNameStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 18))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.MenuTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 20))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.MenuHeaderTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 26))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.WelcomeScreen.WelcomeTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyhbd-Medium", 32))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.DefaultScoreboard.Row.HeaderTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 24))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FVector2D(0,1))
		);

	Style.Set("OceanBoats.DefaultScoreboard.Row.StatTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyhl-Regular", 18))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FVector2D(0,1))
		);

	Style.Set("OceanBoats.SplitScreenLobby.StartMatchTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyhl-Regular", 16))
		.SetColorAndOpacity(FLinearColor::Green)
		.SetShadowOffset(FVector2D(0,1))
		);

	Style.Set("OceanBoats.DemoListCheckboxTextStyle", FTextBlockStyle()
		.SetFont(TTF_FONT("Fonts/msyh-Black", 12))
		.SetColorAndOpacity(FLinearColor::White)
		.SetShadowOffset(FIntPoint(-1,1))
		);

	Style.Set("OceanBoats.Switch.Left", FInlineTextImageStyle()
		.SetImage(IMAGE_BRUSH("Images/SwitchButtonLeft", FVector2D(32, 32)))
		);

	Style.Set("OceanBoats.Switch.Right", FInlineTextImageStyle()
		.SetImage(IMAGE_BRUSH("Images/SwitchButtonRight", FVector2D(32, 32)))
		);

	Style.Set("OceanBoats.Switch.Up", FInlineTextImageStyle()
		.SetImage(IMAGE_BRUSH("Images/SwitchButtonUp", FVector2D(32, 32)))
		);

	Style.Set("OceanBoats.Switch.Down", FInlineTextImageStyle()
		.SetImage(IMAGE_BRUSH("Images/SwitchButtonDown", FVector2D(32, 32)))
		);

	return StyleRef;
}
PRAGMA_ENABLE_OPTIMIZATION

#undef IMAGE_BRUSH
#undef BOX_BRUSH
#undef BORDER_BRUSH
#undef TTF_FONT
#undef OTF_FONT

void FArmySimStyle::ReloadTextures()
{
	FSlateApplication::Get().GetRenderer()->ReloadTextureResources();
}

const ISlateStyle& FArmySimStyle::Get()
{
	return *SimStyleInstance;
}
