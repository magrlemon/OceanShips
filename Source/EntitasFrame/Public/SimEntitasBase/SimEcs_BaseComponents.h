#pragma once
#include "CoreMinimal.h"
#include "SimEcs_Core.h"
#include "SimEcs_BattleComponentsStructData.h"
#include "SimEcs_ComponentWrapper.h"
#include "SimEcs_EntitasFrameData.h"
#include "SimEcs_BaseComponents.generated.h"

UENUM( BlueprintType )
enum class ETransformSyncType : uint8 {
	SimEcs_To_Actor,
	Actor_To_SimEcs,
	BothWays,
	Disabled

};

class ASimEcs_WorldActor;
class ASimEcs_EntitasFrameData;


// Syncs the actor with the SimEcs subsystem. You need this component if you want the others to do anything
UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_ComponentSystemLink : public UActorComponent {
	GENERATED_BODY( )

public:

	USimEcs_ComponentSystemLink( );

public:
	virtual void BeginPlay( ) override;
	virtual void BeginDestroy( ) override;
	virtual void RegisterWithECS( SimEcs_World* _ECS, EntityHandle entity );

	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		ETransformSyncType TransformSync;

	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FCopyTransformToActor CopyToActor;

	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FCopyTransformToECS CopyToECS;

public:
	TWeakPtr<ASimEcs_EntitasFrameData> GetEntityFrameData( );

	void GenSimSceneObjects( );
	void GenSimOceanDefSceneObjects( );

	EntityHandle GenHandEntity( );
public:
	EntityHandle myEntity;
	TSharedPtr<ASimEcs_WorldActor> m_WorldActorPtr = nullptr;
	TSharedPtr<ASimEcs_EntitasFrameData> m_pASimEntitasFrameData ;

private:
	bool m_initSuccess = false;
};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_PositionComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
public:
	USimEcs_PositionComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_POSITION_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FPosition>( uHandleID, Value );
	};

	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FPosition Value;
};



UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_ForceComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
public:
	USimEcs_ForceComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_ENT_FORCE_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FEntForce>( uHandleID, Value );
	};

	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FEntForce Value;
};


UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_DeleterComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )

protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_DeleterComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_DELETER_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FLifetime>( uHandleID, Value );
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FLifetime Value;
};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_FRandomArcSpawnComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};

public:
	USimEcs_FRandomArcSpawnComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_RANDOMARCSPAWN_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FRandomArcSpawn>( uHandleID, Value );
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FRandomArcSpawn Value;
};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_ScaleComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_ScaleComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_SCALE_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FScale>( uHandleID, Value );
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FScale Value;
};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_MovementComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_MovementComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_MOVEMENT_COMPONENT;
	};
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry)
			simRegistry->accommodate<FMovement>( uHandleID );
	};
	void ParseJson( FString& jsonValue );
};

//////////////////////////////////////////////////////////////////////////

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_BoatsMovementComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_BoatsMovementComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_BOATS_MOVEMENT_COMPONENT;
	};
	virtual void AddToEntity( u64 uHandleID, FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FMovement>( uHandleID );
		}
	};
	void ParseJson( FString& jsonValue );
};

//////////////////////////////////////////////////////////////////////////

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_DebugSphereComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_DebugSphereComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_DEBUGSPHERE_COMPONENT;
	}
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FDebugSphere>( uHandleID, Value );
		}
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FDebugSphere Value;
};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_InstancedStaticMeshComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_InstancedStaticMeshComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_INSTANCEDSTATICMESH_COMPONENT;
	};
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FInstancedStaticMesh>( uHandleID, Value );
		}
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FInstancedStaticMesh Value;

};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_VelocityComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_VelocityComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_VELOCITY_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FVelocity>( uHandleID, Value );
		}
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FVelocity Value;

};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_RotationComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:

	USimEcs_RotationComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_ROTATION_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FRotationComponent>( uHandleID, Value );
		}
			
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FRotationComponent Value;

};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_ArchetypeSpawnerComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_ArchetypeSpawnerComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_ARCHETYPESPAWNER_COMPONENT;
	};
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FArchetypeSpawner>( uHandleID, Value );
		}
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FArchetypeSpawner Value;

};

UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_MovementRaycastComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_MovementRaycastComponentWrapper( ) {
		PrimaryComponentTick.bCanEverTick = false;
		IComponentWrapper::STATIC_COMPONENT_TYPE_ID = EComponentClass::ECC_MOVEMENTRAYCAST_COMPONENT;
	};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue ) {
		auto simRegistry = USimOceanSceneManager_Singleton::GetInstance( )->GetSimRegistry( );
		if (simRegistry) {
			ParseJson( jsonValue );
			simRegistry->accommodate<FMovementRaycast>( uHandleID, Value );
		}
	};
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		FMovementRaycast Value;

};