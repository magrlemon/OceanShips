// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SSoldierLeaderboard.h"
#include "ArmySimStyle.h"
#include "SoldierUIHelpers.h"
#include "OnlineSubsystemUtils.h"

#if PLATFORM_XBOXONE
#define INTERACTIVE_LEADERBOARD	1
#else
#define INTERACTIVE_LEADERBOARD	0
#endif

FLeaderboardRow::FLeaderboardRow(const FOnlineStatsRow& Row)
	: Rank(FString::FromInt(Row.Rank))
	, PlayerName(Row.NickName)
	, PlayerId(Row.PlayerId)
{
	if (const FVariantData* KillData = Row.Columns.Find(LEADERBOARD_STAT_KILLS))
	{
		int32 Val;
		KillData->GetValue(Val);
		Kills = FString::FromInt(Val);
	}

	if (const FVariantData* DeathData = Row.Columns.Find(LEADERBOARD_STAT_DEATHS))
	{
		int32 Val;
		DeathData->GetValue(Val);
		Deaths = FString::FromInt(Val);
	}
}

void SSoldierLeaderboard::Construct(const FArguments& InArgs)
{
	PlayerOwner = InArgs._PlayerOwner;
	OwnerWidget = InArgs._OwnerWidget;
	const int32 BoxWidth = 125;
	bReadingStats = false;

	LeaderboardReadCompleteDelegate = FOnLeaderboardReadCompleteDelegate::CreateRaw(this, &SSoldierLeaderboard::OnStatsRead);

	ChildSlot
	.VAlign(VAlign_Fill)
	.HAlign(HAlign_Fill)
	[		
		SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBox)  
			.WidthOverride(600)
			.HeightOverride(600)
			[
				SAssignNew(RowListWidget, SListView< TSharedPtr<FLeaderboardRow> >)
				.ItemHeight(20)
				.ListItemsSource(&StatRows)
				.SelectionMode(ESelectionMode::Single)
				.OnGenerateRow(this, &SSoldierLeaderboard::MakeListViewWidget)
				.OnSelectionChanged(this, &SSoldierLeaderboard::EntrySelectionChanged)
				.HeaderRow(
				SNew(SHeaderRow)
				+ SHeaderRow::Column("Rank").FixedWidth(BoxWidth/3) .DefaultLabel(NSLOCTEXT("LeaderBoard", "PlayerRankColumn", "Rank"))
				+ SHeaderRow::Column("PlayerName").FixedWidth(BoxWidth*2) .DefaultLabel(NSLOCTEXT("LeaderBoard", "PlayerNameColumn", "Equipment Name"))
				+ SHeaderRow::Column("Kills") .DefaultLabel(NSLOCTEXT("LeaderBoard", "KillsColumn", "Kills"))
				+ SHeaderRow::Column("Deaths") .DefaultLabel(NSLOCTEXT("LeaderBoard", "DeathsColumn", "Deaths")))
			]
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		.HAlign(HAlign_Right)
		[
			SNew(STextBlock)
			.Text(SoldierUIHelpers::Get().GetProfileOpenText())
			.TextStyle(FArmySimStyle::Get(), "OceanBoats.ScoreboardListTextStyle")
			.Visibility(this, &SSoldierLeaderboard::GetProfileUIVisibility)
		]
	];
}

UWorld* SSoldierLeaderboard::GetWorld() const
{
	return PlayerOwner.IsValid() ? PlayerOwner->GetWorld() : nullptr;
}

void SSoldierLeaderboard::ReadStatsLoginRequired()
{
	if (!OnLoginCompleteDelegateHandle.IsValid())
	{
		IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
		if (OnlineSub)
		{
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface();
			if (Identity.IsValid())
			{
				OnLoginCompleteDelegateHandle = Identity->AddOnLoginCompleteDelegate_Handle(0, FOnLoginCompleteDelegate::CreateRaw(this, &SSoldierLeaderboard::OnLoginCompleteReadStats));
				Identity->Login(0, FOnlineAccountCredentials());
			}
		}
	}
}

void SSoldierLeaderboard::OnLoginCompleteReadStats(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	Online::GetIdentityInterface(GetWorld())->ClearOnLoginCompleteDelegate_Handle(LocalUserNum, OnLoginCompleteDelegateHandle);
	if (bWasSuccessful)
	{
		ReadStats();
	}
}

/** Starts reading leaderboards for the game */
void SSoldierLeaderboard::ReadStats()
{
	StatRows.Reset();

	IOnlineSubsystem* const OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
		if (Leaderboards.IsValid())
		{
			// We are about to read the stats. The delegate will set this to false once the read is complete.
			LeaderboardReadCompleteDelegateHandle = Leaderboards->AddOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegate);
			bReadingStats = true;

			// There's no reason to request leaderboard requests while one is in progress, so only do it if there isn't one active.
			if (!IsLeaderboardReadInProgress())
			{
				//ReadObject = MakeShareable(new FSoldierAllTimeMatchResultsRead());
				//FOnlineLeaderboardReadRef ReadObjectRef = ReadObject.ToSharedRef();
				//bReadingStats = Leaderboards->ReadLeaderboardsForFriends(0, ReadObjectRef);
			}
		}
		else
		{
			// TODO: message the user?
		}
	}
}

/** Called on a particular leaderboard read */
void SSoldierLeaderboard::OnStatsRead(bool bWasSuccessful)
{
	// It's possible for another read request to happen while another one is ongoing (such as when the player leaves the menu and
	// re-enters quickly). We only want to process a leaderboard read if our read object is done.
	if (!IsLeaderboardReadInProgress())
	{
		ClearOnLeaderboardReadCompleteDelegate();

		if (bWasSuccessful)
		{
			for (int Idx = 0; Idx < ReadObject->Rows.Num(); ++Idx)
			{
				TSharedPtr<FLeaderboardRow> NewLeaderboardRow = MakeShareable(new FLeaderboardRow(ReadObject->Rows[Idx]));

				StatRows.Add(NewLeaderboardRow);
			}

			RowListWidget->RequestListRefresh();
		}

		bReadingStats = false;
	}
}

void SSoldierLeaderboard::OnFocusLost( const FFocusEvent& InFocusEvent )
{
	if (InFocusEvent.GetCause() != EFocusCause::SetDirectly)
	{
		FSlateApplication::Get().SetKeyboardFocus(SharedThis( this ));
	}
}

FReply SSoldierLeaderboard::OnFocusReceived(const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent)
{
	return FReply::Handled().SetUserFocus(RowListWidget.ToSharedRef(), EFocusCause::SetDirectly);
}

void SSoldierLeaderboard::EntrySelectionChanged(TSharedPtr<FLeaderboardRow> InItem, ESelectInfo::Type SelectInfo)
{
	SelectedItem = InItem;
}

bool SSoldierLeaderboard::IsPlayerSelectedAndValid() const
{
#if INTERACTIVE_LEADERBOARD
	if (SelectedItem.IsValid())
	{
		check(SelectedItem->PlayerId.IsValid());
		return true;
	}
#endif
	return false;
}

EVisibility SSoldierLeaderboard::GetProfileUIVisibility() const
{	
	return IsPlayerSelectedAndValid() ? EVisibility::Visible : EVisibility::Hidden;
}

bool SSoldierLeaderboard::ProfileUIOpened() const
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

void SSoldierLeaderboard::MoveSelection(int32 MoveBy)
{
	int32 SelectedItemIndex = StatRows.IndexOfByKey(SelectedItem);

	if (SelectedItemIndex+MoveBy > -1 && SelectedItemIndex+MoveBy < StatRows.Num())
	{
		RowListWidget->SetSelection(StatRows[SelectedItemIndex+MoveBy]);
	}
}

FReply SSoldierLeaderboard::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) 
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
			ClearOnLeaderboardReadCompleteDelegate();
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

TSharedRef<ITableRow> SSoldierLeaderboard::MakeListViewWidget(TSharedPtr<FLeaderboardRow> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	class SLeaderboardRowWidget : public SMultiColumnTableRow< TSharedPtr<FLeaderboardRow> >
	{
	public:
		SLATE_BEGIN_ARGS(SLeaderboardRowWidget){}
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FLeaderboardRow> InItem)
		{
			Item = InItem;
			SMultiColumnTableRow< TSharedPtr<FLeaderboardRow> >::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			FText ItemText = FText::GetEmpty();
			if (ColumnName == "Rank")
			{
				ItemText = FText::FromString(Item->Rank);
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
			else if (ColumnName == "Kills")
			{
				ItemText = FText::FromString(Item->Kills);
			}
			else if (ColumnName == "Deaths")
			{
				ItemText = FText::FromString(Item->Deaths);
			}
			return SNew(STextBlock)
				.Text(ItemText)
				.TextStyle(FArmySimStyle::Get(), "OceanBoats.ScoreboardListTextStyle");		
		}
		TSharedPtr<FLeaderboardRow> Item;
	};
	return SNew(SLeaderboardRowWidget, OwnerTable, Item);
}

void SSoldierLeaderboard::ClearOnLeaderboardReadCompleteDelegate()
{
	IOnlineSubsystem* OnlineSub = Online::GetSubsystem(GetWorld());
	if (OnlineSub)
	{
		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface();
		if (Leaderboards.IsValid())
		{
			Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle(LeaderboardReadCompleteDelegateHandle);
		}
	}
}

bool SSoldierLeaderboard::IsLeaderboardReadInProgress()
{
	return ReadObject.IsValid() && (ReadObject->ReadState == EOnlineAsyncTaskState::InProgress || ReadObject->ReadState == EOnlineAsyncTaskState::NotStarted);
}
