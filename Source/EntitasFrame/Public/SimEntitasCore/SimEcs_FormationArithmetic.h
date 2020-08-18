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

DECLARE_DELEGATE_OneParam( FRunLeaderFormationArithmetic, FTransform&  );				//������
DECLARE_DELEGATE_OneParam( FRunSingleColumnFormationArithmetic, FTransform& );			//���ݶ�
DECLARE_DELEGATE_OneParam( FOnSingleRowFormationArithmetic, FTransform& );				//�����
DECLARE_DELEGATE_OneParam( FOnDoubleRowFormationArithmetic, FTransform& );				//˫���
DECLARE_DELEGATE_OneParam( FOnEchelonFormationArithmetic, FTransform& );					//�ݽ׶�
DECLARE_DELEGATE_OneParam( FOnBearingFormationArithmetic, FTransform& );					//��λ��
	//˫���
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

	FRunLeaderFormationArithmetic		 RunLeaderFormationArithmetic;			//������
	FRunSingleColumnFormationArithmetic  RunSingleColumnFormationArithmetic;	//���ݶ�
	FOnSingleRowFormationArithmetic		 RunSingleRowFormationArithmetic;		//�����
	FOnDoubleRowFormationArithmetic		 RunDoubleRowFormationArithmetic;		//˫���
	FOnEchelonFormationArithmetic		 RunEchelonFormationArithmetic;			//�ݽ׶�
	FOnBearingFormationArithmetic		 RunBearingFormationArithmetic;			//��λ��
			
protected:

private:
	TSharedPtr<SimEcs_FormationArithmetic> m_simFromationArithmeticPtr;
};

class SimEcs_FormationArithmetic {

public:
	SimEcs_FormationArithmetic( ) {}
	SimEcs_FormationArithmetic(  SimEcs_DelegateFormation * pDelegateFromation );
	~SimEcs_FormationArithmetic();
	void FunLeaderFormationArithmetic(  FTransform& leaderTrans );				//������
	void FunSingleColumnFormationArithmetic(  FTransform& leaderTrans );		//���ݶ�
	void FunSingleRowFormationArithmetic(  FTransform& leaderTrans );			//�����
	void FunDoubleRowFormationArithmetic(  FTransform& leaderTrans );			//˫���
	void FunEchelonFormationArithmetic(  FTransform& leaderTrans );			//�ݽ׶�
	void FunBearingFormationArithmetic(  FTransform& leaderTrans );			//��λ��


/////////////////////////////////////////////////////////////////////////////        
//////////////           ������Ӻ���				  /////////////////////                                 
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

