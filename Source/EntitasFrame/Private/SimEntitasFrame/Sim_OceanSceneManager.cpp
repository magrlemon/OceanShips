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


//get tag
TSharedPtr<AActor> USimOceanSceneManager_Singleton::GetSimActorWithTag( FString& strTag ) {
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
TSharedPtr<ASimEcs_Archetype> USimOceanSceneManager_Singleton::FindArchetype(EntityHandleId handleID) {
	if (m_MapArchetypes.Find(handleID))
		return m_MapArchetypes[handleID];
	return nullptr;
}

SimEcs_Registry * USimOceanSceneManager_Singleton::GetSimRegistry( )
{
	UE_LOG( LogFlying, Warning, TEXT( "USimOceanSceneManager_Singleton::GetSimRegistry" ) );
	return  m_ComponentSL_Ptr->m_WorldActorPtr->ECSWorld->GetRegistry( );
}


void USimOceanSceneManager_Singleton::DebugLogger( FString strMode ) {
	GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, strMode );
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
		return m_ScenarioMeshTransform.TransformPosition( ImagePosition );
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
	return strMsg.IsEmpty() == false;
};

void USimOceanSceneManager_Singleton::RemoveSimMessage( ) {
	m_arrSimMessage.RemoveAll( RemoveMsg );
}

USimOceanSceneManager_Singleton* USimOceanSceneManager_Singleton::gSingletonScene = nullptr;