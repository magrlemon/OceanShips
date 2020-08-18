// Fill out your copyright notice in the Description page of Project Settings.


#include "Sim_OceanSceneManager.h"
#include "Engine/Engine.h"
#include "SimEcs_ContextDataEnvironment.h"
#include "SimECS_BattleWorld.h"
#include "Kismet/GameplayStatics.h"
#include "SimEcs_BaseComponents.h"
 //USimOceanSceneManager_Singleton* USimOceanSceneManager_Singleton::gSingletonScene = nullptr;

USimOceanSceneManager_Singleton::USimOceanSceneManager_Singleton( const FObjectInitializer& ObjectInitializer )
	: Super( ObjectInitializer )
{
	//m_gpsDataTransfer.InitData( );
}

void USimOceanSceneManager_Singleton::Initialize( ) {
	MakeRoot( );
	m_PlayerCameraManager = Cast<ASimEcs_PlayerCameraManager>( GetWorld( )->GetFirstPlayerController( )->PlayerCameraManager);
}

void USimOceanSceneManager_Singleton::GenOceanScenarioActor( ) {
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif
	ReSpawnSystemLink( );
	InitialzieScenarioData( );
}

void USimOceanSceneManager_Singleton::GenOceanScenarioBarrier( ) {
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif
	ReSpawnSystemLink( );
	InitialzieOceanDefScenarioData( );
}

void USimOceanSceneManager_Singleton::MakeRoot( ) {

	m_ScenarioMeshTransform.SetRotation( FQuat::MakeFromEuler( FVector( 0.0f, 0.0f, 90.0f ) ) );
	m_ScenarioMeshTransform.SetLocation( m_vecScenarioMesh );

	/* Barrier Area Region A */
	m_ScenarioBarrierMeshTransform.SetRotation( FQuat::MakeFromEuler( FVector( 0.0f, 0.0f, 180.0f ) ) );
	m_ScenarioBarrierMeshTransform.SetLocation( m_vecScenarioBarrierMesh );
}


//get name
using EntityHandleId = uint64_t;
EntityHandleId USimOceanSceneManager_Singleton::GetSimHandleIDWithName( const FString& strName ) {
	for (auto itme : m_MapArchetypesName) {
		if (itme.Value.Compare( *strName ) == 0) {
			return itme.Key;
		}
	}
	return EntityHandleId( 0 );
}

//get tag
TSharedPtr<AActor> USimOceanSceneManager_Singleton::GetSimActorWithTag( const FString& strTag ) {
	if (!m_ScenarioSceneTap.IsValid( )) {
		UWorld* World = GEngine->GameViewport->GetWorld( );
		if (World == nullptr)
			return nullptr;
		TArray<AActor*> ScenarioActors;
		UGameplayStatics::GetAllActorsWithTag( World, *strTag, ScenarioActors );
		for (AActor* actor : ScenarioActors) {
			return MakeShareable( actor );
		}
	}
	return nullptr;
}


ASimEcs_PlayerCameraManager* USimOceanSceneManager_Singleton::GetSimPlayerCamera( ) {
	return nullptr;
}

AActor* USimOceanSceneManager_Singleton::GetOceanActor( ) {
	if (!m_oceanActor)
		m_oceanActor = GetSimActorWithTag( "sim_ocean" ).Get( );
	return   m_oceanActor;
}

//get leader position
FVector USimOceanSceneManager_Singleton::GetSimLeaderPosition( const FString& strGroup ) {

	return FVector( 0.0f, 0.0f, 0.0f );
}

void USimOceanSceneManager_Singleton::InitialzieScenarioData( ) {
	if (m_bLoadScenario) return;
	UWorld* World = GEngine->GameViewport->GetWorld( );
	if (World == nullptr)
		return;
	auto rsl = GetComponentSysLinkPtr( );
	if (rsl) {
		rsl->GenSimSceneObjects( );
		m_bLoadScenario = true;
		UE_LOG( LogTemp, Warning, TEXT( "m_bLoadScenario = true;" ) );
	}
}

// Ocean Sea
/* initialize Scenario xml data */
void USimOceanSceneManager_Singleton::InitialzieOceanDefScenarioData( ) {
	if (m_bLoadBarrier) return;
	UWorld* World = GEngine->GameViewport->GetWorld( );
	if (World == nullptr)
		return;
	auto rsl = GetComponentSysLinkPtr( );
	if (rsl) {
		rsl->GenSimOceanDefSceneObjects( );
		m_bLoadBarrier = true;
		UE_LOG( LogTemp, Warning, TEXT( "m_bLoadScenario = true;" ) );
	}
}


USimEcs_ComponentSystemLink* USimOceanSceneManager_Singleton::GetComponentSysLinkPtr( ) {
	return  m_ComponentSL_Ptr;
}


TWeakPtr<SimEcs_Engine> USimOceanSceneManager_Singleton::GetSimEcsEngineWeakPtr( ) {
	return  m_ComponentSL_Ptr->GetEntityFrameData( ).Pin( )->GetEcsEnginePtr( );
}


void USimOceanSceneManager_Singleton::ReSpawnSystemLink( ) {
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif
	if (!m_ComponentSL_Ptr && !m_bLoadSuccess) {
		UWorld* World = GEngine->GameViewport->GetWorld( );
		if (World == nullptr)
			return;
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsWithTag( World, TEXT( "SimOceanBattle" ), Actors );
		for (AActor* actor : Actors) {
			TArray<UActorComponent*> SimSystemLinkComponents = actor->GetComponentsByClass( UActorComponent::StaticClass( ) );
			for (UActorComponent* systemLinkComponent : SimSystemLinkComponents) {
				if (systemLinkComponent->GetName( ).Compare( "SimEcs_ComponentSystemLink" ) != 0)
					continue;
				m_ComponentSL_Ptr = (Cast<USimEcs_ComponentSystemLink>( systemLinkComponent ));
			}
		}
		MakeRoot( );
		m_bLoadSuccess = true;
	}
}

/*get arcthtype by entity*/
TSharedPtr<ASimEcs_Archetype> USimOceanSceneManager_Singleton::FindArchetype( EntityHandleId handleID ) {
	if (m_MapArchetypes.Find( handleID ))
		return m_MapArchetypes[handleID];
	return nullptr;
}

SimEcs_Registry * USimOceanSceneManager_Singleton::GetSimRegistry( )
{
	if (!m_ComponentSL_Ptr || !m_ComponentSL_Ptr->m_WorldActorPtr.IsValid( ))
		return nullptr;
	return  m_ComponentSL_Ptr->m_WorldActorPtr->ECSWorld->GetRegistry( );
}


void USimOceanSceneManager_Singleton::DebugLogger( FString strMode ) {
	GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, strMode );
}

// is leader by eID
bool USimOceanSceneManager_Singleton::IsLeader( const EntityHandleId eID ) {
	for (auto item : m_MapLeaderArchetypes) {
		return (item.Value == eID ? true : false);
	}
	return false;
}

void USimOceanSceneManager_Singleton::UpdateLeader(const EntityHandleId eID ,FVector& posRef) {
	const FString* grouName = m_MapLeaderArchetypes.FindKey( eID );
	if ((*grouName).IsEmpty( ))return;
	if (eID <= 0)return;

	FVector location = m_MapArchetypes[eID]->GetTransform( ).GetLocation( );
	FVector transPos  = m_MapArchetypes[eID]->GetTransform( ).GetTranslation( );
	FVector leaderPosition = m_MapArchetypes[eID]->GetTransform( ).GetTranslation( );
	TMapFormation& mapFormationRef = m_TTMapBoatFormationInfo[*grouName];
	mapFormationRef[eID].BoatTargetPosition = posRef;
	mapFormationRef[eID].ForwardVector = mapFormationRef[eID].BoatTargetPosition - leaderPosition;
	mapFormationRef[eID].ForwardVector.Z = 0.0f;
	mapFormationRef[eID].ForwardVector.Normalize( );


}


void USimOceanSceneManager_Singleton::Update( float tick_ms ) {

}

void USimOceanSceneManager_Singleton::Terminate( ) {
	m_bLoadSuccess = false;

}

FVector USimOceanSceneManager_Singleton::GetCovertScenePosition( FVector ImagePosition, ESceneRelevantConv  erc ) {
	switch (erc) {
	case E_BARRIER_POINT:
	{
		ConvertPixToWorldPositon( ImagePosition );
		return m_ScenarioBarrierMeshTransform.TransformPosition( ImagePosition );
	}
	case E_SENERAIO_POINT:
	{
		return m_ScenarioMeshTransform.TransformPosition( ImagePosition*100.0f );
	}
	case E_SYSTEM_LINK_POINT:
	default:
	{
		if (m_ComponentSL_Ptr)
			return m_ComponentSL_Ptr->GetOwner( )->GetTransform( ).TransformPosition( ImagePosition );
		break;
	}
	}

	return FVector::ZeroVector;
}

void USimOceanSceneManager_Singleton::ConvertPixToWorldPositon( FVector& fPos ) {
	fPos.X = (fPos.X / 3920)*(40800.0f);
	fPos.Y = (fPos.Y / 1742)*(18600.0f);
}



DVector2 USimOceanSceneManager_Singleton::ConvertWorldPositonToLatLon( FVector fPos ) {
	return m_gpsDataTransfer.GetLatLon( fPos );
}


/*
// the message for ecs system
*/
bool USimOceanSceneManager_Singleton::IsHavSimMessage( ) {
	return m_arrSimMessage.Num( ) > 0 ? true : false;
}

void USimOceanSceneManager_Singleton::PushSimMessage( FString& strMsg ) {
	m_arrSimMessage.Add( strMsg );
}

TArray<FString>& USimOceanSceneManager_Singleton::CopySimMessage( ) {
	return m_arrSimMessage;
}

auto RemoveMsg = [=]( FString& strMsg ) {
	return strMsg.IsEmpty( ) == false;
};

void USimOceanSceneManager_Singleton::RemoveSimMessage( ) {
	m_arrSimMessage.RemoveAll( RemoveMsg );
}

//////////////////////////////////////////////////////////////////////////
/*
//  deal  boat data,replace (boat component )date to ecs system
*/
//////////////////////////////////////////////////////////////////////////
bool  USimOceanSceneManager_Singleton::IsArriving( const FString& strName, const FVector& posRef ) {
	if (strName.IsEmpty( ))return false;
	EntityHandleId ehandleID = GetSimHandleIDWithName( strName );
	if (ehandleID > 0) {
		FVector relativePos = GetCovertScenePosition( posRef, ESceneRelevantConv::E_SENERAIO_POINT );
		auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( ehandleID );
		FVector arcPos = archeType->GetTransform( ).GetLocation( );
		if (FVector::Distance( arcPos, relativePos ) < 300.0f) {
			SetIdle( strName, posRef );
			return true;
		}
	}
	return false;
}

bool  USimOceanSceneManager_Singleton::IsArriving( const EntityHandleId ehandleID, const FVector& posRef ) {

	if (ehandleID > 0) {
		auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( ehandleID );
		FVector arcPos = archeType->GetTransform( ).GetLocation( );
		if (FVector::Distance( arcPos, posRef ) < 300.0f) {
			SetIdle( ehandleID, posRef );
			return true;
		}
	}
	return false;
}

void  USimOceanSceneManager_Singleton::SetIdle( const FString& strName, const FVector& posRef ) {
	if (strName.IsEmpty( ))return;
	EntityHandleId ehandleID = GetSimHandleIDWithName( strName );
	if (ehandleID > 0) {
		GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveMode = BoatMoveMode::EBoatMoveMode_Idle;
		GetSimRegistry( )->get<FOceanShip>( ehandleID ).ExpectSpeed = 0.0f;
	}
}

void  USimOceanSceneManager_Singleton::SetIdle( const EntityHandleId ehandleID, const FVector& posRef ) {

	if (ehandleID > 0) {
		GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveMode = BoatMoveMode::EBoatMoveMode_Idle;
		GetSimRegistry( )->get<FOceanShip>( ehandleID ).ExpectSpeed = 0.0f;
	}
}

void  USimOceanSceneManager_Singleton::MoveEntity( const FString& strName, const FVector& posRef ) {
	if (strName.IsEmpty( ))return;
	EntityHandleId ehandleID = GetSimHandleIDWithName( strName );
	if (ehandleID > 0 && GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveMode != BoatMoveMode::EBoatMoveMode_On) {
		FVector relativePos = GetCovertScenePosition( posRef, ESceneRelevantConv::E_SENERAIO_POINT );
		auto& ship = GetSimRegistry( )->get<FOceanShip>( ehandleID );
		ship.MoveOnPos = relativePos;
		ship.MoveMode = BoatMoveMode::EBoatMoveMode_On;
		ship.ExpectSpeed = 1.0f;
	
		if(ship.MainMeshComponent)
			ship.MainMeshComponent->SetSimulatePhysics( true );

		UpdateLeader( ehandleID , relativePos );

		//FString simMsg = FString::Printf( TEXT( " DroneBoat MoveOn : %s" ), *relativePos.ToString( ) );
		//GEngine->AddOnScreenDebugMessage( -1, 3.f, FColor::Red, *simMsg );
	}
}

void  USimOceanSceneManager_Singleton::MoveEntity( const FString& GroupName, EntityHandleId ehandleID, const FVector& posRef ) {
	if (ehandleID <= 0 || GroupName.IsEmpty())return;
	EntityHandleId leaderID = m_MapLeaderArchetypes[GroupName];
	if (leaderID <= 0) return;
	
	if (GetSimRegistry( )->get<FOceanShip>( leaderID ).MoveMode > BoatMoveMode::EBoatMoveMode_On ) {
		return;
	}

	auto& simRegistryActor = GetSimRegistry( )->get<FOceanShip>( ehandleID );
	GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveOnPos = posRef;
	GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveMode = BoatMoveMode::EBoatMoveMode_On;
	GetSimRegistry( )->get<FOceanShip>( ehandleID ).ExpectSpeed = 1.0f;
	if (GetSimRegistry( )->get<FOceanShip>( ehandleID ).MainMeshComponent) {
		GetSimRegistry( )->get<FOceanShip>( ehandleID ).MainMeshComponent->SetSimulatePhysics( true );
	}


	bool isArrived =IsArriving( ehandleID, posRef );
	if (isArrived)
	{
		FString simMsg = FString::Printf( TEXT( "Actor's  : %s have arrived the positon " ), *(m_MapArchetypesName[ehandleID].ToString()));
		USimOceanSceneManager_Singleton::GetInstance( )->PushSimMessage( simMsg );
	}
}

void  USimOceanSceneManager_Singleton::MoveBackEntity( const FString& strName, const FVector& posRef ) {
	if (strName.IsEmpty( ))return;
	EntityHandleId ehandleID = GetSimHandleIDWithName( strName );
	if (ehandleID > 0 && GetSimRegistry( )->get<FOceanShip>( ehandleID ).MoveMode != BoatMoveMode::EBoatMoveMode_Back) {
		FVector relativePos = GetCovertScenePosition( posRef, ESceneRelevantConv::E_SENERAIO_POINT );
		auto& ship = GetSimRegistry( )->get<FOceanShip>( ehandleID );
		ship.MoveOnPos = relativePos;
		ship.MoveMode = BoatMoveMode::EBoatMoveMode_Back;
		ship.ExpectSpeed = 1.0f;

		FString simMsg = FString::Printf( TEXT( " DroneBoat MoveBack : %s" ), *relativePos.ToString( ) );
		GEngine->AddOnScreenDebugMessage( -1, 3.f, FColor::Red, *simMsg );
	}
}

void USimOceanSceneManager_Singleton::Firing( const FString& strName, const bool bFire ) {
	if (strName.IsEmpty( ))return;
	EntityHandleId ehandleID = GetSimHandleIDWithName( strName );
	if (ehandleID > 0) {
		auto& ship = GetSimRegistry( )->get<FOceanShip>( ehandleID );
		ship.MoveMode = EBoatMoveMode_Fire;

		GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "boat->Get( )->StartFire( );" );
	}
}

USimOceanSceneManager_Singleton* USimOceanSceneManager_Singleton::gSingletonScene = nullptr;