// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "WeatherSettingWidget.h"
#include "SHeaderRow.h"
#include "ArmySimStyle.h"
#include "CoreStyle.h"
#include "ArmyGameLoadingScreen.h"
#include "ArmySimGameInstance.h"
#include "NetworkReplayStreaming.h"
#include "SimDataStructure.h"
#include "Sim_OceanSceneManager.h"
#include "ArmySimGameViewportClient.h"
#include "SoldierMenuWidgetStyle.h"
#define LOCTEXT_NAMESPACE "ArmySim.HUD.Menu"




struct FSceneObjectsEntry {
	FActorsStreamInfo StreamInfo;
	FString		Date;
	FString		Size;
	int32		ResultsIndex;
};

void SWeatherSettingWidget::Construct(const FArguments& InArgs)
{
	PlayerOwner = InArgs._PlayerOwner;
	OwnerWidget = InArgs._OwnerWidget;
	//bUpdatingSceneObjectsList = false;
	StatusText = FText::GetEmpty();

	EnumerateStreamsVersion = FNetworkVersion::GetReplayVersion();

	const int32 BoxWidth = 600;

	const FSoldierMenuStyle* ItemStyle = &FArmySimStyle::Get().GetWidgetStyle<FSoldierMenuStyle>("DefaultSoldierMenuStyle");

	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill)
		[
			SNew(SBox)
			.WidthOverride(BoxWidth)
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SCheckBox)
					.IsChecked(this, &SWeatherSettingWidget::IsShowGloomyChecked)
					.OnCheckStateChanged(this, &SWeatherSettingWidget::OnShowGloomyChecked)
					.Style(FCoreStyle::Get(), "Checkbox")	
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Gloomy", "Show Gloomy"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SCheckBox)
					.IsChecked(this, &SWeatherSettingWidget::IsShowRainyChecked)
					.OnCheckStateChanged(this, &SWeatherSettingWidget::OnShowRainyChecked)
					.Style(FCoreStyle::Get(), "Checkbox")
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Rainy", "Show Rainy"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("TimeOfDay", "Time Of Day"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
					
					+ SHorizontalBox::Slot()
					[
						SNew(SSlider)
						.Value(this, &SWeatherSettingWidget::GetTimeOfDayValue)
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetTimeOfDayValue, /*bFromSlider =*/true)
						.SliderBarColor(FLinearColor(0.48f, 0.48f, 0.48f))
						.Style(FCoreStyle::Get(), "Slider")
						//.MouseUsesStep(true)
						.StepSize(0.01)
						.OnMouseCaptureEnd(this, &SWeatherSettingWidget::OnTimeOfDaySliderMouseEnd)
						.ToolTipText(this, &SWeatherSettingWidget::GetTimeOfDayText)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						//.Text(FText::AsNumber(mTimeOfDay))
						.Text(this,&SWeatherSettingWidget::GetTimeOfDayText)
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Fill)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Day", "Day"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]					
					+ SHorizontalBox::Slot()
					[
						SNew(SSlider)
						.Value(this, &SWeatherSettingWidget::GetDayValue)
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetDayValue, /*bFromSlider =*/true)
						.SliderBarColor(FLinearColor(0.48f, 0.48f, 0.48f))
						.Style(FCoreStyle::Get(), "Slider")
						//.MouseUsesStep(true)
						.StepSize(0.01)
						.OnMouseCaptureEnd(this, &SWeatherSettingWidget::OnDaySliderMouseEnd)
						.ToolTipText(this, &SWeatherSettingWidget::GetDayText)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						//.Text(FText::AsNumber(mDay))
						.Text(this, &SWeatherSettingWidget::GetDayText)
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.AutoHeight()
				[
					SNew(SOverlay)
					+ SOverlay::Slot()
					.VAlign(VAlign_Center)
					.HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(this, &SWeatherSettingWidget::GetBottomText)
						.TextStyle(FArmySimStyle::Get(), "SoldierGame.MenuServerListTextStyle")
					]
				]
			]
			
		];


	ReplayStreamer = FNetworkReplayStreaming::Get( ).GetFactory( ).CreateReplayStreamer( );

	//BuildSceneObjectsList( );
	FSlateApplication::Get( ).PlaySound( ItemStyle->MenuEnterSound, GetOwnerUserIndex( ) );
}

FText SWeatherSettingWidget::GetTimeOfDayText() const
{
	return FText::FromString(FString::SanitizeFloat(mTimeOfDay));
}

FText SWeatherSettingWidget::GetDayText() const
{
	return FText::FromString(FString::SanitizeFloat(mDay));
}

FText SWeatherSettingWidget::GetBottomText( ) const
{
	return StatusText;
}
/**
 * Ticks this widget.  Override in derived classes, but always call the parent implementation.
 *
 * @param  AllottedGeometry The space allotted for this widget
 * @param  InCurrentTime  Current absolute real time
 * @param  InDeltaTime  Real time passed since last tick
 */
void SWeatherSettingWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	SCompoundWidget::Tick( AllottedGeometry, InCurrentTime, InDeltaTime );
}

ECheckBoxState SWeatherSettingWidget::IsShowGloomyChecked() const
{
	return mGloomyState;
}
ECheckBoxState SWeatherSettingWidget::IsShowRainyChecked() const
{
	return mRainyState;
}

void SWeatherSettingWidget::OnShowGloomyChecked(ECheckBoxState NewCheckedState)
{
	mGloomyState = NewCheckedState;
}

void SWeatherSettingWidget::OnShowRainyChecked(ECheckBoxState NewCheckedState)
{
	mRainyState = NewCheckedState;
}
float SWeatherSettingWidget::GetTimeOfDayValue() const
{
	return mTimeOfDay;
}
void SWeatherSettingWidget::OnSetTimeOfDayValue(float NewValue, bool bFromSlider)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mTimeOfDay = NewValue;
}
void SWeatherSettingWidget::OnTimeOfDaySliderMouseEnd()
{

}
float SWeatherSettingWidget::GetDayValue() const
{
	return mDay;
}
void SWeatherSettingWidget::OnSetDayValue(float NewValue, bool bFromSlider)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mDay = NewValue;
}
void SWeatherSettingWidget::OnDaySliderMouseEnd()
{

}

#undef LOCTEXT_NAMESPACE