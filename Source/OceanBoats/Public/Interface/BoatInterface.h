// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OceanCommonTypes.h"
#include "BoatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBoatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OCEANBOATS_API IBoatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetMoveMode(OceanMoveMode mode);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedDown();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void FireImpl(int killerUID, FTransform trans, ExplodeType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetDestroyNum(ObstacleType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void RecordDestroy(ObstacleType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_MoveOn(FVector pos);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_MoveBack(FVector pos);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_HoldOn(FVector nextTargetPos);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSpeed();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSailDistance();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSailAngle();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSpeedDownDistance();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetHorizontalDistance();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetRollbackDistance();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetRollbackAngle();
};
