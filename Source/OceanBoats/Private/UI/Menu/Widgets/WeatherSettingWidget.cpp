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


void SWeatherSettingWidget::Construct(const FArguments& InArgs)
{
	PlayerOwner = InArgs._PlayerOwner;
	OwnerWidget = InArgs._OwnerWidget;
	StatusText = FText::GetEmpty();

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
					.IsChecked(this, &SWeatherSettingWidget::IsGloomyChecked)
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
					.IsChecked(this, &SWeatherSettingWidget::IsRainyChecked)
					.OnCheckStateChanged(this, &SWeatherSettingWidget::OnShowRainyChecked)
					.Style(FCoreStyle::Get(), "Checkbox")
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Rainy", "Show Rainy"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SCheckBox)
					.IsChecked(this, &SWeatherSettingWidget::IsSnowChecked)
					.OnCheckStateChanged(this, &SWeatherSettingWidget::OnShowSnowChecked)
					.Style(FCoreStyle::Get(), "Checkbox")
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Snow", "Show Snow"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
				]

				+ SVerticalBox::Slot()
				.HAlign(HAlign_Left)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SCheckBox)
					.IsChecked(this, &SWeatherSettingWidget::IsThunderChecked)
					.OnCheckStateChanged(this, &SWeatherSettingWidget::OnShowThunderChecked)
					.Style(FCoreStyle::Get(), "Checkbox")
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Thunder", "Show Thunder"))
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
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetTimeOfDayValue /*bFromSlider =*/)
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
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetDayValue)
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
				.HAlign(HAlign_Fill)
				.Padding(FMargin(0.0f, 10.0f, 0.0f, 18.0f))
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					[
						SNew(STextBlock)
						.Text(LOCTEXT("Wave", "Wave"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSlider)
						.Value(this, &SWeatherSettingWidget::GetWaveValue)
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetWaveValue)
						.SliderBarColor(FLinearColor(0.48f, 0.48f, 0.48f))
						.Style(FCoreStyle::Get(), "Slider")
						//.MouseUsesStep(true)
						.StepSize(0.1)
						.OnMouseCaptureEnd(this, &SWeatherSettingWidget::OnWaveSliderMouseEnd)
						.ToolTipText(this, &SWeatherSettingWidget::GetWaveText)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						//.Text(FText::AsNumber(mDay))
						.Text(this, &SWeatherSettingWidget::GetWaveText)
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
						.Text(LOCTEXT("Wind", "Wind"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSlider)
						.Value(this, &SWeatherSettingWidget::GetWindValue)
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetWindValue)
						.SliderBarColor(FLinearColor(0.48f, 0.48f, 0.48f))
						.Style(FCoreStyle::Get(), "Slider")
						//.MouseUsesStep(true)
						.StepSize(0.1)
						.OnMouseCaptureEnd(this, &SWeatherSettingWidget::OnWindSliderMouseEnd)
						.ToolTipText(this, &SWeatherSettingWidget::GetWindText)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						//.Text(FText::AsNumber(mDay))
						.Text(this, &SWeatherSettingWidget::GetWindText)
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
						.Text(LOCTEXT("WindDir", "WindDir"))
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DemoListCheckboxTextStyle")
					]
					+ SHorizontalBox::Slot()
					[
						SNew(SSlider)
						.Value(this, &SWeatherSettingWidget::GetWindDirValue)
						.OnValueChanged(this, &SWeatherSettingWidget::OnSetWindDirValue)
						.SliderBarColor(FLinearColor(0.48f, 0.48f, 0.48f))
						.Style(FCoreStyle::Get(), "Slider")
						//.MouseUsesStep(true)
						.StepSize(0.005)
						.OnMouseCaptureEnd(this, &SWeatherSettingWidget::OnWindDirSliderMouseEnd)
						.ToolTipText(this, &SWeatherSettingWidget::GetWindDirText)
					]
					+ SHorizontalBox::Slot()
					.HAlign(HAlign_Left)
					[
						SNew(STextBlock)
						//.Text(FText::AsNumber(mDay))
						.Text(this, &SWeatherSettingWidget::GetWindDirText)
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
FText SWeatherSettingWidget::GetWaveText() const
{
	return FText::FromString(FString::SanitizeFloat(mWave * 10));
}
FText SWeatherSettingWidget::GetWindText() const
{	
	return FText::FromString(FString::FromInt(mWind*10));
}
FText SWeatherSettingWidget::GetWindDirText() const
{
	return FText::FromString(FString::FromInt(mWindDir*360));
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

ECheckBoxState SWeatherSettingWidget::IsGloomyChecked() const
{
	return mGloomyState;
}
ECheckBoxState SWeatherSettingWidget::IsRainyChecked() const
{
	return mRainyState;
}
ECheckBoxState SWeatherSettingWidget::IsThunderChecked() const
{
	return mThunderState;
}
ECheckBoxState SWeatherSettingWidget::IsSnowChecked() const
{
	return mSnowState;
}
void SWeatherSettingWidget::OnShowGloomyChecked(ECheckBoxState NewCheckedState)
{
	mGloomyState = NewCheckedState;
	if (mGloomyState == ECheckBoxState::Checked)
		IGameModeInterface::Execute_ShowGloomy(UGameplayStatics::GetGameMode(GWorld), true);
	else
		IGameModeInterface::Execute_ShowGloomy(UGameplayStatics::GetGameMode(GWorld), false);
}

void SWeatherSettingWidget::OnShowRainyChecked(ECheckBoxState NewCheckedState)
{
	mRainyState = NewCheckedState;
	if (mRainyState == ECheckBoxState::Checked)
		IGameModeInterface::Execute_ShowRainy(UGameplayStatics::GetGameMode(GWorld), true);
	else
		IGameModeInterface::Execute_ShowRainy(UGameplayStatics::GetGameMode(GWorld), false);
}

void SWeatherSettingWidget::OnShowThunderChecked(ECheckBoxState NewCheckedState)
{
	mThunderState = NewCheckedState;
	if (mRainyState == ECheckBoxState::Checked)
		IGameModeInterface::Execute_ShowThunder(UGameplayStatics::GetGameMode(GWorld), true);
	else
		IGameModeInterface::Execute_ShowThunder(UGameplayStatics::GetGameMode(GWorld), false);
}
void SWeatherSettingWidget::OnShowSnowChecked(ECheckBoxState NewCheckedState)
{
	mSnowState = NewCheckedState;
	if (mRainyState == ECheckBoxState::Checked)
		IGameModeInterface::Execute_ShowSnow(UGameplayStatics::GetGameMode(GWorld), true);
	else
		IGameModeInterface::Execute_ShowSnow(UGameplayStatics::GetGameMode(GWorld), false);
}
float SWeatherSettingWidget::GetTimeOfDayValue() const
{
	return mTimeOfDay;
}
void SWeatherSettingWidget::OnSetTimeOfDayValue(float NewValue)
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
void SWeatherSettingWidget::OnSetDayValue(float NewValue)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mDay = NewValue;
}
void SWeatherSettingWidget::OnDaySliderMouseEnd()
{

}

float SWeatherSettingWidget::GetWaveValue() const
{
	return mWave;
}
void SWeatherSettingWidget::OnSetWaveValue(float NewValue)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mWave = NewValue;

	IGameModeInterface::Execute_SetWaveStrength(UGameplayStatics::GetGameMode(GWorld), NewValue);
}
void SWeatherSettingWidget::OnWaveSliderMouseEnd()
{

}
float SWeatherSettingWidget::GetWindValue() const
{
	return mWind;
}
void SWeatherSettingWidget::OnSetWindValue(float NewValue)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mWind = NewValue;
}
void SWeatherSettingWidget::OnWindSliderMouseEnd()
{

}

float SWeatherSettingWidget::GetWindDirValue() const
{
	return mWindDir;
}
void SWeatherSettingWidget::OnSetWindDirValue(float NewValue)
{
	NewValue = FMath::RoundToFloat(NewValue * 100.f) / 100.f;
	mWindDir = NewValue;
}
void SWeatherSettingWidget::OnWindDirSliderMouseEnd()
{

}
#undef LOCTEXT_NAMESPACE