// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SBoatBoard.h"
#include "ArmySimStyle.h"
#include "SoldierUIHelpers.h"
#include "OnlineSubsystemUtils.h"
#include "GameModeInterface.h"
#include "SoldierPlayerState.h"
#include "Interface/BoatInterface.h"
#include "SimEcs_Archetype.h"
#include "ArmySimMenuItemWidgetStyle.h"
#include "Sim_OceanSceneManager.h"
#include "SoldierMenuWidgetStyle.h"

#define TSHIP_FONT( RelativePath, ... ) FSlateFontInfo( FPaths::ProjectContentDir() / "Slate"/ RelativePath + TEXT(".ttf"), __VA_ARGS__ )
//FBoatboardRow::FBoatboardRow(const FOnlineStatsRow& Row)
//	: Rank(FString::FromInt(Row.Rank))
//	, PlayerName(Row.NickName)
//	, PlayerId(Row.PlayerId)
//{
//	if (const FVariantData* KillData = Row.Columns.Find(LEADERBOARD_STAT_KILLS))
//	{
//		int32 Val;
//		KillData->GetValue(Val);
//		Kills = FString::FromInt(Val);
//	}
//
//	if (const FVariantData* DeathData = Row.Columns.Find(LEADERBOARD_STAT_DEATHS))
//	{
//		int32 Val;
//		DeathData->GetValue(Val);
//		Deaths = FString::FromInt(Val);
//	}
//}

void SBoatBoard::Construct(const FArguments& InArgs)
{
	PlayerOwner = InArgs._PlayerOwner;
	//OwnerWidget = InArgs._OwnerWidget;
	const int32 BoxWidth = 1500;
	bReadingStats = false;

	//LeaderboardReadCompleteDelegate = FOnLeaderboardReadCompleteDelegate::CreateRaw(this, &SBoatBoard::OnStatsRead);
	////ScoreboardStyle = &FArmySimStyle::Get().GetWidgetStyle<FSoldierScoreboardStyle>("DefaultSoldierScoreboardStyle");

	const FSoldierMenuStyle* ItemStyle = &FArmySimStyle::Get( ).GetWidgetStyle<FSoldierMenuStyle>( "DefaultSoldierMenuStyle" );

	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[		
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)  
			.WidthOverride(BoxWidth)
			.HeightOverride(600)			
			[
				SNew(SBorder)
				.BorderBackgroundColor(FLinearColor(0.2f, 0.2f, 0.2f, 0.4f))
				.BorderImage(&ItemStyle->LeftBackgroundBrush )
				[
					SAssignNew(RowListWidget, SListView< TSharedPtr<FBoatboardRow> >)
					.ItemHeight(50)
					.ListItemsSource(&StatRows)
					.SelectionMode(ESelectionMode::Single)
					.OnGenerateRow(this, &SBoatBoard::MakeListViewWidget)
					.OnSelectionChanged(this, &SBoatBoard::EntrySelectionChanged)
					
					.HeaderRow(
						SNew(SHeaderRow)
						.Style(FArmySimStyle::Get(), "OceanBoats.Row.HeaderRowStyle")
						+ SHeaderRow::Column("PlayerName").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "PlayerNameColumn", "BoatName"))
						+ SHeaderRow::Column("Speed").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "PlayerSpeedColumn", "Speed"))
						+ SHeaderRow::Column("SailAngle").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "SailAngleColumn", "Sail Angle"))
						+ SHeaderRow::Column("SailDistance").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "SailDistanceColumn", "Sail Distance"))
						+ SHeaderRow::Column("SpeedDownDistance").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "SpeedDownDistanceColumn", "SpeedDown Distance"))
						+ SHeaderRow::Column("HorizontalDistance").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "HorizontalDistanceColumn", "Horizontal Distance"))
						+ SHeaderRow::Column("FlashingLocation").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "FlashingLocationColumn", "Flashing Location"))
						+ SHeaderRow::Column("FlashingTime").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "FlashingTimeColumn", "Flashing Time"))
						+ SHeaderRow::Column("AttackDistance").FixedWidth(BoxWidth / 9).DefaultLabel(NSLOCTEXT("LeaderBoard", "AttackDistance", "Distance to AttackPos"))
					)
				]
				
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		[
			SNew(STextBlock)
			.Text(SoldierUIHelpers::Get().GetProfileOpenText())
			.TextStyle(FArmySimStyle::Get(), "OceanBoats.ScoreboardListTextStyle")
			.Visibility(this, &SBoatBoard::GetProfileUIVisibility)
		]
	];

	OnStatsRead(true);


	FSlateApplication::Get( ).PlaySound( ItemStyle->MenuEnterSound, GetOwnerUserIndex() );
}
void SBoatBoard::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	SWidget::Tick(AllottedGeometry, InCurrentTime, InDeltaTime);

	OnStatsRead(true);
}

UWorld* SBoatBoard::GetWorld() const
{
	return PlayerOwner.IsValid() ? PlayerOwner->GetWorld() : nullptr;
}
//
//void SBoatBoard::ReadStatsLoginRequired()
//{
//	if (!OnLoginCompleteDelegateHandle.IsValid())
//	{
//		IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
//		if (OnlineSub)
//		{
//			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
//			if (Identity.IsValid())
//			{
//				OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateRaw(this, &SBoatBoard::OnLoginCompleteReadStats));
//				Identity->Login(0, FOnlineAccountCredentials());
//			}
//		}
//	}
//}
//
//void SBoatBoard::OnLoginCompleteReadStats(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
//{
//	Online::GetIdentityInterface(GetWorld())->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
//	if (bWasSuccessful)
//	{
//		ReadStats();
//	}
//}
//
///** Starts reading leaderboards for the game */
//void SBoatBoard::ReadStats()
//{
//	StatRows.Reset();
//
//	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
//	if (OnlineSub)
//	{
//		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
//		if (Leaderboards.IsValid())
//		{
//			// We are about to read the stats. The delegate will set this to false once the read is complete.
//			LeaderboardReadCompleteDelegateHandle = Leaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegate);
//			bReadingStats = true;
//
//			// There's no reason to request leaderboard requests while one is in progress, so only do it if there isn't one active.
//			if (!IsLeaderboardReadInProgress())
//			{
//				//ReadObject = MakeShareable(new FSoldierAllTimeMatchResultsRead());
//				//FOnlineLeaderboardReadRef ReadObjectRef = ReadObject.ToSharedRef();
//				//bReadingStats = Leaderboards->ReadLeaderboardsForFriends(0, ReadObjectRef);
//			}
//		}
//		else
//		{
//			// TODO: message the user?
//		}
//	}
//}

/** Called on a particular leaderboard read */
void SBoatBoard::OnStatsRead(bool bWasSuccessful)
{
	//// It's possible for another read request to happen while another one is ongoing (such as when the player leaves the menu and
	//// re-enters quickly). We only want to process a leaderboard read if our read object is done.
	//if (!IsLeaderboardReadInProgress())
	//{
	//	ClearOnLeaderboardReadCompleteDelegate();

	//	if (bWasSuccessful)
	//	{
	//		for (int Idx = 0; Idx < ReadObject->Rows.Num(); ++Idx)
	//		{
	//			TSharedPtr<FBoatboardRow> NewLeaderboardRow = MakeShareable(new FBoatboardRow(ReadObject->Rows[Idx]));

	//			StatRows.Add(NewLeaderboardRow);
	//		}

	//		RowListWidget->RequestListRefresh();
	//	}

	//	bReadingStats = false;
	//}
	StatRows.Empty();

	FNumberFormattingOptions NumberFormatOptions;
	NumberFormatOptions.MinimumFractionalDigits = 1;
	NumberFormatOptions.MaximumFractionalDigits = 3;

	UWorld* gWorld = GetWorld();
	ASoldierPlayerState* playerState = Cast<ASoldierPlayerState>(gWorld->GetPlayerControllerIterator()->Get()->PlayerState);
	//int num = playerState->GetBoats();

	for(auto boatInstance : USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes)
	{
		ASimEcs_Archetype* boat = boatInstance.Value.Get();
		if (boat == NULL || boat->ArchType >= EEE_COASTDEF_TYPE)
			continue;
	//}
	
	/*for (int i = 0; i < num; ++i)
	{*/
		//ASimEcs_Archetype* boat = Cast<ASimEcs_Archetype>(playerState->GetBoat(i));
		TSharedPtr<FBoatboardRow> NewRow = MakeShareable(new FBoatboardRow(boat->GetName()));
		//NewRow->Kills = FString::FromInt(playerState->GetKilledBy(boat->GetUniqueID()));

		NewRow->Speed = FText::AsNumber(boat->Speed * 0.01f, &NumberFormatOptions).ToString() + FString("m");
		//FString::SanitizeFloat(IBoatInterface::Execute_GetSpeed(boat) * 0.01f,3) + FString("m/s");
		NewRow->SailAngle = FString::SanitizeFloat(boat->SailAngle);
		NewRow->SailDistance = FText::AsNumber(boat->SailDistance * 0.01f, &NumberFormatOptions).ToString() + FString("m");
		//FString::SanitizeFloat(IBoatInterface::Execute_GetSailDistance(boat) * 0.01f,3) + FString("m");
		NewRow->SpeedDownDistance = FText::AsNumber(boat->SpeedDownDistance * 0.01f, &NumberFormatOptions).ToString() + FString("m");
			//FString::SanitizeFloat(IBoatInterface::Execute_GetSpeedDownDistance(boat) * 0.01f,3) + FString("m");
		NewRow->HorizontalDistance = FString::SanitizeFloat(boat->HorizontalDistance * 0.01f);
		NewRow->RollbackDistance = FText::AsNumber(boat->RollbackDistance * 0.01f, &NumberFormatOptions).ToString() + FString("m");
			//FString::SanitizeFloat(IBoatInterface::Execute_GetRollbackDistance(boat) * 0.01f,3) + FString("m");
		NewRow->RollbackAngle = FString::SanitizeFloat(boat->RollbackAngle);		
		NewRow->DistAttack = FString::SanitizeFloat(FVector::Dist2D(boat->AttackPos, boat->GetActorLocation()));

		StatRows.Add(NewRow);
	}
	RowListWidget->RequestListRefresh();
}

void SBoatBoard::OnFocusLost( const FFocusEvent& InFocusEvent )
{
	if (InFocusEvent.GetCause() != EFocusCause::SetDirectly)
	{
		FSlateApplication::Get().SetKeyboardFocus(SharedThis( this ));
	}
}

FReply SBoatBoard::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(RowListWidget.ToSharedRef(), EFocusCause::SetDirectly);
}

void SBoatBoard::EntrySelectionChanged(TSharedPtr<FBoatboardRow> InItem, ESelectInfo::Type SelectInfo)
{
	SelectedItem = InItem;
}

bool SBoatBoard::IsPlayerSelectedAndValid() const
{
	return false;
}

EVisibility SBoatBoard::GetProfileUIVisibility() const
{	
	return IsPlayerSelectedAndValid() ? EVisibility::Visible : EVisibility::Hidden;
}

bool SBoatBoard::ProfileUIOpened() const
{
	if( IsPlayerSelectedAndValid() )
	{
		check( PlayerOwner.IsValid() );
		FUniqueNetIdRepl OwnerNetId = PlayerOwner->GetPreferredUniqueNetId();
		check( OwnerNetId.IsValid() );

		const TSharedPtr<const FUniqueNetId>& PlayerId = SelectedItem->PlayerId;
		check( PlayerId.IsValid() );
		return SoldierUIHelpers::Get().ProfileOpenedUI(GetWorld(), *OwnerNetId, *PlayerId.Get(), NULL);
	}
	return false;
}

void SBoatBoard::MoveSelection(int32 MoveBy)
{
	int32 SelectedItemIndex = StatRows.IndexOfByKey(SelectedItem);

	if (SelectedItemIndex+MoveBy > -1 && SelectedItemIndex+MoveBy < StatRows.Num())
	{
		RowListWidget->SetSelection(StatRows[SelectedItemIndex+MoveBy]);
	}
}

FReply SBoatBoard::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	FReply Result = FReply::Unhandled();
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up)
	{
		MoveSelection(-1);
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down)
	{
		MoveSelection(1);
		Result = FReply::Handled();
	}
	else if (Key == EKeys::Escape || Key == EKeys::Virtual_Back || Key == EKeys::Gamepad_Special_Left)
	{
		if (bReadingStats)
		{
			//ClearOnLeaderboardReadCompleteDelegate();
			bReadingStats = false;
		}
	}
	else if (Key == EKeys::Enter || Key == EKeys::Virtual_Accept)
	{
		// Open the profile UI of the selected item
		ProfileUIOpened();
		Result = FReply::Handled();
		FSlateApplication::Get().SetKeyboardFocus(SharedThis(this));
	}
	return Result;
}

TSharedRef<ITableRow> SBoatBoard::MakeListViewWidget(TSharedPtr<FBoatboardRow> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SLeaderboardRowWidget : public SMultiColumnTableRow< TSharedPtr<FBoatboardRow> >
	{
	public:
		SLATE_BEGIN_ARGS(SLeaderboardRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FBoatboardRow> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow< TSharedPtr<FBoatboardRow> >::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			FText ItemText = FText::GetEmpty();
			if (ColumnName == "SailDistance")
			{
				ItemText = FText::FromString(Item->SailDistance);
			}
			else if (ColumnName == "PlayerName")
			{
				if (Item->PlayerName.Len() > MAX_PLAYER_NAME_LENGTH)
				{
					ItemText = FText::FromString(Item->PlayerName.Left(MAX_PLAYER_NAME_LENGTH) + "...");
				}
				else
				{
					ItemText = FText::FromString(Item->PlayerName);
				}
			}
			else if (ColumnName == "SailAngle")
			{
				ItemText = FText::FromString(Item->SailAngle);
			}
			else if (ColumnName == "Speed")
			{
				ItemText = FText::FromString(Item->Speed);
			}
			else if (ColumnName == "SpeedDownDistance")
			{
				ItemText = FText::FromString(Item->SpeedDownDistance);
			}
			else if (ColumnName == "HorizontalDistance")
			{
				ItemText = FText::FromString(Item->HorizontalDistance);
			}
			else if (ColumnName == "RollbackDistance")
			{
				ItemText = FText::FromString(Item->RollbackDistance);
			}
			else if (ColumnName == "RollbackAngle")
			{
				ItemText = FText::FromString(Item->RollbackAngle);
			}
			else if (ColumnName == "AttackDistance")
			{
				ItemText = FText::FromString(Item->DistAttack);
			}
			return SNew(STextBlock)
				.Text(ItemText)
				//.Font(TSHIP_FONT("Fonts/msyh-Black", 24))
				.TextStyle(FArmySimStyle::Get(), "OceanBoats.DefaultScoreboard.Row.HeaderTextStyle");		
		}
		TSharedPtr<FBoatboardRow> Item;
	};
	return SNew(SLeaderboardRowWidget, OwnerTable, Item);
}

//void SBoatBoard::ClearOnLeaderboardReadCompleteDelegate()
//{
//	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
//	if (OnlineSub)
//	{
//		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
//		if (Leaderboards.IsValid())
//		{
//			Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegateHandle);
//		}
//	}
//}

bool SBoatBoard::IsLeaderboardReadInProgress()
{
	return ReadObject.IsValid() && (ReadObject->ReadState == EOnlineAsyncTaskState::InProgress || ReadObject->ReadState == EOnlineAsyncTaskState::NotStarted);
}
