/*=================================================
* FileName: SimEcs_FormationArithmetic.cpp
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


#include "SimEcs_FormationArithmetic.h"
#include "Sim_OceanSceneManager.h"
/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            SimEcs_FormationArithmetic  /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////
SimEcs_DelegateFormation::SimEcs_DelegateFormation( ) {
	m_simFromationArithmeticPtr = MakeShareable( new SimEcs_FormationArithmetic( this ) );
}

void SimEcs_DelegateFormation::Update( float delta )
{

}

void SimEcs_DelegateFormation::RunFormation( FTransform& leaderTrans, EBoatFormation eFromationType )
{
	switch (eFromationType) {
	case EBoatFormation::E_LEADER_FORMATION:				//人字形
		RunLeaderFormationArithmetic.ExecuteIfBound( leaderTrans );
		break;
	case EBoatFormation::E_SINGLE_COLUMN_FORMATION:			//单纵队
		RunSingleColumnFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	case EBoatFormation::E_SINGLE_ROW_FORMATION:			//单横队
		RunSingleRowFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	case EBoatFormation::E_DOUBLE_ROW_FORMATION:			//双横队
		RunDoubleRowFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	case EBoatFormation::E_ECHELON_FORMATION:				//梯阶队
		RunEchelonFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	case EBoatFormation::E_BEARING_FORMATION:				//方位队
		RunBearingFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	default:
		RunLeaderFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	}
}


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Formation Arithmetic		 /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

using EntityHandleId = uint64_t;
SimEcs_FormationArithmetic::SimEcs_FormationArithmetic(  SimEcs_DelegateFormation * pDelegateFromation ) {

	m_pSimDelegateFormation = pDelegateFromation;
	if (m_pSimDelegateFormation)
	{
		m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunLeaderFormationArithmetic);
		m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunSingleColumnFormationArithmetic);
		m_pSimDelegateFormation->RunSingleRowFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunSingleRowFormationArithmetic);
		m_pSimDelegateFormation->RunDoubleRowFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunDoubleRowFormationArithmetic);
		m_pSimDelegateFormation->RunEchelonFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunEchelonFormationArithmetic);
		m_pSimDelegateFormation->RunBearingFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunBearingFormationArithmetic);
	}
}

SimEcs_FormationArithmetic::~SimEcs_FormationArithmetic() {
	m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunSingleRowFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunDoubleRowFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunEchelonFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunBearingFormationArithmetic.Unbind();
}


/////////////////////////////////////////////////////////////////////////////        
//////////////           人字形						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunLeaderFormationArithmetic( FTransform& leaderTrans ) {
	if (!m_pSimDelegateFormation)return;

	auto& TTMapBoatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo;
	for (auto& itemGroup : TTMapBoatFormationInfo) {
		//sub leader id -1
		for (int32 location = 0; location <= (itemGroup.Value.Num( )) / 2; location++) {
			float followDis = formationLength + location * formationLength;
			float nextAngle = baseFormationAngle + (formationAngle * location) / 2.0f;
			FVector4 doublePoints = CaculateNextLeaderFormationLocate( leaderTrans, followDis, nextAngle );
			TArray<EntityHandleId> handleIDs;
			int32 flag = itemGroup.Value.GetKeys( handleIDs );

			FVector posLeft = FVector( doublePoints.X, doublePoints.Y, leaderTrans.GetTranslation( ).Z );
			FVector posRight = FVector( doublePoints.Z, doublePoints.W, leaderTrans.GetTranslation( ).Z );
			bool bSawpPositionL = false;	bool bSawpPositionR = false;
			if (location * 2 >= itemGroup.Value.Num( ))
				return;
			else if (!itemGroup.Value[handleIDs[location * 2]].IsLeader) {
				FVector btp = itemGroup.Value[handleIDs[location * 2]].BoatTargetPosition;
				if (FVector::DistSquared2D( btp, posLeft ) + baseFormationAngle * 0.5 > FVector::DistSquared2D( btp, posRight ))
					bSawpPositionL = true;

			}
			if (location * 2 + 1 >= itemGroup.Value.Num( )) {
				if (bSawpPositionL && bSawpPositionR) {
					itemGroup.Value[handleIDs[location * 2]].BoatTargetPosition = posRight;
				}
				else {
					itemGroup.Value[handleIDs[location * 2]].BoatTargetPosition = posLeft;
				}
				return;
			}
			else if (!itemGroup.Value[handleIDs[location * 2 + 1]].IsLeader) {
				FVector btp = itemGroup.Value[handleIDs[location * 2 + 1]].BoatTargetPosition;
				if (FVector::DistSquared2D( btp, posRight ) + baseFormationAngle * 0.5 > FVector::DistSquared2D( btp, posLeft ))
					bSawpPositionR = true;

			}
			if (bSawpPositionL && bSawpPositionR) {
				itemGroup.Value[handleIDs[location * 2]].BoatTargetPosition = posRight;
				itemGroup.Value[handleIDs[location * 2 + 1]].BoatTargetPosition = posLeft;
			}
			else {
				itemGroup.Value[handleIDs[location * 2]].BoatTargetPosition = posLeft;
				itemGroup.Value[handleIDs[location * 2 + 1]].BoatTargetPosition = posRight;
			}
		}
	}
}

FVector4 SimEcs_FormationArithmetic::CaculateNextLeaderFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle ) {
	float bastAngle = 180.0f; float l, r;
	l = bastAngle - formationAngle; r = bastAngle + formationAngle;

	FVector ForwardVector = leaderTrans.GetRotation( ).GetForwardVector( );
	const FVector LAxisDirection = ForwardVector.RotateAngleAxis( l, FVector::UpVector );
	FVector leftPosition = LAxisDirection.GetSafeNormal( ) * fNextDistance + leaderTrans.GetTranslation( );

	const FVector RAxisDirection = ForwardVector.RotateAngleAxis( r, FVector::UpVector );
	FVector rightPosition = RAxisDirection.GetSafeNormal( ) * fNextDistance + leaderTrans.GetTranslation( );
	return FVector4( leftPosition.X, leftPosition.Y, rightPosition.X, rightPosition.Y );
}

/////////////////////////////////////////////////////////////////////////////        
//////////////           单纵队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunSingleColumnFormationArithmetic(  FTransform& leaderTrans ) {

}


/////////////////////////////////////////////////////////////////////////////        
//////////////           单横队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunSingleRowFormationArithmetic(  FTransform& leaderTrans ) {

}

/////////////////////////////////////////////////////////////////////////////        
//////////////           方位队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////
//双横队
void SimEcs_FormationArithmetic::FunDoubleRowFormationArithmetic(  FTransform& leaderTrans ) {

}


/////////////////////////////////////////////////////////////////////////////        
//////////////           梯阶队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunEchelonFormationArithmetic(  FTransform& leaderTrans ) {
}



/////////////////////////////////////////////////////////////////////////////        
//////////////           方位队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunBearingFormationArithmetic(  FTransform& leaderTrans ) {

}