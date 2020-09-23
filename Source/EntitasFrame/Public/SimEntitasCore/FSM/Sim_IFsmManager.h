#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "FSM\Sim_IFsmState.h"
#include "FSM\Sim_IFSM.h"
// This class does not need to be modified.
//UINTERFACE( )
//class UFsmManagerInterface : public UInterface {
//	GENERATED_BODY( )
//};

/// <summary>
/// 状态机接口
/// </summary>
class IFsmManagerInterface {

public: 
	IFsmManagerInterface( );
	virtual ~IFsmManagerInterface( );
	/// <summary>
	/// 创建状态机
	/// </summary>
	/// <param name="owner">状态机拥有者</param>
	/// <param name="fsmName">状态机名称</param>
	/// <param name="states">状态集合</param>
	/// <returns>创建状态机</returns>
	virtual IFSMInterface* CreateFsm( AActor* owner, FName fsmName, TArray<IFsmStateInterface*>& states ) abstract;
	/// <summary>
	/// 获得状态机
	/// </summary>
	/// <param name="fsmName">状态机的名称</param>
	/// <returns>获得的状态机</returns>
	virtual IFSMInterface* GetFsm( FName fsmName ) abstract;
	/// <summary>
	/// 是否包含状态机
	/// </summary>
	/// <param name="fsmName">状态机名称</param>
	/// <returns>是否包含状态机</returns>
	virtual bool HasFsm( FName fsmName ) abstract;
	/// <summary>
	/// 销毁状态机
	/// </summary>
	/// <param name="fsmName">状态机名称</param>
	virtual void DestroyFsm( FName fsmName ) abstract;
	/// <summary>
	/// 获得所有的状态机
	/// </summary>
	virtual void GetAllFsm( TArray<IFSMInterface*>& list ) abstract;

};

