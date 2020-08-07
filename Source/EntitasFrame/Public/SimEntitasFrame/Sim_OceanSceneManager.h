/**
* Copyright 2019 ������Ƽ����޹�˾.
* All Rights Reserved.
* http://www.borui.com
*
* @File OceanSceneManager.h
* @Description �����������
*
* @Author LiuKX
* @Date 2019��12��19��
* @Version 1.0
*/
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SimEcs_EntitasFrameData.h"
#include "util/GpsDataTransfer.h"
#include "Sim_OceanSceneManager.generated.h"


/**
 *
 */
class USingletonAttribute;
class UGameInstance;
class USimEcs_ComponentSystemLink;

UCLASS( Blueprintable, BlueprintType )
class ENTITASFRAME_API  USimOceanSceneManager_Singleton : public UObject {
	GENERATED_BODY( )

public:
	/* Construct*/
	USimOceanSceneManager_Singleton( const FObjectInitializer& ObjectInitializer);

	static USimOceanSceneManager_Singleton* GetInstance( ) {
		if (gSingletonScene == nullptr) {
			gSingletonScene = NewObject<USimOceanSceneManager_Singleton>( );
			gSingletonScene->AddToRoot( );
		}
		return gSingletonScene;
	};

	
private:
	static USimOceanSceneManager_Singleton* gSingletonScene;

public:

	void MakeRoot( );
	/* initialize date */
	void Initialize( );

	// generate Ocean Actor
	void GenOceanActorBarrier( );

	//generate Barrier
	void GenOceanDefanceBarrier( );

	//get tag
	TSharedPtr<AActor> GetSimActorWithTag(  FString& strTag );

	/* terminate object date */
	void Terminate( );

	void Update( float tick_ms );

	void DebugLogger( FString strMode = "USimOceanSceneManager_Singleton is Warning!!! " );
	 
	FVector GetCovertScenePosition( FVector ImagePosition , ESceneRelevantConv  erc = ESceneRelevantConv::E_SYSTEM_LINK_POINT);

	DVector2 ConvertWorldPositonToLatLon( FVector fPos );

	SimEcs_Registry * GetSimRegistry() ;


protected:
	/* initialize Scenario xml data */
	void InitialzieScenarioData( );

	/* initialize Scenario xml data */
	void InitialzieOceanDefScenarioData( );
public:

	void ReSpawnSystemLink( );
	///* Get Sim Ecs Root Actor*/
	USimEcs_ComponentSystemLink* GetComponentSysLinkPtr( );

	UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
	USimEcs_ComponentSystemLink* m_ComponentSL_Ptr = nullptr;


	//UCLASS( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
		TSharedPtr<AActor> m_ScenarioSceneTap;

	//UCLASS( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
		TSharedPtr<AActor> m_ScenarioBarrierTap;

		UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
			FVector m_vecScenarioMesh = FVector( -180180.0f, -370000.0f, -6600.0f );
		UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
			FVector m_vecScenarioBarrierMesh = FVector( -112500.0f, -135850.0f, -6600.0f ); 

		FTransform m_ScenarioMeshTransform;

		FTransform m_ScenarioBarrierMeshTransform;
public:
	using EntityHandleId = uint64_t;
	TMap<EntityHandleId, TSharedPtr<ASimEcs_Archetype>> m_MapArchetypes;
private:
	bool m_bLoadSuccess = false;
	bool m_bLoadScenario = false;
	bool m_bLoadBarrier = false;

	/* data be stored here*/
	GpsDataTransfer m_gpsDataTransfer;
};
