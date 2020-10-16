///-------------------------------------------------------------------------------------------------
/// File:	include\API.h.
///
/// Summary:	API.
/// 
/** Copyright 2019 ������Ƽ����޹�˾.
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

#define ECS_ABOUVE_GROUD_HEIGHT				800      //�ϰ�����ؼ�϶


#define ECS_DISATACE_SHIP_THRESHOLD			1500.0f      //�ϰ�����ؼ�϶

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


	//-----------��ͧ-------------
	"/Game/ShipsAndExamples/BP_ECSBoat_XLT",	  //9
	"/Game/ShipsAndExamples/BP_Boats_YSC",
	"/Game/ShipsAndExamples/BP_ECSBoat_GCZYJT",
	//----------------------------

	//�����ϰ�
	"CoastDef",							     //����     11
	"/Game/Blueprints/BP_CoastDef_SJZ",		 //����׶
	"/Game/Blueprints/BP_CoastDef_HLS",		 //����ʯ
	"/Game/Blueprints/BP_CoastDef_GTZ",		 //������
	"/Game/Blueprints/BP_CoastDef_GCW",	     //�ִ��
	"/Game/Blueprints/BP_CoastDef_WaterMine",   //ˮ��

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
	E_LEADER_FORMATION,			//������
	E_SINGLE_COLUMN_FORMATION,	//���ݶ�
	E_SINGLE_ROW_FORMATION,		//�����
	E_DOUBLE_ROW_FORMATION,		//˫���
	E_ECHELON_FORMATION,		//�ݽ׶�
	E_BEARING_FORMATION,		//��λ��
};


enum   EEntityEquips {
	EEE_NONE = 0,
	EEE_TANK_TYPE,				//̹��					1
	EEE_VEHICLE4W_TYPE,		    //����					2
	EEE_CHARACTOR_TYPE,			//��Ա��ɫ				3
	EEE_AIRCRAFT_TYPE,			//���ػ�				4
	EEE_HELICOPTER_TYPE,		//ֱ����				5
	EEE_DRONEBOAT_TYPE,			//���˲ٿؽ�ͧ			6
	EEE_NAVALCRAFT_TYPE,		//��ͧ					7
	EEE_PATROLBOAT_TYPE,		//Ѳ��ͧ				8
	EEE_TRAFFICBOAT_TYPE,		//��ͨͧ				9
	EEE_ENGINEERINBOAT_TYPE,	//������ҵͧ		   10
	EEE_TCC_CAR_TYPE,			//̽�⳵		   10

	EEE_COASTDEF_TYPE = 1000,
	EEE_COASTDEF_SJZ_TYPE,			//����׶		 1001
	EEE_COASTDEF_HLS_TYPE,			//����ʯ		 1002
	EEE_COASTDEF_GTZ_TYPE,			//������		 1003
	EEE_COASTDEF_GCW_TYPE,			//�ִ��		 1004
	EEE_COASTDEF_TSW_SF_TYPE,		//�߸���˿��	 1005
	EEE_COASTDEF_TSW_YL_TYPE,		//һ����˿��	 1006
	EEE_COASTDEF_ZGQ_TYPE,			//���ǽ		 1007
	EEE_COASTDEF_HHLC_TYPE,			//����׳�		 1008
	EEE_COASTDEF_BLDS_TYPE,			//������ɽ		 1009
	EEE_COASTDEF_GTZ_SUIJI_TYPE,	//ˮ��������	 1010
	EEE_COASTDEF_ZJH_TYPE,			//�����		 1011

	EEE_ENTITYEQUIPS_NUM = EEE_COASTDEF_ZJH_TYPE
};


enum   EComponentClass {
	ECC_NONE_COMPONENT = 0,
	ECC_POSITION_COMPONENT,				//�������
	ECC_ROTATE_COMPONENT,				//��ת���
	ECC_MOVEMENT_COMPONENT,				//�˶����
	ECC_BOATS_MOVEMENT_COMPONENT,		//�����˶����
	ECC_VELOCITY_COMPONENT,				//�ٶ����
	ECC_ROTATION_COMPONENT,				//��Ⱦ���
	ECC_MOVEMENTRAYCAST_COMPONENT,		//�������
	ECC_BARRIERFIXEDRAYCAST_COMPONENT,  //����λ�ý������
	ECC_DELETER_COMPONENT,				//ɾ�����
	ECC_RANDOMARCSPAWN_COMPONENT,		//�������
	ECC_ARCHETYPESPAWNER_COMPONENT,		//��Դ�������
	ECC_INSTANCEDSTATICMESH_COMPONENT,	//Mesh���
	ECC_SCALE_COMPONENT,				//�Ŵ����
	ECC_FACTION_COMPONENT,				//�˾����
	ECC_PROJECTILE_COMPONENT,			//���ڷ������
	ECC_EXPLOSION_COMPONENT,			//��ը���
	ECC_HEALTH_COMPONENT,				//�������
	ECC_DEBUGSPHERE_COMPONENT,			//��������� 
	ECC_ENT_FORCE_COMPONENT,			//�������������
	ECC_ENT_FORMAION_COMPONENT,			//������
	ECC_ENT_DLT_ANIMATION_STATE_COMPONENT,	 //��½ͧ����״̬�����
	ECC_COMPONENT_NUM = ECC_ENT_FORCE_COMPONENT
};



#define  BP_ACTOR_TYPE_SPLIT 1000    //��ͼ��Դ�ָ���
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