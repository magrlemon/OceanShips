// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "LibAiAgent/BehaviacManager.h"
#include "LibAiAgent/Behaviac/behaviac_types.h"
#include "LibAiAgent/AiImagineAgent.h"
#include "LibAiAgent/MethodIdDefine.h"
#include "LibAiAgent/network.h"
#include "CoreMinimal.h"
#include "SoldierPlayerController.h"
#include "GameFramework/GameModeBase.h"
#include "Interface/GameModeInterface.h"
#include "OceanCommonTypes.h"
#include "OceanBoatsGameMode.generated.h"

class ASoldierAIController;
//class ASoldierPlayerState;
class ASoldierPickup;
class FUniqueNetId;

struct OceanObstacleCls;

UCLASS( config = Game )
class AOceanBoatsGameMode : public AGameMode, public IGameModeInterface
{
	GENERATED_BODY()
public:
	AOceanBoatsGameMode();
public:
	/** The bot pawn class */
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = GameMode )
	TSubclassOf<APawn> BotPawnClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameMode)
		TArray<AActor*> boats;
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void StartMove();
	virtual void StartMove_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void AddBoat(AActor* boat);
	virtual void AddBoat_Implementation(AActor* boat);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	//	void AddRuinedBoat(AActor* boat);
	//virtual void AddRuinedBoat_Implementation(AActor* boat);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	//	void BroadcastDestroy(AActor* victim, int killerId, ObstacleType type);
	//virtual void BroadcastDestroy_Implementation(AActor* victim, int killerId, ObstacleType type);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	//	int GetKilledNum(ObstacleType type);
	//virtual int GetKilledNum_Implementation(ObstacleType type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetBoatNum();
	virtual int GetBoatNum_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		AActor* GetBoat(int index);
	virtual AActor* GetBoat_Implementation(int index);
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	//	int GetKilledNumByKiller(int uid);
	//virtual int GetKilledNumByKiller_Implementation(int uid);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Killed(AActor* Killer, AActor* Victim);
	virtual void Killed_Implementation(AActor* Killer, AActor* Victim);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void HitDamage(AActor* Victim, float Damage);
	virtual void HitDamage_Implementation(AActor* Victim, float Damage);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void ShowRainy(bool show);
	virtual void ShowRainy_Implementation(bool show);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void ShowGloomy(bool show);
	virtual void ShowGloomy_Implementation(bool show);

	UFUNCTION( exec )
	void SetAllowBots( bool bInAllowBots, int32 InMaxBots = 8 );


	virtual void PreInitializeComponents( ) override;

	/** Initialize the game. This is called before actors' PreInitializeComponents. */
	virtual void InitGame( const FString& MapName, const FString& Options, FString& ErrorMessage ) override;

	/** Accept or reject a player attempting to join the server.  Fails login if you set the ErrorMessage to a non-empty string. */
	virtual void PreLogin( const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage ) override;

	/** starts match warmup */
	virtual void PostLogin( APlayerController* NewPlayer ) override;

	/** Tries to spawn the player's pawn */
	virtual void RestartPlayer( AController* NewPlayer ) override;
	/** prevents friendly fire */
	virtual float ModifyDamage( float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser ) const;

	/** notify about kills */
	virtual void Killed( AController* Killer ,AController* KilledPlayer, APawn* KilledPawn, const UDamageType* DamageType );
	virtual void Killed(AController* Killer);
	/** update remaining time */
	virtual void DefaultTimer( );


	/** initialization for bot after creation */
	virtual void InitBot( ASoldierAIController* AIC, int32 BotNum );

	/** check who won */
	virtual void DetermineMatchWinner( );

	/** check if PlayerState is a winner */
	virtual bool IsWinner( ASoldierPlayerState* PlayerState ) const;

	/** check if player can use spawnpoint */
	virtual bool IsSpawnpointAllowed( APlayerStart* SpawnPoint, AController* Player ) const;

	/** check if player should use spawnpoint */
	virtual bool IsSpawnpointPreferred( APlayerStart* SpawnPoint, AController* Player ) const;
public:

	bool CanDealDamage(  ) const;

	/** hides the onscreen hud and restarts the map */
	virtual void RestartGame( ) override;

	/** Creates AIControllers for all bots */
	void CreateBotControllers( );

	/** Create a bot */
	ASoldierAIController* CreateBot( int32 BotNum );

	virtual void PostInitProperties( ) override;


	/** finish current match and lock players */
	UFUNCTION( exec )
		void FinishMatch( );
	/*Finishes the match and bumps everyone to main menu.*/
	/*Only GameInstance should call this function */
	void RequestFinishAndExitToMainMenu( );

	/** spawning all bots for this game */
	void StartBots( );

	/** get the name of the bots count option used in server travel URL */
	static FString GetBotsCountOptionName( );
protected:

	/** delay between first player login and starting match */
	UPROPERTY( config )
		int32 WarmupTime;

	/** match duration */
	UPROPERTY( config )
		int32 RoundTime;

	UPROPERTY( config )
		int32 TimeBetweenMatches;

	/** score for kill */
	UPROPERTY( config )
		int32 KillScore;

	/** score for death */
	UPROPERTY( config )
		int32 DeathScore;

	/** scale for self instigated damage */
	UPROPERTY( config )
		float DamageSelfScale;

	UPROPERTY( config )
		int32 MaxBots;

	UPROPERTY( )
		TArray<ASoldierAIController*> BotControllers;

	UPROPERTY( config )
		TSubclassOf<ASoldierPlayerController> PlatformPlayerControllerClass;

	/** Handle for efficient management of DefaultTimer timer */
	FTimerHandle TimerHandle_DefaultTimer;

	bool bNeedsBotCreation;

	bool bAllowBots;

	//TArray<OceanObstacleCls> ObstacleCollection;
	//TMap<int, int> DestroyCollection;

public:
	void InitBehaviac();

	RetVar MoveEntity(int argc, void ** argv);
	RetVar MoveBackEntity(int argc, void ** argv);
	RetVar Fire(int argc, void ** argv);
	RetVar EndTask(int argc, void ** argv);
	RetVar MoveEntity(FString name,double PosX,double PosY,double dir);
	void BehaviacExec();
	//void InitObstacles();
	//void AddObsctacleType(FString name, ObstacleType type);
private:
	AiImagineAgent *_AiImagineAgent = nullptr;
	bool _wait = false;
	bool _initilize = false;
	AActor* mRain;

};



