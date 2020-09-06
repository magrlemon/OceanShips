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
	FString strFromation = "";
	switch (eFromationType) {
	case EBoatFormation::E_LEADER_FORMATION:				//人字形
		RunLeaderFormationArithmetic.ExecuteIfBound( leaderTrans );
		strFromation = "LEADER FORMATION";
		break;
	case EBoatFormation::E_SINGLE_COLUMN_FORMATION:			//单纵队
		RunSingleColumnFormationArithmetic.ExecuteIfBound(leaderTrans);
		strFromation = "SINGLE COLUMN FORMATION";
		break;
	case EBoatFormation::E_SINGLE_ROW_FORMATION:			//单横队
		RunSingleRowFormationArithmetic.ExecuteIfBound(leaderTrans);
		strFromation = "SINGLE ROW FORMATION";
		break;
	case EBoatFormation::E_DOUBLE_ROW_FORMATION:			//双横队
		RunDoubleRowFormationArithmetic.ExecuteIfBound(leaderTrans);
		strFromation = "DOUBLE ROW FORMATION";
		break;
	case EBoatFormation::E_ECHELON_FORMATION:				//梯阶队
		RunEchelonFormationArithmetic.ExecuteIfBound(leaderTrans);
		strFromation = "ECHELON FORMATION";
		break;
	case EBoatFormation::E_BEARING_FORMATION:				//方位队
		RunBearingFormationArithmetic.ExecuteIfBound(leaderTrans);
		strFromation = "BEARING FORMATION";
		break;
	default:
		RunLeaderFormationArithmetic.ExecuteIfBound(leaderTrans);
		break;
	}

	FString simMsg = FString::Printf( TEXT( "Run Formation is Name is : %s" ), *strFromation );
	USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
}


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Formation Arithmetic		  /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

using EntityHandleId = uint64_t;
SimEcs_FormationArithmetic::SimEcs_FormationArithmetic(  SimEcs_DelegateFormation * pDelegateFromation ) {

	m_pSimDelegateFormation = pDelegateFromation;
	if (m_pSimDelegateFormation)
	{
		m_pSimDelegateFormation->RunLeaderFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunLeaderFormationArithmetic);
		m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunSingleColumnFormationArithmetic);
		m_pSimDelegateFormation->RunSingleRowFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunSingleRowFormationArithmetic);
		m_pSimDelegateFormation->RunDoubleRowFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunDoubleRowFormationArithmetic);
		m_pSimDelegateFormation->RunEchelonFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunEchelonFormationArithmetic);
		m_pSimDelegateFormation->RunBearingFormationArithmetic.BindRaw(this, &SimEcs_FormationArithmetic::FunBearingFormationArithmetic);
	}
}

SimEcs_FormationArithmetic::~SimEcs_FormationArithmetic() {
	m_pSimDelegateFormation->RunLeaderFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunSingleColumnFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunSingleRowFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunDoubleRowFormationArithmetic.Unbind();
	m_pSimDelegateFormation->RunEchelonFormationArithmetic.Unbind();
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
			float followDis = formationRowLength + location * formationRowLength;
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
				if (FVector::Dist2D( btp, posLeft ) + baseFormationAngle * 0.5 > FVector::Dist2D( btp, posRight ))
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
				if (FVector::Dist2D( btp, posRight ) + formationRowLength * 0.5f > FVector::Dist2D( btp, posLeft ))
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
	if (!m_pSimDelegateFormation)return;

	auto& TTMapBoatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo;
	for (auto& itemGroup : TTMapBoatFormationInfo) {
		//sub leader id -1
		for (int32 location = 0; location < (itemGroup.Value.Num( )); location++) {
			TArray<EntityHandleId> handleIDs;
			int32 flag = itemGroup.Value.GetKeys( handleIDs );
			if (!itemGroup.Value[handleIDs[location]].IsLeader) {
				float followDis = formationRowLength + location * formationRowLength;
				float nextAngle = baseFormationAngle + (formationAngle * location) / 2.0f;
				FVector SingleRowPoints = CaculateSingleColumnFormationLocate( leaderTrans, followDis, nextAngle );
				FVector nextPosition = FVector( SingleRowPoints.X, SingleRowPoints.Y, leaderTrans.GetTranslation( ).Z );
				itemGroup.Value[handleIDs[location]].BoatTargetPosition = nextPosition;
			}
		}
	}
}

FVector SimEcs_FormationArithmetic::CaculateSingleColumnFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle ) {
	float bastAngle = 180.0f;
	FVector ForwardVector = leaderTrans.GetRotation( ).GetForwardVector( );
	const FVector RAxisDirection = ForwardVector.RotateAngleAxis( bastAngle, FVector::UpVector );
	FVector ColumnPosition = RAxisDirection.GetSafeNormal( ) * fNextDistance + leaderTrans.GetTranslation( );
	return FVector( ColumnPosition.X, ColumnPosition.Y, leaderTrans.GetTranslation( ).Z );
}

/////////////////////////////////////////////////////////////////////////////        
//////////////           单横队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunSingleRowFormationArithmetic(  FTransform& leaderTrans ) {
	if (!m_pSimDelegateFormation)return;

	auto& TTMapBoatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo;
	for (auto& itemGroup : TTMapBoatFormationInfo) {
		//sub leader id -1
		for (int32 location = 0; location < (itemGroup.Value.Num( )); location++) {
			TArray<EntityHandleId> handleIDs;
			int32 flag = itemGroup.Value.GetKeys( handleIDs );

			if (!itemGroup.Value[handleIDs[location]].IsLeader) {
				float followDis = formationRowLength + location * formationRowLength;
				float nextAngle = baseFormationAngle + (formationAngle * location) / 2.0f;
				FVector SingleRowPoints = CaculateSingleRowFormationLocate( leaderTrans, followDis, nextAngle );
				FVector nextPosition = FVector( SingleRowPoints.X, SingleRowPoints.Y, leaderTrans.GetTranslation( ).Z );
				itemGroup.Value[handleIDs[location]].BoatTargetPosition = nextPosition;
			}
		}
	}
} 

FVector SimEcs_FormationArithmetic::CaculateSingleRowFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle ) {
	float bastAngle = 90.0f; 

	FVector ForwardVector = leaderTrans.GetRotation( ).GetForwardVector( );
	const FVector LAxisDirection = ForwardVector.RotateAngleAxis( bastAngle, FVector::UpVector );
	FVector rowPosition = LAxisDirection.GetSafeNormal( ) * fNextDistance + leaderTrans.GetTranslation( );
	return rowPosition;
}


/////////////////////////////////////////////////////////////////////////////        
//////////////           双横队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////
//双横队
void SimEcs_FormationArithmetic::FunDoubleRowFormationArithmetic(  FTransform& leaderTrans ) {
	if (!m_pSimDelegateFormation)return;

	auto& TTMapBoatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo;
	for (auto& itemGroup : TTMapBoatFormationInfo) {

		int32 boatOneRowNum = itemGroup.Value.Num( )/2;
		int32 validIndex = 0;
		for (int32 location = 0; location < (itemGroup.Value.Num( )); location++) {
			TArray<EntityHandleId> handleIDs;
			int32 flag = itemGroup.Value.GetKeys( handleIDs );
			if (!itemGroup.Value[handleIDs[location]].IsLeader) {
				float followDis = formationRowLength + validIndex * formationRowLength;
				int32 rowNum = boatOneRowNum > location ? 1 : 2;
				FVector SingleRowPoints = CaculateDoubleRowFormationLocate( leaderTrans, followDis, rowNum );
				FVector nextPosition = FVector( SingleRowPoints.X, SingleRowPoints.Y, leaderTrans.GetTranslation( ).Z );
				itemGroup.Value[handleIDs[location]].BoatTargetPosition = nextPosition;
				validIndex++;
			}
		}
	}
}

FVector SimEcs_FormationArithmetic::CaculateDoubleRowFormationLocate( const FTransform& leaderTrans, 
	float  fNextDistanceRow, float fNextDistanceColumn ) {

	float  jumpIndex = (fNextDistanceRow - 1)*-1.0f;
	FVector OffectValue = FVector( fNextDistanceRow, jumpIndex* fNextDistanceColumn,0.0f );
	return leaderTrans.TransformPosition( OffectValue );
}


/////////////////////////////////////////////////////////////////////////////        
//////////////           梯阶队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunEchelonFormationArithmetic(  FTransform& leaderTrans ) {
	if (!m_pSimDelegateFormation)return;

	auto& TTMapBoatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo;
	for (auto& itemGroup : TTMapBoatFormationInfo) {
		//sub leader id -1
		for (int32 location = 0; location <= itemGroup.Value.Num( ); location++) {
			float followDis = formationRowLength + location * formationRowLength;
			float nextAngle = baseFormationAngle + (formationAngle * location) / 2.0f;
			FVector4 doublePoints = CaculateNextLeaderFormationLocate( leaderTrans, followDis, nextAngle );
			TArray<EntityHandleId> handleIDs;
			int32 flag = itemGroup.Value.GetKeys( handleIDs );
			FVector posRight = FVector( doublePoints.Z, doublePoints.W, leaderTrans.GetTranslation( ).Z );
			if (!itemGroup.Value[handleIDs[location ]].IsLeader) {
				FVector btp = itemGroup.Value[handleIDs[location]].BoatTargetPosition;
				if (FVector::Dist2D( btp, posRight ) + formationRowLength * 0.5f > FVector::Dist2D( btp, posRight ))
					itemGroup.Value[handleIDs[location]].BoatTargetPosition = posRight;
			}
		}
	}
}

FVector SimEcs_FormationArithmetic::CaculateEchelonFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle ) {
	float bastAngle = 180.0f; float l, r;
	l = bastAngle - formationAngle; 
	r = bastAngle + formationAngle;

	FVector ForwardVector = leaderTrans.GetRotation( ).GetForwardVector( );
	const FVector RAxisDirection = ForwardVector.RotateAngleAxis( r, FVector::UpVector );
	FVector rightPosition = RAxisDirection.GetSafeNormal( ) * fNextDistance + leaderTrans.GetTranslation( );
	return FVector( rightPosition.X, rightPosition.Y, leaderTrans.GetTranslation( ).Z);
}

/////////////////////////////////////////////////////////////////////////////        
//////////////           方位队						  /////////////////////                                 
/////////////////////////////////////////////////////////////////////////

void SimEcs_FormationArithmetic::FunBearingFormationArithmetic(  FTransform& leaderTrans ) {

}

FVector SimEcs_FormationArithmetic::CaculateBearingFormationLocate( const FTransform& leaderTrans, float  fNextDistance, float formationAngle ) {

	return FVector::ZeroVector;
}