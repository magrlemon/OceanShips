/*=================================================
* FileName: FishManager.h
* 
* Created by: Magi
* Project name: OceanProject
* Unreal Engine version: 4.18.3
* Created on: 2019/03/17
*
* 
* -------------------------------------------------
* For parts referencing UE4 code, the following copyright applies:
* Copyright 1998-2020 Epic Games, Inc. All Rights Reserved.
*
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject\License.md" for full licensing details.
* =================================================*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimEcs_FormationManager.generated.h"


/**
 * 
 */
UCLASS()
class ASimEcs_FormationManager : public AActor
{
	GENERATED_BODY()

	
public:

	// Type of flocks to spawn
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TArray<UClass*> flockTypes;

	// # of boat in respective flock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	TArray<float> numInFlock;

	// min Z of volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float minZ = -9000.0f;

	// max Z of volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float maxZ = -950.0f;
	
	// Length of underwater box
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	float underwaterBoxLength = 10000;

	// Attach boat manager to player?
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool attachToPlayer = false;

	// Player Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	bool DebugMode = false;

	// Player Type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
	UClass* playerType;

	// Player
	AActor* player;

	// get min z
	float getMinZ();

	// get min z
	float getMaxZ();

	/** Constructor */
	ASimEcs_FormationManager(const FObjectInitializer& ObjectInitializer);

protected:

	/** Overrided tick function */
	virtual void Tick(float val) override;

	/** setup the boat manager */
	void setup();

	/** move boat manager to player */
	void moveToPlayer();

	/* max bounds of water*/
	float maxX;
	float maxY;
	float minX;
	float minY;

	// is set up?
	bool isSetup = false;

	// Are boat spawned?
	bool areFishSpawned = false;
};
