// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "OceanBoats.h"
#include "SSoldierMenuWidget.h"
#include "NetworkReplayStreaming.h"
#include "SoldierScoreboardWidgetStyle.h"
#include "Sim_OceanSceneManager.h"
#include "Misc/NetworkVersion.h"

struct FSceneObjectsEntry;

//class declare
class SWeatherSettingWidget : public SSoldierMenuWidget
{
public:
	SLATE_BEGIN_ARGS(SWeatherSettingWidget)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<ULocalPlayer>, PlayerOwner)
	SLATE_ARGUMENT(TSharedPtr<SWidget>, OwnerWidget)

	SLATE_END_ARGS()

	/** needed for every widget */
	void Construct(const FArguments& InArgs);

	

	/**
	 * Ticks this widget.  Override in derived classes, but always call the parent implementation.
	 *
	 * @param  AllottedGeometry The space allotted for this widget
	 * @param  InCurrentTime  Current absolute real time
	 * @param  InDeltaTime  Real time passed since last tick
	 */
	void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime );

private:
	/** Callback for "show all replay versions" checkbox */
	ECheckBoxState IsShowGloomyChecked() const;
	ECheckBoxState IsShowRainyChecked() const;
	/** Callback fired when "show all replay versions" checkbox is changed */
	void OnShowAllReplaysChecked(ECheckBoxState NewCheckedState);
	void OnShowGloomyChecked(ECheckBoxState NewCheckedState);
	void OnShowRainyChecked(ECheckBoxState NewCheckedState);

	float GetTimeOfDayValue() const;
	void OnSetTimeOfDayValue(float NewValue, bool bFromSlider);	
	void OnTimeOfDaySliderMouseEnd();

	float GetDayValue() const;
	void OnSetDayValue(float NewValue, bool bFromSlider);
	void OnDaySliderMouseEnd();

	float GetWaveValue() const;
	void OnSetWaveValue(float NewValue, bool bFromSlider);
	void OnWaveSliderMouseEnd();

	float GetWindValue() const;
	void OnSetWindValue(float NewValue, bool bFromSlider);
	void OnWindSliderMouseEnd();

	float GetWindDirValue() const;
	void OnSetWindDirValue(float NewValue, bool bFromSlider);
	void OnWindDirSliderMouseEnd();


protected:
	
	/** get current status text */
	FText GetTimeOfDayText() const;
	FText GetDayText() const;
	FText GetBottomText() const;
	FText GetWaveText() const;
	FText GetWindText() const;
	FText GetWindDirText() const;

	/** current status text */
	FText StatusText;

	/** pointer to our owner PC */
	TWeakObjectPtr<class ULocalPlayer> PlayerOwner;

	/** pointer to our parent widget */
	TSharedPtr<class SWidget> OwnerWidget;

	/** Network replay streaming interface */
	TSharedPtr<INetworkReplayStreamer> ReplayStreamer;

	ECheckBoxState mGloomyState;
	ECheckBoxState mRainyState;
	float mTimeOfDay;
	float mDay;
	float mWave;
	float mWind;
	float mWindDir;
};


