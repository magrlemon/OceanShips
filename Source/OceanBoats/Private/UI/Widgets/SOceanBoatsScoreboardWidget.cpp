// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SOceanBoatsScoreboardWidget.h"
#include "ArmySimStyle.h"
#include "SoldierScoreboardWidgetStyle.h"
#include "SoldierUIHelpers.h"
#include "OceanCommonTypes.h"
#include "SoldierPlayerState.h"
#include "SimEcs_Archetype.h"
#include "SimEcs_EntityManager.h"

#define LOCTEXT_NAMESPACE "SoldierScoreboard"

// @todo: prevent interaction on PC for now (see OnFocusReceived for reasons)
#if PLATFORM_XBOXONE
#define INTERACTIVE_SCOREBOARD	1
#else
#define INTERACTIVE_SCOREBOARD	0
#endif

#define	NORM_PADDING	(FMargin(5))

void SOceanBoatsScoreboardWidget::Construct(const FArguments& InArgs)
{
	ScoreboardStyle = &FArmySimStyle::Get().GetWidgetStyle<FSoldierScoreboardStyle>("DefaultSoldierScoreboardStyle");

	PCOwner = InArgs._PCOwner;
	ScoreboardTint = FLinearColor(0.0f,0.0f,0.0f,0.4f);
	ScoreBoxWidth = 120.0f;
	ScoreCountUpTime = 2.0f;

	ScoreboardStartTime = FPlatformTime::Seconds();
	MatchState = InArgs._MatchState.Get();

	UpdatePlayerStateMaps();
	
	//������
	Columns.Add( FBoatsColumnData( LOCTEXT( "BarrierGtzColumn", "GTZ" ),
		ScoreboardStyle->KillStatColor,
		FOnGetArchetypeAttribute::CreateSP( this, &SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_GTZ ) ) );

	//����׶
	Columns.Add( FBoatsColumnData( LOCTEXT( "BarrierSjzColumn", "SJZ" ),
		ScoreboardStyle->DeathStatColor,
		FOnGetArchetypeAttribute::CreateSP( this, &SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_SJZ ) ) );

	//����ʯ
	Columns.Add( FBoatsColumnData( LOCTEXT( "BarrierHlsColumn", "HLS" ),
		ScoreboardStyle->ScoreStatColor,
		FOnGetArchetypeAttribute::CreateSP( this, &SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_HLS ) ) );

	//�ִ��
	Columns.Add(FBoatsColumnData(LOCTEXT("BarrierGCWColumn", "GCW"),
		ScoreboardStyle->ScoreStatColor,
		FOnGetArchetypeAttribute::CreateSP(this, &SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_GCW)));
	
	TSharedPtr<SHorizontalBox> HeaderCols;

	const TSharedRef<SVerticalBox> ScoreboardGrid = SNew(SVerticalBox)
	// HEADER ROW			
	+SVerticalBox::Slot() .AutoHeight()
	[
		//Padding in the header row (invisible) for speaker icon
		SAssignNew(HeaderCols, SHorizontalBox)
		+ SHorizontalBox::Slot().Padding(NORM_PADDING+FMargin(2,0,0,0)).AutoWidth()
		[
			SNew(SImage)
			.Image(&ScoreboardStyle->ItemHeaderBrush/*FArmySimStyle::Get().GetBrush("OceanBoats.Speaker")*/)
			.Visibility(EVisibility::Hidden)
		]

		//Player Name autosized column
		+SHorizontalBox::Slot() .Padding(NORM_PADDING)
		[
			SNew(SBorder)
			.Padding(NORM_PADDING)
			.VAlign(VAlign_Center)	
			.HAlign(HAlign_Center)			
			.BorderImage(&ScoreboardStyle->ItemBorderBrush)
			.BorderBackgroundColor(ScoreboardTint)
			[
				SNew(SBox)
				.WidthOverride(ScoreBoxWidth  * 2)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("PlayerNameColumn", "BoatName"))
					.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.HeaderTextStyle")
					
				]
			]
		]
	];
	
	for (uint8 ColIdx = 0; ColIdx < Columns.Num( ); ColIdx++ )
	{
		//Header constant sized columns
		HeaderCols->AddSlot() .VAlign(VAlign_Center) .HAlign(HAlign_Center) .AutoWidth() .Padding(NORM_PADDING)
		[
			SNew(SBorder)
			.Padding(NORM_PADDING)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			.BorderImage(&ScoreboardStyle->ItemBackGround)
			.BorderBackgroundColor(ScoreboardTint)
			[
				SNew(SBox)
				.WidthOverride(ScoreBoxWidth * 2)
				.HAlign(HAlign_Center)
				[
					SNew(SHorizontalBox)
					+SHorizontalBox::Slot() .AutoWidth() .VAlign(VAlign_Center) .HAlign(HAlign_Center)
					[
						SNew(STextBlock)
						.Text(Columns[ColIdx].Name)						
						.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.HeaderTextStyle")
						.ColorAndOpacity(Columns[ColIdx].Color)
					]
				]
			]
		];
	}

	ScoreboardGrid->AddSlot() .AutoHeight()
	[
		SAssignNew(ScoreboardData, SVerticalBox)
	];
	UpdateScoreboardGrid();

	SBorder::Construct(
		SBorder::FArguments()
		.BorderImage(&ScoreboardStyle->ItemBackGround)
		.BorderBackgroundColor(ScoreboardTint)
		[
			ScoreboardGrid
		]			
	);
}
int SOceanBoatsScoreboardWidget::GetColumnType(int col) const
{
	if (col == 0)
		return EEE_COASTDEF_GTZ_TYPE;
	if (col == 1)
		return EEE_COASTDEF_SJZ_TYPE;
	if (col == 2)
		return EEE_COASTDEF_HLS_TYPE;
	if (col == 3)
		return EEE_COASTDEF_GCW_TYPE;
	return EEE_ENTITYEQUIPS_NUM;
}
void SOceanBoatsScoreboardWidget::StoreTalkingPlayerData(const FUniqueNetId& PlayerId, bool bIsTalking)
{
	static TMap<FString, double> LastTimeSpoken;
	int32 FoundIndex = -1;
	const double IconTimeout = 0.1;
	double CurrentTime = FPlatformTime::Seconds();

	for (int32 i = 0; i < PlayersTalkingThisFrame.Num(); ++i)
	{
		if (PlayersTalkingThisFrame[i].Key.Get() == PlayerId)
		{
			FoundIndex = i;
		}
	}

	if (FoundIndex < 0)
	{
		FoundIndex = PlayersTalkingThisFrame.Emplace(PlayerId.AsShared(), false);
	}

	if (bIsTalking)
	{
		double* Value = LastTimeSpoken.Find(PlayerId.ToString());
		if (Value)
		{
			*Value = CurrentTime;
		}
		else
		{
			LastTimeSpoken.Add(PlayerId.ToString(), CurrentTime);
		}
	}

	if (LastTimeSpoken.FindRef(PlayerId.ToString()) > CurrentTime - IconTimeout)
	{
		PlayersTalkingThisFrame[FoundIndex].Value = true;
	}
	else
	{
		PlayersTalkingThisFrame[FoundIndex].Value = false;
	}
}

//FText SOceanBoatsScoreboardWidget::GetMatchRestartText() const
//{
//	if (PCOwner.IsValid() && (PCOwner->GetWorld() != NULL ))
//	{
//		ASoldierGameState* const GameState = PCOwner->GetWorld()->GetGameState<ASoldierGameState>();
//		if (GameState)
//		{
//			if (GameState->RemainingTime > 0)
//			{
//				return FText::Format(LOCTEXT("MatchRestartTimeString", "New match begins in: {0}"), FText::AsNumber(GameState->RemainingTime));
//			}
//			else
//			{
//				return LOCTEXT("MatchRestartingString", "Starting new match...");
//			}
//		}
//	}
//
//	return FText::GetEmpty();
//}

//FText SOceanBoatsScoreboardWidget::GetMatchOutcomeText() const
//{
//	FText OutcomeText = FText::GetEmpty();
//	OutcomeText = LOCTEXT("DamageStates", "Damage States");
//	/*if (MatchState == ESimMatchState::Won)
//	{
//		OutcomeText = LOCTEXT("Winner", "YOU ARE THE WINNER!");
//	}
//	else if (MatchState == ESimMatchState::Lost)
//	{
//		OutcomeText = LOCTEXT("Loser", "Match has finished");
//	}*/
//
//	return OutcomeText;
//}

void SOceanBoatsScoreboardWidget::UpdateScoreboardGrid()
{
	if (!ScoreboardData.IsValid())
		return;
	ScoreboardData->ClearChildren();
	//for (uint8 TeamNum = 0; TeamNum < PlayerStateMaps.Num(); TeamNum++)
	{
		//Player rows from each team
		ScoreboardData->AddSlot() .AutoHeight()
			[
				MakePlayerRows(/*TeamNum*/)
			];
		////If we have more than one team, we are playing team based game mode, add totals
		//if (PlayerStateMaps.Num() > 1 && PlayerStateMaps[TeamNum].Num() > 0)
		//{
			// Horizontal Ruler
			ScoreboardData->AddSlot() .AutoHeight() .Padding(NORM_PADDING)
				[
					SNew(SBorder)
					.Padding(1)
					.BorderImage(&ScoreboardStyle->ItemBorderBrush)
				];
			ScoreboardData->AddSlot() .AutoHeight()
				[
					MakeTotalsRow(/*TeamNum*/)
				];
		//}
	}

	//if (MatchState >= ESimMatchState::Playing)
	//{
		//ScoreboardData->AddSlot() .AutoHeight() .Padding(NORM_PADDING)
		//	[
		//		SNew(SHorizontalBox)
		//		+SHorizontalBox::Slot() .HAlign(HAlign_Fill)
		//		[
		//			SNew(SBox)
		//			.HAlign(HAlign_Center)
		//			[
		//				SNew(STextBlock)
		//				.Text(this, &SOceanBoatsScoreboardWidget::GetMatchOutcomeText)
		//				.TextStyle(FArmySimStyle::Get(), "OceanBoats.MenuHeaderTextStyle")
		//			]
		//		]
		//	];

		//ScoreboardData->AddSlot() .AutoHeight() .Padding(NORM_PADDING)
		//	[
		//		SNew(SHorizontalBox)
		//		+SHorizontalBox::Slot() .HAlign(HAlign_Fill)
		//		[
		//			SNew(SBox)
		//			.HAlign(HAlign_Center)
		//			[
		//				SNew(STextBlock)
		//				.Text(this, &SOceanBoatsScoreboardWidget::GetMatchRestartText)
		//				.TextStyle(FArmySimStyle::Get(), "OceanBoats.MenuHeaderTextStyle")
		//			]
		//		]
		//	];

		//ScoreboardData->AddSlot() .AutoHeight() .Padding(NORM_PADDING)
		//	[
		//		SNew(SHorizontalBox)
		//		+SHorizontalBox::Slot() .HAlign(HAlign_Right)
		//		[
		//			SNew(SBox)
		//			.HAlign(HAlign_Right)
		//			[
		//				SNew(STextBlock)
		//				.Text(SoldierUIHelpers::Get().GetProfileOpenText())
		//				.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.StatTextStyle")
		//				.Visibility(this, &SOceanBoatsScoreboardWidget::GetProfileUIVisibility)
		//			]
		//		]
		//	];
	//}
}

void SOceanBoatsScoreboardWidget::UpdatePlayerStateMaps()
{
	if (PCOwner.IsValid())
	{
		ASoldierGameState* const GameState = PCOwner->GetWorld()->GetGameState<ASoldierGameState>();
		if (GameState)
		{
			bool bRequiresWidgetUpdate = false;
			const int32 NumTeams = FMath::Max(GameState->NumTeams, 10);
			LastTeamPlayerCount.Reset();
			LastTeamPlayerCount.AddZeroed(PlayerStateMaps.Num());
			for (int32 i = 0; i < PlayerStateMaps.Num(); i++)
			{
				LastTeamPlayerCount[i] = PlayerStateMaps[i].Num();
			}

			PlayerStateMaps.Reset();
			PlayerStateMaps.AddZeroed(NumTeams);
		
			for (int32 i = 0; i < NumTeams; i++)
			{
					GameState->GetRankedMap(i, PlayerStateMaps[i]);

				if (LastTeamPlayerCount.Num() > 0 && PlayerStateMaps[i].Num() != LastTeamPlayerCount[i])
				{
					bRequiresWidgetUpdate = true;
				}
			}

			if (bRequiresWidgetUpdate)
			{
				UpdateScoreboardGrid();
			}
		}
	}

	UpdateSelectedPlayer();
}

void SOceanBoatsScoreboardWidget::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	UpdatePlayerStateMaps();
}

bool SOceanBoatsScoreboardWidget::SupportsKeyboardFocus() const
{
#if INTERACTIVE_SCOREBOARD
	if (MatchState > ESimMatchState::Playing)
	{
		return true;
	}
#endif
	return false;
}

FReply SOceanBoatsScoreboardWidget::OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent )
{
	// @todo: may not want to affect all controllers if split screen

	// @todo: not-pc: need to support mouse focus too (alt+tabbing, windowed, etc)
	// @todo: not-pc: after each round, the mouse is released but as soon as you click it's recaptured and input stops working
	return FReply::Handled().ReleaseMouseCapture().SetUserFocus(SharedThis(this), EFocusCause::SetDirectly, true);
}

FReply SOceanBoatsScoreboardWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) 
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();
	if (MatchState > ESimMatchState::Playing)
	{
		if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up)
		{
			OnSelectedPlayerPrev();
			Result = FReply::Handled();
		}
		else if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down)
		{
			OnSelectedPlayerNext();
			Result = FReply::Handled();
		}
		else if (Key == EKeys::Enter || Key == EKeys::Virtual_Accept)
		{
			ProfileUIOpened();
			Result = FReply::Handled();
		}
		else if ((Key == EKeys::Escape || Key == EKeys::Gamepad_Special_Right || Key == EKeys::Global_Play || Key == EKeys::Global_Menu) && !InKeyEvent.IsRepeat())
		{
			// Allow the user to pause from the scoreboard still
			if (ASoldierPlayerController* const PC = Cast<ASoldierPlayerController>(PCOwner.Get()))
			{
				PC->OnToggleInGameMenu();
			}
			Result = FReply::Handled();
		}
	}
	return Result;
}

void SOceanBoatsScoreboardWidget::PlaySound(const FSlateSound& SoundToPlay) const
{
	if( PCOwner.IsValid() )
	{
		if( ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(PCOwner->Player) )
		{
			FSlateApplication::Get().PlaySound(SoundToPlay, LocalPlayer->GetControllerId());	
		}
	}
}

FReply SOceanBoatsScoreboardWidget::OnMouseOverPlayer(const FGeometry& Geometry, const FPointerEvent& Event, const FTeamBoats TeamPlayer)
{
#if INTERACTIVE_SCOREBOARD
	if( !(SelectedPlayer == TeamPlayer) )
	{
		SelectedPlayer = TeamPlayer;
		PlaySound(ScoreboardStyle->PlayerChangeSound);
	}
#endif
	return FReply::Handled();
}

void SOceanBoatsScoreboardWidget::OnSelectedPlayerPrev()
{
	// Make sure we have a valid index to start with
	if( !SelectedPlayer.IsValid() && !SetSelectedPlayerUs())
	{
		return;
	}

	// We don't want to decrease the player Id, we want to find the current one in the list and then select the one before it
	int32 PrevPlayerId = -1;
	for (RankedPlayerMap::TConstIterator PlayerIt(PlayerStateMaps[SelectedPlayer.TeamNum]); PlayerIt; ++PlayerIt)	// This would be easier with a .ReverseIterator call
	{
		const int32 PlayerId = PlayerIt.Key();
		if( PlayerId == SelectedPlayer.PlayerId )
		{
			break;
		}
		PrevPlayerId = PlayerId;
	}

	if( PrevPlayerId == -1 )
	{
		// If the id is still invalid, that means the current selection was first in their team, try the previous team...
		SelectedPlayer.TeamNum--;
		if (!PlayerStateMaps.IsValidIndex(SelectedPlayer.TeamNum))
		{
			// If there isn't a previous team, move to the last team
			SelectedPlayer.TeamNum = PlayerStateMaps.Num() - 1;
			check(PlayerStateMaps.IsValidIndex(SelectedPlayer.TeamNum));
		}

		// We want the last player in the team
		for (RankedPlayerMap::TConstIterator PlayerIt(PlayerStateMaps[SelectedPlayer.TeamNum]); PlayerIt; ++PlayerIt)	// This would be easier with a .Last call
		{
			const int32 PlayerId = PlayerIt.Key();
			PrevPlayerId = PlayerId;
		}
	}

	check( SelectedPlayer.PlayerId != -1 );
	SelectedPlayer.PlayerId = PrevPlayerId;
	PlaySound(ScoreboardStyle->PlayerChangeSound);
}

void SOceanBoatsScoreboardWidget::OnSelectedPlayerNext()
{
	// Make sure we have a valid index to start with
	if( !SelectedPlayer.IsValid() && !SetSelectedPlayerUs())
	{
		return;
	}

	// We don't want to increase the player Id, we want to find the current one in the list and then select the one after
	bool bNext = false;
	for (RankedPlayerMap::TConstIterator PlayerIt(PlayerStateMaps[SelectedPlayer.TeamNum]); PlayerIt; ++PlayerIt)
	{
		const int32 PlayerId = PlayerIt.Key();
		if( PlayerId == SelectedPlayer.PlayerId )
		{
			bNext = true;
		}
		else if( bNext == true )
		{
			SelectedPlayer.PlayerId = PlayerId;
			PlaySound(ScoreboardStyle->PlayerChangeSound);

			bNext = false;
			break;
		}
	}

	if( bNext == true )
	{
		// If next is still true, our current selection was last in their team, try the next team...
		SelectedPlayer.TeamNum++;
		if (!PlayerStateMaps.IsValidIndex(SelectedPlayer.TeamNum))
		{
			// If there isn't a next team, move to the first team
			SelectedPlayer.TeamNum = 0;
			check(PlayerStateMaps.IsValidIndex(SelectedPlayer.TeamNum));
		}

		SelectedPlayer.PlayerId = 0;
		PlaySound(ScoreboardStyle->PlayerChangeSound);
	}
}

void SOceanBoatsScoreboardWidget::ResetSelectedPlayer()
{
	SelectedPlayer = FTeamBoats();
}

void SOceanBoatsScoreboardWidget::UpdateSelectedPlayer()
{
	// Make sure the selected player is still valid...
	if( SelectedPlayer.IsValid() )
	{
		const ASoldierPlayerState* PlayerState = GetSortedPlayerState(SelectedPlayer);
		if( !PlayerState )
		{
			// Player is no longer valid, reset (note: reset implies 'us' in IsSelectedPlayer and IsPlayerSelectedAndValid)
			ResetSelectedPlayer();
		}
	}
}

bool SOceanBoatsScoreboardWidget::SetSelectedPlayerUs()
{
	ResetSelectedPlayer();

	// Set the owner player to be the default focused one
	if( APlayerController* const PC = PCOwner.Get() )
	{
		for (uint8 TeamNum = 0; TeamNum < PlayerStateMaps.Num(); TeamNum++)
		{
			for (RankedPlayerMap::TConstIterator PlayerIt(PlayerStateMaps[TeamNum]); PlayerIt; ++PlayerIt)
			{
				const TWeakObjectPtr<ASoldierPlayerState> PlayerState = PlayerIt.Value();
				if( PlayerState.IsValid() && PC->PlayerState && Cast<ASoldierPlayerState>(PC->PlayerState) == PlayerState.Get() )
				{
					SelectedPlayer = FTeamBoats(TeamNum, PlayerIt.Key());
					return true;
				}
			}
		}
	}
	return false;
}

bool SOceanBoatsScoreboardWidget::IsSelectedPlayer(const FTeamBoats& TeamPlayer) const
{
	if( !SelectedPlayer.IsValid() )
	{
		// If not explicitly set, test to see if the owner player was passed.
		if( IsOwnerPlayer(TeamPlayer) )
		{
			return true;
		}
	}
	else if( SelectedPlayer == TeamPlayer )
	{
		return true;
	}
	return false;
}

bool SOceanBoatsScoreboardWidget::IsPlayerSelectedAndValid() const
{
#if INTERACTIVE_SCOREBOARD
	if( !SelectedPlayer.IsValid() )
	{
		// Nothing is selected, default to the player
		if( PCOwner.IsValid() && PCOwner->PlayerState )
		{
			const TSharedPtr<const FUniqueNetId> OwnerNetId = PCOwner->PlayerState->UniqueId.GetUniqueNetId();
			return OwnerNetId.IsValid();
		}
	}
	else if( const ASoldierPlayerState* PlayerState = GetSortedPlayerState(SelectedPlayer) ) 
	{
		const TSharedPtr<const FUniqueNetId>& PlayerId = PlayerState->UniqueId.GetUniqueNetId();
		return PlayerId.IsValid();
	}
#endif
	return false;
}

EVisibility SOceanBoatsScoreboardWidget::GetProfileUIVisibility() const
{
	return IsPlayerSelectedAndValid() ? EVisibility::Visible : EVisibility::Hidden;
}

bool SOceanBoatsScoreboardWidget::ProfileUIOpened() const
{
	if( IsPlayerSelectedAndValid() )
	{
		check( PCOwner.IsValid() && PCOwner->PlayerState );
		const TSharedPtr<const FUniqueNetId>& OwnerNetId = PCOwner->PlayerState->UniqueId.GetUniqueNetId();
		check( OwnerNetId.IsValid() );

		const TSharedPtr<const FUniqueNetId>& PlayerId = ( !SelectedPlayer.IsValid() ? OwnerNetId : GetSortedPlayerState(SelectedPlayer)->UniqueId.GetUniqueNetId() );
		check( PlayerId.IsValid() );
		return SoldierUIHelpers::Get().ProfileOpenedUI(PCOwner->GetWorld(), *OwnerNetId.Get(), *PlayerId.Get(), NULL);
	}
	return false;
}

EVisibility SOceanBoatsScoreboardWidget::PlayerPresenceToItemVisibility(const FTeamBoats TeamPlayer) const
{
	return GetSortedPlayerState(TeamPlayer) ? EVisibility::Visible : EVisibility::Collapsed;
}

EVisibility SOceanBoatsScoreboardWidget::SpeakerIconVisibility(const FTeamBoats TeamPlayer) const
{
	ASoldierPlayerState* PlayerState = GetSortedPlayerState(TeamPlayer);
	if (PlayerState)
	{
		for (int32 i = 0; i < PlayersTalkingThisFrame.Num(); ++i)
		{
			if (PlayerState->UniqueId == PlayersTalkingThisFrame[i].Key && PlayersTalkingThisFrame[i].Value)
			{
				return EVisibility::Visible;
			}
		}
	}
	return EVisibility::Hidden;
}

FSlateColor SOceanBoatsScoreboardWidget::GetScoreboardBorderColor(int index) const
{
	const bool bIsSelected = false;// IsSelectedPlayer(TeamPlayer);
	const int32 RedTeam = 0;
	const float BaseValue = bIsSelected == true ? 0.15f : 0.0f;
	const float AlphaValue = bIsSelected == true ? 1.0f : 0.3f;
	/*float RedValue = TeamPlayer.TeamNum == RedTeam ? 0.25f : 0.0f;
	float BlueValue = TeamPlayer.TeamNum != RedTeam ? 0.25f : 0.0f;*/
	return FLinearColor(BaseValue, BaseValue, BaseValue , AlphaValue);
}

FText SOceanBoatsScoreboardWidget::GetPlayerName(const FTeamBoats TeamPlayer) const
{
	const ASoldierPlayerState* PlayerState = GetSortedPlayerState(TeamPlayer);
	if (PlayerState)
	{
		return FText::FromString(PlayerState->GetShortPlayerName());
	}

	return FText::GetEmpty();
}

//FText SOceanBoatsScoreboardWidget::GetBoatName(int index) const
//{
//	AActor* boat = IGameModeInterface::Execute_GetBoat(UGameplayStatics::GetGameMode(PCOwner->GetWorld()), index);
//	if(boat)
//	{
//		TSharedPtr<ASimEcs_Archetype> boatInstance = USimOceanSceneManager_Singleton::GetInstance()->FindArchetype(Cast<ASimEcs_Archetype>(boat)->EntId);
//		return FText::FromString(boatInstance->GetName());
//	}
//
//	return FText::GetEmpty();
//}
FText SOceanBoatsScoreboardWidget::GetObstacleDetail(ASimEcs_Archetype* boat, int archetype) const
{
	if (boat)
	{
		return FText::FromString(FString::FromInt(boat->GetDamageResult(archetype)));
	}

	return FText::GetEmpty();
}
bool SOceanBoatsScoreboardWidget::ShouldPlayerBeDisplayed(const FTeamBoats TeamPlayer) const
{
	const ASoldierPlayerState* PlayerState = GetSortedPlayerState(TeamPlayer);
	
	return PlayerState != nullptr && !PlayerState->bOnlySpectator;
}

FSlateColor SOceanBoatsScoreboardWidget::GetPlayerColor(/*const FTeamBoats TeamPlayer*/) const
{
	//// If this is the owner players row, tint the text color to show ourselves more clearly
	//if( IsOwnerPlayer(TeamPlayer) )
	//{
	//	return FSlateColor(FLinearColor::Yellow);
	//}

	const FTextBlockStyle& TextStyle = FArmySimStyle::Get().GetWidgetStyle<FTextBlockStyle>("OceanBoats.DefaultScoreboard.Row.StatTextStyle");
	return TextStyle.ColorAndOpacity;
}

FSlateColor SOceanBoatsScoreboardWidget::GetColumnColor(const FTeamBoats TeamPlayer, uint8 ColIdx) const
{
	return FSlateColor(FLinearColor::White);
	//// If this is the owner players row, tint the text color to show ourselves more clearly
	//if( IsOwnerPlayer(TeamPlayer) )
	//{
	//	return FSlateColor(FLinearColor::Yellow);
	//}

	//check(Columns.IsValidIndex(ColIdx));
	//return Columns[ColIdx].Color;
}

bool SOceanBoatsScoreboardWidget::IsOwnerPlayer(const FTeamBoats& TeamPlayer) const
{
	return ( PCOwner.IsValid() && PCOwner->PlayerState && PCOwner->PlayerState == GetSortedPlayerState(TeamPlayer) );
}

FText SOceanBoatsScoreboardWidget::GetStat() const
{
	int32 StatTotal = 0;
	for (auto boatInstance : USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes)
	{
		ASimEcs_Archetype* boat = boatInstance.Value.Get();
		if (boat == NULL || boat->ArchType >= EEE_COASTDEF_TYPE)
			continue;
		StatTotal += boat->GetTotalDamage();
	}

	return FText::AsNumber(LerpForCountup(StatTotal));
}

int32 SOceanBoatsScoreboardWidget::LerpForCountup(int32 ScoreValue) const
{
	if (MatchState > ESimMatchState::Playing)
	{
		const float LerpAmount = FMath::Min<float>(1.0f, (FPlatformTime::Seconds() - ScoreboardStartTime) / ScoreCountUpTime);
		return FMath::Lerp(0, ScoreValue, LerpAmount);
	}
	else
	{
		return ScoreValue;
	}
}

TSharedRef<SWidget> SOceanBoatsScoreboardWidget::MakeTotalsRow() const
{
	TSharedPtr<SHorizontalBox> TotalsRow;

	SAssignNew(TotalsRow, SHorizontalBox)
	+SHorizontalBox::Slot() .Padding(NORM_PADDING)
	[
		SNew(SBorder)
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.Padding(NORM_PADDING)
		.BorderImage(&ScoreboardStyle->ItemBorderBrush)
		.BorderBackgroundColor(ScoreboardTint)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ScoreTotal", "Total Damage"))
			.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.HeaderTextStyle") 
		]
	];

	////Leave two columns empty
	//for (uint8 i = 0; i < 2; i++)
	//{
	//	TotalsRow->AddSlot() .Padding(NORM_PADDING) .AutoWidth() .HAlign(HAlign_Center) .VAlign(VAlign_Center)
	//	[
	//		SNew(SBorder)
	//		.Padding(NORM_PADDING)
	//		.VAlign(VAlign_Center)
	//		.HAlign(HAlign_Center)
	//		.BorderImage(FStyleDefaults::GetNoBrush())
	//		.BorderBackgroundColor(ScoreboardTint)
	//		[
	//			SNew(SBox)
	//			.WidthOverride(ScoreBoxWidth)
	//			.HAlign(HAlign_Center)
	//		]
	//	];
	//}
	//Total team score / captures in CTF mode
	TotalsRow->AddSlot() .Padding(NORM_PADDING) .AutoWidth() .HAlign(HAlign_Center) .VAlign(VAlign_Center)
	[
		SNew(SBorder)
		.Padding(NORM_PADDING)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.BorderImage(&ScoreboardStyle->ItemBorderBrush)
		.BorderBackgroundColor(ScoreboardTint)
		[
			SNew(SBox)
			.WidthOverride(ScoreBoxWidth*2)
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(this, &SOceanBoatsScoreboardWidget::GetStat)
				.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.HeaderTextStyle")
			]
		]
	];

	return TotalsRow.ToSharedRef();
}

TSharedRef<SWidget> SOceanBoatsScoreboardWidget::MakePlayerRows(/*uint8 TeamNum*/) const
{
	TSharedRef<SVerticalBox> PlayerRows = SNew(SVerticalBox);

	/*for (int32 PlayerIndex=0; PlayerIndex < PlayerStateMaps[TeamNum].Num(); PlayerIndex++ )
	{
		FTeamBoats TeamPlayer(TeamNum, PlayerIndex);

		if (ShouldPlayerBeDisplayed(TeamPlayer))
		{
			PlayerRows->AddSlot().AutoHeight()
				[
					MakePlayerRow(TeamPlayer)
				];
		}
	}*/
			
	for (auto boatInformation : USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes)
	{
		ASimEcs_Archetype* boat = boatInformation.Value.Get();
		if (boat && boat->ArchType < EEE_COASTDEF_TYPE)
		{
			PlayerRows->AddSlot().AutoHeight()
				[
					MakePlayerRow(boat)
				];
		}

	}	

	return PlayerRows;
}

TSharedRef<SWidget> SOceanBoatsScoreboardWidget::MakePlayerRow(ASimEcs_Archetype* boat) const
{
	// Make the padding here slightly smaller than NORM_PADDING, to fit in more players
	const FMargin Pad = FMargin(5,1);

	TSharedPtr<SHorizontalBox> PlayerRow;
	//Speaker Icon display
	SAssignNew(PlayerRow, SHorizontalBox)
		+ SHorizontalBox::Slot().Padding(Pad + FMargin(2, 0, 0, 0)).AutoWidth()
		[
			SNew(SImage)
			.Image(&ScoreboardStyle->ItemHeaderBrush)
		//.Visibility(this, &SOceanBoatsScoreboardWidget::SpeakerIconVisibility, TeamPlayer)
		];

	//first autosized row with player name
	PlayerRow->AddSlot().Padding(Pad).AutoWidth()
		[
			SNew(SBorder)
			.Padding(Pad)
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Center)
		//.Visibility(this, &SOceanBoatsScoreboardWidget::PlayerPresenceToItemVisibility, TeamPlayer)
		//.OnMouseMove(const_cast<SOceanBoatsScoreboardWidget*>(this), &SOceanBoatsScoreboardWidget::OnMouseOverPlayer, TeamPlayer)
		.BorderBackgroundColor(FLinearColor(0.35, 0.35, 0.65, 0.5))
		.BorderImage(&ScoreboardStyle->ItemBorderBrush)
		[
			SNew(SBox)
			.WidthOverride(ScoreBoxWidth * 2)
		[
			SNew(STextBlock)
			.Text(FText::FromString(boat->GetName()))
				.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.StatTextStyle")
				.ColorAndOpacity(this, &SOceanBoatsScoreboardWidget::GetPlayerColor)
			]			
		]
	];
	//attributes rows (Obstacles)
	for (uint8 ColIdx = 0; ColIdx < Columns.Num(); ColIdx++)
	{
		int type = GetColumnType(int(ColIdx));

		PlayerRow->AddSlot()
		.Padding(Pad) .AutoWidth() .HAlign(HAlign_Center) .VAlign(VAlign_Center)
		[
			SNew(SBorder)
			.Padding(Pad)
			.VAlign(VAlign_Center)
			.HAlign(HAlign_Center)
			//.Visibility(this, &SOceanBoatsScoreboardWidget::PlayerPresenceToItemVisibility, TeamPlayer)
			//.OnMouseMove(const_cast<SOceanBoatsScoreboardWidget*>(this), &SOceanBoatsScoreboardWidget::OnMouseOverPlayer, TeamPlayer)
			.BorderBackgroundColor(FLinearColor(0.35, 0.35, 0.65, 0.5))
			.BorderImage(&ScoreboardStyle->ItemBorderBrush)
			[
				SNew(SBox)
				.WidthOverride(ScoreBoxWidth * 2)
				.HAlign(HAlign_Center)
				[
					SNew(STextBlock)
					.Text(this, &SOceanBoatsScoreboardWidget::GetObstacleDetail,boat, type)
					.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.StatTextStyle")
					.ColorAndOpacity(FSlateColor(FLinearColor::White))
				]
			]
		];
	}
	return PlayerRow.ToSharedRef();
}

ASoldierPlayerState* SOceanBoatsScoreboardWidget::GetSortedPlayerState(const FTeamBoats& TeamPlayer) const
{
	if (PlayerStateMaps.IsValidIndex(TeamPlayer.TeamNum) && PlayerStateMaps[TeamPlayer.TeamNum].Contains(TeamPlayer.PlayerId))
	{
		return PlayerStateMaps[TeamPlayer.TeamNum].FindRef(TeamPlayer.PlayerId).Get();
	}
	
	return NULL;
}

//int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Kills(ASoldierPlayerState* PlayerState) const
//{
//	return PlayerState->GetKills();
//}
//
//int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Deaths(ASoldierPlayerState* PlayerState) const
//{
//	return PlayerState->GetDeaths();
//}
//
//int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Score(ASoldierPlayerState* PlayerState) const
//{
//	return FMath::TruncToInt(PlayerState->Score);
//}

//������
int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_GTZ(class ASimEcs_Archetype* killer) const
{	
	//return PlayerState->GetKills(Obstacle_GTZ); //FMath::TruncToInt(1);
	if (killer)
		return killer->GetDamageResult(EEE_COASTDEF_GTZ_TYPE);
	return 0;
}

// ����׶
/** get attribute value for Barrier */
int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_SJZ( class ASimEcs_Archetype* killer) const {
	
	//return PlayerState->GetKills(Obstacle_STZ);// FMath::TruncToInt(1);
	if (killer)
		return killer->GetDamageResult(EEE_COASTDEF_SJZ_TYPE);
	return 0;
}

// ����ʯ
/** get attribute value for Barrier */
int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_HLS( class ASimEcs_Archetype* killer) const {
	////return GetWorld()->GetAuthGameMode<AOceanBoatsGameMode>()->GetDestroyNum(Obstacle_HLS);
	//return PlayerState->GetKills(Obstacle_HLS); //FMath::TruncToInt(1);
	if (killer)
		return killer->GetDamageResult(EEE_COASTDEF_HLS_TYPE);
	return 0;
}

// �ִ��
/** get attribute value for Barrier */
int32 SOceanBoatsScoreboardWidget::GetAttributeValue_Barrier_GCW(class ASimEcs_Archetype* killer) const {
	////return GetWorld()->GetAuthGameMode<AOceanBoatsGameMode>()->GetDestroyNum(Obstacle_HLS);
	//return PlayerState->GetKills(Obstacle_GCW); //FMath::TruncToInt(1);
	if (killer)
		return killer->GetDamageResult(EEE_COASTDEF_GCW_TYPE);
	return 0;
}

#undef LOCTEXT_NAMESPACE
