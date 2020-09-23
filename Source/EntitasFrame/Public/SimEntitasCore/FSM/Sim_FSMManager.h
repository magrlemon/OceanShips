

#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "UObject/Interface.h"
#include "FSM\Sim_FSM.h"
#include "FSM\Sim_IFsmManager.h"

/// <summary>
/// 状态机管理类
/// </summary>

class FsmManager : public IFsmManagerInterface {

private:
	TMap<FName, FSMachine*> m_Fsms ;

public:

	FsmManager( );
	virtual ~FsmManager( );
	virtual IFSMInterface* CreateFsm( AActor* owner, FName fsmName, TArray<IFsmStateInterface*>& states ) override;

	virtual void DestroyFsm( FName fsmName ) override;
	
	virtual void GetAllFsm( TArray<IFSMInterface*>& list ) override;
	
	virtual IFSMInterface* GetFsm( FName fsmName ) override ;
	
	virtual bool HasFsm( FName fsmName ) override;
	
	/// <summary>
	/// 模块优先级
	/// </summary>
	int32 GetPriority()
	{
		return 0;
	}
		/// <summary>
		/// 初始化
		/// </summary>
	void Init( );
		
		/// <summary>
		/// 销毁
		/// </summary>
		void Destroy( );
		
		/// <summary>
		/// 更新函数
		/// </summary>
		/// <param name="frameTime">逻辑一帧时间</param>
		/// <param name="realTime">实际一帧时间</param>
		void Update( float frameTime, float realTime );

		/// <summary>
		/// 重置接口
		/// </summary>
		void Reset( );
		
};

