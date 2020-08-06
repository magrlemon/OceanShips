// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/SimEcs_OceanCommonTypes.h"
#include "SimEcs_BoatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class USimEcs_BoatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ENTITASFRAME_API ISimEcs_BoatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetMoveMode(ESimOceanMoveMode mode);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedUp();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedDown();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void FireImpl(int killerUID, FTransform trans, ESimExplodeType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetDestroyNum( ESimObstacleType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void RecordDestroy( ESimObstacleType type);

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
