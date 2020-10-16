///-------------------------------------------------------------------------------------------------
/// File:	include\API.h.
///
/// Summary:	API.
/// 
/** Copyright 2019 北京伯睿科技有限公司.
*	Author :	magi
	Date   :	11th July, 2019
	File   :	include\SimApi.h.
	Summary:	Declares the  assest resource.
	All Rights Reserved.
	*http://www.borui.com	(c) Copyright 2019.
*/
/*
Preprocessor defines:

	ECS_DISABLE_LOGGING			- Disable logging feature.



*/

#ifndef __SIM_API_H__
#define __SIM_API_H__
#pragma once

#define ENITY_LUT_GROW						2048

#define ENITY_T_CHUNK_SIZE					512

#define COMPONENT_LUT_GROW					1024

#define COMPONENT_T_CHUNK_SIZE				512

// 4MB 
#define ECS_EVENT_MEMORY_BUFFER_SIZE		4194304

// 8MB
#define ECS_SYSTEM_MEMORY_BUFFER_SIZE		8388608

#define ECS_ABOUVE_GROUD_HEIGHT				800      //障碍物离地间隙


#define ECS_DISATACE_SHIP_THRESHOLD			1500.0f      //障碍物离地间隙

#include "util/Handle.h"
#include "SimEcs_Platform.h"
#include "Memory/Allocator/PoolAllocator.h"



static const FString bp_type_names[] = {
	"/Game/Blueprints/BP_Dummy",
	"/Game/TropicalIsland/BluePrints/BP_Tank",
	"/Game/TropicalIsland/BluePrints/BP_Vehicle",
	"/Game/TropicalIsland/BluePrints/BP_Charactor",
	"/Game/TropicalIsland/BluePrints/BP_AirCraft",
	"/Game/TropicalIsland/BluePrints/BP_Helicopter",
	"/Game/ShipsAndExamples/BP_Boat",
	"/Game/ShipsAndExamples/BP_DengLT_271",


	//-----------船艇-------------
	"/Game/ShipsAndExamples/BP_ECSBoat_XLT",	  //9
	"/Game/ShipsAndExamples/BP_Boats_YSC",
	"/Game/ShipsAndExamples/BP_ECSBoat_GCZYJT",
	//----------------------------

	//海防障碍
	"CoastDef",							     //海防     11
	"/Game/Blueprints/BP_CoastDef_SJZ",		 //三角锥
	"/Game/Blueprints/BP_CoastDef_HLS",		 //海立石
	"/Game/Blueprints/BP_CoastDef_GTZ",		 //轨条砦
	"/Game/Blueprints/BP_CoastDef_GCW",	     //钢刺猬
	"/Game/Blueprints/BP_CoastDef_WaterMine",   //水雷

};

enum EMissionState
{
	E_MissionStart,
	E_MissionPause,
	E_MissionStop
};


enum   ESceneRelevantConv {
	E_SENERAIO_POINT,
	E_BARRIER_POINT,
	E_SYSTEM_LINK_POINT
};


enum   EBoatFormation {
	E_LEADER_FORMATION,			//人字形
	E_SINGLE_COLUMN_FORMATION,	//单纵队
	E_SINGLE_ROW_FORMATION,		//单横队
	E_DOUBLE_ROW_FORMATION,		//双横队
	E_ECHELON_FORMATION,		//梯阶队
	E_BEARING_FORMATION,		//方位队
};


enum   EEntityEquips {
	EEE_NONE = 0,
	EEE_TANK_TYPE,				//坦克					1
	EEE_VEHICLE4W_TYPE,		    //车辆					2
	EEE_CHARACTOR_TYPE,			//人员角色				3
	EEE_AIRCRAFT_TYPE,			//舰载机				4
	EEE_HELICOPTER_TYPE,		//直升机				5
	EEE_DRONEBOAT_TYPE,			//无人操控舰艇			6
	EEE_NAVALCRAFT_TYPE,		//舰艇					7
	EEE_PATROLBOAT_TYPE,		//巡逻艇				8
	EEE_TRAFFICBOAT_TYPE,		//交通艇				9
	EEE_ENGINEERINBOAT_TYPE,	//工程作业艇		   10
	EEE_TCC_CAR_TYPE,			//探测车		   10

	EEE_COASTDEF_TYPE = 1000,
	EEE_COASTDEF_SJZ_TYPE,			//三角锥		 1001
	EEE_COASTDEF_HLS_TYPE,			//海立石		 1002
	EEE_COASTDEF_GTZ_TYPE,			//轨条砦		 1003
	EEE_COASTDEF_GCW_TYPE,			//钢刺猬		 1004
	EEE_COASTDEF_TSW_SF_TYPE,		//蛇腹铁丝网	 1005
	EEE_COASTDEF_TSW_YL_TYPE,		//一列铁丝网	 1006
	EEE_COASTDEF_ZGQ_TYPE,			//组隔墙		 1007
	EEE_COASTDEF_HHLC_TYPE,			//混合雷场		 1008
	EEE_COASTDEF_BLDS_TYPE,			//玻璃刀山		 1009
	EEE_COASTDEF_GTZ_SUIJI_TYPE,	//水迹轨条砦	 1010
	EEE_COASTDEF_ZJH_TYPE,			//阻绝壕		 1011

	EEE_ENTITYEQUIPS_NUM = EEE_COASTDEF_ZJH_TYPE
};


enum   EComponentClass {
	ECC_NONE_COMPONENT = 0,
	ECC_POSITION_COMPONENT,				//坐标组件
	ECC_ROTATE_COMPONENT,				//旋转组件
	ECC_MOVEMENT_COMPONENT,				//运动组件
	ECC_BOATS_MOVEMENT_COMPONENT,		//船体运动组件
	ECC_VELOCITY_COMPONENT,				//速度组件
	ECC_ROTATION_COMPONENT,				//渲染组件
	ECC_MOVEMENTRAYCAST_COMPONENT,		//射线组件
	ECC_BARRIERFIXEDRAYCAST_COMPONENT,  //射线位置矫正组件
	ECC_DELETER_COMPONENT,				//删除组件
	ECC_RANDOMARCSPAWN_COMPONENT,		//创建组件
	ECC_ARCHETYPESPAWNER_COMPONENT,		//资源创建组件
	ECC_INSTANCEDSTATICMESH_COMPONENT,	//Mesh组件
	ECC_SCALE_COMPONENT,				//放大组件
	ECC_FACTION_COMPONENT,				//盟军组件
	ECC_PROJECTILE_COMPONENT,			//火炮发射组件
	ECC_EXPLOSION_COMPONENT,			//爆炸组件
	ECC_HEALTH_COMPONENT,				//毁伤组件
	ECC_DEBUGSPHERE_COMPONENT,			//调试球组件 
	ECC_ENT_FORCE_COMPONENT,			//物理作用力组件
	ECC_ENT_FORMAION_COMPONENT,			//编队组件
	ECC_ENT_DLT_ANIMATION_STATE_COMPONENT,	 //登陆艇动画状态机组件
	ECC_COMPONENT_NUM = ECC_ENT_FORCE_COMPONENT
};



#define  BP_ACTOR_TYPE_SPLIT 1000    //蓝图资源分割线
//namespace Internal

class  MemoryManager;
extern MemoryManager*				ECSMemoryManager;


class   GlobalMemoryUser {
private:

	MemoryManager* ECS_MEMORY_MANAGER;

public:

	GlobalMemoryUser( );
	virtual ~GlobalMemoryUser( );

	 const void* Allocatex( size_t memSize, const char* user = nullptr );
	 void Free( void* pMem );
};



namespace Event {
	class EventHandler;
}


class EntityManager;
class SystemManager;
class ComponentManager;


///-------------------------------------------------------------------------------------------------
/// Class:	GlobalMemoryUser
///
/// Summary:	Any class that wants to use the global memory must derive from this class.
///
/// Author:	magi
///
/// Date:	9/09/2019
///-------------------------------------------------------------------------------------------------


using SimEcs_EntityId = util::SimHandle64;

class SimEcs_Engine;
class ComponentManager;


void		CheckMemoryLeaks( );



#endif // __SIM_API_H__