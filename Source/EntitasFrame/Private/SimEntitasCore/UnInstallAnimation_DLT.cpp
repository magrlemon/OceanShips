#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "FSM\Sim_IFSM.h"
#include "FSM\Sim_FsmStateDataBase.h"
#include "UnInstallAnimation_DLT.h"
#include "Sim_OceanSceneManager.h"


#define  DLT_ZHIJIA_BASE "DengLuTing_ZhiJia_Base"    //船艇支架底部

#define  DLT_ZHIJIA_MAIN_FRONT "DengLuTing_ZhiJia_Main"	//船艇支架上部

#define  DLT_ZHIJIA_MAIN_BACK "DengLuTing_ZhiJia_Main_Back" //船艇后支架上部
/*
 *  DLT 状态机动画 待机
 */
IdleZJAnimationState::IdleZJAnimationState( ) {}
// Use this for initialization
/// <summary>
/// 状态初始化接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;

	bAnimation = false;
	iLocation = 0;
}

FName IdleZJAnimationState::GetType() {
	return "class IdleZJAnimationState";
}
/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void IdleZJAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	iLocation = data.iLocation;
}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Exit( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void IdleZJAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void IdleZJAnimationState::Update( float frameTime, float realTime ) {
	
	if (!bAnimation || iLocation == 0) return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;
	FsmStateDataBase fsdb;
	fsdb.entHandleId = entHandleId;
	fsdb.bAnimation = bAnimation;
	fsdb.iLocation = 1;
	USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm( 
		*(archeType->GetName( )) )->ChangeState<SliderFrontZJAnimationState>( fsdb );

	
}

/*
 *  活动前向支架到船头
 */
SliderFrontZJAnimationState::SliderFrontZJAnimationState( ) {}
// Use this for initialization
/// <summary>
/// 状态初始化接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;
	sliderFrontLocation = 2100.0f;
	bAnimation = false;
	iLocation = 0;
}

FName SliderFrontZJAnimationState::GetType() {
	return "class SliderFrontZJAnimationState";
}
/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void SliderFrontZJAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	entHandleId = data.iLocation;
	iLocation = data.iLocation;
}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Exit( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderFrontZJAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void SliderFrontZJAnimationState::Update( float frameTime, float realTime ) {

	if (!bAnimation)return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;
	FString componentName = FString( DLT_ZHIJIA_BASE );
	auto SubStaticMesh = archeType->GetSubUStaticMeshComponentByName( componentName );
	if (!SubStaticMesh)return;
	FTransform CurTransform = SubStaticMesh->GetRelativeTransform( );
	if (CurTransform.GetLocation( ).X <= sliderFrontLocation) {
		const FVector detal = FVector( sliderFrontLocation *0.066f * frameTime, 0.0f, 0.0f );
		SubStaticMesh->AddRelativeLocation( detal );
	}
	else{
		FsmStateDataBase fsdb;
		fsdb.entHandleId = entHandleId;
		fsdb.bAnimation = bAnimation;
		fsdb.entFirstId = entDroneBoatId;
		USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
			*(archeType->GetName( )) )->ChangeState<UpWarpZJAnimationState>( fsdb );
	}
}




/*
 *  前支架翘起
 */
UpWarpZJAnimationState::UpWarpZJAnimationState( ) {}
// Use this for initialization
/// <summary>
/// 状态初始化接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;
	iLocation = 0;
	bAnimation = false;
}

FName UpWarpZJAnimationState::GetType() {
	return "class UpWarpZJAnimationState";
}
/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void UpWarpZJAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	iLocation = data.iLocation;

}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Exit( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UpWarpZJAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
/// 



void UpWarpZJAnimationState::Update( float frameTime, float realTime ) {
	if (!bAnimation)return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;

	FString DeviceName = FString( DLT_ZHIJIA_MAIN_FRONT );
	auto SubStaticMesh = archeType->GetSubUStaticMeshComponentByName( DeviceName );
	if (!SubStaticMesh) return;
	
	FRotator CurRotator = SubStaticMesh->GetComponentRotation( );
	if (CurRotator.Pitch >= LastRotationAngle) {
		FRotator DeltaRotation( 0.0f, frameTime * LastRotationAngle * 0.01f, 0.0f );
		SubStaticMesh->AddRelativeRotation( DeltaRotation );
	}
	else {
		FsmStateDataBase fsdb;
		fsdb.entHandleId = entHandleId;
		fsdb.bAnimation = bAnimation;
		fsdb.iLocation = iLocation;
		fsdb.entFirstId = entDroneBoatId;
		EntityHandleId DroneBoatID = USimOceanSceneManager_Singleton::GetInstance( )->GetUnstallDroneBoatIDByParentDeviceName( archeType->GetName(), iLocation );
		if (DroneBoatID <= 0) {
			fsdb.bAnimation = false;
			fsdb.iLocation = 0;
			USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
				*(archeType->GetName( )) )->ChangeState<IdleZJAnimationState>( fsdb );
		}
	
		USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
			*(archeType->GetName( )) )->ChangeState<UnInstallBoatAnimationState>( fsdb );
	}
}


/*
 *  卸载破障艇，滑落水中
 */

UnInstallBoatAnimationState::UnInstallBoatAnimationState( ) {}
// Use this for initialization
/// <summary>
/// 状态初始化接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;
	iLocation = 0;
	bAnimation = false;
}

FName UnInstallBoatAnimationState::GetType() {
	return "class UnInstallBoatAnimationState";
}
/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void UnInstallBoatAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	iLocation = data.iLocation;
}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Exit( IFSMInterface* fsm ) {
	//m_realKeepTime = 0.0f;
}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void UnInstallBoatAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void UnInstallBoatAnimationState::Update( float frameTime, float realTime ) {
	if (!bAnimation)return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;
	//EntityHandleId DroneBoatID = USimOceanSceneManager_Singleton::GetInstance( )->GetUnstallDroneBoatIDByParentDeviceName( archeType->GetName( ), iLocation );
	if (entDroneBoatId > 0) {
		auto archeDroneBoatType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entDroneBoatId );
		FVector CurActorLocation = archeDroneBoatType.Get( )->GetActorLocation();
		float sliderLenght = 550.0f;
		if (CurActorLocation.X < 200.0f ) {
			const FVector detal = FVector( sliderLenght *0.02f * frameTime, 0.0f, 0.0f );
			archeDroneBoatType.Get( )->AddActorLocalOffset( detal );
		}
		else {
			archeDroneBoatType->DetachFromActor( FDetachmentTransformRules::KeepRelativeTransform );

			FsmStateDataBase fsdb;
			fsdb.entHandleId = entHandleId;
			fsdb.bAnimation = bAnimation;
			fsdb.iLocation = iLocation;
			fsdb.entFirstId = entDroneBoatId;
			USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
				*(archeType->GetName( )) )->ChangeState<DowningZJAnimationState>( fsdb );
		}
	}
}



/*
 *  放下支架
 */
DowningZJAnimationState::DowningZJAnimationState( ) {}
 // Use this for initialization
 /// <summary>
 /// 状态初始化接口
 /// </summary>
 /// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;
	iLocation = 0;
	bAnimation = false;
}

FName DowningZJAnimationState::GetType() {
	return "class DowningZJAnimationState";
}
/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void DowningZJAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	iLocation = data.iLocation;
}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Exit( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void DowningZJAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void DowningZJAnimationState::Update( float frameTime, float realTime ) {
	if (!bAnimation)return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;

	FString DeviceName = FString( DLT_ZHIJIA_MAIN_FRONT );
	auto SubStaticMesh = archeType->GetSubUStaticMeshComponentByName( DeviceName );
	if (!SubStaticMesh) return;

	FRotator CurRotator = SubStaticMesh->GetComponentRotation( );
	if (CurRotator.Pitch <= 0.0f) {
		FRotator DeltaRotation( 0.0f, frameTime * LastRotationAngle * 0.01f, 0.0f );
		SubStaticMesh->AddRelativeRotation( DeltaRotation );
	}
	else {
		FsmStateDataBase fsdb;
		fsdb.entHandleId = entHandleId;
		fsdb.bAnimation = bAnimation;
		fsdb.iLocation = iLocation;
	
		EntityHandleId DroneBoatID = USimOceanSceneManager_Singleton::GetInstance( )->GetUnstallDroneBoatIDByParentDeviceName( archeType->GetName( ), iLocation );
		if (DroneBoatID <= 0) {
			fsdb.bAnimation = false;
			fsdb.iLocation = 0;
			USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
				*(archeType->GetName( )) )->ChangeState<IdleZJAnimationState>( fsdb );
			return;
		}
		fsdb.entSecondId = DroneBoatID;
		USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
			*(archeType->GetName( )) )->ChangeState<SliderBackZJAnimationState>( fsdb );
	}
}


//
///*
// *  检测登陆艇船体装备 ，是否还有等待需要卸载的破障船艇
// */
//
//
//CheckoutEquipmentState::CheckoutEquipmentState( ) {}
//// Use this for initialization
///// <summary>
///// 状态初始化接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Init( IFSMInterface* fsm ) {
//	entHandleId = 0;
//	iLocation = 0;
//	bAnimation = false;
//}
///// <summary>
///// 状态开始接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
///// <param name="data">状态数据</param>
//void CheckoutEquipmentState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
//	entHandleId = data.entHandleId;
//	bAnimation = data.bAnimation;
//	iLocation = data.iLocation;
//}
///// 状态暂停接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Pause( IFSMInterface* fsm ) {
//}
///// <summary>
///// 状态恢复接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Resume( IFSMInterface* fsm ) {
//}
///// <summary>
///// 状态重置接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Reset( IFSMInterface* fsm ) {
//}
///// <summary>
///// 状态离开接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Exit( IFSMInterface* fsm ) {
//
//}
///// <summary>
///// 状态销毁接口
///// </summary>
///// <param name="fsm">状态属于的状态机</param>
//void CheckoutEquipmentState::Destroy( IFSMInterface* fsm ) {
//}
///// <summary>
///// 状态更新函数
///// </summary>
///// <param name="frameTime">逻辑一帧时间</param>
///// <param name="realTime">实际一帧时间</param>
//void CheckoutEquipmentState::Update( float frameTime, float realTime ) {
//
//	if (!bAnimation)return;
//
//	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
//	if (!archeType)return;
//	const EntityHandleId backShipID = USimOceanSceneManager_Singleton::GetInstance( )->GetUnstallDroneBoatIDByParentDeviceName( archeType->GetName( ), 2 );
//	if (backShipID < 0) {
//		FsmStateDataBase fsdb;
//		fsdb.entHandleId = entHandleId;
//		fsdb.bAnimation = false;
//		fsdb.iLocation = iLocation;
//		fsdb.entFirstId = 0;
//		USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
//			*(archeType->GetName( )) )->ChangeState<UnInstallBoatAnimationState>( fsdb );
//	}
//}

/*
 *  支架划向后端，去承载第二艘破障艇
 */

SliderBackZJAnimationState::SliderBackZJAnimationState( ) {}
// Use this for initialization
/// <summary>
 /// 状态初始化接口
 /// </summary>
 /// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Init( IFSMInterface* fsm ) {
	entHandleId = 0;
	iLocation = 0;
	bAnimation = false;

}

FName SliderBackZJAnimationState::GetType() {
	return "class SliderBackZJAnimationState";
}


/// <summary>
/// 状态开始接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
/// <param name="data">状态数据</param>
void SliderBackZJAnimationState::Enter( IFSMInterface* fsm, FsmStateDataBase data ) {
	entHandleId = data.entHandleId;
	bAnimation = data.bAnimation;
	iLocation = data.iLocation;
	entDroneBoatId = data.entSecondId;
}
/// 状态暂停接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Pause( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态恢复接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Resume( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态重置接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Reset( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态离开接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Exit( IFSMInterface* fsm ) {

}
/// <summary>
/// 状态销毁接口
/// </summary>
/// <param name="fsm">状态属于的状态机</param>
void SliderBackZJAnimationState::Destroy( IFSMInterface* fsm ) {
}
/// <summary>
/// 状态更新函数
/// </summary>
/// <param name="frameTime">逻辑一帧时间</param>
/// <param name="realTime">实际一帧时间</param>
void SliderBackZJAnimationState::Update( float frameTime, float realTime ) {

	if (!bAnimation)return;

	auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
	if (!archeType)return;

	FString componentName = FString( DLT_ZHIJIA_BASE );
	auto SubStaticMesh = archeType->GetSubUStaticMeshComponentByName( componentName );
	if (!SubStaticMesh)return;

	FTransform CurTransform = SubStaticMesh->GetRelativeTransform( );
	if (CurTransform.GetLocation( ).X <= 0.0f) {
		const FVector detal = FVector( sliderFrontLocation *0.066f * frameTime, 0.0f, 0.0f );
		SubStaticMesh->AddRelativeLocation( detal );
	}
	else {

		//把第二艘船艇放到前端支架上

		auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entHandleId );
		if (!archeType)return;
		if (entDroneBoatId < 0){

			FsmStateDataBase fsdb;
			fsdb.entHandleId = entHandleId;
			fsdb.bAnimation = false;
			fsdb.entFirstId = 0;
			fsdb.iLocation = 0;
			USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
				*(archeType->GetName( )) )->ChangeState<IdleZJAnimationState>( fsdb );
			return;
		}

		FsmStateDataBase fsdb;
		fsdb.entHandleId = entHandleId;
		fsdb.bAnimation = bAnimation;
		fsdb.entFirstId = entDroneBoatId;
		USimOceanSceneManager_Singleton::GetInstance( )->GetFsmManager( )->GetFsm(
			*(archeType->GetName( )) )->ChangeState<SliderFrontZJAnimationState>( fsdb );
	}
}




