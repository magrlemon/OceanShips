///-------------------------------------------------------------------------------------------------
/// File:	src\Engine.cpp.
///
/// Summary:	Implements the engine class.
///-------------------------------------------------------------------------------------------------

#include "SimEcs_BattleComponents.h"
#include "SimEcs_Engine.h"
#include "SimEcs_EntityManager.h"
#include "SimEcs_ComponentManager.h"
#include "Sim_OceanSceneManager.h"

//* FFaction
USimEcs_FactionComponentWrapper::USimEcs_FactionComponentWrapper()
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_FACTION_COMPONENT;
}
	

void USimEcs_FactionComponentWrapper::AddToEntity( u64 uHandleID ,FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FFaction>( uHandleID, Value );
	}
};

void USimEcs_FactionComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize(Reader,JsonObject))
	{
		Value.faction = (EFaction)(JsonObject->GetIntegerField( "Faction:" ));
		Value.parentDevice = *(JsonObject->GetStringField( "ParentDevice:" ));
	}
}

//* FHealth
USimEcs_HealthComponentWrapper::USimEcs_HealthComponentWrapper( )
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_HEALTH_COMPONENT;
}


void USimEcs_HealthComponentWrapper::AddToEntity( u64 uHandleID ,FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FHealth>( uHandleID, Value );
	}
};
void USimEcs_HealthComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
		Value.Health = JsonObject->GetIntegerField( "Health:" );
		
	}
}


//* FOceanShip
USimEcs_OceanShipComponentWrapper::USimEcs_OceanShipComponentWrapper( )
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_NONE_COMPONENT;
}


void USimEcs_OceanShipComponentWrapper::AddToEntity( u64 uHandleID ,FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FOceanShip>( uHandleID, Value );
	}
};

void USimEcs_OceanShipComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
	
		Value.Name = *(JsonObject->GetStringField( "name:" ));
		Value.AvoidanceStrenght = JsonObject->GetIntegerField( "AvoidanceStrenght:" );
		Value.MaxVelocity = JsonObject->GetIntegerField( "MaxVelocity:" );
		Value.BoatMass = JsonObject->GetIntegerField( "BoatMass:" );
		Value.StepSpeed = JsonObject->GetIntegerField( "StepSpeed:" );
		Value.SteeringSpeed = JsonObject->GetIntegerField( "SteeringSpeed:" );
		Value.ForwardSpeed = JsonObject->GetIntegerField( "ForwardSpeed:" );
		Value.TurnStep = JsonObject->GetIntegerField( "TurnStep:" );
		Value.isLeader = JsonObject->GetBoolField( "Leader:" );
		Value.DltLocate = JsonObject->GetBoolField( "DltLocate:" );
		Value.bInitializeParentDevice = Value.DltLocate;  
	}
}


//* FProjectile
USimEcs_ProjectileComponentWrapper::USimEcs_ProjectileComponentWrapper()
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_PROJECTILE_COMPONENT;
}


void USimEcs_ProjectileComponentWrapper::AddToEntity( u64 uHandleID ,FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FProjectile>( uHandleID, Value );
	}
};

void USimEcs_ProjectileComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

//* FExplosion
USimEcs_ExplosionComponentWrapper::USimEcs_ExplosionComponentWrapper(  )
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_EXPLOSION_COMPONENT;
}


void USimEcs_ExplosionComponentWrapper::AddToEntity( u64 uHandleID ,FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FExplosion>( uHandleID, Value );
	}
};

void USimEcs_ExplosionComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {

	}
}

	 // namespace SimEcs



/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Formation             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////
//

//* FExplosion
USimEcs_FormationComponentWrapper::USimEcs_FormationComponentWrapper( )
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_ENT_FORMAION_COMPONENT;
}

void USimEcs_FormationComponentWrapper::AddToEntity( u64 uHandleID, FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FFormation>( uHandleID, Value );
	}
}

void USimEcs_FormationComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
		
		Value.FormationValue = JsonObject->GetIntegerField( "FormationValue:" );
		Value.GroupName = JsonObject->GetStringField( "FormationGroup:" );
		GEngine->AddOnScreenDebugMessage( -1, 3.f, FColor::Red, *Value.GroupName );
	}
}



//
///////////////////////////////////////////////////////////////////////////////
////////////////                                        //////////////////////            
////////////////             Formation             /////////////////////                                 
////////////////                                        ////////////////////              
///////////////////////////////////////////////////////////////////////////
////
//
////* FExplosion
USimEcs_FSMAnimationComponentWrapper::USimEcs_FSMAnimationComponentWrapper( )
{
	PrimaryComponentTick.bCanEverTick = false;
	IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_ENT_DLT_ANIMATION_STATE_COMPONENT;
}

void USimEcs_FSMAnimationComponentWrapper::AddToEntity( u64 uHandleID, FString& jsonValue ) {
	auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
	if (simRegistry) {
		ParseJson( jsonValue );
		simRegistry->accommodate<FFSMAnimation>( uHandleID, Value );
	}
}

void USimEcs_FSMAnimationComponentWrapper::ParseJson( FString& jsonValue ) {
	if (jsonValue.IsEmpty( ))
		return;
	TSharedPtr<FJsonObject> JsonObject;
	const TSharedRef< TJsonReader<> >& Reader = TJsonReaderFactory<>::Create( jsonValue );
	if (FJsonSerializer::Deserialize( Reader, JsonObject )) {
		Value.Time = JsonObject->GetIntegerField( "Time:" );
		Value.bFsmAnim = JsonObject->GetBoolField( "FsmAnim:" );  
	}
}

