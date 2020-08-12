/**
* Copyright 2019 北京伯睿科技有限公司.
* All Rights Reserved.
* http://www.borui.com
*
* @File OceanSceneManager.h
* @Description 场景组件管理
*
* @Author LiuKX
* @Date 2019年12月19日
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

	using EntityHandleId = uint64_t;
	TMap<EntityHandleId, TSharedPtr<ASimEcs_Archetype>> m_MapArchetypes;


	struct BoatFormationStruct {
		BoatFormationStruct( ) {
			Name = "";
			BoatLocate = FVector( 0.0f, 0.0f, 0.0f );
			ForwardVector = FVector( 0.0f, 0.0f, 0.0f );
			IsLeader = false;
		}
		BoatFormationStruct( FName name, FVector locate, FVector forwardVector ,bool isLeader ) : Name( name ), 
			BoatLocate( locate ), ForwardVector(forwardVector), IsLeader( isLeader ) {};
		FName Name= "";
		FVector BoatLocate = FVector( 0.0f, 0.0f, 0.0f );
		FVector ForwardVector = FVector( 0.0f, 0.0f, 0.0f );
		bool IsLeader = false;
	};


	using EntityHandleId = uint64_t;
	TMap<FString, TMap<EntityHandleId, BoatFormationStruct>> m_TTMapBoatFormationInfo; //FVector4(pos3,dir)

	/* save boat'name for scenario'file */
	TMap<EntityHandleId, FName> m_MapArchetypesName;
private:
	static USimOceanSceneManager_Singleton* gSingletonScene;
	TArray<FString> BrocastMessages;
public:

	void MakeRoot( );
	/* initialize date */
	void Initialize( );
	// generate Ocean Actor
	void GenOceanScenarioActor( );
	//generate Barrier
	void GenOceanScenarioBarrier( );

	/*get arcthtype by entity*/
	TSharedPtr<ASimEcs_Archetype>  FindArchetype(EntityHandleId handleID);
	//////////////////////////////////////////////////////////////////////////
	/*
	// the message for ecs system
	*/
	bool IsHavSimMessage( );
	void RemoveSimMessage( );
	void PushSimMessage( FString& strMsg );
	TArray<FString>& CopySimMessage( );

	//////////////////////////////////////////////////////////////////////////
	/*
	//  deal  boat data,replace (boat component )date to ecs system
	*/
	//////////////////////////////////////////////////////////////////////////
	void  MoveEntity( const FString& strName, const FVector& posRef );
	void  MoveBackEntity( const FString& strName, const FVector& posRef );
	void Firing( const FString& strName, const bool bFire );
	//////////////////////////////////////////////////////////////////////////
	//get tag
	TSharedPtr<AActor> GetSimActorWithTag( const  FString& strTag );

	//get name
	EntityHandleId GetSimHandleIDWithName( const FString& strName );

	//get leader position
	FVector GetSimLeaderPosition( const FString& strGroup );
	/* terminate object date */
	void Terminate( );

	void Update( float tick_ms );

	void DebugLogger( FString strMode = "USimOceanSceneManager_Singleton is Warning!!! " );
	 
	FVector GetCovertScenePosition( FVector ImagePosition , ESceneRelevantConv  erc = ESceneRelevantConv::E_SYSTEM_LINK_POINT);

	DVector2 ConvertWorldPositonToLatLon( FVector fPos );

	void ConvertPixToWorldPositon( FVector& fPos );

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

	TWeakPtr<SimEcs_Engine> GetSimEcsEngineWeakPtr();

	UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
	USimEcs_ComponentSystemLink* m_ComponentSL_Ptr = nullptr;


	//UCLASS( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
		TSharedPtr<AActor> m_ScenarioSceneTap;

	//UCLASS( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
		TSharedPtr<AActor> m_ScenarioBarrierTap;

		UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
			FVector m_vecScenarioMesh = FVector( -180180.0f, -370000.0f, -6600.0f );
 
		/* Barrier Area Region A */
		UPROPERTY( EditAnywhere, AdvancedDisplay, BlueprintReadWrite, Category = "Ocean Boats Settings" )
			FVector m_vecScenarioBarrierMesh = FVector( -112500.0f, -135850.0f, -6600.0f );

		FTransform m_ScenarioMeshTransform;

		FTransform m_ScenarioBarrierMeshTransform;

private:
	bool m_bLoadSuccess = false;
	bool m_bLoadScenario = false;
	bool m_bLoadBarrier = false;

	/* data be stored here*/
	GpsDataTransfer m_gpsDataTransfer;

	TArray<FString> m_arrSimMessage;

};
