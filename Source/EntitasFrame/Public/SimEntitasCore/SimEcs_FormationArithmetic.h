/*=================================================
* FileName: SimEcs_FormationArithmetic.h
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

#pragma once

#include "CoreMinimal.h"
#include "SimApi.h"

/**
 *  DECLARE_DELEGATE_OneParam  Arithmetic
 */

DECLARE_DELEGATE_OneParam( FRunLeaderFormationArithmetic, FTransform&  );				//人字形
DECLARE_DELEGATE_OneParam( FRunSingleColumnFormationArithmetic, FTransform& );			//单纵队
DECLARE_DELEGATE_OneParam( FOnSingleRowFormationArithmetic, FTransform& );				//单横队
DECLARE_DELEGATE_OneParam( FOnDoubleRowFormationArithmetic, FTransform& );				//双横队
DECLARE_DELEGATE_OneParam( FOnEchelonFormationArithmetic, FTransform& );					//梯阶队
DECLARE_DELEGATE_OneParam( FOnBearingFormationArithmetic, FTransform& );					//方位队
	//双横队
/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            SimEcs_FormationArithmetic  /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

class SimEcs_FormationArithmetic;
class SimEcs_DelegateFormation : public TSharedFromThis<SimEcs_DelegateFormation>
{
public:
	friend class SimEcs_FormationArithmetic;
	SimEcs_DelegateFormation();

	void Update(float delta) ;

	/* run boat's formation,be changed */
	void RunFormation(  FTransform& leaderTrans, EBoatFormation eFromationType );

	FRunLeaderFormationArithmetic		 RunLeaderFormationArithmetic;			//人字形
	FRunSingleColumnFormationArithmetic  RunSingleColumnFormationArithmetic;	//单纵队
	FOnSingleRowFormationArithmetic		 RunSingleRowFormationArithmetic;		//单横队
	FOnDoubleRowFormationArithmetic		 RunDoubleRowFormationArithmetic;		//双横队
	FOnEchelonFormationArithmetic		 RunEchelonFormationArithmetic;			//梯阶队
	FOnBearingFormationArithmetic		 RunBearingFormationArithmetic;			//方位队
			
protected:

private:
	TSharedPtr<SimEcs_FormationArithmetic> m_simFromationArithmeticPtr;
};

class SimEcs_FormationArithmetic {

public:
	SimEcs_FormationArithmetic( ) {}
	SimEcs_FormationArithmetic(  SimEcs_DelegateFormation * pDelegateFromation );
	~SimEcs_FormationArithmetic();
	void FunLeaderFormationArithmetic(  FTransform& leaderTrans );				//人字形
	void FunSingleColumnFormationArithmetic(  FTransform& leaderTrans );		//单纵队
	void FunSingleRowFormationArithmetic(  FTransform& leaderTrans );			//单横队
	void FunDoubleRowFormationArithmetic(  FTransform& leaderTrans );			//双横队
	void FunEchelonFormationArithmetic(  FTransform& leaderTrans );			//梯阶队
	void FunBearingFormationArithmetic(  FTransform& leaderTrans );			//方位队


/////////////////////////////////////////////////////////////////////////////        
//////////////           辅助编队函数				  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

//LeaderFormation
	FVector4 CaculateNextLeaderFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle );


private:
	SimEcs_DelegateFormation * m_pSimDelegateFormation;
	float baseFormationAngle = 30.0f;
	float formationAngle = 5.0f;
	float formationLength = 5000.0f;
	float thresholdValue = 100.0f;

	int   FormationCoolDownTime = 3.0f;
	int   FormationLastTime = 0.0f;
	int   FormationChildLastTime = 0.0f;
};

