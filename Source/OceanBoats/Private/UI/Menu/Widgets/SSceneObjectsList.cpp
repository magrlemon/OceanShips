// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "SSceneObjectsList.h"
#include "SHeaderRow.h"
#include "ArmySimStyle.h"
#include "CoreStyle.h"
#include "ArmyGameLoadingScreen.h"
#include "ArmySimGameInstance.h"
#include "NetworkReplayStreaming.h"
#include "SimDataStructure.h"
#include "Sim_OceanSceneManager.h"
#include "ArmySimGameViewportClient.h"

#define LOCTEXT_NAMESPACE "ArmySim.HUD.Menu"




struct FSceneObjectsEntry {
	FActorsStreamInfo StreamInfo;
	FString		Date;
	FString		Size;
	int32		ResultsIndex;
};

void SSceneObjectsList::Construct( const FArguments& InArgs )
{
	PlayerOwner = InArgs._PlayerOwner;
	OwnerWidget = InArgs._OwnerWidget;
	bUpdatingSceneObjectsList = false;
	StatusText = FText::GetEmpty( );

	EnumerateStreamsVersion = FNetworkVersion::GetReplayVersion( );

	const int32 NameWidth = 280;
	const int32 ViewersWidth = 64;
	const int32 DateWidth = 210;
	const int32 LengthWidth = 64;


	const FSoldierMenuStyle* ItemStyle = &FArmySimStyle::Get( ).GetWidgetStyle<FSoldierMenuStyle>( "DefaultSoldierMenuStyle" );

	ChildSlot
		.VAlign( VAlign_Fill )
		.HAlign( HAlign_Fill )
		[
			/*SNew( SBorder )
			.BorderBackgroundColor( FLinearColor( 0.2f, 0.2f, 0.2f, 0.4f ) )
			.BorderImage( &ItemStyle->LeftBackgroundBrush )
			[*/
				SNew( SVerticalBox )
				+ SVerticalBox::Slot( )
				.HAlign( HAlign_Left )
				.Padding( FMargin( 0.0f, 50.0f, 0.0f, 18.0f ) )
				[
					SNew( SCheckBox )
					.IsChecked( this, &SSceneObjectsList::IsShowAllReplaysChecked )
					.OnCheckStateChanged( this, &SSceneObjectsList::OnShowAllReplaysChecked )
					.Style( FCoreStyle::Get( ), "Checkbox" )
					[
						SNew( STextBlock )
						.Text( LOCTEXT( "Show Barriers", "Show Barriers from all Scene" ) )
					.TextStyle( FArmySimStyle::Get( ), "SoldierGame.DemoListCheckboxTextStyle" )
					]
				]
				+ SVerticalBox::Slot( )
				.AutoHeight( )
				[
					SNew( SBox )
					.WidthOverride( 800 )
					.HeightOverride( 600 )
					[
			
						SAssignNew( ObjectsListWidget, SListView<TSharedPtr<FSceneObjectsEntry>> )
						.ItemHeight( 60 )
						.ListItemsSource( &SceneObjectsList )
						.SelectionMode( ESelectionMode::Single )
						.OnGenerateRow( this, &SSceneObjectsList::MakeListViewWidget )
						.OnSelectionChanged( this, &SSceneObjectsList::EntrySelectionChanged )
						.OnMouseButtonDoubleClick( this, &SSceneObjectsList::OnListItemDoubleClicked )
						.HeaderRow(
							SNew( SHeaderRow )
							+ SHeaderRow::Column( "ObjectName" ).FixedWidth( NameWidth ).DefaultLabel( NSLOCTEXT( "ObjectList", "ObjectNameColumn", "Object Name" ) )
							+ SHeaderRow::Column( "Viewers" ).FixedWidth( ViewersWidth ).DefaultLabel( NSLOCTEXT( "Viewers", "ViewersColumn", "Viewers" ) )
							+ SHeaderRow::Column( "Date" ).FixedWidth( DateWidth ).DefaultLabel( NSLOCTEXT( "DemoList", "DateColumn", "Date" ) )
							+ SHeaderRow::Column( "Length" ).FixedWidth( LengthWidth ).DefaultLabel( NSLOCTEXT( "Length", "LengthColumn", "Length" ) )
							+ SHeaderRow::Column( "Size" ).HAlignHeader( HAlign_Left ).HAlignCell( HAlign_Right ).DefaultLabel( NSLOCTEXT( "DemoList", "SizeColumn", "Size" ) ) )
					]
				]
			
	
			+ SVerticalBox::Slot( )
				.AutoHeight( )
				[
					SNew( SOverlay )
					+ SOverlay::Slot( )
					.VAlign( VAlign_Center )
					.HAlign( HAlign_Center )
					[
						SNew( STextBlock )
						.Text( this, &SSceneObjectsList::GetBottomText )
						.TextStyle( FArmySimStyle::Get( ), "SoldierGame.MenuServerListTextStyle" )
					]
				]
			
		];


	ReplayStreamer = FNetworkReplayStreaming::Get( ).GetFactory( ).CreateReplayStreamer( );

	BuildSceneObjectsList( );
	FSlateApplication::Get( ).PlaySound( ItemStyle->MenuEnterSound, GetOwnerUserIndex( ) );
}

void SSceneObjectsList::OnEnumerateStreamsComplete( const FEnumerateObjectsStreamsResult& Result )
{
	check( bUpdatingSceneObjectsList ); // should not be called otherwise

	bool bFinished = true;

	for (const auto& StreamInfo : Result.FoundObjectsStreams) {
		//float SizeInKilobytes = StreamInfo.SizeInBytes / 1024.0f;

		TSharedPtr<FSceneObjectsEntry> NewDemoEntry = MakeShareable( new FSceneObjectsEntry( ) );
		NewDemoEntry->StreamInfo = StreamInfo;
		NewDemoEntry->Date = StreamInfo.Timestamp.ToString( TEXT( "%m/%d/%Y %h:%M %A" ) );	// UTC time

		SceneObjectsList.Add( NewDemoEntry );
	}

	// Sort demo names by date
	struct FCompareDateTime {
		FORCEINLINE bool operator()( const TSharedPtr<FSceneObjectsEntry> & A, const TSharedPtr<FSceneObjectsEntry> & B ) const
		{
			return A->StreamInfo.Name > B->StreamInfo.Name;
		}
	};

	Sort( SceneObjectsList.GetData( ), SceneObjectsList.Num( ), FCompareDateTime( ) );

	//StatusText = "";
	StatusText = LOCTEXT( "DemoSelectionInfo", "Press ENTER to Play. Press DEL to delete." );

	if (bFinished) {
		OnBuildSceneObjectsListFinished( );
	}
}

FText SSceneObjectsList::GetBottomText( ) const
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
void SSceneObjectsList::Tick( const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime )
{
	SCompoundWidget::Tick( AllottedGeometry, InCurrentTime, InDeltaTime );
}

ECheckBoxState SSceneObjectsList::IsShowAllReplaysChecked( ) const
{
	if (EnumerateStreamsVersion.NetworkVersion == 0 && EnumerateStreamsVersion.Changelist == 0) {
		return ECheckBoxState::Checked;
	}

	return ECheckBoxState::Unchecked;
}

void SSceneObjectsList::OnShowAllReplaysChecked( ECheckBoxState NewCheckedState )
{
	EnumerateStreamsVersion = FNetworkVersion::GetReplayVersion( );

	// Always set CL to 0, we only want to ever check NetworkVersion (now that we have backwards compat)
	EnumerateStreamsVersion.Changelist = 0;

	if (NewCheckedState == ECheckBoxState::Checked) {
		EnumerateStreamsVersion.NetworkVersion = 0;
	}

	BuildSceneObjectsList( );
}

/** Populates the demo list */
void SSceneObjectsList::BuildSceneObjectsList( )
{
	bUpdatingSceneObjectsList = true;
	SceneObjectsList.Empty( );


	USimOceanSceneManager_Singleton::GetInstance( )->EnumerateObjectsStreams(
		FEnumerateObjectsStreamsCallback::CreateSP( this, &SSceneObjectsList::OnEnumerateStreamsComplete ) );
}

/** Called when demo list building is finished */
void SSceneObjectsList::OnBuildSceneObjectsListFinished( )
{
	bUpdatingSceneObjectsList = false;

	int32 SelectedItemIndex = SceneObjectsList.IndexOfByKey( SelectedItem );

	ObjectsListWidget->RequestListRefresh( );
	if (SceneObjectsList.Num( ) > 0) {
		ObjectsListWidget->UpdateSelectionSet( );
		ObjectsListWidget->SetSelection( SceneObjectsList[SelectedItemIndex > -1 ? SelectedItemIndex : 0], ESelectInfo::OnNavigation );
	}
}

void SSceneObjectsList::LocateObjects( )
{
	if (bUpdatingSceneObjectsList) {
		// We're still building the list
		return;
	}

	if (SelectedItem.IsValid( )) {
		UArmySimGameInstance* const GI = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance( ) );

		if (GI != NULL) {
			ASoldierPlayerController* const PrimaryPC = Cast<ASoldierPlayerController>( GI->GetFirstLocalPlayerController( ) );
			FString ActorName = SelectedItem->StreamInfo.Name;
			if (ActorName.IsEmpty( ))return;
			APlayerCameraManager* camMgr = UGameplayStatics::GetPlayerCameraManager( PrimaryPC, 0 );
			if (camMgr) {
				TSharedPtr<ASimEcs_Archetype> boat = USimOceanSceneManager_Singleton::GetInstance()->GetSimActorWithName( ActorName );
				if (boat.IsValid()) {
					PrimaryPC->SetReservedViewTarget( boat.Get( ) );
					FViewTargetTransitionParams TransitionParams;
					TransitionParams.BlendTime = 2.0f;
					PrimaryPC->SetViewTarget( boat.Get(), TransitionParams );

				}
				else {
					PrimaryPC->SetViewTarget( NULL );

				}
			}

		}
	}
}

void SSceneObjectsList::DeleteDemo( )
{
	if (bUpdatingSceneObjectsList) {
		// We're still building the list
		return;
	}

	if (SelectedItem.IsValid( )) {
		UArmySimGameInstance* const GI = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance( ) );

		if (GI != NULL) {
			UArmySimGameViewportClient* SoldierViewport = Cast<UArmySimGameViewportClient>( GI->GetGameViewportClient( ) );

		/*	if (SoldierViewport) {
				SoldierViewport->ShowDialog(
					PlayerOwner,
					EArmySimDialogType::Generic,
					FText::Format( LOCTEXT( "DeleteDemoFmt", "Delete {0}?" ), FText::FromString( SelectedItem->StreamInfo.FriendlyName ) ),
					LOCTEXT( "EnterYes", "ENTER - YES" ),
					LOCTEXT( "EscapeNo", "ESC - NO" ),
					FOnClicked::CreateRaw( this, &SSceneObjectsList::OnDemoDeleteConfirm ),
					FOnClicked::CreateRaw( this, &SSceneObjectsList::OnDemoDeleteCancel )
				);
			}*/
		}
	}
}

FReply SSceneObjectsList::OnDemoDeleteConfirm( )
{
	if (SelectedItem.IsValid( ) && ReplayStreamer.IsValid( )) {
		bUpdatingSceneObjectsList = true;
		SceneObjectsList.Empty( );

		ReplayStreamer->DeleteFinishedStream( SelectedItem->StreamInfo.Name, FDeleteFinishedStreamCallback::CreateSP( this, &SSceneObjectsList::OnDeleteFinishedStreamComplete ) );
	}

	UArmySimGameInstance* const GI = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance( ) );

	if (GI != NULL) {
		UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GI->GetGameViewportClient( ) );

		if (SoldierViewport) {
			SoldierViewport->HideDialog( );
		}
	}

	return FReply::Handled( );
}

FReply SSceneObjectsList::OnDemoDeleteCancel( )
{
	UArmySimGameInstance* const GI = Cast<UArmySimGameInstance>( PlayerOwner->GetGameInstance( ) );

	if (GI != NULL) {
		UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GI->GetGameViewportClient( ) );

		if (SoldierViewport) {
			SoldierViewport->HideDialog( );
		}
	}

	return FReply::Handled( );
}

void SSceneObjectsList::OnDeleteFinishedStreamComplete( const FDeleteFinishedStreamResult& Result )
{
	BuildSceneObjectsList( );
}

void SSceneObjectsList::OnFocusLost( const FFocusEvent& InFocusEvent )
{
	if (InFocusEvent.GetCause( ) != EFocusCause::SetDirectly) {
		FSlateApplication::Get( ).SetKeyboardFocus( SharedThis( this ) );
	}
}

FReply SSceneObjectsList::OnFocusReceived( const FGeometry& MyGeometry, const FFocusEvent& InFocusEvent )
{
	return FReply::Handled( ).SetUserFocus( SharedThis( this ), EFocusCause::SetDirectly, true );
}

void SSceneObjectsList::EntrySelectionChanged( TSharedPtr<FSceneObjectsEntry> InItem, ESelectInfo::Type SelectInfo )
{
	SelectedItem = InItem;
}

void SSceneObjectsList::OnListItemDoubleClicked( TSharedPtr<FSceneObjectsEntry> InItem )
{
	SelectedItem = InItem;
	LocateObjects( );
	FSlateApplication::Get( ).SetKeyboardFocus( SharedThis( this ) );
}

void SSceneObjectsList::MoveSelection( int32 MoveBy )
{
	const int32 SelectedItemIndex = SceneObjectsList.IndexOfByKey( SelectedItem );

	if (SelectedItemIndex + MoveBy > -1 && SelectedItemIndex + MoveBy < SceneObjectsList.Num( )) {
		ObjectsListWidget->SetSelection( SceneObjectsList[SelectedItemIndex + MoveBy] );
	}
}

FReply SSceneObjectsList::OnKeyDown( const FGeometry& MyGeometry, const FKeyEvent& InKeyboardEvent )
{
	if (bUpdatingSceneObjectsList) // lock input
	{
		return FReply::Handled( );
	}

	FReply Result = FReply::Unhandled( );
	const FKey Key = InKeyboardEvent.GetKey( );
	if (Key == EKeys::Enter || Key == EKeys::Virtual_Accept) {
		LocateObjects( );
		Result = FReply::Handled( );
		FSlateApplication::Get( ).SetKeyboardFocus( SharedThis( this ) );
	}
	else if (Key == EKeys::Delete) {
		DeleteDemo( );
		Result = FReply::Handled( );
		FSlateApplication::Get( ).SetKeyboardFocus( SharedThis( this ) );
	}
	//hit space bar or left gamepad face button to search for demos again / refresh the list, only when not searching already
	else if (Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Left) {
		// Refresh demo list
		BuildSceneObjectsList( );
	}
	else if (Key == EKeys::Up || Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Gamepad_LeftStick_Up) {
		MoveSelection( -1 );
		Result = FReply::Handled( );
	}
	else if (Key == EKeys::Down || Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Gamepad_LeftStick_Down) {
		MoveSelection( 1 );
		Result = FReply::Handled( );
	}
	return Result;
}

TSharedRef<ITableRow> SSceneObjectsList::MakeListViewWidget( TSharedPtr<FSceneObjectsEntry> Item, const TSharedRef<STableViewBase>& OwnerTable )
{
	class SDemoEntryWidget : public SMultiColumnTableRow< TSharedPtr<FSceneObjectsEntry> > {
	public:
		SLATE_BEGIN_ARGS( SDemoEntryWidget ) {}
		SLATE_END_ARGS( )

			void Construct( const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<FSceneObjectsEntry> InItem )
		{
			Item = InItem;
			SMultiColumnTableRow< TSharedPtr<FSceneObjectsEntry> >::Construct( FSuperRowType::FArguments( ), InOwnerTable );
		}

		TSharedRef<SWidget> GenerateWidgetForColumn( const FName& ColumnName )
		{
			FText ItemText = FText::GetEmpty( );

			if (ColumnName == "ObjectName") {
				FString NameString =  Item->StreamInfo.Name ;

				const int MAX_DEMO_NAME_DISPLAY_LEN = 18;
				if (NameString.Len( ) > MAX_DEMO_NAME_DISPLAY_LEN) {
					NameString = NameString.Left( MAX_DEMO_NAME_DISPLAY_LEN ) + TEXT( "..." );
				}

				if (Item->StreamInfo.bIsLive) {
					NameString += " (Live)";
				}

				ItemText = FText::FromString( NameString );
			}
			else if (ColumnName == "Viewers") {
			//	ItemText = FText::FromString( FString::Printf( TEXT( "%i" ), Item->StreamInfo.NumViewers ) );
			}
			else if (ColumnName == "Date") {
				ItemText = FText::FromString( Item->Date );
			}
			else if (ColumnName == "Length") {
				//const int32 Minutes = Item->StreamInfo.LengthInMS / (1000 * 60);
				//const int32 Seconds = (Item->StreamInfo.LengthInMS / 1000) % 60;

				//ItemText = FText::FromString( FString::Printf( TEXT( "%02i:%02i" ), Minutes, Seconds ) );
			}
			else if (ColumnName == "Size") {
				ItemText = FText::FromString( Item->Size );
			}

			return SNew( STextBlock )
				.Text( ItemText )
				.TextStyle( FArmySimStyle::Get( ), "OceanBoats.MenuServerListTextStyle" );
		}
		TSharedPtr<FSceneObjectsEntry> Item;
	};
	return SNew( SDemoEntryWidget, OwnerTable, Item );
}

#undef LOCTEXT_NAMESPACE