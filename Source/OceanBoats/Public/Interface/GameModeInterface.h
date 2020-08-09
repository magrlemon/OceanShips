// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OceanCommonTypes.h"
#include "GameModeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UGameModeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OCEANBOATS_API IGameModeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void StartMove();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void AddBoat(AActor* boat);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void AddRuinedBoat(AActor* boat);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void BroadcastDestroy(AActor* victim, int killerId, ObstacleType type);	

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetKilledNum(ObstacleType type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetBoatNum();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		AActor* GetBoat(int index);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetKilledNumByKiller(int uid);
};