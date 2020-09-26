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
	EntityHandleId   entFirstId;
	EntityHandleId   entSecondId;
	bool bAnimation;
	int32 iType;
	int32 iLocation = 0;
	FName name;


	FsmStateDataBase( );
	void Copy( FsmStateDataBase SData )
	{
		iType = SData.iType;
		entHandleId = SData.entHandleId;    //index
		name = "";
		bAnimation = false;
		iLocation = 0;
	}
};

