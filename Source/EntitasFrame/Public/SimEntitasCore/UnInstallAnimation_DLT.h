#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "FSM\Sim_IFSM.h"
#include "FSM\Sim_FsmStateDataBase.h"



/*
 *  DLT 无卸船相关动画
 */
class IdleZJAnimationState : public  IFsmStateInterface {
public:
	IdleZJAnimationState( );
	EntityHandleId   entHandleId;
	EntityHandleId   entDroneBoatId;
	bool bAnimation = false;
	int32 iLocation = 0;
	// Use this for initialization
	/// <summary>
	/// 状态初始化接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  ) ;

	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	void Update( float frameTime, float realTime );



};

/*
 *  活动前向支架到船头
 */
class SliderFrontZJAnimationState : public  IFsmStateInterface {
public:
	SliderFrontZJAnimationState( );
	EntityHandleId   entDroneBoatId;
	EntityHandleId   entHandleId;
	float sliderFrontLocation = 2100.0f;
	bool bAnimation = false;
	int32 iLocation = 0;
	// Use this for initialization
	/// <summary>
	/// 状态初始化接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  );

	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	void Update( float frameTime, float realTime );



};


/*
 *  前支架翘起
 */
class UpWarpZJAnimationState : public  IFsmStateInterface {

public:
	UpWarpZJAnimationState( );
	EntityHandleId   entHandleId;
	EntityHandleId   entDroneBoatId;
	float LastRotationAngle = -20.0f ;
	bool bAnimation = false;
	int32 iLocation = 0;
   // Use this for initialization
   /// <summary>
   /// 状态初始化接口
   /// </summary>
   /// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  );

	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	/// 



	void Update( float frameTime, float realTime );
};


/*
 *  卸载破障艇，滑落水中
 */
class UnInstallBoatAnimationState : public  IFsmStateInterface {

public:
	UnInstallBoatAnimationState( );
	EntityHandleId   entHandleId;
	EntityHandleId   entDroneBoatId;
	bool bAnimation = false;
	int32 iLocation = 0;
   // Use this for initialization
   /// <summary>
   /// 状态初始化接口
   /// </summary>
   /// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  );
	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	void Update( float frameTime, float realTime );
};


/*
 *  放下支架
 */
class DowningZJAnimationState : public  IFsmStateInterface {

public:
	DowningZJAnimationState( );
	EntityHandleId   entHandleId;
	EntityHandleId   entDroneBoatId;
	bool bAnimation = false;
	float LastRotationAngle = 20.0f;
	int32 iLocation = 0;
	// Use this for initialization
	/// <summary>
	/// 状态初始化接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  );

	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );

	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	void Update( float frameTime, float realTime );
};

//
///*
// *  检测登陆艇船体装备 ，是否还有等待需要卸载的破障船艇
// */
//class  CheckoutEquipmentState : public  IFsmStateInterface {
//
//public:
//	CheckoutEquipmentState( );
//	EntityHandleId   entHandleId;
//	EntityHandleId   entDroneBoatId;
//	bool bAnimation = false;
//	int32 iLocation = 0;
//								   // Use this for initialization
//								   /// <summary>
//								   /// 状态初始化接口
//								   /// </summary>
//								   /// <param name="fsm">状态属于的状态机</param>
//	void Init( IFSMInterface* fsm );
//
//	FName GetType(  );
//	/// <summary>
//	/// 状态开始接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	/// <param name="data">状态数据</param>
//	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
//	/// 状态暂停接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	void Pause( IFSMInterface* fsm );
//	/// <summary>
//	/// 状态恢复接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	void Resume( IFSMInterface* fsm );
//	/// <summary>
//	/// 状态重置接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	void Reset( IFSMInterface* fsm );
//
//	/// <summary>
//	/// 状态离开接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	void Exit( IFSMInterface* fsm );
//	/// <summary>
//	/// 状态销毁接口
//	/// </summary>
//	/// <param name="fsm">状态属于的状态机</param>
//	void Destroy( IFSMInterface* fsm );
//	/// <summary>
//	/// 状态更新函数
//	/// </summary>
//	/// <param name="frameTime">逻辑一帧时间</param>
//	/// <param name="realTime">实际一帧时间</param>
//	void Update( float frameTime, float realTime );
//};



/*
 *  检测登陆艇船体装备 ，是否还有等待需要卸载的破障船艇
 */
class  SliderBackZJAnimationState : public  IFsmStateInterface {

public:
	SliderBackZJAnimationState( );
	EntityHandleId   entHandleId;
	EntityHandleId   entDroneBoatId;
	bool bAnimation = false;
	int32 iLocation = 0;
	float sliderFrontLocation = -2100.0f;
								   // Use this for initialization
								   /// <summary>
								   /// 状态初始化接口
								   /// </summary>
								   /// <param name="fsm">状态属于的状态机</param>
	void Init( IFSMInterface* fsm );

	FName GetType(  );
	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	void Enter( IFSMInterface* fsm, FsmStateDataBase data );
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Pause( IFSMInterface* fsm );
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Resume( IFSMInterface* fsm );
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Reset( IFSMInterface* fsm );
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Exit( IFSMInterface* fsm );
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	void Destroy( IFSMInterface* fsm );
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	void Update( float frameTime, float realTime );
};




