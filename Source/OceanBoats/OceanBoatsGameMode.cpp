// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoatsGameMode.h"
#include "OceanBoatsCharacter.h"
#include "SoldierCharacter.h"
#include "UI/SoldierHUD.h"
#include "Player/SoldierSpectatorPawn.h"
#include "Player/SoldierDemoSpectator.h"
#include "UObject/ConstructorHelpers.h"
#include "Bots/SoldierAIController.h"
#include "ArmySimGameInstance.h"
#include "SoldierPlayerState.h"
#include "BoatInterface.h"
#include "Sim_OceanSceneManager.h"
#include "SimDataStructure.h"
#include "SimEcs_EntityManager.h"
#include "UnInstallAnimation_DLT.h"
#include "FSM\Sim_FsmStateDataBase.h"
#include "SoldierPawn.h"
#include "SimApi.h"
#include "OceanManager.h"

#define RAINCLASS "/Game/GoodRain/Blueprints/BP_GR_GoodRain_Environment.BP_GR_GoodRain_Environment_C"
#define SNOWCLASS "/Game/Snow/BP_Snow.BP_Snow_C"

AOceanBoatsGameMode::AOceanBoatsGameMode( /*const FObjectInitializer& ObjectInitializer*/ ) //: Super( ObjectInitializer )
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/Pawns/SoldierPawn_C"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//}

	//static ConstructorHelpers::FClassFinder<APawn> BotPawnOb( TEXT( "/Game/Blueprints/Pawns/BotPawn_C" ) );
	//BotPawnClass = BotPawnOb.Class;

	HUDClass = ASoldierHUD::StaticClass( );
	PlayerControllerClass = ASoldierPlayerController::StaticClass( );
	SpectatorClass = ASoldierSpectatorPawn::StaticClass( );
	GameStateClass = ASoldierGameState::StaticClass( );
	ReplaySpectatorPlayerControllerClass = ASoldierDemoSpectator::StaticClass( );
	PlayerStateClass = ASoldierPlayerState::StaticClass();
	MinRespawnDelay = 5.0f;
	DefaultPawnClass = ASoldierPawn::StaticClass();

	bAllowBots = true;
	bNeedsBotCreation = true; 
	bUseSeamlessTravel = FParse::Param( FCommandLine::Get( ), TEXT( "NoSeamlessTravel" ) ) ? false : true;
}

void AOceanBoatsGameMode::PostInitProperties( )
{
	Super::PostInitProperties( );
	if (PlatformPlayerControllerClass != nullptr) {
		PlayerControllerClass = PlatformPlayerControllerClass;
	}
}


FString AOceanBoatsGameMode::GetBotsCountOptionName( )
{
	return FString( TEXT( "Bots" ) );
}

void AOceanBoatsGameMode::InitBehaviac() {

	NetWork::InitUdp("192.168.2.200",9888);
	behaviac::InitBehaviorLoader();
	FString strPath = FPaths::ProjectDir() + TEXT("Config/BehaviacXML");
	behaviac::Workspace::GetInstance()->SetFilePath(TCHAR_TO_ANSI(*strPath));
	behaviac::Workspace::GetInstance()->SetFileFormat(behaviac::Workspace::EFF_xml);
	BahaviacManager::GetInstance().GetBehaviacHandler().ReginsterHandler<AOceanBoatsGameMode>(ENUM_METHOD_END, this, &AOceanBoatsGameMode::EndTask);
	BahaviacManager::GetInstance().GetBehaviacHandler().ReginsterHandler<AOceanBoatsGameMode>(ENUM_METHOD_ENTITY_MOVE, this, &AOceanBoatsGameMode::MoveEntity);
	BahaviacManager::GetInstance().GetBehaviacHandler().ReginsterHandler<AOceanBoatsGameMode>(ENUM_METHOD_ENTITY_MOVE_BACK, this, &AOceanBoatsGameMode::MoveBackEntity);
	BahaviacManager::GetInstance().GetBehaviacHandler().ReginsterHandler<AOceanBoatsGameMode>(ENUM_METHOD_ENTITY_FIRE, this, &AOceanBoatsGameMode::Fire);
	BahaviacManager::GetInstance( ).GetBehaviacHandler( ).ReginsterHandler<AOceanBoatsGameMode>( ENUM_METHOD_OPEN_CABIN_ANIMATION, this, &AOceanBoatsGameMode::OpenCabinAnimation );
	BahaviacManager::GetInstance( ).GetBehaviacHandler( ).ReginsterHandler<AOceanBoatsGameMode>( ENUM_METHOD_CLOSE_CABIN_ANIMATION, this, &AOceanBoatsGameMode::ClosedCabinAnimation );
	BahaviacManager::GetInstance( ).GetBehaviacHandler( ).ReginsterHandler<AOceanBoatsGameMode>( ENUM_METHOD_UNINSTALL_BOAT_ANIMATION, this, &AOceanBoatsGameMode::UnInstallBoatAnimation );
	_AiImagineAgent = behaviac::Agent::Create<AiImagineAgent>();
	bool bRet = _AiImagineAgent->btload("RootBehavior");
	_AiImagineAgent->btsetcurrent("RootBehavior");


	GetWorldTimerManager( ).SetTimer( TimerHandle_DefaultTimer, this, &AOceanBoatsGameMode::DefaultTimer, GetWorldSettings( )->GetEffectiveTimeDilation( ), true );

	_initilize = true;
}


RetVar AOceanBoatsGameMode::EndTask(int argc, void ** argv)
{
	_wait = true;
	RetVar ret;
	ret.iRet = 0;
	_initilize = false;
	return ret;
}

RetVar AOceanBoatsGameMode::MoveEntity(int argc, void ** argv) 
{
	RetVar ret;
	if (argc>0&&argv) 
	{
		FString name;
		name += FString(UTF8_TO_TCHAR((char *)argv[0]));
		double PosX = *((double*)(argv[1]));
		double PosY = *((double*)(argv[2]));
		double dir  = *((double*)(argv[3]));

		if (FMath::Abs(PosX - 720)<0.1f && FMath::Abs( PosY - 2140 )<0.1)
		{
			USimOceanSceneManager_Singleton::GetInstance( )->ChangeFormationType( name,EBoatFormation::E_SINGLE_ROW_FORMATION );
		}
		USimOceanSceneManager_Singleton::GetInstance()->MoveEntity(name, FVector(PosX, PosY, 0));
		bool isArrived = USimOceanSceneManager_Singleton::GetInstance( )->IsArriving( name, FVector( PosX, PosY, 0 ) );
		if (isArrived) {
			USimOceanSceneManager_Singleton::GetInstance( )->SetIdle( name, FVector( PosX, PosY, 0 ) );
			ret.iRet = behaviac::BT_SUCCESS;
			GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "leader arrived" );
			return ret;
		}
	}
	ret.iRet = behaviac::BT_RUNNING;
	return ret;
}

RetVar AOceanBoatsGameMode::MoveBackEntity(int argc, void ** argv)
{
	RetVar ret;
	if (argc > 0 && argv)
	{
		FString name;
		name += FString(UTF8_TO_TCHAR((char *)argv[0]));
		double PosX = *((double*)(argv[1]));
		double PosY = *((double*)(argv[2]));
		double dir = *((double*)(argv[3]));
		//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "MoveBackEntity");
		USimOceanSceneManager_Singleton::GetInstance()->MoveBackEntity(name, FVector(PosX, PosY, 0));
		bool isArrived = USimOceanSceneManager_Singleton::GetInstance()->IsArriving(name, FVector(PosX, PosY, 0));
		if (isArrived) {
			USimOceanSceneManager_Singleton::GetInstance( )->SetIdle( name, FVector( PosX, PosY, 0 ) );
			ret.iRet = behaviac::BT_SUCCESS;
			return ret;
		}
		//return MoveEntity(name, PosX, PosY, dir);
	}
	ret.iRet = behaviac::BT_RUNNING;
	return ret;
}
RetVar AOceanBoatsGameMode::Fire(int argc, void ** argv) {
	RetVar ret;
	if (argc > 0 && argv)
	{
		FString name;
		name += FString(UTF8_TO_TCHAR((char *)argv[0]));
		bool bFire = *((bool*)(argv[1]));
		//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "Fire");
		USimOceanSceneManager_Singleton::GetInstance( )->Firing( name, bFire );
	}
	ret.iRet = behaviac::BT_SUCCESS;
	return ret;
}

RetVar AOceanBoatsGameMode::OpenCabinAnimation( int argc, void ** argv ) {
	RetVar ret;
	if (argc > 0 && argv) {
		FString name;
		name += FString( UTF8_TO_TCHAR( (char *)argv[0] ) );
		//bool bFire = *((bool*)(argv[1]));
		//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "Fire");
		//USimOceanSceneManager_Singleton::GetInstance( )->Firing( name, bFire );
	}
	ret.iRet = behaviac::BT_SUCCESS;
	return ret;
}

RetVar AOceanBoatsGameMode::ClosedCabinAnimation( int argc, void ** argv ) {
	RetVar ret;
	if (argc > 0 && argv) {
		FString name;
		name += FString( UTF8_TO_TCHAR( (char *)argv[0] ) );
		bool bFire = *((bool*)(argv[1]));
		//GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, "Fire");
		//USimOceanSceneManager_Singleton::GetInstance( )->Firing( name, bFire );
	}
	ret.iRet = behaviac::BT_SUCCESS;
	return ret;
}

RetVar AOceanBoatsGameMode::UnInstallBoatAnimation( int argc, void ** argv ) {
	RetVar ret;
	if (argc > 0 && argv) {
		FString name;
		name += FString( UTF8_TO_TCHAR( (char *)argv[0] ) );
		//bool bFire = *((bool*)(argv[1]));
		//auto interface = USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager()->GetFsm( *name );
		//if (interface->GetName( )== "") {

		//	USimOceanSceneManager_Singleton::GetInstance( )->ChangeDLTAnimationState( *name );
		//}
		//if(interface->IsRunning())
		//	ret.iRet = behaviac::BT_RUNNING;
		//else
			ret.iRet = behaviac::BT_SUCCESS;
	}
	
	return ret;
}


RetVar AOceanBoatsGameMode::MoveEntity(FString name, double PosX, double PosY, double dir) {

	RetVar ret;
	if (GEngine)
	{
		TArray<AActor*> BoatActors;
		UGameplayStatics::GetAllActorsWithTag( GetWorld( ), TEXT( "Sim_BoatArchetype" ), BoatActors );
		for (AActor* actor : BoatActors) {
			UE_LOG( LogTemp, Warning, TEXT( "-------%s------" ), *(actor->GetName( )) );
			if (name.Compare( actor->GetName( )) == 0 ) {

				FVector relativePos; relativePos.Set( PosX*100.0f, PosY*100.0f, 0 );
				relativePos = USimOceanSceneManager_Singleton::GetInstance( )->GetCovertScenePosition(relativePos );
				//GEngine->AddOnScreenDebugMessage( -1, 18.f, FColor::Red, relativePos.ToString() );
				/*if(auto Boat_Arche = Cast<ABoat_Archetype>( actor ))
					Boat_Arche->Active_MoveOn( relativePos );*/
			}
		}
	}
	
	ret.iRet = behaviac::BT_SUCCESS;
	return ret;
}
void AOceanBoatsGameMode::BehaviacExec()
{
	if (!_wait)
	{
		_AiImagineAgent->btexec();
	}
	
	time_t now = FDateTime::Now().GetTicks() / ETimespan::TicksPerMillisecond;
	static time_t _last_beat = 0;
	static time_t run_time = 0;
	static time_t tick_time = 0;
	static time_t max_time = 100;
	if (_last_beat != 0 && now > _last_beat)
	{
		run_time += (now - _last_beat);
		behaviac::Workspace::GetInstance()->SetDoubleValueSinceStartup((double)run_time);
	}


	/*if (run_time - tick_time > max_time)
	{
		
		
		FString  jsonstr;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonstr);
		Writer->WriteObjectStart();
		Writer->WriteValue("Num", 1);
		Writer->WriteValue("J", 1);
		Writer->WriteValue("W", 1);
		Writer->WriteValue("Speed", 1);
		Writer->WriteObjectEnd();
		Writer->Close();
		NetWork::SendMsg(TCHAR_TO_UTF8(*jsonstr));

		tick_time = run_time;
	}*/
	_last_beat = now;
	
}


void AOceanBoatsGameMode::InitGame( const FString& MapName, const FString& Options, FString& ErrorMessage )
{
	//const int32 BotsCountOptionValue = UGameplayStatics::GetIntOption( Options, GetBotsCountOptionName( ), 0 );
	//SetAllowBots( BotsCountOptionValue > 0 ? true : false, BotsCountOptionValue );
	Super::InitGame( MapName, Options, ErrorMessage );
	//GEngine->AddOnScreenDebugMessage( -1, 18.f, FColor::Blue, MapName );
	//InitObstacles();
}

//void AOceanBoatsGameMode::InitObstacles()
//{
//	AddObsctacleType("GTZ",Obstacle_GTZ);//轨条砦
//	AddObsctacleType("SJZ",Obstacle_STZ);//三角锥
//	AddObsctacleType("HLS",Obstacle_HLS);//海立石
//}

void AOceanBoatsGameMode::SetAllowBots( bool bInAllowBots, int32 InMaxBots )
{
	bAllowBots = bInAllowBots;
	MaxBots = InMaxBots;
}


void AOceanBoatsGameMode::PreInitializeComponents( )
{
	Super::PreInitializeComponents( );

	
}

float AOceanBoatsGameMode::ModifyDamage( float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) const
{
	float ActualDamage = Damage;

	ASoldierCharacter* DamagedPawn = Cast<ASoldierCharacter>( DamagedActor );
	if (DamagedPawn && EventInstigator) {
		//ASoldierPlayerState* DamagedPlayerState = Cast<ASoldierPlayerState>( DamagedPawn->GetPlayerState( ) );
		//ASoldierPlayerState* InstigatorPlayerState = Cast<ASoldierPlayerState>( EventInstigator->PlayerState );

		// disable friendly fire
		if (!CanDealDamage( )) {
			ActualDamage = 0.0f;
		}

		// scale self instigated damage
		/*if (InstigatorPlayerState == DamagedPlayerState) {
			ActualDamage *= DamageSelfScale;
		}*/
	}

	return ActualDamage;
}

bool AOceanBoatsGameMode::CanDealDamage(  ) const
{
	return true;
}


void AOceanBoatsGameMode::PreLogin( const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage )
{
	// GameSession can be NULL if the match is over
	Super::PreLogin( Options, Address, UniqueId, ErrorMessage );
	
}


void AOceanBoatsGameMode::PostLogin( APlayerController* NewPlayer )
{
	Super::PostLogin( NewPlayer );

	// update spectator location for client
	ASoldierPlayerController* NewPC = Cast<ASoldierPlayerController>( NewPlayer );
	if (NewPC && NewPC->GetPawn( ) == NULL) {
		NewPC->ClientSetSpectatorCamera( NewPC->GetSpawnLocation( ), NewPC->GetControlRotation( ) );
	}

	// notify new player if match is already in progress
	if (NewPC && IsMatchInProgress( )) {
		NewPC->ClientGameStarted( );
		NewPC->ClientStartOnlineGame( );
	}
}

void AOceanBoatsGameMode::Killed(AController* Killer, AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType)
{
	ASoldierPlayerState* state = Cast<ASoldierPlayerState>(Killer->PlayerState);
	state->ScoreKill(Killer->PlayerState,KillScore);
}

void AOceanBoatsGameMode::Killed( AController* Killer )
{
	ASoldierPlayerState* state = Cast<ASoldierPlayerState>(Killer->PlayerState);
	state->ScoreKill();
}

void AOceanBoatsGameMode::DefaultTimer()
{

	ASoldierGameState* const MyGameState = Cast<ASoldierGameState>( GameState );
	if (MyGameState && MyGameState->RemainingTime > 0 && !MyGameState->bTimerPaused) {
		MyGameState->RemainingTime--;

		if (MyGameState->RemainingTime <= 0) {
			if (GetMatchState( ) == MatchState::InProgress) {
				FinishMatch( );

				// Send end round events 
				for (FConstControllerIterator It = GetWorld( )->GetControllerIterator( ); It; ++It) {
					ASoldierPlayerController* PlayerController = Cast<ASoldierPlayerController>( *It );

					if (PlayerController && MyGameState) {
						ASoldierPlayerState* PlayerState = Cast<ASoldierPlayerState>( (*It)->PlayerState );
						const bool bIsWinner = false;// IsBattleResult( PlayerState );
    
						
					}
				}
			}
			StartMatch( );
		}
	}
	if(_initilize)
		BehaviacExec();

}



bool AOceanBoatsGameMode::IsSpawnpointAllowed( APlayerStart* SpawnPoint, AController* Player ) const
{
	return false;
}

bool AOceanBoatsGameMode::IsSpawnpointPreferred( APlayerStart* SpawnPoint, AController* Player ) const
{
	return false;
	ACharacter* MyPawn = Cast<ACharacter>( (*DefaultPawnClass)->GetDefaultObject<ACharacter>( ) );
	ASoldierAIController* AIController = Cast<ASoldierAIController>( Player );
	if (AIController != nullptr) {
		//MyPawn = Cast<ACharacter>( BotPawnClass->GetDefaultObject<ACharacter>( ) );
	}

	if (MyPawn) {
		const FVector SpawnLocation = SpawnPoint->GetActorLocation( );
		for (ACharacter* OtherPawn : TActorRange<ACharacter>( GetWorld( ) )) {
			if (OtherPawn != MyPawn) {
				const float CombinedHeight = (MyPawn->GetCapsuleComponent( )->GetScaledCapsuleHalfHeight( ) + OtherPawn->GetCapsuleComponent( )->GetScaledCapsuleHalfHeight( )) * 2.0f;
				const float CombinedRadius = MyPawn->GetCapsuleComponent( )->GetScaledCapsuleRadius( ) + OtherPawn->GetCapsuleComponent( )->GetScaledCapsuleRadius( );
				const FVector OtherLocation = OtherPawn->GetActorLocation( );

				// check if player start overlaps this pawn
				if (FMath::Abs( SpawnLocation.Z - OtherLocation.Z ) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D( ) < CombinedRadius) {
					return false;
				}
			}
		}
	}
	else {
		return false;
	}

	return true;
}

void AOceanBoatsGameMode::CreateBotControllers( )
{
	return;
	UWorld* World = GetWorld( );
	int32 ExistingBots = 0;
	for (FConstControllerIterator It = World->GetControllerIterator( ); It; ++It) {
		ASoldierAIController* AIC = Cast<ASoldierAIController>( *It );
		if (AIC) {
			++ExistingBots;
		}
	}

	// Create any necessary AIControllers.  Hold off on Pawn creation until pawns are actually necessary or need recreating.	
	int32 BotNum = ExistingBots;
	for (int32 i = 0; i < MaxBots - ExistingBots; ++i) {
		CreateBot( BotNum + i );
	}
}

ASoldierAIController* AOceanBoatsGameMode::CreateBot( int32 BotNum )
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = nullptr;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = nullptr;

	UWorld* World = GetWorld( );
	ASoldierAIController* AIC = World->SpawnActor<ASoldierAIController>( SpawnInfo );
	InitBot( AIC, BotNum );

	return AIC;
}

void AOceanBoatsGameMode::StartBots( )
{
	// checking number of existing human player.
	UWorld* World = GetWorld( );
	for (FConstControllerIterator It = World->GetControllerIterator( ); It; ++It) {
		ASoldierAIController* AIC = Cast<ASoldierAIController>( *It );
		if (AIC) {
			RestartPlayer( AIC );
		}
	}
}


void AOceanBoatsGameMode::RestartPlayer( AController* NewPlayer )
{
	Super::RestartPlayer( NewPlayer );

	ASoldierPlayerController* PC = Cast<ASoldierPlayerController>( NewPlayer );
	if (PC) {
		// Since initial weapon is equipped before the pawn is added to the replication graph, need to resend the notify so that it can be added as a dependent actor
	/*	ASoldierCharacter* Character = Cast<ASoldierCharacter>( PC->GetCharacter( ) );
		if (Character) {
			ASoldierCharacter::NotifyEquipWeapon.Broadcast( Character, Character->GetWeapon( ) );
		}*/

		PC->ClientGameStarted( );
	}
}


void AOceanBoatsGameMode::InitBot( ASoldierAIController* AIController, int32 BotNum )
{
	if (AIController) {
		if (AIController->PlayerState) {
			FString BotName = FString::Printf( TEXT( "Bot %d" ), BotNum );
			AIController->PlayerState->SetPlayerName( BotName );
		}
	}
}

void AOceanBoatsGameMode::DetermineMatchWinner( )
{
	// nothing to do here
}

bool AOceanBoatsGameMode::IsWinner( class ASoldierPlayerState* PlayerState ) const
{
	return false;
}


void AOceanBoatsGameMode::RestartGame( )
{
	// Hide the scoreboard too !
	for (FConstControllerIterator It = GetWorld( )->GetControllerIterator( ); It; ++It) {
		ASoldierPlayerController* PlayerController = Cast<ASoldierPlayerController>( *It );
		if (PlayerController != nullptr) {
			ASoldierHUD* SoldierHUD = Cast<ASoldierHUD>( PlayerController->GetHUD( ) );
			if (SoldierHUD != nullptr) {
				// Passing true to bFocus here ensures that focus is returned to the game viewport.
				SoldierHUD->ShowScoreboard( false, true );
			}
		}
	}

	Super::RestartGame( );
}


void AOceanBoatsGameMode::FinishMatch( )
{
	
}


void AOceanBoatsGameMode::RequestFinishAndExitToMainMenu( )
{
	FinishMatch( );

	UArmySimGameInstance* const GameInstance = Cast<UArmySimGameInstance>( GetGameInstance( ) );
	if (GameInstance) {
		GameInstance->RemoveSplitScreenPlayers( );
	}

	ASoldierPlayerController* LocalPrimaryController = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld( )->GetPlayerControllerIterator( ); Iterator; ++Iterator) {
		ASoldierPlayerController* Controller = Cast<ASoldierPlayerController>( *Iterator );

		if (Controller == NULL) {
			continue;
		}

		if (!Controller->IsLocalController( )) {
			const FText RemoteReturnReason = NSLOCTEXT( "NetworkErrors", "HostHasLeft", "Host has left the game." );
			Controller->ClientReturnToMainMenuWithTextReason( RemoteReturnReason );
		}
		else {
			LocalPrimaryController = Controller;
		}
	}

	// GameInstance should be calling this from an EndState.  So call the PC function that performs cleanup, not the one that sets GI state.
	if (LocalPrimaryController != NULL) {
		LocalPrimaryController->HandleReturnToMainMenu( );
	}
}

//void AOceanBoatsGameMode::AddObsctacleType(FString name, ObstacleType type)
//{
//	OceanObstacleCls ob;
//	ob.name = name;
//	ob.type = type;
//
//	int index = ObstacleCollection.Find(ob);
//	if (index < 0)
//		index = ObstacleCollection.Add(ob);
//}

void AOceanBoatsGameMode::StartMove_Implementation()
{
	/*for (int i = 0; i < boats.Num(); ++i)
	{
		IBoatInterface::Execute_SetMoveMode(boats[i], OceanMoveMode::EMoveMode_On);
		IBoatInterface::Execute_SpeedUp(boats[i]);
	}*/
	InitBehaviac( );
}

void AOceanBoatsGameMode::AddBoat_Implementation(AActor* boat)
{
	boats.Add(boat);
}

//void AOceanBoatsGameMode::AddRuinedBoat_Implementation(AActor* boat)
//{
//
//}

//void AOceanBoatsGameMode::BroadcastDestroy_Implementation(AActor* victim, int killerId, ObstacleType type)
//{
//	OceanObstacleCls ob;
//	ob.type = type;
//	int index = ObstacleCollection.Find(ob);
//	if (index >= 0)
//	{
//		ObstacleCollection[index].destroyed++;
//	}
//
//	FConstControllerIterator iter = GWorld->GetControllerIterator();
//	Killed(iter->Get());
//}

//int AOceanBoatsGameMode::GetKilledNum_Implementation(ObstacleType type)
//{
//	OceanObstacleCls ob;
//	ob.type = type;
//	int index = ObstacleCollection.Find(ob);
//	if (index >= 0)
//		return ObstacleCollection[index].destroyed;
//
//	return 0;
//}

int AOceanBoatsGameMode::GetBoatNum_Implementation()
{
	int num = 0;
	for (auto boat : USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes)
	{
		if (boat.Value.Get()->ArchType >= EEE_COASTDEF_TYPE)
			continue;
		num++;
	}
	return num;// USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes.Num();//boats.Num();
}

AActor* AOceanBoatsGameMode::GetBoat_Implementation(int index)
{
	int num = USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes.Num();
	int step = 0;
	for (auto boat : USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes)
	{
		if(boat.Value.Get()->ArchType >= EEE_COASTDEF_TYPE)
			continue;

		if (step == num)
		{
			break;
		}
		if (step != index)
		{
			step++;
			continue;
		}
		else
			return boat.Value.Get();

	}
	//if(index < boats.Num())
	//	return boats[index];
	return nullptr;
}

//int AOceanBoatsGameMode::GetKilledNumByKiller_Implementation(int uid)
//{
//	for(int i = 0; i < boats.Num(); ++i)
//	{
//		if (boats[i]->GetUniqueID() == uid)
//		{			
//			int num = 0;
//			for (int k = 0; k < ObstacleCollection.Num(); ++k)
//			{
//				num += IBoatInterface::Execute_GetDestroyNum(boats[i], ObstacleCollection[k].type);
//			}
//			return num;
//		}
//	}
//	return 0;
//}

void AOceanBoatsGameMode::Killed_Implementation(AActor* Killer, AActor* Victim)
{
	if(Killer && Victim)
		Cast<ASimEcs_Archetype>(Killer)->AddResult(Cast<ASimEcs_Archetype>(Victim)->ArchType);
}

void AOceanBoatsGameMode::HitDamage_Implementation(AActor* Victim, float Damage)
{
	ASimEcs_Archetype* vic = Cast<ASimEcs_Archetype>(Victim);
	if(vic)
		vic->HitDamge(Damage);
}
void AOceanBoatsGameMode::ShowRainy_Implementation(bool show)
{
	if(mRain == NULL)
	{
		UClass* RainClass = LoadClass<AActor>(NULL, TEXT(RAINCLASS));
		if (RainClass != NULL)
		{
			FTransform const SpawnTransform(FRotator::ZeroRotator, FVector(0, 0, 0));
			mRain = GWorld->SpawnActor<AActor>(RainClass, SpawnTransform);

			mRain->AttachToActor(UGameplayStatics::GetPlayerController(GWorld, 0)->GetPawn(), FAttachmentTransformRules::KeepRelativeTransform);
		}
	}
	if(mRain)
		mRain->SetActorHiddenInGame(!show);
}
void AOceanBoatsGameMode::ShowGloomy_Implementation(bool show)
{

}

void AOceanBoatsGameMode::ShowThunder_Implementation(bool show)
{

}

void AOceanBoatsGameMode::ShowSnow_Implementation(bool show)
{
	if(mSnow == NULL)
	{
		UClass* SnowClass = LoadClass<AActor>(NULL, TEXT(SNOWCLASS));
		if (SnowClass != NULL)
		{
			FTransform const SpawnTransform(FRotator::ZeroRotator, FVector(0, 0, 0));
			mSnow = GWorld->SpawnActor<AActor>(SnowClass, SpawnTransform);
			AActor* mainActor = UGameplayStatics::GetPlayerController(GWorld, 0)->GetPawn();
			mSnow->AttachToActor(mainActor, FAttachmentTransformRules::KeepRelativeTransform);
			mSnow->SetActorLocation(mainActor->GetActorLocation() + FVector(0, 0, 1000));
		}
	}
	if(mSnow)
		mSnow->SetActorHiddenInGame(!show);
}
void AOceanBoatsGameMode::ArrivedLandscape_Implementation(AActor* boat)
{
	USimOceanSceneManager_Singleton::GetInstance()->SetIdle(boat->GetName(), boat->GetActorLocation());
}
void AOceanBoatsGameMode::SetWaveStrength_Implementation(float strength)
{
	if (m_ocean == NULL)
	{
		TArray<AActor*> oceans;
		UGameplayStatics::GetAllActorsOfClass(GWorld, AOceanManager::StaticClass(), oceans);
		if (oceans.Num() > 0)
		{
			m_ocean = Cast<AOceanManager>(oceans[0]);
		}
	}

	if (m_ocean)
	{
		m_ocean->GlobalWaveAmplitude = strength;
	}
}

void AOceanBoatsGameMode::ReStartMission_Implementation()
{

}
void AOceanBoatsGameMode::PauseMission_Implementation()
{

}
void AOceanBoatsGameMode::StopMission_Implementation()
{

}