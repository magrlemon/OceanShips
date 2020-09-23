#pragma once

#include "FSM\Sim_FsmStateDataBase.h"
#include "CoreMinimal.h"
#include "SimEcs_Engine.h"

/// <summary>
/// 状态机状态数据
/// </summary>

FsmStateDataBase::FsmStateDataBase( )
{
	iType = 7;
	entHandleId = 0;    //index
	name = "";
	bAnimation = false;
}
