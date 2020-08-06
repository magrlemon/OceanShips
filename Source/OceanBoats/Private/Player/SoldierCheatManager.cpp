// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "Player/SoldierCheatManager.h"
#include "Bots/SoldierAIController.h"

USoldierCheatManager::USoldierCheatManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USoldierCheatManager::ToggleInfiniteAmmo()
{
	ASoldierPlayerController* MyPC = GetOuterASoldierPlayerController();

	MyPC->SetInfiniteAmmo(!MyPC->HasInfiniteAmmo());
	MyPC->ClientMessage(FString::Printf(TEXT("Infinite ammo: %s"), MyPC->HasInfiniteAmmo() ? TEXT("ENABLED") : TEXT("off")));
}

void USoldierCheatManager::ToggleInfiniteClip()
{
	ASoldierPlayerController* MyPC = GetOuterASoldierPlayerController();

	MyPC->SetInfiniteClip(!MyPC->HasInfiniteClip());
	MyPC->ClientMessage(FString::Printf(TEXT("Infinite clip: %s"), MyPC->HasInfiniteClip() ? TEXT("ENABLED") : TEXT("off")));
}

void USoldierCheatManager::ToggleMatchTimer()
{
	ASoldierPlayerController* MyPC = GetOuterASoldierPlayerController();

	/*ASoldierGameState* const MyGameState = MyPC->GetWorld()->GetGameState<ASoldierGameState>();
	if (MyGameState && MyGameState->GetLocalRole() == ROLE_Authority)
	{
		MyGameState->bTimerPaused = !MyGameState->bTimerPaused;
		MyPC->ClientMessage(FString::Printf(TEXT("Match timer: %s"), MyGameState->bTimerPaused ? TEXT("PAUSED") : TEXT("running")));
	}*/
}

void USoldierCheatManager::ForceMatchStart()
{
	ASoldierPlayerController* const MyPC = GetOuterASoldierPlayerController();

	AOceanBoatsGameMode* const MyGame = MyPC->GetWorld()->GetAuthGameMode<AOceanBoatsGameMode>();
	if (MyGame && MyGame->GetMatchState() == MatchState::WaitingToStart)
	{
		MyGame->StartMatch();
	}
}


void USoldierCheatManager::SpawnBot()
{
	ASoldierPlayerController* const MyPC = GetOuterASoldierPlayerController();
	APawn* const MyPawn = MyPC->GetPawn();
	AOceanBoatsGameMode* const MyGame = MyPC->GetWorld()->GetAuthGameMode<AOceanBoatsGameMode>();
	UWorld* World = MyPC->GetWorld();
	if (MyPawn && MyGame && World)
	{
		static int32 CheatBotNum = 50;
		ASoldierAIController* SoldierAIController = MyGame->CreateBot(CheatBotNum++);
		MyGame->RestartPlayer(SoldierAIController);		
	}
}