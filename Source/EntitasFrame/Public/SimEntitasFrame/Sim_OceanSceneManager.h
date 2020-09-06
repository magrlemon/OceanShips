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
#include "SimEcs_CameraManager.h"
#include "SimDataStructure.h"
#include "Sim_OceanSceneManager.generated.h"


/**
 *
 */
class USingletonAttribute;
class UGameInstance;
class USimEcs_ComponentSystemLink;


/**
 *  DECLARE_DELEGATE_OneParam  Arithmetic
 */

DECLARE_DELEGATE_OneParam( FEnumerateObjectsStreamsCallback, const FEnumerateObjectsStreamsResult& );



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

    /* per group'leader*/
	TMap<FString, EntityHandleId> m_MapLeaderArchetypes;

	struct BoatFormationStruct {
		BoatFormationStruct( ) {
			Name = "";
			BoatTargetPosition = FVector( 0.0f, 0.0f, 0.0f );
			ForwardVector = FVector( 0.0f, 0.0f, 0.0f );
			IsLeader = false;
			AvoidForceMultiplier = 2.0f;
			// Avoidance force
			AvoidanceForce = 2000.0f;

			AvoidanceDistance = 1000.0f;

			ExpectAvoidOffect = FVector::ZeroVector;
		}
		BoatFormationStruct( FName name, FVector locate, FVector forwardVector , FVector expectAvoidOffect,bool isLeader ) : Name( name ),
			BoatTargetPosition( locate ), ForwardVector(forwardVector), ExpectAvoidOffect( expectAvoidOffect ),IsLeader( isLeader ) {};
		FName Name = "";
		FVector BoatTargetPosition = FVector( 0.0f, 0.0f, 0.0f );
		FVector ForwardVector = FVector( 0.0f, 0.0f, 0.0f );
		FVector ExpectAvoidOffect = FVector::ZeroVector;
		bool IsLeader = false;
		// Avoid Distance Multiplier
		float AvoidForceMultiplier = 2.0f;
		// Avoidance force
		float AvoidanceForce = 20000.0f;
		// Avoidance Distance
		float AvoidanceDistance = 5000.0f;

		
	};


	using EntityHandleId = uint64_t;
	typedef TMap<FString, TMap<EntityHandleId, BoatFormationStruct>>   TTMapFormation;
	typedef  TMap<EntityHandleId, BoatFormationStruct> TMapFormation;
	TTMapFormation m_TTMapBoatFormationInfo; //FVector4(pos3,dir)

	/* save boat'name for scenario'file */
	TMap<EntityHandleId, FName> m_MapArchetypesName;
private:
	static USimOceanSceneManager_Singleton* gSingletonScene;
	TArray<FString> BrocastMessages;

	ASimEcs_PlayerCameraManager* m_PlayerCameraManager = nullptr;
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

	void  SetIdle( const FString& strName, const FVector& posRef );
	void  SetIdle( const EntityHandleId ehandleID, const FVector& posRef );
	bool  IsArriving( const FString& strName, const FVector& posRef );
	bool  IsArriving( const EntityHandleId ehandleID, const FVector& posRef );
	void  MoveEntity( const FString& strName, const FVector& posRef );
	void  MoveEntity(const FString& GroupName,EntityHandleId ehandleID, const FVector& posRef);
	void  MoveBackEntity( const FString& strName, const FVector& posRef );
	void  Firing( const FString& strName, const bool bFire );
	//////////////////////////////////////////////////////////////////////////
	//get tag
	TSharedPtr<AActor> GetSimActorWithTag( const  FString& strTag );

	//get name
	EntityHandleId GetSimHandleIDWithName( const FString& strName );

	TSharedPtr<ASimEcs_Archetype>  GetSimActorWithName( const FString& strName );

	TSharedPtr<AActor>  GetOceanActor( );

	ASimEcs_PlayerCameraManager* GetSimPlayerCamera( );

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

	void  ChangeFormationType( const FString& strName, const EBoatFormation eforamtion );
	/* is leader*/
	bool IsLeader( const EntityHandleId eID );

	/* update leader'position && direction*/
	void UpdateLeader( const EntityHandleId eID, FVector& posRef  );

	/* get leader by group name*/
	EntityHandleId GetGroupLeader( const FString& strGroup );


	/**
	 * Retrieves the Objects streams that are available for object list. May execute asynchronously.
	 *
	 * @param Delegate A delegate that will be executed if b ound when the list of streams is available
	 */
	//virtual void EnumerateObjectsStreams( const FNetworkReplayVersion& ReplayVersion, const FString& UserString, const FString& MetaString, const FEnumerateStreamsCallback& Delegate ) = 0;

	void EnumerateObjectsStreams( const FEnumerateObjectsStreamsCallback& Delegate );
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

	AActor* m_oceanActor;

};
