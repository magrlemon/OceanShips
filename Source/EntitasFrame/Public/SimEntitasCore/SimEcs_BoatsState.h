/*=================================================
* FileName: SimEcs_BoatState.h
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
#include "SimEcs_FlockBoats.h"


class ASimEcs_FlockBoats;

/**
 * 
 */

/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat State		          /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////
class SimEcs_BoatState
{
protected:
	ASimEcs_FlockBoats* Boat;

public:
	// Constructors
	SimEcs_BoatState(){}
	SimEcs_BoatState(ASimEcs_FlockBoats* aBoat)
	{
		Boat = aBoat;
	};
	virtual void Update(float val){};
	virtual void HandleOverlapEvent(AActor *otherActor, UPrimitiveComponent *otherComponent, FString aColliderString){};
};


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Seek State             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

class SimEcs_SeekState : public SimEcs_BoatState
{
public:

	SimEcs_SeekState(ASimEcs_FlockBoats* aBoat) : SimEcs_BoatState(aBoat){};
	virtual void Update(float delta) override;

protected:

	virtual void SeekTarget(float delta);
	virtual void Flock(float delta);
};


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Flee State             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

class SimEcs_FleeState : public SimEcs_BoatState
{
protected:
	AActor* Enemy;

public:

	SimEcs_FleeState(ASimEcs_FlockBoats* aBoat, AActor* aEnemy) : SimEcs_BoatState(aBoat)
	{
		Enemy = aEnemy;
	};
	virtual void Update(float delta) override;

protected:

	virtual void FleeFromEnemy(float delta);

};


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Chase State            /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

class SimEcs_ChaseState : public SimEcs_BoatState
{
protected:
	AActor* Prey;

public:

	SimEcs_ChaseState(ASimEcs_FlockBoats* aBoat, AActor* aPrey) : SimEcs_BoatState(aBoat)
	{
		Prey = aPrey;
	};
	virtual void Update(float delta) override;

protected:

	virtual void ChasePrey(float delta);
	virtual void EatPrey();
};
