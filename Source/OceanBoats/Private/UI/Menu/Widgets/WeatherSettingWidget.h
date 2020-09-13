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

	///** if we want to receive focus */
	//virtual bool SupportsKeyboardFocus() const override { return true; }

	///** focus received handler - keep the ActionBindingsList focused */
	//virtual FReply OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent) override;
	//
	///** focus lost handler - keep the ActionBindingsList focused */
	//virtual void OnFocusLost(const FFocusEvent& InFocusEvent) override;

	///** key down handler */
	//virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyboardEvent) override;

	///** SListView item double clicked */
	//void OnListItemDoubleClicked(TSharedPtr<FSceneObjectsEntry> InItem);

	///** creates single item widget, called for every list item */
	//TSharedRef<ITableRow> MakeListViewWidget(TSharedPtr<FSceneObjectsEntry> Item, const TSharedRef<STableViewBase>& OwnerTable);

	///** selection changed handler */
	//void EntrySelectionChanged(TSharedPtr<FSceneObjectsEntry> InItem, ESelectInfo::Type SelectInfo);

	///** Updates the list until it's completely populated */
	//void UpdateBuildSceneObjectsListStatus();

	///** Populates the demo list */
	//void BuildSceneObjectsList();

	///** Called when demo list building finished */
	//void OnBuildSceneObjectsListFinished();

	///** Called when we get results from the replay streaming interface */
	//void OnEnumerateStreamsComplete(const FEnumerateObjectsStreamsResult& Result);

	///** Play chosen demo */
	//void LocateObjects();

	///** Delete chosen demo */
	//void DeleteDemo();

	///** Delete chosen demo (really) */
	//FReply OnDemoDeleteConfirm();

	///** Cancel delete chosen demo */
	//FReply OnDemoDeleteCancel();

	///** Called by delegate when the replay streaming interface has finished deleting */
	//void OnDeleteFinishedStreamComplete(const FDeleteFinishedStreamResult& Result);

	///** selects item at current + MoveBy index */
	//void MoveSelection(int32 MoveBy);

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

	/** Version used for enumerating replays. This is manipulated depending on whether we want to show all versions or not. */
	FNetworkReplayVersion EnumerateStreamsVersion;

protected:

	///** Whether we're building the demo list or not */
	//bool bUpdatingSceneObjectsList;

	///** action bindings array */
	//TArray< TSharedPtr<FSceneObjectsEntry> > SceneObjectsList;

	///** action bindings list slate widget */
	//TSharedPtr< SListView< TSharedPtr<FSceneObjectsEntry> > > ObjectsListWidget; 

	///** currently selected list item */
	//TSharedPtr<FSceneObjectsEntry> SelectedItem;

	/** get current status text */
	FText GetTimeOfDayText() const;
	FText GetDayText() const;
	FText GetBottomText() const;

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
};


