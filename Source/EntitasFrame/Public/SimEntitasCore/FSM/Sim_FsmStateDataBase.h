#pragma once


#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
/// <summary>
/// 状态机状态数据
/// </summary>
using EntityHandleId = uint64_t;


class FsmStateDataBase {
public:
	EntityHandleId   entHandleId;
	bool bAnimation;
	int32 iType;
	FName name;

	FsmStateDataBase( );
	void Copy( FsmStateDataBase SData )
	{
		iType = SData.iType;
		entHandleId = SData.entHandleId;    //index
		name = "";
		bAnimation = false;
	}
};

