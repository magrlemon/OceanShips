// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"
#include "Sim_OceanSceneManager.h"

DECLARE_DELEGATE_RetVal_OneParam(int32, FOnGetArchetypeAttribute, ASimEcs_Archetype*);

namespace SpecialBoatsIndex
{
	const int32 All = -1;
}

struct FBoatsColumnData
{
	/** Column name */
	FText Name;

	/** Column color */
	FSlateColor Color;

	/** Stat value getter delegate */
	FOnGetArchetypeAttribute AttributeGetter;

	/** defaults */
	FBoatsColumnData()
	{
		Color = FLinearColor::White;
	}

	FBoatsColumnData(FText InName, FSlateColor InColor, FOnGetArchetypeAttribute InAtrGetter)
		: Name(InName)
		, Color(InColor)
		, AttributeGetter(InAtrGetter)
	{
	}
};

struct FTeamBoats
{
	/** The team the player belongs to */
	uint8 TeamNum;
		
	/** The number within that team */
	int32 PlayerId;

	/** defaults */
	FTeamBoats()
		: TeamNum(0)
		, PlayerId(SpecialBoatsIndex::All)
	{
	}

	FTeamBoats(uint8 InTeamNum, int32 InPlayerId)
		: TeamNum(InTeamNum)
		, PlayerId(InPlayerId)
	{
	}

	/** comparator */
	bool operator==( const FTeamBoats& Other ) const
	{
		return (TeamNum == Other.TeamNum && PlayerId == Other.PlayerId);
	}

	/** check to see if we have valid player data */
	bool IsValid() const
	{
		return !(*this == FTeamBoats());
	}
};


//class declare
class SOceanBoatsScoreboardWidget : public SBorder
{

	SLATE_BEGIN_ARGS(SOceanBoatsScoreboardWidget)
	{}

	SLATE_ARGUMENT(TWeakObjectPtr<APlayerController>, PCOwner)

	SLATE_ATTRIBUTE(ESimMatchState::Type, MatchState)

	SLATE_END_ARGS()

	/** needed for every widget */
	void Construct(const FArguments& InArgs);

	/** update PlayerState maps with every tick when scoreboard is shown */
	virtual void Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime ) override;

	/** if we want to receive focus */
	virtual bool SupportsKeyboardFocus() const override;

	/** when the widget recieves keyboard focus */
	virtual FReply OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent ) override;

	/** handle keyboard input */
	virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;

	/** Called when the scoreboard is displayed, Stores the name and whether or not a player is currently talking */
	void StoreTalkingPlayerData(const FUniqueNetId& PlayerId, bool bIsTalking);

protected:

	/** updates widgets when players leave or join */
	void UpdateScoreboardGrid();

	/** makes total row widget */
	TSharedRef<SWidget> MakeTotalsRow(/*uint8 TeamNum*/) const;

	/** makes player rows */
	TSharedRef<SWidget> MakePlayerRows(/*uint8 TeamNum*/) const;

	/** makes player row */
	TSharedRef<SWidget> MakePlayerRow(ASimEcs_Archetype* boat) const;

	FText GetObstacleDetail(ASimEcs_Archetype* boat, int archetype) const;

	/** updates PlayerState maps to display accurate scores */
	void UpdatePlayerStateMaps();

	/** gets ranked map for specific team */
	void GetRankedMap(int32 TeamIndex, RankedPlayerMap& OutRankedMap) const;

	/** gets PlayerState for specific team and player */
	ASoldierPlayerState* GetSortedPlayerState(const FTeamBoats& TeamPlayer) const;

	/** get player visibility */
	EVisibility PlayerPresenceToItemVisibility(const FTeamBoats TeamPlayer) const;

	/** get speaker icon visibility */
	EVisibility SpeakerIconVisibility(const FTeamBoats TeamPlayer) const;

	/** get scoreboard border color */
	FSlateColor GetScoreboardBorderColor(int index) const;

	/** get player name */
	FText GetPlayerName(const FTeamBoats TeamPlayer) const;

	//FText GetBoatName(int index) const;
	
	/** get whether or not the player should be displayed on the scoreboard */
	bool ShouldPlayerBeDisplayed(const FTeamBoats TeamPlayer) const;

	/** get player color */
	FSlateColor GetPlayerColor(/*const FTeamBoats TeamPlayer*/) const;

	/** get the column color */
	FSlateColor GetColumnColor(const FTeamBoats TeamPlayer, uint8 ColIdx) const;

	/** checks to see if the specified player is the owner */
	bool IsOwnerPlayer(const FTeamBoats& TeamPlayer) const;

	/** get specific stat for team number and optionally player */
	FText GetStat() const;

	/** linear interpolated score for match outcome animation */
	int32 LerpForCountup(int32 ScoreValue) const;

	int GetColumnType(int col) const;

	///** get match outcome text */
	//FText GetMatchOutcomeText() const;

	///** Get text for match-restart notification. */
	//FText GetMatchRestartText() const;

	///** get attribute value for kills */
	//int32 GetAttributeValue_Kills(class ASoldierPlayerState* PlayerState) const;

	///** get attribute value for deaths */
	//int32 GetAttributeValue_Deaths(class ASoldierPlayerState* PlayerState) const;

	///** get attribute value for score */
	//int32 GetAttributeValue_Score(class ASoldierPlayerState* PlayerState) const;

	// ������
	/** get attribute value for Barrier */
	int32 GetAttributeValue_Barrier_GTZ( class ASimEcs_Archetype* killer ) const;

	// ����׶
	/** get attribute value for Barrier */
	int32 GetAttributeValue_Barrier_SJZ( class ASimEcs_Archetype* killer) const;

	// ����ʯ
	/** get attribute value for Barrier */
	int32 GetAttributeValue_Barrier_HLS( class ASimEcs_Archetype* killer) const;

	// �ִ��
	int32 GetAttributeValue_Barrier_GCW(class ASimEcs_Archetype* killer) const;
	/** triggers a sound effect to play */
	void PlaySound(const FSlateSound& SoundToPlay) const;

	/** handle the mouse moving over scoreboard entry */
	FReply OnMouseOverPlayer(const FGeometry& Geometry, const FPointerEvent& Event, const FTeamBoats TeamPlayer);

	/** called when the previous player wants to be selected */
	void OnSelectedPlayerPrev();

	/** called when the next player wants to be selected */
	void OnSelectedPlayerNext();

	/** resets the selected player to be that of the local user */
	void ResetSelectedPlayer();

	/** makes sure the selected player is valid */
	void UpdateSelectedPlayer();

	/** sets the currently selected player to be ourselves */
	bool SetSelectedPlayerUs();

	/** checks to see if the specified player is the selected one */
	bool IsSelectedPlayer(const FTeamBoats& TeamPlayer) const;

	/** is there a valid selected item */
	bool IsPlayerSelectedAndValid() const;

	/** check to see if we can open the profile ui */
	EVisibility GetProfileUIVisibility() const;

	/** profile open ui handler */
	bool ProfileUIOpened() const;

	/** scoreboard tint color */
	FLinearColor ScoreboardTint;

	/** width of scoreboard item */
	int32 ScoreBoxWidth;

	/** scoreboard count up time */
	float ScoreCountUpTime;

	/** when the scoreboard was brought up. */
	double ScoreboardStartTime;

	/** the player currently selected in the scoreboard */
	FTeamBoats SelectedPlayer;

	/** the Ranked PlayerState map...cleared every frame */
	TArray<RankedPlayerMap> PlayerStateMaps;

	/** player count in each team in the last tick */
	TArray<int32> LastTeamPlayerCount;

	/** holds talking player data */
	TArray<TPair<TSharedRef<const FUniqueNetId>, bool>> PlayersTalkingThisFrame;

	/** holds player info rows */
	TSharedPtr<SVerticalBox> ScoreboardData;

	/** stat columns data */
	TArray<FBoatsColumnData> Columns;

	/** get state of current match */
	ESimMatchState::Type MatchState;

	/** pointer to our parent HUD */
	TWeakObjectPtr<class APlayerController> PCOwner;

	/** style for the scoreboard */
	const struct FSoldierScoreboardStyle *ScoreboardStyle;
};
