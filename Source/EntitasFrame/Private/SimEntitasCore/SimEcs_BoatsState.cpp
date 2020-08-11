/*=================================================
* FileName: FishState.cpp
* 
* Created by: Magi
* Project name: OceanProject
* Unreal Engine version: 4.18.3
* Created on: 2020/03/17
*
* Last Edited on: 2018/01/30
* Last Edited by: SaschaElble
* 
* -------------------------------------------------
* For parts referencing UE4 code, the following copyright applies:
* Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
*
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject\License.md" for full licensing details.
* =================================================*/

#include "SimEcs_BoatsState.h"
#include "SimEcs_FlockBoats.h"

/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Seek State             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

void SimEcs_SeekState::Update(float delta)
{	
	Boat->isFleeing = false;
	Boat->fleeTarget = NULL;
	Boat->preyTarget = NULL;

	if (Boat->isLeader)
	{
		// Seek target
		SeekTarget(delta);
	}
	else
	{
		// School with buddies
		Flock(delta);
	}
}

void SimEcs_SeekState::SeekTarget(float delta)
{	
	// Set Speed
	Boat->curSpeed = FMath::Lerp(Boat->curSpeed, Boat->speed, delta * Boat->SeekDecelerationMultiplier);

	// Set Rotation   
	FVector targetRotation = (Boat->getSeekTarget() - Boat->GetActorLocation() + Boat->AvoidObstacle());
	FRotator leaderRotation = FRotationMatrix::MakeFromX(targetRotation).Rotator();
	leaderRotation = FMath::RInterpTo(Boat->GetActorRotation(), leaderRotation, delta, Boat->turnSpeed);
	Boat->setRotation(leaderRotation);
	
	// Set Velocity Vector
	FVector leaderVelocity = Boat->GetActorForwardVector() * (delta * Boat->curSpeed);
	Boat->setVelocity(leaderVelocity);
}

void SimEcs_SeekState::Flock(float delta)
{
	// Get a list of Boat neighbors and calculate seperation
	FVector seperation = FVector(0, 0, 0);
	if (Boat->nearbyFriends.IsValidIndex(0))
	{
		TArray<AActor*> neighborList = Boat->nearbyFriends;
		int neighborCount = 0;
		for (int i = 0; i < Boat->nearbyFriends.Num(); i++)
		{
			if (neighborList.IsValidIndex(i))
			{
				seperation += neighborList[i]->GetActorLocation() - Boat->GetActorLocation();
				neighborCount++;
			}

			if (i == Boat->NumNeighborsToEvaluate && i != 0)
			{
				break;
			}
		}
		seperation = ((seperation / neighborCount) * -1);
		seperation.Normalize();
		seperation *= Boat->neighborSeperation;
	}

	// Maintain distance behind Leader
	FVector distBehind = (Cast<ASimEcs_FlockBoats>(Boat->leader)->getVelocity() * -1);
	distBehind.Normalize();
	distBehind *= Boat->followDist;

	// Calculate all seperation and distance behind leader into one vector
	FVector leaderLocation = Boat->leader->GetActorLocation();
	FVector flockerVelocity = distBehind + leaderLocation + seperation + Boat->AvoidObstacle();
	FRotator flockerRotation = FRotationMatrix::MakeFromX(flockerVelocity - Boat->GetActorLocation()).Rotator();

	// If boat is too far behind leader, speed up 
	float newSpeed = Boat->speed;
	if (Boat->GetDistanceTo(Boat->leader) > Boat->distBehindSpeedUpRange)
	{
		// Set Speed
		Boat->curSpeed = FMath::Lerp(Boat->curSpeed, Boat->maxSpeed, delta);
	}
	else
	{
		// Set Speed
		Boat->curSpeed = FMath::Lerp(Boat->curSpeed, Boat->speed, delta);
	}


	// Set Velocity
	FVector flockVelocity = Boat->GetActorForwardVector() * (delta * Boat->curSpeed);
	Boat->setVelocity(flockVelocity);

	// Set Rotation
	FRotator flockRotation = FMath::RInterpTo(Boat->GetActorRotation(), flockerRotation, delta, Boat->turnSpeed);
	Boat->setRotation(flockRotation);
}


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Flee State             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

void SimEcs_FleeState::Update(float delta)
{
	Boat->isFleeing = true;
	Boat->fleeTarget = Enemy;
	Boat->preyTarget = NULL;
	if (Boat->GetDistanceTo(Enemy) >= Boat->fleeDistance)
	{
		Boat->setState(new SimEcs_SeekState(Boat));
	}
	FleeFromEnemy(delta);
}

void SimEcs_FleeState::FleeFromEnemy(float delta)
{
	// Set Speed
	Boat->curSpeed = FMath::Lerp(Boat->curSpeed, Boat->maxSpeed, (delta * Boat->FleeAccelerationMultiplier));
	
	// Set Velocity
	FVector fleeVelocity = Boat->GetActorForwardVector() * (delta * Boat->curSpeed);
	Boat->setVelocity(fleeVelocity);

	// Set Rotation
	FVector targetRotation = (Boat->GetActorLocation() - Enemy->GetActorLocation()) + Boat->AvoidObstacle();
	FRotator fleeRotation = FRotationMatrix::MakeFromX(targetRotation).Rotator();
	fleeRotation = FMath::RInterpTo(Boat->GetActorRotation(), fleeRotation, delta, Boat->turnSpeed);
	Boat->setRotation(fleeRotation);

}

/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Chase State            /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

void SimEcs_ChaseState::Update(float delta)
{
	Boat->isFleeing = false;
	Boat->fleeTarget = NULL;
	Boat->preyTarget = Prey;
	EatPrey();
	ChasePrey(delta);
}

void SimEcs_ChaseState::EatPrey()
{
	if (Boat->GetDistanceTo(Prey) < 1000)
	{
		float zLoc = Boat->minZ + FMath::Abs(0.25 * Boat->maxZ);
		Prey->SetActorLocation(FVector(FMath::FRandRange(Boat->minX, Boat->maxX), FMath::FRandRange(Boat->minY, Boat->maxX), zLoc));
		Boat->isFull = true;
		Boat->setState(new SimEcs_SeekState(Boat));
	}
}

void SimEcs_ChaseState::ChasePrey(float delta)
{
	// Set Speed
	Boat->curSpeed = FMath::Lerp(Boat->curSpeed, Boat->maxSpeed, (delta * Boat->ChaseAccelerationMultiplier));

	// Set Velocity
	FVector chaseVelocity = Boat->GetActorForwardVector() * (delta * Boat->curSpeed);
	Boat->setVelocity(chaseVelocity);

	// Set Rotation
	FVector seperation = FVector(0, 0, 0);
	if (Boat->nearbyFriends.IsValidIndex(0))
	{
		int neighborCount = 0;
		TArray<AActor*> neighborList = Boat->nearbyFriends;
		for (int i = 0; i < Boat->NumNeighborsToEvaluate; i++)
		{
			if (neighborList.IsValidIndex(i))
			{
				seperation += neighborList[i]->GetActorLocation() - Boat->GetActorLocation();
				neighborCount++;
			}
		}
		seperation = ((seperation / neighborCount) * -1);
		seperation.Normalize();
		seperation *= Boat->neighborSeperation;
	}

	FVector preyLocation = Prey->GetActorLocation();
	FVector flockerVelocity = ((preyLocation + seperation) - Boat->GetActorLocation()) + Boat->AvoidObstacle();
	FRotator flockerRotation = FRotationMatrix::MakeFromX(flockerVelocity).Rotator();
	FRotator chaseRotation = FMath::RInterpTo(Boat->GetActorRotation(), flockerRotation, delta, Boat->turnSpeed);
	Boat->setRotation(chaseRotation);
}
