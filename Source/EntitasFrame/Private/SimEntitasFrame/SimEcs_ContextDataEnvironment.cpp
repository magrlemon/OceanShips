// Fill out your copyright notice in the Description page of Project Settings.


#include "SimEcs_ContextDataEnvironment.h"
#include "SimEcs_Entity.h"
#include "SimEcs_EntityManager.h"
#include "Dom/JsonObject.h"

#include "SimEcs_BaseComponents.h"
//
//static FactoryCreator<typename ASimEnvironment, AssetEnvironment> AssetEnvironment_Creator(static_cast<int>(1));


ASimEnvironment::ASimEnvironment( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer ) {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickEnabled( true );
}


void ASimEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

};

// Sets default values for this component's properties
ASimContextDataTankEnvironment::ASimContextDataTankEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts

void ASimContextDataTankEnvironment::BeginPlay( ) {

	// ...

}


void ASimContextDataTankEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSTankPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
#define  _GEN_COMPONENT_(TYPE)\
		//pEnt->AddComponent<typename Sim##TYPE##Component, int32>( std::move( entType ) );\

		_GEN_COMPONENT_( Asset )
			_GEN_COMPONENT_( Transform )
			_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Destroyed )
			_GEN_COMPONENT_( Interactive )
			_GEN_COMPONENT_( Shell )
			_GEN_COMPONENT_( Fired )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( Hit )

#undef  _GEN_COMPONENT_
	}
}


void ASimContextDataTankEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties

ASimContextDataCharactorEnvironment::ASimContextDataCharactorEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts

void ASimContextDataCharactorEnvironment::BeginPlay( ) {

	// ...
}


void ASimContextDataCharactorEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCharactorPropertyData>, int32, FVector>(
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
#define  _GEN_COMPONENT_(TYPE)\
		//pEnt->AddComponent<typename Sim##TYPE##Component, int32>( std::move( entType ) );\

		_GEN_COMPONENT_( Asset )
			_GEN_COMPONENT_( Transform )
			_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Destroyed )
			_GEN_COMPONENT_( Interactive )
			_GEN_COMPONENT_( Fired )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( Hit )

#undef  _GEN_COMPONENT_
	}

}


void ASimContextDataCharactorEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
		}
	}
}
//////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataHelicopterEnvironment::ASimContextDataHelicopterEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataHelicopterEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataHelicopterEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	//GetEcsEnginePtr( )->CreateEntity<SimEcs_Entity>( );
}



void ASimContextDataHelicopterEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataAirCraftEnvironment::ASimContextDataAirCraftEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataAirCraftEnvironment::BeginPlay( ) {

	// ...
}


void ASimContextDataAirCraftEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	//GetEcsEnginePtr( )->CreateEntity<SimEcs_Entity>( );
}


void ASimContextDataAirCraftEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties
ASimContextDataDroneBoatEnvironment::ASimContextDataDroneBoatEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataDroneBoatEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataDroneBoatEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSDroneBoatPropertyData>, int32, FVector>( 
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create DroneBoat Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId(),m_JsonValue );\

		_GEN_COMPONENT_( OceanShip )
			_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Force )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( Velocity )
			_GEN_COMPONENT_( MovementRaycast )
			_GEN_COMPONENT_( Faction )
			_GEN_COMPONENT_( Projectile )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataDroneBoatEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "number:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}// 
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}//BoatMass 
		else if (key.Key.Compare( TEXT( "质量" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "BoatMass:", *(key.Value) );
		}//StepSpeed 
		else if (key.Key.Compare( TEXT( "转速" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "StepSpeed:", *(key.Value) );
		}//SteeringSpeed 
		else if (key.Key.Compare( TEXT( "转向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "SteeringSpeed:", *(key.Value) );
		}//ForwardSpeed 
		else if (key.Key.Compare( TEXT( "前向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "ForwardSpeed:", *(key.Value) );
		}//MaxVelocity;
		else if (key.Key.Compare( TEXT( "最大速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "MaxVelocity:", *(key.Value) );
		}//AvoidanceStrenght;
		else if (key.Key.Compare( TEXT( "回避距离" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "AvoidanceStrenght:", *(key.Value) );
		}//TurnStep 
		else if (key.Key.Compare( TEXT( "旋转步率" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}
//////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties
ASimContextDataNavalCraftEnvironment::ASimContextDataNavalCraftEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataNavalCraftEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataNavalCraftEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSDroneBoatPropertyData>, int32, FVector>( 
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create NavalCraft Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId(),m_JsonValue );\

		_GEN_COMPONENT_( OceanShip )
			_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Force )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( Faction )
			_GEN_COMPONENT_( Projectile )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}






void ASimContextDataNavalCraftEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "number:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}// 
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}//BoatMass 
		else if (key.Key.Compare( TEXT( "质量" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "BoatMass:", *(key.Value) );
		}//StepSpeed 
		else if (key.Key.Compare( TEXT( "转速" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "StepSpeed:", *(key.Value) );
		}//SteeringSpeed 
		else if (key.Key.Compare( TEXT( "转向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "SteeringSpeed:", *(key.Value) );
		}//ForwardSpeed 
		else if (key.Key.Compare( TEXT( "前向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "ForwardSpeed:", *(key.Value) );
		}//MaxVelocity;
		else if (key.Key.Compare( TEXT( "最大速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "MaxVelocity:", *(key.Value) );
		}//AvoidanceStrenght;
		else if (key.Key.Compare( TEXT( "回避距离" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "AvoidanceStrenght:", *(key.Value) );
		}//TurnStep 
		else if (key.Key.Compare( TEXT( "旋转步率" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////

// Sets default values for this component's properties

ASimContextDataVehicle4WEnvironment::ASimContextDataVehicle4WEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataVehicle4WEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataVehicle4WEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {

}


void ASimContextDataVehicle4WEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}




//////////////////////////////////////////////////////////////////////////
// 巡逻艇
// Sets default values for this component's properties
ASimContextDataPatrolBoatEnvironment::ASimContextDataPatrolBoatEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataPatrolBoatEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataPatrolBoatEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;
		GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "CreateEntity  PatrolBoat!!!" );
		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSPatrolBoatPropertyData>, int32, FVector>( 
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		return;
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Force )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( MovementRaycast )
			_GEN_COMPONENT_( Faction )
			_GEN_COMPONENT_( Projectile )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}

}


void ASimContextDataPatrolBoatEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );
	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "number:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			//m_STPropData.entName = (key.Value);
			Writer->WriteValue( "name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			//m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			//m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}// 
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}//BoatMass 
		else if (key.Key.Compare( TEXT( "质量" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "BoatMass:", *(key.Value) );
		}//StepSpeed 
		else if (key.Key.Compare( TEXT( "转速" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "StepSpeed:", *(key.Value) );
		}//SteeringSpeed 
		else if (key.Key.Compare( TEXT( "转向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "SteeringSpeed:", *(key.Value) );
		}//ForwardSpeed 
		else if (key.Key.Compare( TEXT( "前向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "ForwardSpeed:", *(key.Value) );
		}//MaxVelocity;
		else if (key.Key.Compare( TEXT( "最大速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "MaxVelocity:", *(key.Value) );
		}//AvoidanceStrenght;
		else if (key.Key.Compare( TEXT( "回避距离" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "AvoidanceStrenght:", *(key.Value) );
		}//TurnStep 
		else if (key.Key.Compare( TEXT( "旋转步率" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 巡逻艇
// Sets default values for this component's properties
ASimContextDataTrafficBoatEnvironment::ASimContextDataTrafficBoatEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataTrafficBoatEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataTrafficBoatEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSTrafficBoatPropertyData>, int32, FVector>( 
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		return;
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Force )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( MovementRaycast )
			_GEN_COMPONENT_( Faction )
			_GEN_COMPONENT_( Projectile )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}

}


void ASimContextDataTrafficBoatEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );
	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "number:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			//m_STPropData.entName = (key.Value);
			Writer->WriteValue( "name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			//m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			//m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}// 
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}//BoatMass 
		else if (key.Key.Compare( TEXT( "质量" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "BoatMass:", *(key.Value) );
		}//StepSpeed 
		else if (key.Key.Compare( TEXT( "转速" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "StepSpeed:", *(key.Value) );
		}//SteeringSpeed 
		else if (key.Key.Compare( TEXT( "转向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "SteeringSpeed:", *(key.Value) );
		}//ForwardSpeed 
		else if (key.Key.Compare( TEXT( "前向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "ForwardSpeed:", *(key.Value) );
		}//MaxVelocity;
		else if (key.Key.Compare( TEXT( "最大速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "MaxVelocity:", *(key.Value) );
		}//AvoidanceStrenght;
		else if (key.Key.Compare( TEXT( "回避距离" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "AvoidanceStrenght:", *(key.Value) );
		}//TurnStep 
		else if (key.Key.Compare( TEXT( "旋转步率" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
// 巡逻艇
// Sets default values for this component's properties
ASimContextDataEngineeringBoatEnvironment::ASimContextDataEngineeringBoatEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}


// Called when the game starts

void ASimContextDataEngineeringBoatEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataEngineeringBoatEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSEngineeringBoatPropertyData>, int32, FVector>(
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		return;
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Movement )
			_GEN_COMPONENT_( Force )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( MovementRaycast )
			_GEN_COMPONENT_( Faction )
			_GEN_COMPONENT_( Projectile )
			_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataEngineeringBoatEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "number:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			//m_STPropData.entName = (key.Value);
			Writer->WriteValue( "name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			//m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			//m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}// 
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}//BoatMass 
		else if (key.Key.Compare( TEXT( "质量" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "BoatMass:", *(key.Value) );
		}//StepSpeed 
		else if (key.Key.Compare( TEXT( "转速" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "StepSpeed:", *(key.Value) );
		}//SteeringSpeed 
		else if (key.Key.Compare( TEXT( "转向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "SteeringSpeed:", *(key.Value) );
		}//ForwardSpeed 
		else if (key.Key.Compare( TEXT( "前向速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "ForwardSpeed:", *(key.Value) );
		}//MaxVelocity;
		else if (key.Key.Compare( TEXT( "最大速度" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "MaxVelocity:", *(key.Value) );
		}//AvoidanceStrenght;
		else if (key.Key.Compare( TEXT( "回避距离" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "AvoidanceStrenght:", *(key.Value) );
		}//TurnStep 
		else if (key.Key.Compare( TEXT( "旋转步率" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "TurnStep:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//海立石//

// Sets default values for this component's properties
ASimContextDataGTZEnvironment::ASimContextDataGTZEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	// ...
}

// Called when the game starts

void ASimContextDataGTZEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataGTZEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefGTZPropertyData>, int32, FVector>( 
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create GTZ Actor's Name is : %s" ), *m_STPropData.entName   );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataGTZEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "方位" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteValue( "distance:", 10000 );
	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////
// 轨条砦
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataHLSEnvironment::ASimContextDataHLSEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}
// Called when the game starts

void ASimContextDataHLSEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataHLSEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefHLSPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create HLS Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataHLSEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );
	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////
/// 三角锥

// Sets default values for this component's properties

ASimContextDataSJZEnvironment::ASimContextDataSJZEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}

// Called when the game starts
void ASimContextDataSJZEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataSJZEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType <= 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>(
			std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create SJZ Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataSJZEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}


//////////////////////////////////////////////////////////////////////////
//钢刺猬//
//////////////////////////////////////////////////////////////////////////
// Sets default values for this component's properties
ASimContextDataGCWEnvironment::ASimContextDataGCWEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
}

// Called when the game starts

void ASimContextDataGCWEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataGCWEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefGTZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create GCW Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataGCWEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}

	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////
// 蛇腹铁丝网	 1005
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataTSWSFEnvironment::ASimContextDataTSWSFEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}
// Called when the game starts

void ASimContextDataTSWSFEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataTSWSFEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefHLSPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create TSWSF Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataTSWSFEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}

//////////////////////////////////////////////////////////////////////////
// 一列铁丝网	 1006
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataTSWYLEnvironment::ASimContextDataTSWYLEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.

	// ...
}

// Called when the game starts
void ASimContextDataTSWYLEnvironment::BeginPlay( ) {
	// ...

}


void ASimContextDataTSWYLEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );
		if (pEnt) {
			FString simMsg = FString::Printf( TEXT( "Create TSWYL Actor's Name is : %s" ), *m_STPropData.entName );
			USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
		}
#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataTSWYLEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}


//////////////////////////////////////////////////////////////////////////
// 组隔墙	 1007
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataZGQEnvironment::ASimContextDataZGQEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts
void ASimContextDataZGQEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataZGQEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ) );
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );

#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataZGQEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}



//////////////////////////////////////////////////////////////////////////
// 混合雷场		 1008
// Sets default values for this component's properties
//////////////////////////////////////////////////////////////////////////

ASimContextDataHHLCEnvironment::ASimContextDataHHLCEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts
void ASimContextDataHHLCEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataHHLCEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ));
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );

#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataHHLCEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}



//////////////////////////////////////////////////////////////////////////
// 玻璃刀山		 1009
// Sets default values for this component's properties

ASimContextDataBLDSEnvironment::ASimContextDataBLDSEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts
void ASimContextDataBLDSEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataBLDSEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ));
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );

#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataBLDSEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}



//////////////////////////////////////////////////////////////////////////
// 水迹轨条砦	 1010
// Sets default values for this component's properties

ASimContextDataGTZSUIJIEnvironment::ASimContextDataGTZSUIJIEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts
void ASimContextDataGTZSUIJIEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataGTZSUIJIEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ));
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );

#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataGTZSUIJIEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}
	Writer->WriteObjectEnd( );
	Writer->Close( );
}


//////////////////////////////////////////////////////////////////////////
// 水迹轨条砦	 1010
// Sets default values for this component's properties

ASimContextDataZJHEnvironment::ASimContextDataZJHEnvironment( const FObjectInitializer& ObjectInitializer )
	: ASimEnvironment( ObjectInitializer ) {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.


	// ...
}

// Called when the game starts
void ASimContextDataZJHEnvironment::BeginPlay( ) {
	// ...
}


void ASimContextDataZJHEnvironment::CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {
	if (pSimEcs_Engine.IsValid( )) {
		int32 entType = GetEntityType( );
		if (entType < 0)
			return;

		SimEntityId entID = pSimEcs_Engine.Pin( )->GetEntityManager( )->CreateEntity<SimEcs_Entity<FSCoastDefSJZPropertyData>, int32, FVector>( std::move( entType ), std::move( m_STPropData.entPos ), std::move( m_STPropData.entDirection ));
		SimEcs_IEntity* pEnt = pSimEcs_Engine.Pin( )->GetEntityManager( )->GetEntity( entID );

#define  _GEN_COMPONENT_(TYPE)\
	IComponentWrapper* p##TYPE##Component = pEnt->AddComponent<typename USimEcs_##TYPE##ComponentWrapper>( pEnt->GetEntityHandleId() );\

		_GEN_COMPONENT_( Health )
			_GEN_COMPONENT_(BarrierFixedRaycast)
			_GEN_COMPONENT_( Position )
			_GEN_COMPONENT_( Rotation )
			_GEN_COMPONENT_( DebugSphere )

#undef  _GEN_COMPONENT_	
	}
}


void ASimContextDataZJHEnvironment::SerializeStructure( TMapScenario & refScenarixData ) {

	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create( &m_JsonValue );
	Writer->WriteObjectStart( );

	for (auto key : refScenarixData) {
		if (key.Key.Compare( TEXT( "编号" ) ) == 0) {
			m_STPropData.entID = *(key.Value);
			Writer->WriteValue( "ID:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "名字" ) ) == 0) {
			m_STPropData.entName = (key.Value);
			Writer->WriteValue( "Name:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "类型" ) ) == 0) {
			m_STPropData.entType = FCString::Atoi( *(key.Value) );
			Writer->WriteValue( "Type:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "dir" ) ) == 0) {
			m_STPropData.entDirection = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "heading:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "x" ) ) == 0) {
			m_STPropData.entPos.X = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "x:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "y" ) ) == 0) {
			m_STPropData.entPos.Y = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "y:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "z" ) ) == 0) {
			m_STPropData.entPos.Z = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "z:", *(key.Value) );
		}
		else if (key.Key.Compare( TEXT( "毁伤" ) ) == 0) {
			m_STPropData.heath = FCString::Atof( *(key.Value) );
			Writer->WriteValue( "health:", *(key.Value) );
		}
	}

	Writer->WriteObjectEnd( );
	Writer->Close( );
}