/**
* Copyright 2019 ������Ƽ����޹�˾.
* All Rights Reserved.
* http://www.borui.com
*
* @File ASimEcs_EntitasFrameData.h
* @Description ������ݲ���������
*
* @Author LiuKX
* @Date 2019��12��19��
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
		EEE_TANK_TYPE,				//̹��
		EEE_VEHICLE4W_TYPE,		    //����
		EEE_CHARACTOR_TYPE,			//��Ա��ɫ
		EEE_AIRCRAFT_TYPE,			//���ػ�
		EEE_HELICOPTER_TYPE,		//ֱ����
		EEE_DRONEBOAT_TYPE,			//���˲ٿؽ�ͧ
		EEE_NAVALCRAFT_TYPE,		//��ͧ
		EEE_PATROLBOAT_TYPE,		//Ѳ��ͧ
		EEE_TRAFFICBOAT_TYPE,		//��ͨ����ͧ
		EEE_ENGINEERINGBOAT_TYPE,	//������ҵͧ
		EEE_ENTITYEQUIPS_NUM = EEE_ENGINEERINGBOAT_TYPE,

		EEE_OBSTACLE_TYPE = 1000,
		EEE_OBSTACLE_SJZ_TYPE,			//����׶		 1001
		EEE_OBSTACLE_HLS_TYPE,			//����ʯ		 1002
		EEE_OBSTACLE_GTZ_TYPE,			//������		 1003
		EEE_OBSTACLE_GCW_TYPE,			//�ִ��		 1004
		EEE_OBSTACLE_TSW_SF_TYPE,		//�߸���˿��	 1005
		EEE_OBSTACLE_TSW_YL_TYPE,		//һ����˿��	 1006
		EEE_OBSTACLE_ZGQ_TYPE,			//���ǽ		 1007
		EEE_OBSTACLE_HHLC_TYPE,			//����׳�		 1008
		EEE_OBSTACLE_BLDS_TYPE,			//������ɽ		 1009
		EEE_OBSTACLE_GTZ_SUIJI_TYPE,	//ˮ��������	 1010
		EEE_OBSTACLE_ZJH_TYPE,			//�����		 1011

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

	// �����붨���ɷ���ʵ������
	void GenScenarioObjects( );

	// �����붨�����ϰ�����
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
