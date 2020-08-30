// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "Player/SoldierPlayerController.h"
#include "Player/SoldierPlayerCameraManager.h"
#include "Player/SoldierCheatManager.h"
#include "Player/SoldierLocalPlayer.h"
#include "Weapons/SoldierWeapon.h"
#include "UI/Menu/SoldierIngameMenu.h"
#include "UI/Style/ArmySimStyle.h"
#include "UI/SoldierHUD.h"
#include "Online.h"
#include "OnlineAchievementsInterface.h"
#include "OnlineEventsInterface.h"
#include "OnlineStatsInterface.h"
#include "OnlineIdentityInterface.h"
#include "OnlineSessionInterface.h"
#include "ArmySimGameViewportClient.h"
#include "Sound/SoundNodeLocalPlayer.h"
#include "AudioThread.h"
#include "OnlineSubsystemUtils.h"
#include "Sim_OceanSceneManager.h"
#include "ArmySimGameInstance.h"

#define  ACH_FRAG_SOMEONE	TEXT("ACH_FRAG_SOMEONE")
#define  ACH_SOME_KILLS		TEXT("ACH_SOME_KILLS")
#define  ACH_LOTS_KILLS		TEXT("ACH_LOTS_KILLS")
#define  ACH_FINISH_MATCH	TEXT("ACH_FINISH_MATCH")
#define  ACH_LOTS_MATCHES	TEXT("ACH_LOTS_MATCHES")
#define  ACH_FIRST_WIN		TEXT("ACH_FIRST_WIN")
#define  ACH_LOTS_WIN		TEXT("ACH_LOTS_WIN")
#define  ACH_MANY_WIN		TEXT("ACH_MANY_WIN")
#define  ACH_SHOOT_BULLETS	TEXT("ACH_SHOOT_BULLETS")
#define  ACH_SHOOT_ROCKETS	TEXT("ACH_SHOOT_ROCKETS")
#define  ACH_GOOD_SCORE		TEXT("ACH_GOOD_SCORE")
#define  ACH_GREAT_SCORE	TEXT("ACH_GREAT_SCORE")
#define  ACH_PLAY_SANCTUARY	TEXT("ACH_PLAY_SANCTUARY")
#define  ACH_PLAY_HIGHRISE	TEXT("ACH_PLAY_HIGHRISE")

static const int32 SomeKillsCount = 10;
static const int32 LotsKillsCount = 20;
static const int32 LotsMatchesCount = 5;
static const int32 LotsWinsCount = 3;
static const int32 ManyWinsCount = 5;
static const int32 LotsBulletsCount = 100;
static const int32 LotsRocketsCount = 10;
static const int32 GoodScoreCount = 10;
static const int32 GreatScoreCount = 15;

ASoldierPlayerController::ASoldierPlayerController( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	PlayerCameraManagerClass = ASoldierPlayerCameraManager::StaticClass( );
	CheatClass = USoldierCheatManager::StaticClass( );
	bAllowGameActions = true;
	bGameEndedFrame = false;
	LastDeathLocation = FVector::ZeroVector;

	ServerSayString = TEXT( "Say" );
	SoldierFriendUpdateTimer = 0.0f;
	bHasSentStartEvents = false;

	StatMatchesPlayed = 0;
	StatKills = 0;
	StatDeaths = 0;
	bHasFetchedPlatformData = false;

	m_SwitchScenelight = false;
}

void ASoldierPlayerController::SetupInputComponent( )
{
	Super::SetupInputComponent( );

	// UI input
	InputComponent->BindAction( "InMainGameMenu", IE_Released, this, &ASoldierPlayerController::OnToggleGameMainMenu );
	InputComponent->BindAction( "InGameMenu", IE_Pressed, this, &ASoldierPlayerController::OnToggleInGameMenu );
	InputComponent->BindAction( "Scoreboard", IE_Pressed, this, &ASoldierPlayerController::OnToggleScoreboard);
	//InputComponent->BindAction( "Scoreboard", IE_Released, this, &ASoldierPlayerController::OnHideScoreboard );
	InputComponent->BindAction( "ConditionalCloseScoreboard", IE_Pressed, this, &ASoldierPlayerController::OnConditionalCloseScoreboard );
	InputComponent->BindAction( "ToggleScoreboard", IE_Pressed, this, &ASoldierPlayerController::OnToggleScoreboard );

	InputComponent->BindAxis( "MoveForwardAxisName", this, &ASoldierPlayerController::MoveForward );
	InputComponent->BindAxis("Turn", this, &ASoldierPlayerController::Turn);
	InputComponent->BindAxis("LookUp", this, &ASoldierPlayerController::LookUp);

	// voice chat
	InputComponent->BindAction( "PushToTalk", IE_Pressed, this, &APlayerController::StartTalking );
	InputComponent->BindAction( "PushToTalk", IE_Released, this, &APlayerController::StopTalking );

	InputComponent->BindAction( "ToggleChat", IE_Pressed, this, &ASoldierPlayerController::ToggleChatWindow );
	InputComponent->BindAction( "LoadSce", IE_Pressed, this, &ASoldierPlayerController::LoadScenario );
	InputComponent->BindAction( "LoadBarrier", IE_Pressed, this, &ASoldierPlayerController::LoadOceanBarrierScenario);

	InputComponent->BindAction( "BoatDetail", IE_Pressed, this, &ASoldierPlayerController::BoatDetails );
	InputComponent->BindAction( "ViewBoat", IE_Pressed, this, &ASoldierPlayerController::ViewBoat);
	InputComponent->BindAction("ResetView", IE_Pressed, this, &ASoldierPlayerController::ResetView);
	InputComponent->BindAction("StartSimulate", IE_Pressed, this, &ASoldierPlayerController::StartSimulate);
}


void ASoldierPlayerController::PostInitializeComponents( )
{
	Super::PostInitializeComponents( );
	FArmySimStyle::Initialize( );
	SoldierFriendUpdateTimer = 0;
}

void ASoldierPlayerController::ClearLeaderboardDelegate( )
{
	/*IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
	if (OnlineSub) {
		IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface( );
		if (Leaderboards.IsValid( )) {
			Leaderboards->ClearOnLeaderboardReadCompleteDelegate_Handle( LeaderboardReadCompleteDelegateHandle );
		}
	}*/
}

void ASoldierPlayerController::TickActor( float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction )
{
	Super::TickActor( DeltaTime, TickType, ThisTickFunction );
	USimOceanSceneManager_Singleton::GetInstance( )->Update( DeltaTime );

	if (IsGameMenuVisible( )) {
		if (SoldierFriendUpdateTimer > 0) {
			SoldierFriendUpdateTimer -= DeltaTime;
		}
		else {
			TSharedPtr<class FSoldierFriends> SoldierFriends = SoldierIngameMenu->GetSoldierFriends( );
			ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
			if (SoldierFriends.IsValid( ) && LocalPlayer && LocalPlayer->GetControllerId( ) >= 0) {
				SoldierFriends->UpdateFriends( LocalPlayer->GetControllerId( ) );
			}

			// Make sure the time between calls is long enough that we won't trigger (0x80552C81) and not exceed the web api rate limit
			// That value is currently 75 requests / 15 minutes.
			SoldierFriendUpdateTimer = 15;

		}
	}

	// Is this the first frame after the game has ended
	if (bGameEndedFrame) {
		bGameEndedFrame = false;

		// ONLY PUT CODE HERE WHICH YOU DON'T WANT TO BE DONE DUE TO HOST LOSS

		// Do we need to show the end of round scoreboard?
		if (IsPrimaryPlayer( )) {
			ASoldierHUD* SoldierHUD = GetSoldierHUD( );
			if (SoldierHUD) {
				SoldierHUD->ShowScoreboard( true, true );
			}
		}
	}

	const bool bLocallyControlled = IsLocalController( );
	const uint32 UniqueID = GetUniqueID( );
	FAudioThread::RunCommandOnAudioThread( [UniqueID, bLocallyControlled]( ) {
		USoundNodeLocalPlayer::GetLocallyControlledActorCache( ).Add( UniqueID, bLocallyControlled );
	} );
	ShowBroadcastMessage( );
//	USimOceanSceneManager_Singleton::GetInstance( )->Update( DeltaTime );
};

void ASoldierPlayerController::BeginPlay( ) {

}

void ASoldierPlayerController::BeginDestroy( )
{
	Super::BeginDestroy( );
	ClearLeaderboardDelegate( );

	// clear any online subsystem references
	SoldierIngameMenu = nullptr;

	if (!GExitPurge) {
		const uint32 UniqueID = GetUniqueID( );
		FAudioThread::RunCommandOnAudioThread( [UniqueID]( ) {
			USoundNodeLocalPlayer::GetLocallyControlledActorCache( ).Remove( UniqueID );
		} );
	}
}

void ASoldierPlayerController::SetPlayer( UPlayer* InPlayer )
{
	Super::SetPlayer( InPlayer );

	if (ULocalPlayer* const LocalPlayer = Cast<ULocalPlayer>( Player )) {
		//Build menu only after game is initialized
		SoldierIngameMenu = MakeShareable( new FSoldierIngameMenu( ) );
		SoldierIngameMenu->Construct( Cast<ULocalPlayer>( Player ) );

		FInputModeGameOnly InputMode;
		SetInputMode( InputMode );
	}
}

void ASoldierPlayerController::ShowBroadcastMessage( ) {
	if (USimOceanSceneManager_Singleton::GetInstance( )->IsHavSimMessage( )) {
		TArray<FString>& arryMessage = USimOceanSceneManager_Singleton::GetInstance( )->CopySimMessage( );
		for (auto msg: arryMessage)
		{
			FString strFrom= "-> ";
			OnBroadcastMessage( strFrom, msg );
		}
		USimOceanSceneManager_Singleton::GetInstance( )->RemoveSimMessage( );
	}
}



void ASoldierPlayerController::SwitchLight( TSubclassOf<class AActor> classToFind, uint8 bToggleLight ) {
	m_SwitchScenelight = bToggleLight;
	if (classToFind.Get( )) {
		float fIntensity = 25000.0f;
		TArray<AActor*> ActorLightList;
		UGameplayStatics::GetAllActorsOfClass( GWorld->GetWorld( ), classToFind, ActorLightList );
		for (auto l : ActorLightList) {
			TSet<UActorComponent*> arrSpot = l->GetComponents(  );
			for (auto spotLight : arrSpot) {
				UStaticMeshComponent* SMeshCom = Cast< UStaticMeshComponent >( spotLight );
				if (!SMeshCom || SMeshCom->GetName( ).Compare( "StreetLight" ) != 0) {
					continue;
				}
				if (UMaterialInstanceDynamic* MatInstance = Cast<UMaterialInstanceDynamic>( SMeshCom->GetMaterial( 0 ) )) {
						FName ParamName( "LightIntensity" );
						MatInstance->SetScalarParameterValue( ParamName, bToggleLight * 20.0f );
				}
				const bool bIncludeAllDescendants = false;
				TArray<USceneComponent*> OutSpotComChildren;
				SMeshCom->GetChildrenComponents( bIncludeAllDescendants, OutSpotComChildren );
				for (USceneComponent* SceneComponent : OutSpotComChildren) {
					if (!SceneComponent) {
						continue;
					}
					USpotLightComponent* USLC = Cast<USpotLightComponent>( SceneComponent );
					if (!USLC) {
						continue;
					}
					USLC->SetVisibility( bToggleLight );
					USLC->SetIntensity( bToggleLight ? 25000.0f : 0.0f );
				}
			}
		}
	}
}

void ASoldierPlayerController::QueryAchievements( )
{
	// precache achievements
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
	if (LocalPlayer && LocalPlayer->GetControllerId( ) != -1) {
		IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
		if (OnlineSub) {
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface( );
			if (Identity.IsValid( )) {
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId( LocalPlayer->GetControllerId( ) );

				if (UserId.IsValid( )) {
					IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface( );

					if (Achievements.IsValid( )) {
						Achievements->QueryAchievements( *UserId.Get( ), FOnQueryAchievementsCompleteDelegate::CreateUObject( this, &ASoldierPlayerController::OnQueryAchievementsComplete ) );
					}
				}
				else {
					UE_LOG( LogOnline, Warning, TEXT( "No valid user id for this controller." ) );
				}
			}
			else {
				UE_LOG( LogOnline, Warning, TEXT( "No valid identity interface." ) );
			}
		}
		else {
			UE_LOG( LogOnline, Warning, TEXT( "No default online subsystem." ) );
		}
	}
	else {
		UE_LOG( LogOnline, Warning, TEXT( "No local player, cannot read achievements." ) );
	}
}

void ASoldierPlayerController::OnQueryAchievementsComplete( const FUniqueNetId& PlayerId, const bool bWasSuccessful )
{
	UE_LOG( LogOnline, Display, TEXT( "ASoldierPlayerController::OnQueryAchievementsComplete(bWasSuccessful = %s)" ), bWasSuccessful ? TEXT( "TRUE" ) : TEXT( "FALSE" ) );
}

void ASoldierPlayerController::OnLeaderboardReadComplete( bool bWasSuccessful )
{
	if (ReadObject.IsValid( ) && ReadObject->ReadState == EOnlineAsyncTaskState::Done && !bHasFetchedPlatformData) {
		bHasFetchedPlatformData = true;
		ClearLeaderboardDelegate( );

		// We should only have one stat.
		if (bWasSuccessful && ReadObject->Rows.Num( ) == 1) {
			FOnlineStatsRow& RowData = ReadObject->Rows[0];
			/*if (const FVariantData* KillData = RowData.Columns.Find( LEADERBOARD_STAT_KILLS )) {
				KillData->GetValue( StatKills );
			}

			if (const FVariantData* DeathData = RowData.Columns.Find( LEADERBOARD_STAT_DEATHS )) {
				DeathData->GetValue( StatDeaths );
			}

			if (const FVariantData* MatchData = RowData.Columns.Find( LEADERBOARD_STAT_MATCHESPLAYED )) {
				MatchData->GetValue( StatMatchesPlayed );
			}*/

			//UE_LOG( LogOnline, Log, TEXT( "Fetched player stat data. Kills %d Deaths %d Matches %d" ), StatKills, StatDeaths, StatMatchesPlayed );
		}
	}
}

void ASoldierPlayerController::QueryStats( )
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
	if (LocalPlayer && LocalPlayer->GetControllerId( ) != -1) {
		IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
		if (OnlineSub) {
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface( );
			if (Identity.IsValid( )) {
				TSharedPtr<const FUniqueNetId> UserId = Identity->GetUniquePlayerId( LocalPlayer->GetControllerId( ) );

				if (UserId.IsValid( )) {
					IOnlineLeaderboardsPtr Leaderboards = OnlineSub->GetLeaderboardsInterface( );
					if (Leaderboards.IsValid( ) && !bHasFetchedPlatformData) {
						TArray<TSharedRef<const FUniqueNetId>> QueryPlayers;
						QueryPlayers.Add( UserId.ToSharedRef( ) );

					/*	LeaderboardReadCompleteDelegateHandle = Leaderboards->OnLeaderboardReadCompleteDelegates.AddUObject( this, &ASoldierPlayerController::OnLeaderboardReadComplete );
						ReadObject = MakeShareable( new FSoldierAllTimeMatchResultsRead( ) );
						FOnlineLeaderboardReadRef ReadObjectRef = ReadObject.ToSharedRef( );
						if (Leaderboards->ReadLeaderboards( QueryPlayers, ReadObjectRef )) {
							UE_LOG( LogOnline, Log, TEXT( "Started process to fetch stats for current user." ) );
						}
						else {
							UE_LOG( LogOnline, Warning, TEXT( "Could not start leaderboard fetch process. This will affect stat writes for this session." ) );
						}*/

					}
				}
			}
		}
	}
}

void ASoldierPlayerController::UnFreeze( )
{
	ServerRestartPlayer( );
}

void ASoldierPlayerController::FailedToSpawnPawn( )
{
	if (StateName == NAME_Inactive) {
		BeginInactiveState( );
	}
	Super::FailedToSpawnPawn( );
}

void ASoldierPlayerController::PawnPendingDestroy( APawn* P )
{
	LastDeathLocation = P->GetActorLocation( );

	FVector CameraLocation = LastDeathLocation + FVector( 0, 0, 300.0f );

	FRotator CameraRotation( -90.0f, 0.0f, 0.0f );

	FindDeathCameraSpot( CameraLocation, CameraRotation );

	Super::PawnPendingDestroy( P );

	ClientSetSpectatorCamera( CameraLocation, CameraRotation );
}

void ASoldierPlayerController::GameHasEnded( class AActor* EndGameFocus, bool bIsWinner )
{
	Super::GameHasEnded( EndGameFocus, bIsWinner );
}

void ASoldierPlayerController::ClientSetSpectatorCamera_Implementation( FVector CameraLocation, FRotator CameraRotation )
{
	SetInitialLocationAndRotation( CameraLocation, CameraRotation );
	SetViewTarget( this );
}

bool ASoldierPlayerController::FindDeathCameraSpot( FVector& CameraLocation, FRotator& CameraRotation )
{
	const FVector PawnLocation = GetPawn( )->GetActorLocation( );
	FRotator ViewDir = GetControlRotation( );
	ViewDir.Pitch = -45.0f;

	const float YawOffsets[] = { 0.0f, -180.0f, 90.0f, -90.0f, 45.0f, -45.0f, 135.0f, -135.0f };
	const float CameraOffset = 600.0f;
	FCollisionQueryParams TraceParams( SCENE_QUERY_STAT( DeathCamera ), true, GetPawn( ) );

	FHitResult HitResult;
	for (int32 i = 0; i < ARRAY_COUNT( YawOffsets ); i++) {
		FRotator CameraDir = ViewDir;
		CameraDir.Yaw += YawOffsets[i];
		CameraDir.Normalize( );

		const FVector TestLocation = PawnLocation - CameraDir.Vector( ) * CameraOffset;

		const bool bBlocked = GetWorld( )->LineTraceSingleByChannel( HitResult, PawnLocation, TestLocation, ECC_Camera, TraceParams );

		if (!bBlocked) {
			CameraLocation = TestLocation;
			CameraRotation = CameraDir;
			return true;
		}
	}

	return false;
}

bool ASoldierPlayerController::ServerCheat_Validate( const FString& Msg )
{
	return true;
}

void ASoldierPlayerController::ServerCheat_Implementation( const FString& Msg )
{
	if (CheatManager) {
		ClientMessage( ConsoleCommand( Msg ) );
	}
}

void ASoldierPlayerController::SimulateInputKey( FKey Key, bool bPressed )
{
	InputKey( Key, bPressed ? IE_Pressed : IE_Released, 1, false );
}

void ASoldierPlayerController::OnKill( )
{
	UpdateAchievementProgress( ACH_FRAG_SOMEONE, 100.0f );
}

void ASoldierPlayerController::OnDeathMessage( class ASoldierPlayerState* KillerPlayerState, class ASoldierPlayerState* KilledPlayerState, const UDamageType* KillerDamageType )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD) {
		SoldierHUD->ShowDeathMessage( KillerPlayerState, KilledPlayerState, KillerDamageType );
	}

	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );

}

void ASoldierPlayerController::OnBroadcastMessage( const FString& strFrom, const FString& strTo )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD) {
		SoldierHUD->ShowBroadcastMessage( strFrom, strTo );
	}

}

void ASoldierPlayerController::UpdateAchievementProgress( const FString& Id, float Percent )
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
	if (LocalPlayer) {
		IOnlineSubsystem* OnlineSub = Online::GetSubsystem( GetWorld( ) );
		if (OnlineSub) {
			IOnlineIdentityPtr Identity = OnlineSub->GetIdentityInterface( );
			if (Identity.IsValid( )) {
				FUniqueNetIdRepl UserId = LocalPlayer->GetCachedUniqueNetId( );

				if (UserId.IsValid( )) {

					IOnlineAchievementsPtr Achievements = OnlineSub->GetAchievementsInterface( );
					if (Achievements.IsValid( ) && (!WriteObject.IsValid( ) || WriteObject->WriteState != EOnlineAsyncTaskState::InProgress)) {
						WriteObject = MakeShareable( new FOnlineAchievementsWrite( ) );
						WriteObject->SetFloatStat( *Id, Percent );

						FOnlineAchievementsWriteRef WriteObjectRef = WriteObject.ToSharedRef( );
						Achievements->WriteAchievements( *UserId, WriteObjectRef );
					}
					else {
						UE_LOG( LogOnline, Warning, TEXT( "No valid achievement interface or another write is in progress." ) );
					}
				}
				else {
					UE_LOG( LogOnline, Warning, TEXT( "No valid user id for this controller." ) );
				}
			}
			else {
				UE_LOG( LogOnline, Warning, TEXT( "No valid identity interface." ) );
			}
		}
		else {
			UE_LOG( LogOnline, Warning, TEXT( "No default online subsystem." ) );
		}
	}
	else {
		UE_LOG( LogOnline, Warning, TEXT( "No local player, cannot update achievements." ) );
	}
}

void ASoldierPlayerController::OnToggleGameMainMenu( ) {
	if (GEngine->GameViewport == nullptr) {
		return;
	}

	// this is not ideal, but necessary to prevent both players from pausing at the same time on the same frame
	UWorld* GameWorld = GEngine->GameViewport->GetWorld( );

	for (auto It = GameWorld->GetControllerIterator( ); It; ++It) {
		ASoldierPlayerController* Controller = Cast<ASoldierPlayerController>( *It );
		if (Controller && Controller->IsPaused( )) {
			return;
		}
	}
	UArmySimGameInstance* SGI = GameWorld != NULL ? Cast<UArmySimGameInstance>( GameWorld->GetGameInstance( ) ) : NULL;
	if (SGI == nullptr)return;
	SGI->ToggleMainMenu( );
}

void ASoldierPlayerController::OnToggleInGameMenu( )
{
	
	if (GEngine->GameViewport == nullptr) {
		return;
	}

	// this is not ideal, but necessary to prevent both players from pausing at the same time on the same frame
	UWorld* GameWorld = GEngine->GameViewport->GetWorld( );

	for (auto It = GameWorld->GetControllerIterator( ); It; ++It) {
		ASoldierPlayerController* Controller = Cast<ASoldierPlayerController>( *It );
		if (Controller && Controller->IsPaused( )) {
			return;
		}
	}

	// if no one's paused, pause
	if (SoldierIngameMenu.IsValid( )) {
		SoldierIngameMenu->ToggleGameMenu( );
	}
}

void ASoldierPlayerController::OnConditionalCloseScoreboard( )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD && (SoldierHUD->IsMatchOver( ) == false)) {
		SoldierHUD->ConditionalCloseScoreboard( );
	}
}

void ASoldierPlayerController::MoveForward( float Val ) {

		//const USceneComponent* const pComp = (TranslationDirection ? TranslationDirection : RootComponent);
		//AddMovementInput( FVector::RightVector, Val*10.0f );
		// const AOceanBoatsGameMode* DefGame = GetWorld( )->GetGameState( )->GetDefaultGameMode<AOceanBoatsGameMode>( );

		 /*	UClass* PawnClass = DefGame->GetDefaultPawnClassForController(  this );
				 APawn* PawnToFit = PawnClass ? PawnClass->GetDefaultObject<APawn>( ) : nullptr;
				 PawnToFit->AddMovementInput( FVector::RightVector, 10.0f );*/
	
}

void ASoldierPlayerController::OnToggleScoreboard( )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD && (SoldierHUD->IsMatchOver( ) == false)) {
		SoldierHUD->ToggleScoreboard( );
	}
}

void ASoldierPlayerController::OnShowScoreboard( )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD) {
		SoldierHUD->ShowScoreboard( true );
	}
}

void ASoldierPlayerController::OnHideScoreboard( )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	// If have a valid match and the match is over - hide the scoreboard
	if ((SoldierHUD != NULL) && (SoldierHUD->IsMatchOver( ) == false)) {
		SoldierHUD->ShowScoreboard( false );
	}
}

bool ASoldierPlayerController::IsGameMenuVisible( ) const
{
	bool Result = false;
	if (SoldierIngameMenu.IsValid( )) {
		Result = SoldierIngameMenu->GetIsGameMenuUp( );
	}

	return Result;
}

void ASoldierPlayerController::SetInfiniteAmmo( bool bEnable )
{
	bInfiniteAmmo = bEnable;
}

void ASoldierPlayerController::SetInfiniteClip( bool bEnable )
{
	bInfiniteClip = bEnable;
}

void ASoldierPlayerController::SetHealthRegen( bool bEnable )
{
	bHealthRegen = bEnable;
}

void ASoldierPlayerController::SetGodMode( bool bEnable )
{
	bGodMode = bEnable;
}

void ASoldierPlayerController::SetIsVibrationEnabled( bool bEnable )
{
	bIsVibrationEnabled = bEnable;
}

void ASoldierPlayerController::ClientGameStarted_Implementation( )
{
	bAllowGameActions = true;

	// Enable controls mode now the game has started
	SetIgnoreMoveInput( false );

	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD) {
		SoldierHUD->SetMatchState( ESimMatchState::Playing );
		SoldierHUD->ShowScoreboard( false );
	}
	bGameEndedFrame = false;

	QueryAchievements( );

	QueryStats( );

}

/** Starts the online game using the session name in the PlayerState */
void ASoldierPlayerController::ClientStartOnlineGame_Implementation( )
{
	if (!IsPrimaryPlayer( ))
		return;

	{
		// Keep retrying until player state is replicated
		GetWorld( )->GetTimerManager( ).SetTimer( TimerHandle_ClientStartOnlineGame, this, &ASoldierPlayerController::ClientStartOnlineGame_Implementation, 0.2f, false );
	}
}

/** Ends the online game using the session name in the PlayerState */
void ASoldierPlayerController::ClientEndOnlineGame_Implementation( )
{
	if (!IsPrimaryPlayer( ))
		return;

}

void ASoldierPlayerController::HandleReturnToMainMenu( )
{
	OnHideScoreboard( );
	CleanupSessionOnReturnToMenu( );
}

void ASoldierPlayerController::ClientReturnToMainMenu_Implementation( const FString& InReturnReason )
{
	const UWorld* World = GetWorld( );
	UArmySimGameInstance* SGI = World != NULL ? Cast<UArmySimGameInstance>( World->GetGameInstance( ) ) : NULL;

	if (!ensure( SGI != NULL )) {
		return;
	}

	if (GetNetMode( ) == NM_Client) {
		const FText ReturnReason = NSLOCTEXT( "NetworkErrors", "HostQuit", "The host has quit the match." );
		const FText OKButton = NSLOCTEXT( "DialogButtons", "OKAY", "OK" );

		SGI->ShowMessageThenGotoState( ReturnReason, OKButton, FText::GetEmpty( ), ArmySimGameInstanceState::MainMenu );
	}
	else {
		SGI->GotoState( ArmySimGameInstanceState::MainMenu );
	}

	// Clear the flag so we don't do normal end of round stuff next
	bGameEndedFrame = false;
}

/** Ends and/or destroys game session */
void ASoldierPlayerController::CleanupSessionOnReturnToMenu( )
{

}

void ASoldierPlayerController::ClientGameEnded_Implementation( class AActor* EndGameFocus, bool bIsWinner )
{
	Super::ClientGameEnded_Implementation( EndGameFocus, bIsWinner );

	// Disable controls now the game has ended
	SetIgnoreMoveInput( true );

	bAllowGameActions = false;

	// Make sure that we still have valid view target
	SetViewTarget( GetPawn( ) );

	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierHUD) {
		SoldierHUD->SetMatchState( bIsWinner ? ESimMatchState::Won : ESimMatchState::Lost );
	}

	UpdateSaveFileOnGameEnd( bIsWinner );
	UpdateAchievementsOnGameEnd( );
	UpdateLeaderboardsOnGameEnd( );
	UpdateStatsOnGameEnd( bIsWinner );

	// Flag that the game has just ended (if it's ended due to host loss we want to wait for ClientReturnToMainMenu_Implementation first, incase we don't want to process)
	bGameEndedFrame = true;
}

void ASoldierPlayerController::ClientSendRoundEndEvent_Implementation( bool bIsWinner, int32 ExpendedTimeInSeconds )
{
	
}

void ASoldierPlayerController::SetCinematicMode( bool bInCinematicMode, bool bHidePlayer, bool bAffectsHUD, bool bAffectsMovement, bool bAffectsTurning )
{
	Super::SetCinematicMode( bInCinematicMode, bHidePlayer, bAffectsHUD, bAffectsMovement, bAffectsTurning );

	// If we have a pawn we need to determine if we should show/hide the weapon
	ASoldierCharacter* MyPawn = Cast<ASoldierCharacter>( GetPawn( ) );
	ASoldierWeapon* MyWeapon = MyPawn ? MyPawn->GetWeapon( ) : NULL;
	if (MyWeapon) {
		if (bInCinematicMode && bHidePlayer) {
			MyWeapon->SetActorHiddenInGame( true );
		}
		else if (!bCinematicMode) {
			MyWeapon->SetActorHiddenInGame( false );
		}
	}
}

bool ASoldierPlayerController::IsMoveInputIgnored( ) const
{
	if (IsInState( NAME_Spectating )) {
		return false;
	}
	else {
		return Super::IsMoveInputIgnored( );
	}
}

bool ASoldierPlayerController::IsLookInputIgnored( ) const
{
	if (IsInState( NAME_Spectating )) {
		return false;
	}
	else {
		return Super::IsLookInputIgnored( );
	}
}

void ASoldierPlayerController::InitInputSystem( )
{
	Super::InitInputSystem( );

	USoldierPersistentUser* PersistentUser = GetPersistentUser( );
	if (PersistentUser) {
		PersistentUser->TellInputAboutKeybindings( );
	}
}

void ASoldierPlayerController::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );

	DOREPLIFETIME_CONDITION( ASoldierPlayerController, bInfiniteAmmo, COND_OwnerOnly );
	DOREPLIFETIME_CONDITION( ASoldierPlayerController, bInfiniteClip, COND_OwnerOnly );

	DOREPLIFETIME( ASoldierPlayerController, bHealthRegen );
}

void ASoldierPlayerController::Suicide( )
{
	if (IsInState( NAME_Playing )) {
		ServerSuicide( );
	}
}

bool ASoldierPlayerController::ServerSuicide_Validate( )
{
	return true;
}

void ASoldierPlayerController::ServerSuicide_Implementation( )
{
	if ((GetPawn( ) != NULL) && ((GetWorld( )->TimeSeconds - GetPawn( )->CreationTime > 1) || (GetNetMode( ) == NM_Standalone))) {
		ASoldierCharacter* MyPawn = Cast<ASoldierCharacter>( GetPawn( ) );
		if (MyPawn) {
			MyPawn->Suicide( );
		}
	}
}

bool ASoldierPlayerController::HasInfiniteAmmo( ) const
{
	return bInfiniteAmmo;
}

bool ASoldierPlayerController::HasInfiniteClip( ) const
{
	return bInfiniteClip;
}

bool ASoldierPlayerController::HasHealthRegen( ) const
{
	return bHealthRegen;
}

bool ASoldierPlayerController::HasGodMode( ) const
{
	return bGodMode;
}

bool ASoldierPlayerController::IsVibrationEnabled( ) const
{
	return bIsVibrationEnabled;
}

bool ASoldierPlayerController::IsGameInputAllowed( ) const
{
	return bAllowGameActions && !bCinematicMode;
}

void ASoldierPlayerController::ToggleChatWindow( )
{

	ASoldierHUD* SoldierHUD = Cast<ASoldierHUD>( GetHUD( ) );
	if (SoldierHUD) {
		SoldierHUD->ToggleChat( );
	}
}

void ASoldierPlayerController::LoadScenario( )
{
	USimOceanSceneManager_Singleton::GetInstance( )->GenOceanScenarioActor( );
}

void ASoldierPlayerController::LoadOceanBarrierScenario( )
{
	USimOceanSceneManager_Singleton::GetInstance( )->GenOceanScenarioBarrier( );
}

void ASoldierPlayerController::BoatDetails()
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD();
	if (SoldierHUD && (SoldierHUD->IsMatchOver() == false)) {
		SoldierHUD->ToggleBoatDetails();		
	}
}

void ASoldierPlayerController::ViewBoat()
{
	APlayerCameraManager* camMgr = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (camMgr)
	{		
		AActor* boat = IGameModeInterface::Execute_GetBoat(UGameplayStatics::GetGameMode(this), SwitchBoatIndex);
		if (boat)
		{
			SetViewTarget(boat);
			ReservedViewTarget = boat;
			SwitchBoatIndex++;
		}
		else
		{
			SetViewTarget(NULL);
			ReservedViewTarget = NULL;
			SwitchBoatIndex = 0;
		}		
	}
}

void ASoldierPlayerController::UpdateRotation(float DeltaTime)
{
	Super::UpdateRotation(DeltaTime);

	FRotator rot;
	//AActor* target = GetViewTarget();
	APlayerCameraManager* camMgr = UGameplayStatics::GetPlayerCameraManager(this, 0);
	if (camMgr)
	{		
		if (ReservedViewTarget)
		{
			rot.Yaw = GetControlRotation().Yaw;
			rot.Pitch = GetControlRotation().Pitch;
			UCameraComponent* cam = Cast<UCameraComponent>(ReservedViewTarget->GetComponentByClass(UCameraComponent::StaticClass()));
			if (cam)
				cam->SetWorldRotation(rot);
		}

	}
}

void ASoldierPlayerController::ResetView()
{
	SetViewTarget(NULL);
	ReservedViewTarget = NULL;
	SwitchBoatIndex = 0;
}

void ASoldierPlayerController::Turn(float val)
{
	AddYawInput(val);
}

void ASoldierPlayerController::LookUp(float val)
{
	AddPitchInput(val);
}

void ASoldierPlayerController::StartSimulate()
{
	IGameModeInterface::Execute_StartMove(UGameplayStatics::GetGameMode(this));
	USimOceanSceneManager_Singleton::GetInstance( )->ChangeFormationType( "pz01_5", EBoatFormation::E_SINGLE_COLUMN_FORMATION );
}

void ASoldierPlayerController::ClientTeamMessage_Implementation( APlayerState* SenderPlayerState, const FString& S, FName Type, float MsgLifeTime )
{
	ASoldierHUD* SoldierHUD = Cast<ASoldierHUD>( GetHUD( ) );
	if (SoldierHUD) {
		if (Type == ServerSayString) {
			if (SenderPlayerState != PlayerState) {
				SoldierHUD->AddChatLine( FText::FromString( S ), false );
			}
		}
	}
}

void ASoldierPlayerController::Say( const FString& Msg )
{
	ServerSay( Msg.Left( 128 ) );
}

bool ASoldierPlayerController::ServerSay_Validate( const FString& Msg )
{
	return true;
}

void ASoldierPlayerController::ServerSay_Implementation( const FString& Msg )
{
	GetWorld( )->GetAuthGameMode<AOceanBoatsGameMode>( )->Broadcast( this, Msg, ServerSayString );
}

ASoldierHUD* ASoldierPlayerController::GetSoldierHUD( ) const
{
	return Cast<ASoldierHUD>( GetHUD( ) );
}


USoldierPersistentUser* ASoldierPlayerController::GetPersistentUser( ) const
{
	USoldierLocalPlayer* const SoldierLocalPlayer = Cast<USoldierLocalPlayer>( Player );
	return SoldierLocalPlayer ? SoldierLocalPlayer->GetPersistentUser( ) : nullptr;
}

bool ASoldierPlayerController::SetPause( bool bPause, FCanUnpause CanUnpauseDelegate /*= FCanUnpause()*/ )
{
	const bool Result = APlayerController::SetPause( bPause, CanUnpauseDelegate );
	return Result;
}

FVector ASoldierPlayerController::GetFocalLocation( ) const
{
	const ASoldierCharacter* SoldierCharacter = Cast<ASoldierCharacter>( GetPawn( ) );

	// On death we want to use the player's death cam location rather than the location of where the pawn is at the moment
	// This guarantees that the clients see their death cam correctly, as their pawns have delayed destruction.
	if (SoldierCharacter && SoldierCharacter->bIsDying) {
		return GetSpawnLocation( );
	}

	return Super::GetFocalLocation( );
}

void ASoldierPlayerController::ShowInGameMenu( )
{
	ASoldierHUD* SoldierHUD = GetSoldierHUD( );
	if (SoldierIngameMenu.IsValid( ) && !SoldierIngameMenu->GetIsGameMenuUp( ) && SoldierHUD && (SoldierHUD->IsMatchOver( ) == false)) {
		SoldierIngameMenu->ToggleGameMenu( );
	}
}
void ASoldierPlayerController::UpdateAchievementsOnGameEnd( )
{
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>( Player );
	if (LocalPlayer) {
		ASoldierPlayerState* SoldierPlayerState = Cast<ASoldierPlayerState>( PlayerState );
		if (SoldierPlayerState) {
			const USoldierPersistentUser*  PersistentUser = GetPersistentUser( );

			if (PersistentUser) {
				const int32 Wins = PersistentUser->GetWins( );
				const int32 Losses = PersistentUser->GetLosses( );
				const int32 Matches = Wins + Losses;

				const int32 TotalKills = PersistentUser->GetKills( );
				const int32 MatchScore = 1;

				const int32 TotalBulletsFired = PersistentUser->GetBulletsFired( );
				const int32 TotalRocketsFired = PersistentUser->GetRocketsFired( );

				float TotalGameAchievement = 0;
				float CurrentGameAchievement = 0;

				///////////////////////////////////////
				// Kill achievements
				if (TotalKills >= 1) {
					CurrentGameAchievement += 100.0f;
				}
				TotalGameAchievement += 100;

				{
					float fSomeKillPct = ((float)TotalKills / (float)SomeKillsCount) * 100.0f;
					fSomeKillPct = FMath::RoundToFloat( fSomeKillPct );
					UpdateAchievementProgress( ACH_SOME_KILLS, fSomeKillPct );

					CurrentGameAchievement += FMath::Min( fSomeKillPct, 100.0f );
					TotalGameAchievement += 100;
				}

				{
					float fLotsKillPct = ((float)TotalKills / (float)LotsKillsCount) * 100.0f;
					fLotsKillPct = FMath::RoundToFloat( fLotsKillPct );
					UpdateAchievementProgress( ACH_LOTS_KILLS, fLotsKillPct );

					CurrentGameAchievement += FMath::Min( fLotsKillPct, 100.0f );
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Match Achievements
				{
					UpdateAchievementProgress( ACH_FINISH_MATCH, 100.0f );

					CurrentGameAchievement += 100;
					TotalGameAchievement += 100;
				}

				{
					float fLotsRoundsPct = ((float)Matches / (float)LotsMatchesCount) * 100.0f;
					fLotsRoundsPct = FMath::RoundToFloat( fLotsRoundsPct );
					UpdateAchievementProgress( ACH_LOTS_MATCHES, fLotsRoundsPct );

					CurrentGameAchievement += FMath::Min( fLotsRoundsPct, 100.0f );
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Win Achievements
				if (Wins >= 1) {
					UpdateAchievementProgress( ACH_FIRST_WIN, 100.0f );

					CurrentGameAchievement += 100.0f;
				}
				TotalGameAchievement += 100;

				{
					float fLotsWinPct = ((float)Wins / (float)LotsWinsCount) * 100.0f;
					fLotsWinPct = FMath::RoundToInt( fLotsWinPct );
					UpdateAchievementProgress( ACH_LOTS_WIN, fLotsWinPct );

					CurrentGameAchievement += FMath::Min( fLotsWinPct, 100.0f );
					TotalGameAchievement += 100;
				}

				{
					float fManyWinPct = ((float)Wins / (float)ManyWinsCount) * 100.0f;
					fManyWinPct = FMath::RoundToInt( fManyWinPct );
					UpdateAchievementProgress( ACH_MANY_WIN, fManyWinPct );

					CurrentGameAchievement += FMath::Min( fManyWinPct, 100.0f );
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Ammo Achievements
				{
					float fLotsBulletsPct = ((float)TotalBulletsFired / (float)LotsBulletsCount) * 100.0f;
					fLotsBulletsPct = FMath::RoundToFloat( fLotsBulletsPct );
					UpdateAchievementProgress( ACH_SHOOT_BULLETS, fLotsBulletsPct );

					CurrentGameAchievement += FMath::Min( fLotsBulletsPct, 100.0f );
					TotalGameAchievement += 100;
				}

				{
					float fLotsRocketsPct = ((float)TotalRocketsFired / (float)LotsRocketsCount) * 100.0f;
					fLotsRocketsPct = FMath::RoundToFloat( fLotsRocketsPct );
					UpdateAchievementProgress( ACH_SHOOT_ROCKETS, fLotsRocketsPct );

					CurrentGameAchievement += FMath::Min( fLotsRocketsPct, 100.0f );
					TotalGameAchievement += 100;
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Score Achievements
				{
					float fGoodScorePct = ((float)MatchScore / (float)GoodScoreCount) * 100.0f;
					fGoodScorePct = FMath::RoundToFloat( fGoodScorePct );
					UpdateAchievementProgress( ACH_GOOD_SCORE, fGoodScorePct );
				}

				{
					float fGreatScorePct = ((float)MatchScore / (float)GreatScoreCount) * 100.0f;
					fGreatScorePct = FMath::RoundToFloat( fGreatScorePct );
					UpdateAchievementProgress( ACH_GREAT_SCORE, fGreatScorePct );
				}
				///////////////////////////////////////

				///////////////////////////////////////
				// Map Play Achievements
				UWorld* World = GetWorld( );
				if (World) {
					FString MapName = *FPackageName::GetShortName( World->PersistentLevel->GetOutermost( )->GetName( ) );
					if (MapName.Find( TEXT( "BreakBoats-20200408" ) ) != -1) {
						UpdateAchievementProgress( ACH_PLAY_HIGHRISE, 100.0f );
					}
					else if (MapName.Find( TEXT( "BreakBoats-20200408" ) ) != -1) {
						UpdateAchievementProgress( ACH_PLAY_SANCTUARY, 100.0f );
					}
				}
				///////////////////////////////////////			

				
			}
		}
	}
}

void ASoldierPlayerController::UpdateLeaderboardsOnGameEnd( )
{

}

void ASoldierPlayerController::UpdateStatsOnGameEnd( bool bIsWinner )
{
	
}


void ASoldierPlayerController::UpdateSaveFileOnGameEnd( bool bIsWinner )
{
	
}

void ASoldierPlayerController::PreClientTravel( const FString& PendingURL, ETravelType TravelType, bool bIsSeamlessTravel )
{
	Super::PreClientTravel( PendingURL, TravelType, bIsSeamlessTravel );

	if (const UWorld* World = GetWorld( )) {
		UArmySimGameViewportClient* SoldierViewport = Cast<UArmySimGameViewportClient>( World->GetGameViewport( ) );

		if (SoldierViewport != NULL) {
			SoldierViewport->ShowLoadingScreen( );
		}

		ASoldierHUD* SoldierHUD = Cast<ASoldierHUD>( GetHUD( ) );
		if (SoldierHUD != nullptr) {
			// Passing true to bFocus here ensures that focus is returned to the game viewport.
			SoldierHUD->ShowScoreboard( false, true );
		}
	}
}
