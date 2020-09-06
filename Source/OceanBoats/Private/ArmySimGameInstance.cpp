// Fill out your copyright notice in the Description page of Project Settings.

#include "ArmySimGameInstance.h"
#include "OceanBoats.h"
#include "SoldierMainMenu.h"
#include "SoldierWelcomeMenu.h"
#include "SoldierMainMenu.h"
#include "SoldierWelcomeMenu.h"
#include "SoldierMessageMenu.h"
#include "ArmyGameLoadingScreen.h"
#include "OnlineKeyValuePair.h"
#include "ArmySimStyle.h"
#include "ArmySimMenuItemWidgetStyle.h"
#include "ArmySimGameViewportClient.h"
#include "Engine/GameInstance.h"
#include "Player/SoldierPlayerController_Menu.h"
#include "Vehicles/Vehicle.h"
#include "Sim_OceanSceneManager.h"
#include "Online/SoldierPlayerState.h"

FAutoConsoleVariable CVarShooterGameTestEncryption( TEXT( "OceanBoats.Encryption" ), 0, TEXT( "If true, clients will send an encryption token with their request to join the server and attempt to encrypt the connection using a debug key. This is NOT SECURE and for demonstration purposes only." ) );


void SArmySimWaitDialog::Construct(const FArguments& InArgs)
{
	const FArmySimMenuItemStyle* ItemStyle = &FArmySimStyle::Get().GetWidgetStyle<FArmySimMenuItemStyle>("DefaultSimMenuItemStyle");
	const FButtonStyle* ButtonStyle = &FArmySimStyle::Get().GetWidgetStyle<FButtonStyle>("DefaultSimButtonStyle");
	ChildSlot
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(20.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		[
			SNew(SBorder)
			.Padding(50.0f)
		.VAlign(VAlign_Center)
		.HAlign(HAlign_Center)
		.BorderImage(&ItemStyle->BackgroundBrush)
		.BorderBackgroundColor(FLinearColor(1.0f, 1.0f, 1.0f, 1.0f))
		[
			SNew(STextBlock)
			.TextStyle(FArmySimStyle::Get(), "OceanBoats.MenuHeaderTextStyle")
		.ColorAndOpacity(this, &SArmySimWaitDialog::GetTextColor)
		.Text(InArgs._MessageText)
		.WrapTextAt(500.0f)
		]
		]
		];

	//Setup a curve
	const float StartDelay = 0.0f;
	const float SecondDelay = 0.0f;
	const float AnimDuration = 2.0f;

	WidgetAnimation = FCurveSequence();
	TextColorCurve = WidgetAnimation.AddCurve(StartDelay + SecondDelay, AnimDuration, ECurveEaseFunction::QuadInOut);
	WidgetAnimation.Play(this->AsShared(), true);
}

FSlateColor SArmySimWaitDialog::GetTextColor() const
{
	//instead of going from black -> white, go from white -> grey.
	float fAlpha = 1.0f - TextColorCurve.GetLerp();
	fAlpha = fAlpha * 0.5f + 0.5f;
	return FLinearColor(FColor(155, 164, 182, FMath::Clamp((int32)(fAlpha * 255.0f), 0, 255)));
}

namespace ArmySimGameInstanceState
{
	const FName None = FName(TEXT("None"));
	const FName PendingInvite = FName(TEXT("PendingInvite"));
	const FName WelcomeScreen = FName(TEXT("WelcomeScreen"));
	const FName MainMenu = FName(TEXT("MainMenu"));
	const FName MessageMenu = FName(TEXT("MessageMenu"));
	const FName Playing = FName(TEXT("Playing"));
}

UArmySimGameInstance::UArmySimGameInstance( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
	, OnlineMode( EOnlineMode::Offline ) // Default to online
	, bIsLicensed( true ) // Default to licensed (should have been checked by OS on boot)
{
	CurrentState = ArmySimGameInstanceState::None;
	bLoginVehicle = false;
	bEnterVehicle = false;
	ActiveVehicle = nullptr;
}

void UArmySimGameInstance::Init()
{
	Super::Init();
	// Register delegate for ticker callback
	TickDelegate = FTickerDelegate::CreateUObject( this, &UArmySimGameInstance::Tick );
	TickDelegateHandle = FTicker::GetCoreTicker( ).AddTicker( TickDelegate );

	FCoreUObjectDelegates::PreLoadMap.AddUObject( this, &UArmySimGameInstance::OnPreLoadMap );
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject( this, &UArmySimGameInstance::OnPostLoadMap );
}

void UArmySimGameInstance::Shutdown()
{
	Super::Shutdown();

	// Unregister ticker delegate
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	USimOceanSceneManager_Singleton::GetInstance( )->Terminate( );
}


void UArmySimGameInstance::StartGameInstance() {
	GotoInitialState();
}



FName UArmySimGameInstance::GetInitialState()
{
#if SHOOTER_CONSOLE_UI	
	// Start in the welcome screen state on consoles
	return ArmySimGameInstanceState::WelcomeScreen;
#else
	// On PC, go directly to the main menu
	return ArmySimGameInstanceState::MainMenu;
#endif
}

void UArmySimGameInstance::GotoInitialState()
{
	GotoState(GetInitialState());
}

const FName UArmySimGameInstance::GetCurrentState() const
{
	return CurrentState;
}


void UArmySimGameInstance::GotoState(FName NewState)
{
	UE_LOG(LogOnline, Log, TEXT("GotoState: NewState: %s"), *NewState.ToString());

	PendingState = NewState;
}


bool UArmySimGameInstance::HostGame( ULocalPlayer* LocalPlayer, const FString& GameType, const FString& InTravelURL )
{
	if (GetOnlineMode( ) == EOnlineMode::Offline) {
		//
		// Offline game, just go straight to map
		//

		ShowLoadingScreen( );
		GotoState( ArmySimGameInstanceState::Playing );

		// Travel to the specified match URL
		TravelURL = InTravelURL;
		GetWorld( )->ServerTravel( TravelURL );
		return true;
	}

	//
	// Online game
	//

	return false;
}



void UArmySimGameInstance::ShowLoadingScreen( )
{
	// This can be confusing, so here is what is happening:
	//	For LoadMap, we use the ISoldierGameLoadingScreenModule interface to show the load screen
	//  This is necessary since this is a blocking call, and our viewport loading screen won't get updated.
	//  We can't use ISoldierGameLoadingScreenModule for seamless travel though
	//  In this case, we just add a widget to the viewport, and have it update on the main thread 
	//  To simplify things, we just do both, and you can't tell, one will cover the other if they both show at the same time
	IArmyGameLoadingScreenModule* const LoadingScreenModule = FModuleManager::LoadModulePtr<IArmyGameLoadingScreenModule>( "ArmyGameLoadingScreen" );
	if (LoadingScreenModule != nullptr) {
		LoadingScreenModule->StartInGameLoadingScreen( );
	}

	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );

	if (SoldierViewport != NULL) {
		SoldierViewport->ShowLoadingScreen( );
	}
}

bool UArmySimGameInstance::LoadFrontEndMap( const FString& MapName )
{
	bool bSuccess = true;

	// if already loaded, do nothing
	UWorld* const World = GetWorld( );
	if (World) {
		FString const CurrentMapName = *World->PersistentLevel->GetOutermost( )->GetName( );
		if (CurrentMapName == MapName) {
			return bSuccess;
		}
	}

	FString Error;
	EBrowseReturnVal::Type BrowseRet = EBrowseReturnVal::Failure;
	FURL URL(
		*FString::Printf( TEXT( "%s" ), *MapName )
	);

	if (URL.Valid && !HasAnyFlags( RF_ClassDefaultObject )) //CastChecked<UEngine>() will fail if using Default__ShooterGameInstance, so make sure that we're not default
	{
		BrowseRet = GetEngine( )->Browse( *WorldContext, URL, Error );

		// Handle failure.
		if (BrowseRet != EBrowseReturnVal::Success) {
			UE_LOG( LogLoad, Fatal, TEXT( "%s" ), *FString::Printf( TEXT( "Failed to enter %s: %s. Please check the log for errors." ), *MapName, *Error ) );
			bSuccess = false;
		}
	}
	return bSuccess;
}

void UArmySimGameInstance::MaybeChangeState()
{
	if ((PendingState != CurrentState) && (PendingState != ArmySimGameInstanceState::None))
	{
		FName const OldState = CurrentState;

		// end current state
		EndCurrentState(PendingState);

		// begin new state
		BeginNewState(PendingState, OldState);

		// clear pending change
		PendingState = ArmySimGameInstanceState::None;
	}
}

void UArmySimGameInstance::EndCurrentState(FName NextState)
{
	// per-state custom ending code here
	if (CurrentState == ArmySimGameInstanceState::PendingInvite)
	{
		EndPendingInviteState();
	}
	else if (CurrentState == ArmySimGameInstanceState::WelcomeScreen)
	{
		EndWelcomeScreenState();
	}
	else if (CurrentState == ArmySimGameInstanceState::MainMenu)
	{
		EndMainMenuState();
	}
	else if (CurrentState == ArmySimGameInstanceState::MessageMenu)
	{
		EndMessageMenuState();
	}
	else if (CurrentState == ArmySimGameInstanceState::Playing)
	{
		EndPlayingState();
	}

	CurrentState = ArmySimGameInstanceState::None;
}

void UArmySimGameInstance::BeginNewState(FName NewState, FName PrevState)
{
	// per-state custom starting code here

	if (NewState == ArmySimGameInstanceState::PendingInvite)
	{
		BeginPendingInviteState();
	}
	else if (NewState == ArmySimGameInstanceState::WelcomeScreen)
	{
		BeginWelcomeScreenState();
	}
	else if (NewState == ArmySimGameInstanceState::MainMenu)
	{
		BeginMainMenuState();
	}
	else if (NewState == ArmySimGameInstanceState::MessageMenu)
	{
		BeginMessageMenuState();
	}
	else if (NewState == ArmySimGameInstanceState::Playing)
	{
		BeginPlayingState();
	}

	CurrentState = NewState;
}

void UArmySimGameInstance::BeginPendingInviteState()
{
	if (LoadFrontEndMap( MainMenuMap )) {
		//StartOnlinePrivilegeTask( IOnlineIdentity::FOnGetUserPrivilegeCompleteDelegate::CreateUObject( this, &UArmySimGameInstance::OnUserCanPlayInvite ), EUserPrivileges::CanPlayOnline, PendingInvite.UserId );
	}
	else {
		GotoState( ArmySimGameInstanceState::WelcomeScreen );
	}
}

void UArmySimGameInstance::EndPendingInviteState()
{
	// cleanup in case the state changed before the pending invite was handled.
	//CleanupOnlinePrivilegeTask();
}

void UArmySimGameInstance::BeginWelcomeScreenState()
{
	//this must come before split screen player removal so that the OSS sets all players to not using online features.
	//this must come before split screen player removal so that the OSS sets all players to not using online features.
	SetOnlineMode( EOnlineMode::Offline );

	// Remove any possible splitscren players
	RemoveSplitScreenPlayers( );

	LoadFrontEndMap( WelcomeScreenMap );

	ULocalPlayer* const LocalPlayer = GetFirstGamePlayer( );
	LocalPlayer->SetCachedUniqueNetId( nullptr );
	check( !WelcomeMenuUI.IsValid( ) );
	WelcomeMenuUI = MakeShareable( new FSoldierWelcomeMenu );
	WelcomeMenuUI->Construct( this );
	WelcomeMenuUI->AddToGameViewport( );

	// Disallow splitscreen (we will allow while in the playing state)
	GetGameViewportClient( )->SetDisableSplitscreenOverride( true );
}

void UArmySimGameInstance::EndWelcomeScreenState()
{
	/*if (WelcomeMenuUI.IsValid())
	{
		WelcomeMenuUI->RemoveFromGameViewport();
		WelcomeMenuUI = nullptr;
	}*/
}


void UArmySimGameInstance::SetPresenceForLocalPlayers( const FString& StatusStr, const FVariantData& PresenceData )
{
	/*const IOnlinePresencePtr Presence = Online::GetPresenceInterface( GetWorld( ) );
	if (Presence.IsValid( )) {
		for (int i = 0; i < LocalPlayers.Num( ); ++i) {
			FUniqueNetIdRepl UserId = LocalPlayers[i]->GetPreferredUniqueNetId( );

			if (UserId.IsValid( )) {
				FOnlineUserPresenceStatus PresenceStatus;
				PresenceStatus.StatusStr = StatusStr;
				PresenceStatus.State = EOnlinePresenceState::Online;
				PresenceStatus.Properties.Add( DefaultPresenceKey, PresenceData );

				Presence->SetPresence( *UserId, PresenceStatus );
			}
		}
	}*/
}


void UArmySimGameInstance::BeginMainMenuState()
{
	// Make sure we're not showing the loadscreen
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );

	if (SoldierViewport != NULL) {
		SoldierViewport->HideLoadingScreen( );
	}

	SetOnlineMode( EOnlineMode::Offline );

	// Disallow splitscreen
	UGameViewportClient* GameViewportClient = GetGameViewportClient( );

	if (GameViewportClient) {
		GetGameViewportClient( )->SetDisableSplitscreenOverride( true );
	}

	// Remove any possible splitscren players
	RemoveSplitScreenPlayers( );

	// Set presence to menu state for the owning player
	SetPresenceForLocalPlayers( FString( TEXT( "In Menu" ) ), FVariantData( FString( TEXT( "OnMenu" ) ) ) );

	// load startup map
	LoadFrontEndMap( MainMenuMap );

	// player 0 gets to own the UI
	ULocalPlayer* const Player = GetFirstGamePlayer( );

	MainMenuUI = MakeShareable( new FSoldierMainMenu( ) );
	MainMenuUI->Construct( this, Player );
	MainMenuUI->AddMenuToGameViewport( );

	// It's possible that a play together event was sent by the system while the player was in-game or didn't
	// have the application launched. The game will automatically go directly to the main menu state in those cases
	// so this will handle Play Together if that is why we transitioned here.
	if (PlayTogetherInfo.UserIndex != -1) {
		MainMenuUI->OnPlayTogetherEventReceived( );
	}

#if !SHOOTER_CONSOLE_UI
	// The cached unique net ID is usually set on the welcome screen, but there isn't
	// one on PC/Mac, so do it here.
	if (Player != nullptr) {
		Player->SetControllerId( 0 );
		Player->SetCachedUniqueNetId( Player->GetUniqueNetIdFromCachedControllerId( ).GetUniqueNetId( ) );
	}
#endif

	RemoveNetworkFailureHandlers( );
}


void UArmySimGameInstance::EndMainMenuState( ) {
	if (MainMenuUI.IsValid( )) {
		MainMenuUI->RemoveMenuFromGameViewport( );
		MainMenuUI = nullptr;
	}
}


void UArmySimGameInstance::BeginMessageMenuState()
{
	if (PendingMessage.DisplayString.IsEmpty( )) {
		UE_LOG( LogOnlineGame, Warning, TEXT( "USoldierGameInstance::BeginMessageMenuState: Display string is empty" ) );
		GotoInitialState( );
		return;
	}

	// Make sure we're not showing the loadscreen
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );

	if (SoldierViewport != NULL) {
		SoldierViewport->HideLoadingScreen( );
	}

	check( !MessageMenuUI.IsValid( ) );
	MessageMenuUI = MakeShareable( new FSoldierMessageMenu );
	MessageMenuUI->Construct( this, PendingMessage.PlayerOwner, PendingMessage.DisplayString, PendingMessage.OKButtonString, PendingMessage.CancelButtonString, PendingMessage.NextState );

	PendingMessage.DisplayString = FText::GetEmpty( );
}

void UArmySimGameInstance::EndMessageMenuState( ) {
	if (MessageMenuUI.IsValid( )) {
		MessageMenuUI->RemoveFromGameViewport( );
		MessageMenuUI = nullptr;
	}
}

void UArmySimGameInstance::BeginPlayingState() {
	bPendingEnableSplitscreen = true;

	// Set presence for playing in a map
	SetPresenceForLocalPlayers( FString( TEXT( "In Game" ) ), FVariantData( FString( TEXT( "InGame" ) ) ) );

	// Make sure viewport has focus
	FSlateApplication::Get( ).SetAllUserFocusToGameViewport( );
}


void UArmySimGameInstance::EndPlayingState( )
{
	// Disallow splitscreen
	GetGameViewportClient( )->SetDisableSplitscreenOverride( true );

	// Clear the players' presence information
	SetPresenceForLocalPlayers( FString( TEXT( "In Menu" ) ), FVariantData( FString( TEXT( "OnMenu" ) ) ) );

	UWorld* const World = GetWorld( );
	ASoldierGameState* const GameState = World != NULL ? World->GetGameState<ASoldierGameState>( ) : NULL;

	if (GameState) {
		// Send round end events for local players
		for (int i = 0; i < LocalPlayers.Num( ); ++i) {
			ASoldierPlayerController* ShooterPC = Cast<ASoldierPlayerController>( LocalPlayers[i]->PlayerController );
			if (ShooterPC) {
				// Assuming you can't win if you quit early
				ShooterPC->ClientSendRoundEndEvent( false, GameState->ElapsedTime );
			}
		}

		// Give the game state a chance to cleanup first
		GameState->RequestFinishAndExitToMainMenu( );
	}
	else {
		// If there is no game state, make sure the session is in a good state

	} 
}

void UArmySimGameInstance::VehicleDetect( bool enter, AActor* vehicle, AActor* enterActor )
{
	bEnterVehicle = enter;
	if (enter) {
		if (vehicle->GetClass( )->ImplementsInterface( UVehicle::StaticClass( ) )) {
			SetActiveVehicle( vehicle );
		}
	}
	else if (vehicle && enterActor) {
		float dist = FVector::Dist( vehicle->GetActorLocation( ), enterActor->GetActorLocation( ) );
		//IVehicle* vehicleInterface = Cast<IVehicle>(vehicle);
		if (!IVehicle::Execute_DetectInArea( vehicle, enterActor )) {
			SetActiveVehicle( nullptr );
		}
	}
}

void UArmySimGameInstance::SetActiveVehicle( AActor* vehicle )
{
	ActiveVehicle = vehicle;
}


bool UArmySimGameInstance::ValidatePlayerIsSignedIn( ULocalPlayer* LocalPlayer )
{
	return false;
	// Get the viewport
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );

	if (!IsLocalPlayerSignedIn( LocalPlayer )) {
		// Don't let them play online if they aren't online
		if (SoldierViewport != NULL) {
			const FText Msg = NSLOCTEXT( "NetworkFailures", "MustBeSignedIn", "You must be signed in to play online" );
			const FText OKButtonString = NSLOCTEXT( "DialogButtons", "OKAY", "OK" );

			SoldierViewport->ShowDialog(
				NULL,
				EArmySimDialogType::Generic,
				Msg,
				OKButtonString,
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}

		return false;
	}

	return true;
}


FReply UArmySimGameInstance::OnConfirmGeneric( )
{
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );
	if (SoldierViewport) {
		SoldierViewport->HideDialog( );
	}

	return FReply::Handled( );
}


bool UArmySimGameInstance::IsLocalPlayerSignedIn( ULocalPlayer* LocalPlayer )
{
	if (LocalPlayer == NULL) {
		return false;
	}
	/*
		const IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
		if (OnlineSub) {
			const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface( );
			if (IdentityInterface.IsValid( )) {
				FUniqueNetIdRepl UniqueId = LocalPlayer->GetCachedUniqueNetId( );
				if (UniqueId.IsValid( )) {
					return true;
				}
			}
		}*/

	return false;
}


bool UArmySimGameInstance::IsLocalPlayerOnline( ULocalPlayer* LocalPlayer )
{
	if (LocalPlayer == NULL) {
		return false;
	}
	/*const IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
	if (OnlineSub) {
		const IOnlineIdentityPtr IdentityInterface = OnlineSub->GetIdentityInterface( );
		if (IdentityInterface.IsValid( )) {
			FUniqueNetIdRepl UniqueId = LocalPlayer->GetCachedUniqueNetId( );
			if (UniqueId.IsValid( )) {
				const ELoginStatus::Type LoginStatus = IdentityInterface->GetLoginStatus( *UniqueId );
				if (LoginStatus == ELoginStatus::LoggedIn) {
					return true;
				}
			}
		}
	}*/

	return false;
}


void UArmySimGameInstance::AddNetworkFailureHandlers( )
{
	// Add network/travel error handlers (if they are not already there)
	if (GEngine->OnTravelFailure( ).IsBoundToObject( this ) == false) {
		//TravelLocalSessionFailureDelegateHandle = GEngine->OnTravelFailure( ).AddUObject( this, &UArmySimGameInstance::TravelLocalSessionFailure );
	}
}

void UArmySimGameInstance::RemoveNetworkFailureHandlers( )
{
	// Remove the local session/travel failure bindings if they exist
	if (GEngine->OnTravelFailure( ).IsBoundToObject( this ) == true) {
		GEngine->OnTravelFailure( ).Remove( TravelLocalSessionFailureDelegateHandle );
	}
}

bool UArmySimGameInstance::ValidatePlayerForOnlinePlay( ULocalPlayer* LocalPlayer )
{
	return false;
	// Get the viewport
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );

	if (!IsLocalPlayerOnline( LocalPlayer )) {
		// Don't let them play online if they aren't online
		if (SoldierViewport != NULL) {
			const FText Msg = NSLOCTEXT( "NetworkFailures", "MustBeSignedIn", "You must be signed in to play online" );
			const FText OKButtonString = NSLOCTEXT( "DialogButtons", "OKAY", "OK" );

			SoldierViewport->ShowDialog(
				NULL,
				EArmySimDialogType::Generic,
				Msg,
				OKButtonString,
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}

		return false;
	}

	return true;
}


void UArmySimGameInstance::DisplayOnlinePrivilegeFailureDialogs( const FUniqueNetId& UserId, EUserPrivileges::Type Privilege, uint32 PrivilegeResults )
{
	return;
	// Show warning that the user cannot play due to age restrictions
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );
	TWeakObjectPtr<ULocalPlayer> OwningPlayer;
	if (GEngine) {
		for (auto It = GEngine->GetLocalPlayerIterator( GetWorld( ) ); It; ++It) {
			FUniqueNetIdRepl OtherId = (*It)->GetPreferredUniqueNetId( );
			if (OtherId.IsValid( )) {
				if (UserId == (*OtherId)) {
					OwningPlayer = *It;
				}
			}
		}
	}

	if (SoldierViewport != NULL && OwningPlayer.IsValid( )) {
		if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::AccountTypeFailure) != 0) {
			IOnlineExternalUIPtr ExternalUI = Online::GetExternalUIInterface(  FName(*(GetWorld( )->GetName())) );
			if (ExternalUI.IsValid( )) {
				ExternalUI->ShowAccountUpgradeUI( UserId );
			}
		}
		else if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::RequiredSystemUpdate) != 0) {
			SoldierViewport->ShowDialog(
				OwningPlayer.Get( ),
				EArmySimDialogType::Generic,
				NSLOCTEXT( "OnlinePrivilegeResult", "RequiredSystemUpdate", "A required system update is available.  Please upgrade to access online features." ),
				NSLOCTEXT( "DialogButtons", "OKAY", "OK" ),
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}
		else if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::RequiredPatchAvailable) != 0) {
			SoldierViewport->ShowDialog(
				OwningPlayer.Get( ),
				EArmySimDialogType::Generic,
				NSLOCTEXT( "OnlinePrivilegeResult", "RequiredPatchAvailable", "A required game patch is available.  Please upgrade to access online features." ),
				NSLOCTEXT( "DialogButtons", "OKAY", "OK" ),
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}
		else if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::AgeRestrictionFailure) != 0) {
			SoldierViewport->ShowDialog(
				OwningPlayer.Get( ),
				EArmySimDialogType::Generic,
				NSLOCTEXT( "OnlinePrivilegeResult", "AgeRestrictionFailure", "Cannot play due to age restrictions!" ),
				NSLOCTEXT( "DialogButtons", "OKAY", "OK" ),
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}
		else if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::UserNotFound) != 0) {
			SoldierViewport->ShowDialog(
				OwningPlayer.Get( ),
				EArmySimDialogType::Generic,
				NSLOCTEXT( "OnlinePrivilegeResult", "UserNotFound", "Cannot play due invalid user!" ),
				NSLOCTEXT( "DialogButtons", "OKAY", "OK" ),
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}
		else if ((PrivilegeResults & (uint32)IOnlineIdentity::EPrivilegeResults::GenericFailure) != 0) {
			SoldierViewport->ShowDialog(
				OwningPlayer.Get( ),
				EArmySimDialogType::Generic,
				NSLOCTEXT( "OnlinePrivilegeResult", "GenericFailure", "Cannot play online.  Check your network connection." ),
				NSLOCTEXT( "DialogButtons", "OKAY", "OK" ),
				FText::GetEmpty( ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric ),
				FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnConfirmGeneric )
			);
		}
	}
}


TSharedPtr< const FUniqueNetId > UArmySimGameInstance::GetUniqueNetIdFromControllerId( const int ControllerId )
{
	IOnlineIdentityPtr OnlineIdentityInt = Online::GetIdentityInterface( FName( *(GetWorld( )->GetName( )) ) );

	if (OnlineIdentityInt.IsValid( )) {
		TSharedPtr<const FUniqueNetId> UniqueId = OnlineIdentityInt->GetUniquePlayerId( ControllerId );

		if (UniqueId.IsValid( )) {
			return UniqueId;
		}
	}
	return nullptr;
}


void UArmySimGameInstance::ShowMessageThenGotoState( const FText& Message, const FText& OKButtonString, const FText& CancelButtonString, const FName& NewState, const bool OverrideExisting, TWeakObjectPtr< ULocalPlayer > PlayerOwner )
{
	UE_LOG( LogOnline, Log, TEXT( "ShowMessageThenGotoState: Message: %s, NewState: %s" ), *Message.ToString( ), *NewState.ToString( ) );

	const bool bAtWelcomeScreen = PendingState == ArmySimGameInstanceState::WelcomeScreen || CurrentState == ArmySimGameInstanceState::WelcomeScreen;

	// Never override the welcome screen
	if (bAtWelcomeScreen) {
		UE_LOG( LogOnline, Log, TEXT( "ShowMessageThenGotoState: Ignoring due to higher message priority in queue (at welcome screen)." ) );
		return;
	}

	const bool bAlreadyAtMessageMenu = PendingState == ArmySimGameInstanceState::MessageMenu || CurrentState == ArmySimGameInstanceState::MessageMenu;
	const bool bAlreadyAtDestState = PendingState == NewState || CurrentState == NewState;

	// If we are already going to the message menu, don't override unless asked to
	
	 {
		GotoState( ArmySimGameInstanceState::MessageMenu );
	}
}

void UArmySimGameInstance::RemoveExistingLocalPlayer( ULocalPlayer* ExistingPlayer )
{
	check( ExistingPlayer );
	if (ExistingPlayer->PlayerController != NULL) {
		// Kill the player
		ASoldierCharacter* MyPawn = Cast<ASoldierCharacter>( ExistingPlayer->PlayerController->GetPawn( ) );
		if (MyPawn) {
			MyPawn->KilledBy( NULL );
		}
	}

	// Remove local split-screen players from the list
	RemoveLocalPlayer( ExistingPlayer );
}

void UArmySimGameInstance::RemoveSplitScreenPlayers( )
{
	// if we had been split screen, toss the extra players now
	// remove every player, back to front, except the first one
	while (LocalPlayers.Num( ) > 1) {
		ULocalPlayer* const PlayerToRemove = LocalPlayers.Last( );
		RemoveExistingLocalPlayer( PlayerToRemove );
	}
}



void UArmySimGameInstance::SetIgnorePairingChangeForControllerId( const int32 ControllerId )
{
	IgnorePairingChangeForControllerId = ControllerId;
}



bool UArmySimGameInstance::Tick( float DeltaSeconds )
{
	// Dedicated server doesn't need to worry about game state
	if (IsDedicatedServerInstance( ) == true) {
		return true;
	}

	UArmySimGameViewportClient* SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );
	if (FSlateApplication::IsInitialized( ) && SoldierViewport != nullptr) {
		if (FSlateApplication::Get( ).GetGameViewport( ) != SoldierViewport->GetGameViewportWidget( )) {
			return true;
		}
	}

	// Because this takes place outside the normal UWorld tick, we need to register what world we're ticking/modifying here to avoid issues in the editor
	FScopedConditionalWorldSwitcher WorldSwitcher( GetWorld( ) );

	MaybeChangeState( );

	if (CurrentState != ArmySimGameInstanceState::WelcomeScreen && SoldierViewport != nullptr) {
		// If at any point we aren't licensed (but we are after welcome screen) bounce them back to the welcome screen
		if (!bIsLicensed && CurrentState != ArmySimGameInstanceState::None && SoldierViewport != nullptr && !SoldierViewport->IsShowingDialog( )) {
			const FText ReturnReason = NSLOCTEXT( "ProfileMessages", "NeedLicense", "" );
			const FText OKButton = NSLOCTEXT( "DialogButtons", "OKAY", "OK" );

			ShowMessageThenGotoState( ReturnReason, OKButton, FText::GetEmpty( ), ArmySimGameInstanceState::WelcomeScreen );
		}

		// Show controller disconnected dialog if any local players have an invalid controller
		/*if (!SoldierViewport->IsShowingDialog( )) {
			for (int i = 0; i < LocalPlayers.Num( ); ++i) {
				if (LocalPlayers[i] && LocalPlayers[i]->GetControllerId( ) == -1) {
					SoldierViewport->ShowDialog(
						LocalPlayers[i],
						EArmySimDialogType::ControllerDisconnected,
						FText::Format( NSLOCTEXT( "ProfileMessages", "PlayerReconnectControllerFmt", "Player {0}, please reconnect your controller." ), FText::AsNumber( i + 1 ) ),
#if PLATFORM_PS4
						NSLOCTEXT( "DialogButtons", "PS4_CrossButtonContinue", "Cross Button - Continue" ),
#else
						NSLOCTEXT( "DialogButtons", "AButtonContinue", "A - Continue" ),
#endif
						FText::GetEmpty( ),
						FOnClicked::CreateUObject( this, &UArmySimGameInstance::OnControllerReconnectConfirm ),
						FOnClicked( )
					);
				}
			}
		}*/
	}

	//// If we have a pending invite, and we are at the welcome screen, and the session is properly shut down, accept it
	//if (PendingInvite.UserId.IsValid( ) && PendingInvite.bPrivilegesCheckedAndAllowed && CurrentState == SoldierGameInstanceState::PendingInvite) {
	//	IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
	//	IOnlineSessionPtr Sessions = (OnlineSub != NULL) ? OnlineSub->GetSessionInterface( ) : NULL;

	//	if (Sessions.IsValid( )) {
	//		EOnlineSessionState::Type SessionState = Sessions->GetSessionState( NAME_GameSession );

	//		if (SessionState == EOnlineSessionState::NoSession) {
	//			ULocalPlayer * NewPlayerOwner = GetFirstGamePlayer( );

	//			if (NewPlayerOwner != nullptr) {
	//				NewPlayerOwner->SetControllerId( PendingInvite.ControllerId );
	//				NewPlayerOwner->SetCachedUniqueNetId( PendingInvite.UserId );
	//				SetOnlineMode( EOnlineMode::Online );

	//				const bool bIsLocalPlayerHost = PendingInvite.UserId.IsValid( ) && PendingInvite.InviteResult.Session.OwningUserId.IsValid( ) && *PendingInvite.UserId == *PendingInvite.InviteResult.Session.OwningUserId;
	//				if (bIsLocalPlayerHost) {
	//					HostQuickSession( *NewPlayerOwner, PendingInvite.InviteResult.Session.SessionSettings );
	//				}
	//				else {
	//					JoinSession( NewPlayerOwner, PendingInvite.InviteResult );
	//				}
	//			}

	//			PendingInvite.UserId.Reset( );
	//		}
	//	}
	//}

	return true;
}

void UArmySimGameInstance::SetOnlineMode( EOnlineMode InOnlineMode )
{
	OnlineMode = InOnlineMode;
	UpdateUsingMultiplayerFeatures( InOnlineMode == EOnlineMode::Online );
}

void UArmySimGameInstance::UpdateUsingMultiplayerFeatures( bool bIsUsingMultiplayerFeatures )
{
	
}

void UArmySimGameInstance::LabelPlayerAsQuitter( ULocalPlayer* LocalPlayer ) const
{
	ASoldierPlayerState* const PlayerState = LocalPlayer && LocalPlayer->PlayerController ? Cast<ASoldierPlayerState>( LocalPlayer->PlayerController->PlayerState ) : nullptr;
	if (PlayerState) {
		PlayerState->SetQuitter( true );
	}
}



void UArmySimGameInstance::OnPreLoadMap( const FString& MapName )
{

	if (bPendingEnableSplitscreen) {
		// Allow splitscreen
		UGameViewportClient* GameViewportClient = GetGameViewportClient( );
		if (GameViewportClient != nullptr) {
			GameViewportClient->SetDisableSplitscreenOverride( false );

			bPendingEnableSplitscreen = false;
		}
	}
}

void UArmySimGameInstance::OnPostLoadMap( UWorld* )
{
	// Make sure we hide the loading screen when the level is done loading
	UArmySimGameViewportClient * SoldierViewport = Cast<UArmySimGameViewportClient>( GetGameViewportClient( ) );
	if (SoldierViewport != nullptr) {
		SoldierViewport->HideLoadingScreen( );
	}

	USimOceanSceneManager_Singleton::GetInstance( )->ReSpawnSystemLink( );

}

void UArmySimGameInstance::ToggleMainMenu( ) {
	
	if(MainMenuUI)
		MainMenuUI->ToggleGameMainMenu( );

}

void UArmySimGameInstance::ToggleSceneObjectsList( ) {

	if (MainMenuUI)
		MainMenuUI->OnShowSceneObjectsBrowser( );

}