/*=================================================
* FileName: FlockBoat.h
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

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SimEcs_FormationManager.h"
#include "SimEcs_FlockBoats.generated.h"


/**
 * 
 */
UCLASS()
class ASimEcs_FlockBoats : public APawn
{
	GENERATED_BODY()

	// Current state of the Boat
	class SimEcs_BoatState* currentState;

	/** Static mesh component */
	class UStaticMeshComponent* base;

	/** Boat interaction sphere */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* BoatInteractionSphere;

public:
	// Is this boat a leader
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	bool isLeader;

	// Enemies that boat will flee from
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	TArray<UClass*> enemyTypes;

	// Prey that the boat will attack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	TArray<UClass*> preyTypes;

	// Neighbor type of the boat ( typically itself, but needed for code atm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	UClass* neighborType;

	// Distance to that boat will try to remain behind leader
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float followDist;

	// Defualt Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float speed;

	// Max Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float maxSpeed;

	// Speed at which the boat turns (Try to have predators be slightly faster)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float turnSpeed;

	// Frequency of turning ( turn every "turnFrequency" seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float turnFrequency;

	// time after eating until it is hungry again
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float hungerResetTime;

	// Go faster when boat is this distance behind leader
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Attributes")
	float distBehindSpeedUpRange;

	// Multiplies With Radius of Boat Interaction Sphere for neighbor seperation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float SeperationDistanceMultiplier;

	// Multiplies With Radius of Boat Interaction Sphere for Flee Distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float FleeDistanceMultiplier;

	// Multiplies with delta time since last tick when lerping to max speed (While Fleeing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float FleeAccelerationMultiplier;

	// Multiplies with delta time since last tick when lerping to max speed (While Chasing)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float ChaseAccelerationMultiplier;

	// Multiplies with delta time since last tick when lerping to regular speed (While Seeking)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float SeekDecelerationMultiplier;

	// Avoid Distance Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float AvoidForceMultiplier;

	// Avoidance force
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Advanced Settings")
	float AvoidanceForce;

	// Player the boat will avoid
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Variables")
	UClass* playerType;

	// volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Variables")
	FVector underwaterMin;

	// volume that is considered underwater
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Variables")
	FVector underwaterMax;

	// Custom Z range for targeting (NULL will use full range of min/max)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Variables")
	float CustomZSeekMin;

	// Custom Z range for targeting (NULL will use full range of min/max)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment Variables")
	float CustomZSeekMax;

	// Max number of neighbors to evaluate on a Tick (0 for all neighbors)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	float NumNeighborsToEvaluate;

	// When set to 0, update every tick, otherwise update after specified time
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Optimization")
	float UpdateEveryTick;

	// This is the target the boat will path to
	FVector target;

	float InteractionSphereRadius;

	// Run boat in debug mode
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TESTING")
	bool DebugMode;

	// Flee distance
	// NOTE: CURRENTLY SET IN CODE
	float fleeDistance;

	// Weight to multiply with cohesion
	// NOTE: CURRENTLY SET IN CODE
	float neighborSeperation;

	// current speed
	float curSpeed;

	// This boat's leader
	AActor *leader;

	// These are the boat's flocking buddies
	TArray<AActor*> neighbors;

	// Nearby Enemies
	TArray<AActor*> nearbyEnemies;

	// Nearby Prey
	TArray<AActor*> nearbyPrey;

	// Nearby Friends (non-threatning boat and neighbors)
	TArray<AActor*> nearbyFriends;

	// Flee target
	AActor *fleeTarget;

	// current prey
	AActor *preyTarget;

	// is boat currently fleeing
	// Current state of the Boat
	bool isFleeing;

	// is the boat currently full?
	bool isFull;

	/* bounds of underwater box*/
	float underwaterBoxLength;

	FVector AvoidObstacle();

	float AvoidanceDistance;

	// Max bounds of box
	float maxX;
	float maxY;
	float maxZ;
	float minX;
	float minY;
	float minZ;

	/** Constructor */
	ASimEcs_FlockBoats(const FObjectInitializer& ObjectInitializer);

	// Getters and Setters
	FVector getSeekTarget()
	{
		return target;
	}

	void setState(class SimEcs_BoatState* newState)
	{
		currentState = newState;
	}

	void setRotation(FRotator newRotation)
	{
		curRotation = newRotation;
	};

	void setVelocity(FVector newVelocity)
	{
		curVelocity = newVelocity;
	};

	FRotator getRotation()
	{
		return curRotation;
	};

	FVector getVelocity()
	{
		return curVelocity;
	};

protected:
	/** Overrided tick function */
	virtual void Tick(float delta) override;

	/* setup the game on first tick */
	void Setup();

	/* Manage the various necessary timers*/
	void ManageTimers(float delta);

	/* Choose what state to be in*/
	void ChooseState();

	/* Move Bounds */
	void MoveBounds(float delta);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* activatedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	/* update the current state*/ 
	void UpdateState(float delta);

	/* places the target randomly within the postprocessvolume*/
	void spawnTarget();

	/* Does a bunch of debug stuff if debug mode is active*/
	void Debug();

	// current velocity
	FVector curVelocity;

	// current rotation
	FRotator curRotation;

	// turning timer
	float turnTimer;

	// Are the array's setup?
	bool isSetup;

	// current hunger timer
	float hungerTimer;

	// Tick Timer
	float updateTimer;

	// player holder
	//AActor* player;

	// has boat manager?
	bool hasBoatManager;

	// boat manager
	AActor* boatManager;


};
