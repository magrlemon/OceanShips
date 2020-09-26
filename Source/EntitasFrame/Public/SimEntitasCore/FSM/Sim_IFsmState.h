#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
//#include "FSM\Sim_IFSM.h"
#include "FSM\Sim_FsmStateDataBase.h"


class IFSMInterface;
/// <summary>
/// 状态机状态接口
/// </summary>
class  IFsmStateInterface {

public:
	IFsmStateInterface( );
	virtual ~IFsmStateInterface( ) {};
		/// <summary>
		/// 状态初始化接口
		/// </summary>
		/// <param name="fsm">状态属于的状态机</param>
	virtual void Init( IFSMInterface* fsm ) abstract;

	virtual FName GetType( ) abstract;

	/// <summary>
	/// 状态开始接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	/// <param name="data">状态数据</param>
	virtual void Enter( IFSMInterface* fsm, FsmStateDataBase data ) abstract;
	/// <summary>
	/// 状态暂停接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	virtual void Pause( IFSMInterface* fsm ) abstract;
	/// <summary>
	/// 状态恢复接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	virtual void Resume( IFSMInterface* fsm ) abstract;
	/// <summary>
	/// 状态重置接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	virtual void Reset( IFSMInterface* fsm ) abstract;
	/// <summary>
	/// 状态离开接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	virtual void Exit( IFSMInterface* fsm ) abstract;
	/// <summary>
	/// 状态销毁接口
	/// </summary>
	/// <param name="fsm">状态属于的状态机</param>
	 virtual void Destroy( IFSMInterface* fsm ) abstract;
	/// <summary>
	/// 状态更新函数
	/// </summary>
	/// <param name="frameTime">逻辑一帧时间</param>
	/// <param name="realTime">实际一帧时间</param>
	 virtual void Update( float frameTime, float realTime ) abstract;
};

