/**
* Copyright 2019 北京伯睿科技有限公司.
* All Rights Reserved.
* http://www.borui.com
*
* @File ASimEcs_EntitasFrameData.h
* @Description 组件数据产生管理器
*
* @Author LiuKX
* @Date 2019年12月19日
* @Version 1.0
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimEcs_Engine.h"
#include "Engine.h"
#include "SimEcs_BattleComponents.h"
#include "ArmyScenarioLoader.h"
#include "ArmyScenarioXmlParserModule.h"
#include "ArmyCoastDefLoader.h"
#include "ArmyCoastDefXmlParserModule.h"
#include "GameFramework/Actor.h"
#include "SimEcs_Archetype.h"
#include "SimEcs_EntitasFrameData.generated.h"


UCLASS( )
class ENTITASFRAME_API  ASimEcs_EntitasFrameData : public AActor
{
	GENERATED_BODY( )
		
public:
	enum   EEntityEquips {
		EEE_NONE = 0,
		EEE_TANK_TYPE,				//坦克
		EEE_VEHICLE4W_TYPE,		    //车辆
		EEE_CHARACTOR_TYPE,			//人员角色
		EEE_AIRCRAFT_TYPE,			//舰载机
		EEE_HELICOPTER_TYPE,		//直升机
		EEE_DRONEBOAT_TYPE,			//无人操控舰艇
		EEE_NAVALCRAFT_TYPE,		//舰艇
		EEE_PATROLBOAT_TYPE,		//巡逻艇
		EEE_TRAFFICBOAT_TYPE,		//交通运输艇
		EEE_ENGINEERINGBOAT_TYPE,	//工程作业艇
		EEE_ENTITYEQUIPS_NUM = EEE_ENGINEERINGBOAT_TYPE,

		EEE_OBSTACLE_TYPE = 1000,
		EEE_OBSTACLE_SJZ_TYPE,			//三角锥		 1001
		EEE_OBSTACLE_HLS_TYPE,			//海立石		 1002
		EEE_OBSTACLE_GTZ_TYPE,			//轨条砦		 1003
		EEE_OBSTACLE_GCW_TYPE,			//钢刺猬		 1004
		EEE_OBSTACLE_TSW_SF_TYPE,		//蛇腹铁丝网	 1005
		EEE_OBSTACLE_TSW_YL_TYPE,		//一列铁丝网	 1006
		EEE_OBSTACLE_ZGQ_TYPE,			//组隔墙		 1007
		EEE_OBSTACLE_HHLC_TYPE,			//混合雷场		 1008
		EEE_OBSTACLE_BLDS_TYPE,			//玻璃刀山		 1009
		EEE_OBSTACLE_GTZ_SUIJI_TYPE,	//水迹轨条砦	 1010
		EEE_OBSTACLE_ZJH_TYPE,			//阻绝壕		 1011

		EEE_OBSTACLE_NUM = EEE_OBSTACLE_ZJH_TYPE,
	};
	ASimEcs_EntitasFrameData( );
	virtual void BeginPlay() override;
	virtual void BeginDestroy( ) override;


	/** Update the character. (Running, health etc). */
	virtual void Tick(float DeltaSeconds) override;


public:

	using TMapScenario = const  TMap<FString, FString>;
	bool GenerateOBEntitys( int entType, TMapScenario & refScenarioXmlData );

	// 根据想定生成仿真实体物体
	void GenScenarioObjects( );

	// 根据想定生成障碍物体
	void GenCoastDefBarrierObjects( );

	/* Get SimEcs System Engine Ptr*/
	TWeakPtr<SimEcs_Engine> GetEcsEnginePtr( ) { return  m_SimEcsEnginePtr; }

	TSubclassOf<ASimEcs_Archetype> GetSubClassByType( int type );
	int32 MakeFStringToInt( const FString* strType );

	using TArrScenario = const  TArray<TMap<FString, FString>>;

private:
	TSharedPtr<SimEcs_Engine> m_SimEcsEnginePtr = nullptr;
	TMap<int32, TSubclassOf<ASimEcs_Archetype> > m_MapSubClass_BP;
};
