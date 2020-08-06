#include "SimEcs_BaseComponents.h"
#include "GameFramework/Actor.h"
#include "SimEcs_BattleWorld.h"
#include "EngineUtils.h"
#include "EngineMinimal.h"

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

USimEcs_ComponentSystemLink::USimEcs_ComponentSystemLink( )
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
	m_WorldActorPtr = nullptr;
	m_pASimEntitasFrameData = nullptr;
}


void USimEcs_ComponentSystemLink::BeginPlay( )
{
	Super::BeginPlay( );
#if WITH_EDITOR
	if (GIsEditor == true) {
		return;
	}
#endif

	UWorld* World = GEngine->GetWorldFromContextObject( GetOwner( ), EGetWorldErrorMode::Assert );

	// We do nothing if no is class provided, rather than giving ALL actors!

	if (World) {

		for (TActorIterator<ASimEcs_WorldActor> It( World, ASimEcs_WorldActor::StaticClass( ) ); It; ++It) {
			if (IsValid( *It )) {
				m_WorldActorPtr = MakeShareable( *It );
				break;
			}
		}

		if (!m_WorldActorPtr.IsValid( )) {
			m_WorldActorPtr = MakeShareable( World->SpawnActor<ASimEcs_WorldActor>( ASimEcs_WorldActor::StaticClass( ) ) );
		}
		if (m_WorldActorPtr.IsValid( )) {
			myEntity = m_WorldActorPtr.Get( )->ECSWorld.Get( )->NewEntity( );
			RegisterWithECS( m_WorldActorPtr.Get( )->ECSWorld.Get( ), myEntity );
		}

		////////////////////////////////////////////////////////////////////////
		//* initialize m_pASimEntitasFrameData
		if (!m_pASimEntitasFrameData.IsValid( )) {
			TSubclassOf<AActor> ActorClass = nullptr;

			FString ActorClassName = FString( "ASimEcs_EntitasFrameData" );

			// Attempt to use the fully qualified path first.  If not use the expensive slow path.
			if (true/*ActorClass = LoadClass<ASimEcs_EntitasFrameData>( nullptr, *ActorClassName)*/) {
				m_pASimEntitasFrameData = TSharedPtr<ASimEcs_EntitasFrameData>( World->SpawnActor<ASimEcs_EntitasFrameData>( ASimEcs_EntitasFrameData::StaticClass( ) ) );
				m_initSuccess = true;
			}
			UE_LOG( LogFlying, Warning, TEXT( "TSharedPtr<ASimEcs_EntitasFrameData>( World->SpawnActor<ASimEcs_EntitasFrameData>( ASimEcs_EntitasFrameData::StaticClass( ) ) )" ) );
		}
	}
}
void USimEcs_ComponentSystemLink::BeginDestroy( ) 
{
	 m_WorldActorPtr = nullptr;
	 m_pASimEntitasFrameData = nullptr;
 	Super::BeginDestroy( );
}


EntityHandle USimEcs_ComponentSystemLink::GenHandEntity( ) {
	if (m_WorldActorPtr.IsValid( ))
		return m_WorldActorPtr.Get( )->ECSWorld->NewEntity( );

	return EntityHandle( 0 );

}

TWeakPtr<ASimEcs_EntitasFrameData> USimEcs_ComponentSystemLink::GetEntityFrameData( ) {
	if (!m_pASimEntitasFrameData.IsValid( ))
		return nullptr;
	return m_pASimEntitasFrameData;
}  


void USimEcs_ComponentSystemLink::GenSimSceneObjects( ) {
	if (m_pASimEntitasFrameData.IsValid( )) {
		m_pASimEntitasFrameData.Get( )->GenScenarioObjects( );
	}
}

// Generate Ocean Defance Barrior Objects
void USimEcs_ComponentSystemLink::GenSimOceanDefSceneObjects( ) {
	if (m_pASimEntitasFrameData.IsValid( )) {
		m_pASimEntitasFrameData.Get( )->GenCoastDefBarrierObjects( );
	}
} 

void USimEcs_ComponentSystemLink::RegisterWithECS( SimEcs_World* _ECS, EntityHandle entity )
{
	auto reg = _ECS->GetRegistry( );
	auto ent = entity.handle;

	reg->accommodate<FActorReference>( ent, GetOwner( ) );

	if (TransformSync == ETransformSyncType::Actor_To_SimEcs || TransformSync == ETransformSyncType::BothWays) {
		reg->accommodate<FCopyTransformToECS>( ent, CopyToECS );
	}
	if (TransformSync == ETransformSyncType::SimEcs_To_Actor || TransformSync == ETransformSyncType::BothWays) {
		reg->accommodate<FCopyTransformToActor>( ent, CopyToActor );
	}
	//reg->accommodate<FGridMap>( ent );


	//request all other components to add their SimEcs component to this actor
	//for (auto c : GetOwner( )->GetComponents( )) {
	//	auto wr = Cast<IComponentWrapper>( c );
	//	if (wr) {
	//		wr->AddToEntity( entity.handle );
	//	}
	//}
}


//////////////////////////////////////////////////////////////////////////
// Position Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_PositionComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
		Value.pos.X = JsonObject->GetIntegerField( "x:" );
		Value.pos.Y = JsonObject->GetIntegerField( "y:" );
		Value.pos.Y = JsonObject->GetIntegerField( "z:" );
	}
}


//////////////////////////////////////////////////////////////////////////
void USimEcs_ForceComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}


//////////////////////////////////////////////////////////////////////////
void USimEcs_DeleterComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}


//////////////////////////////////////////////////////////////////////////
// FRandomArcSpawn Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_FRandomArcSpawnComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}



//////////////////////////////////////////////////////////////////////////
// Scale Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_ScaleComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

//////////////////////////////////////////////////////////////////////////
// Movement Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_MovementComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}

}

//////////////////////////////////////////////////////////////////////////
// BoatsMovement Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_BoatsMovementComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

//////////////////////////////////////////////////////////////////////////
// DebugSphere Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_DebugSphereComponentWrapper::ParseJson( FString& jsonValue ) {

}

//////////////////////////////////////////////////////////////////////////
// InstancedStaticMesh Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_InstancedStaticMeshComponentWrapper::ParseJson( FString& jsonValue ) {

}


//////////////////////////////////////////////////////////////////////////
// Velocity Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_VelocityComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

//////////////////////////////////////////////////////////////////////////
// Rotation Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_RotationComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
		float yaw = JsonObject->GetIntegerField( "heading:" );
		FRotator fRot = FRotator::MakeFromEuler( FVector( 0.0f, 0.0f, yaw ) );
		Value.rot = fRot.Quaternion( );
	}
}


//////////////////////////////////////////////////////////////////////////
// DebugSphere Wrapper
//////////////////////////////////////////////////////////////////////////
void USimEcs_ArchetypeSpawnerComponentWrapper::ParseJson( FString& jsonValue ) {

}




//////////////////////////////////////////////////////////////////////////
void USimEcs_MovementRaycastComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

