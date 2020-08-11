/*=================================================
* FileName: FlockBoat.cpp
* 
* Created by: Magi
* Project name: OceanProject
* Unreal Engine version: 4.18.3
* Created on: 2020/03/17
*
* Last Edited on: 2018/03/15
* Last Edited by: Felipe "Zoc" Silveira
* 
* -------------------------------------------------
* For parts referencing UE4 code, the following copyright applies:
* Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.
*
* Feel free to use this software in any commercial/free game.
* Selling this as a plugin/item, in whole or part, is not allowed.
* See "OceanProject\License.md" for full licensing details.
* =================================================*/

#include "SimEcs_FlockBoats.h"
#include "SimEcs_BoatsState.h"
#include "SimEcs_FormationManager.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"


#define COLLISION_TRACE ECC_GameTraceChannel4

ASimEcs_FlockBoats::ASimEcs_FlockBoats(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	base = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BoatMesh"));
	RootComponent = base;

	// Boat interaction sphere
	BoatInteractionSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("BoatInteractionSphere"));
	BoatInteractionSphere->SetSphereRadius(10);
	BoatInteractionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	BoatInteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASimEcs_FlockBoats::OnBeginOverlap);
	BoatInteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ASimEcs_FlockBoats::OnEndOverlap);

	if (isLeader == true)
	{
		spawnTarget();
	}

	//Defaults
	followDist = 50.0;
	speed = 1200.0;
	maxSpeed = 2400.0;
	turnSpeed = 3.0;
	turnFrequency = 1.0;
	turnFrequency = 1.0;
	hungerResetTime = 20.0;
	distBehindSpeedUpRange = 3000.0;
	SeperationDistanceMultiplier = 0.75;
	FleeDistanceMultiplier = 5.0;
	FleeAccelerationMultiplier = 2.0;
	ChaseAccelerationMultiplier = 2.0;
	SeekDecelerationMultiplier = 1.0;
	AvoidForceMultiplier = 1.0;
	AvoidanceForce = 20000.0;
	underwaterMin = FVector(-40000, -40000, -9000);
	underwaterMax = FVector(40000, 40000, -950);
	CustomZSeekMin = 0.0;
	CustomZSeekMax = 0.0;
	NumNeighborsToEvaluate = 5.0;
	UpdateEveryTick = 0.0;
	DebugMode = true;
	fleeDistance = 0.0;
	neighborSeperation = 300.0;
	curSpeed = speed;
	isFleeing = false;
	isFull = false;
	underwaterBoxLength = 10000.0;
	AvoidanceDistance = 5000.0;
	curVelocity = FVector(0, 0, 0);
	curRotation = FRotator(0, 0, 0);
	turnTimer = 0.0;
	isSetup = false;
	hungerTimer = 0.0;
	updateTimer = 0.0;
	hasBoatManager = false;
}

void ASimEcs_FlockBoats::Tick(float delta)
{

	// Setup the boat (happens on first tick only)
	// NOTE TO SELF: consider creating a beginplay event that does this stuff (although beginplay is buggy as hell x.x)
	Setup();

	// If debug mode true, draw interaction sphere and avoiddistance
	Debug();

	// Move Bounds based on location of BoatManager (if applicable)
	MoveBounds(delta);

	// Manage Timers (hungerTimer, updateTimer, and turnTimer)
	ManageTimers(delta);

	// Decide what state to be in
	ChooseState();

	// Update curVelocity and curRotation through current state
	UpdateState(delta);

	// Update world rotation and velocity
	this->SetActorRotation(curRotation);
	this->AddActorWorldOffset(curVelocity);

	Super::Tick(delta);
}

void ASimEcs_FlockBoats::Debug()
{
	if (DebugMode)
	{
		FVector actorLocation = this->GetActorLocation();
		FVector forwardVector = (this->GetActorForwardVector() * AvoidanceDistance) + actorLocation;
		FVector forwardVector2 = (this->GetActorForwardVector() * (AvoidanceDistance * 0.1)) + actorLocation;

		DrawDebugLine(
			GetWorld(),
			actorLocation,
			forwardVector,
			FColor::Magenta,
			false, -1, 0,
			10
			);

		FColor indicatorColor = FColor::Cyan;
		if (nearbyEnemies.IsValidIndex(0))
		{
			indicatorColor = FColor::Red;
		}
		else if (nearbyPrey.IsValidIndex(0) && isFull == false)
		{
			indicatorColor = FColor::Green;
		}
		DrawDebugSphere(
			GetWorld(),
			actorLocation,
			BoatInteractionSphere->GetScaledSphereRadius(),
			20,
			indicatorColor
			);
		DrawDebugLine(
			GetWorld(),
			actorLocation,
			forwardVector2,
			indicatorColor,
			true, 10, 0,
			20
			);
	}

}

FVector ASimEcs_FlockBoats::AvoidObstacle()
{
	FVector actorLocation = this->GetActorLocation();
	FVector forwardVector = (this->GetActorForwardVector() * AvoidanceDistance) + actorLocation;

	FHitResult OutHitResult;
	FCollisionQueryParams Line(FName("Collision param"), true);
	bool const bHadBlockingHit = GetWorld()->LineTraceSingleByChannel(OutHitResult, actorLocation, forwardVector, COLLISION_TRACE, Line);
	FVector returnVector = FVector(0, 0, 0);
	float distanceToBound = distanceToBound = (this->GetActorLocation() - OutHitResult.ImpactPoint).Size();
	if (bHadBlockingHit)
	{
		if (OutHitResult.ImpactPoint.X > this->GetActorLocation().X)
		{
			returnVector.X += (1 / (distanceToBound * (1 / AvoidForceMultiplier))) * -1;
		}
		else if (OutHitResult.ImpactPoint.X < this->GetActorLocation().X)
		{
			
			returnVector.X += (1 / (distanceToBound * (1 / AvoidForceMultiplier))) * 1;
		}

		if (OutHitResult.ImpactPoint.Y > this->GetActorLocation().Y)
		{
			returnVector.Y += (1 / (distanceToBound * (1 / AvoidForceMultiplier))) * -1;
		}
		else if (OutHitResult.ImpactPoint.Y < this->GetActorLocation().Y)
		{

			returnVector.Y  += (1 / (distanceToBound * (1 / AvoidForceMultiplier))) * 1;
		}

		returnVector.Normalize();
		FVector avoidance = returnVector * AvoidanceForce;
		return avoidance;
	}
	return FVector(0, 0, 0);
}

void ASimEcs_FlockBoats::UpdateState(float delta)
{
	if (UpdateEveryTick == 0)
	{
		currentState->Update(delta);
	}
	else if (updateTimer >= UpdateEveryTick)
	{
		currentState->Update(delta);
		updateTimer = 0;
	}
}


void ASimEcs_FlockBoats::OnBeginOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, 
	UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult)
{
	// Is overlapping with enemy?
	if (enemyTypes.Find(otherActor->GetClass()) >= 0)
	{	
		nearbyEnemies.Add(otherActor);
	}
	else if (preyTypes.Find(otherActor->GetClass()) >= 0)
	{	
		if (otherActor->GetClass() == this->GetClass())
		{
			if (!Cast<ASimEcs_FlockBoats>(otherActor)->isLeader)
			{
				nearbyPrey.Add(otherActor);
			}
		}
		else
		{
			nearbyPrey.Add(otherActor);
		}
	}
	else if (otherActor->GetClass() == this->GetClass())
	{
		nearbyFriends.Add(otherActor);
	}
}

void ASimEcs_FlockBoats::OnEndOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex)
{
	if (nearbyEnemies.Find(otherActor) >= 0)
	{
		nearbyEnemies.Remove(otherActor);
	}
	else if (nearbyPrey.Find(otherActor) >= 0)
	{
		nearbyPrey.Remove(otherActor);
	}
	else if (nearbyFriends.Find(otherActor) >= 0)
	{
		nearbyFriends.Remove(otherActor);
	}
}

void ASimEcs_FlockBoats::ChooseState()
{
	if (nearbyEnemies.IsValidIndex(0))
	{
		currentState = new SimEcs_FleeState(this, nearbyEnemies[0]);
	}
	else if (nearbyPrey.IsValidIndex(0) && isFull == false)
	{
		currentState = new SimEcs_ChaseState(this, nearbyPrey[0]);
	}
	else
	{
		currentState = new SimEcs_SeekState(this);
	}
}

void ASimEcs_FlockBoats::ManageTimers(float delta)
{
	// Check if the boat is full or not
	if (isFull)
	{
		hungerTimer += delta;

		if (hungerTimer >= hungerResetTime)
		{
			hungerTimer = 0.0f;
			isFull = false;
		}
	}

	// decide on wether to move target this tick
	if (turnTimer >= turnFrequency && isLeader == true)
	{
		spawnTarget();
		turnTimer = 0.0;
	}

	updateTimer += delta;
	turnTimer += delta;
}



void ASimEcs_FlockBoats::MoveBounds(float delta)
{
	if (hasBoatManager)
	{
		FVector boatManagerPosition = boatManager->GetActorLocation();
		maxX = boatManagerPosition.X + underwaterBoxLength;
		minX = boatManagerPosition.X - underwaterBoxLength;
		maxY = boatManagerPosition.Y + underwaterBoxLength;
		minY = boatManagerPosition.Y - underwaterBoxLength;

		FVector actorLocation = this->GetActorLocation();


		if (actorLocation.X > maxX)
		{
			actorLocation.X = minX + FMath::Abs((0.1 * maxX));
		}
		else if (actorLocation.X < minX)
		{
			actorLocation.X = maxX - FMath::Abs((0.1 * maxX));
		}

		if (actorLocation.Y > maxY)
		{
			actorLocation.Y = minY + FMath::Abs((0.1 * maxY));
		}
		else if (actorLocation.Y < minY)
		{
			actorLocation.Y = maxY - FMath::Abs((0.1 * maxY));
		}

		this->SetActorLocation(actorLocation);
	}
}

void ASimEcs_FlockBoats::spawnTarget()
{
	target = FVector(FMath::FRandRange(minX, maxX), FMath::FRandRange(minY, maxY), FMath::FRandRange(minZ, maxZ));
}


void ASimEcs_FlockBoats::Setup()
{
	// Setup the enemies list on first tick
	if (isSetup == false)
	{
		maxX = underwaterMax.X;
		maxY = underwaterMax.Y;
		minX = underwaterMin.X;
		minY = underwaterMin.Y;

		InteractionSphereRadius = BoatInteractionSphere->GetScaledSphereRadius();

		if (CustomZSeekMax == 0.0)
		{
			minZ = underwaterMin.Z;
			maxZ = underwaterMax.Z;
		}
		else
		{
			minZ = CustomZSeekMin;
			maxZ = CustomZSeekMax;
		}

		fleeDistance = BoatInteractionSphere->GetScaledSphereRadius() * FleeDistanceMultiplier;
		neighborSeperation = BoatInteractionSphere->GetScaledSphereRadius() * SeperationDistanceMultiplier;
		AvoidanceDistance = BoatInteractionSphere->GetScaledSphereRadius() * 2;

		currentState = new SimEcs_SeekState(this);

		TArray<AActor*> aBoatManagerList;
		UGameplayStatics::GetAllActorsOfClass(this, ASimEcs_FormationManager::StaticClass(), aBoatManagerList);
		if (aBoatManagerList.Num() > 0)
		{
			hasBoatManager = true;
			boatManager = aBoatManagerList[0];
		}

		// Setup Neighbors
		if (!boatManager)
		{
			TArray<AActor*> aNeighborList;
			UGameplayStatics::GetAllActorsOfClass(this, neighborType, aNeighborList);
			neighbors.Append(aNeighborList);
			for (int i = 0; i < neighbors.Num(); i++)
			{
				if (Cast<ASimEcs_FlockBoats>(neighbors[i])->isLeader)
				{
					leader = neighbors[i];
					break;
				}
			}
		}
		//nearbyFriends.Append(neighbors);


		isSetup = true;
	}
}
