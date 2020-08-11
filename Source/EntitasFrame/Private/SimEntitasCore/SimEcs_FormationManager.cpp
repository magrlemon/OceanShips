/*=================================================
* FileName: FishManager.cpp
* 
* Created by: Magi
* Project name: OceanProject
* Unreal Engine version: 4.18.3
* Created on: 2020/03/17
* 
* -------------------------------------------------
* For parts referencing UE4 code, the following copyright applies:
* Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
*
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject\License.md" for full licensing details.
* =================================================*/

#include "SimEcs_FormationManager.h"
#include "SimEcs_FlockBoats.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"


ASimEcs_FormationManager::ASimEcs_FormationManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASimEcs_FormationManager::Tick(float val)
{
	setup();

	if (attachToPlayer)
	{
		moveToPlayer();
	}
}

void ASimEcs_FormationManager::moveToPlayer()
{
	if (player)
		this->SetActorLocation(player->GetActorLocation());
}

float ASimEcs_FormationManager::getMinZ()
{
	return minZ;
}

float ASimEcs_FormationManager::getMaxZ()
{
	return maxZ;
}

void ASimEcs_FormationManager::setup()
{
	if (isSetup == false){
		if (!areFishSpawned)
		{
			maxX = GetActorLocation().X + underwaterBoxLength;
			maxY = GetActorLocation().Y + underwaterBoxLength;
			minX = GetActorLocation().X - underwaterBoxLength;
			minY = GetActorLocation().Y - underwaterBoxLength;

			UWorld* const world = GetWorld();
			int numFlocks = flockTypes.Num();
			for (int i = 0; i < numFlocks; i++)
			{
				FVector spawnLoc = FVector(FMath::FRandRange(minX, maxX), FMath::FRandRange(minY, maxY), FMath::FRandRange(minZ, maxZ));
				ASimEcs_FlockBoats *leaderFish = NULL;
				for (int j = 0; j < numInFlock[i]; j++)
				{
					ASimEcs_FlockBoats *aBoat = Cast<ASimEcs_FlockBoats>(world->SpawnActor(flockTypes[i]));
					aBoat->isLeader = false;
					aBoat->DebugMode = DebugMode;
					aBoat->underwaterMax = FVector(maxX, maxY, maxZ);
					aBoat->underwaterMin = FVector(minX, minY, minZ);
					aBoat->underwaterBoxLength = underwaterBoxLength;
					spawnLoc = FVector(FMath::FRandRange(minX, maxX), FMath::FRandRange(minY, maxY), FMath::FRandRange(minZ, maxZ));
					if (j == 0)
					{
						aBoat->isLeader = true;
						leaderFish = aBoat;
					}
					else if (leaderFish != NULL)
					{	
						aBoat->leader = leaderFish;
					}
					aBoat->SetActorLocation(spawnLoc);
				}
			}
			areFishSpawned = true;
		}

		if (attachToPlayer)
		{
			TArray<AActor*> aPlayerList;
			UGameplayStatics::GetAllActorsOfClass(this, playerType, aPlayerList);
			if (aPlayerList.Num() > 0)
			{	
				player = aPlayerList[0];
				isSetup = true;
			}
		} 
		else
		{
			isSetup = true;
		}

	}
}
