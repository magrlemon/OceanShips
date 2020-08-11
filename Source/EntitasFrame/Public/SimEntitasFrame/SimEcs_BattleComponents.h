#pragma once
#include "SimEcs_ComponentManager.h"
#include "SimEcs_BattleComponentsStructData.h"
#include "SimEcs_ComponentWrapper.h"
#include "SimEcs_BattleWorld.h"
#include "SimEcs_Core.h"
#include "SimEcs_BattleComponents.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDamagedDelegate,float, Damage);

UCLASS(ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent))
class ENTITASFRAME_API USimEcs_FactionComponentWrapper : public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()
public:
	USimEcs_FactionComponentWrapper( );
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY(EditAnywhere, Category = SimEcs)
	FFaction Value;
};

UCLASS(ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent))
class ENTITASFRAME_API USimEcs_HealthComponentWrapper : public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_HealthComponentWrapper( );
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY(BlueprintAssignable, Category = "SimEcs")
		FDamagedDelegate OnDamaged;

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FHealth Value;
};

UCLASS(ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent))
class ENTITASFRAME_API USimEcs_OceanShipComponentWrapper : public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_OceanShipComponentWrapper( );
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY(EditAnywhere, Category = SimEcs)
		FOceanShip Value;
};
UCLASS(ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent))
class ENTITASFRAME_API USimEcs_ProjectileComponentWrapper : public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_ProjectileComponentWrapper( );

	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY(EditAnywhere, Category = SimEcs)
		FProjectile Value;
};

UCLASS(ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent))
class ENTITASFRAME_API USimEcs_ExplosionComponentWrapper: public UActorComponent, public IComponentWrapper
{
	GENERATED_BODY()
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_ExplosionComponentWrapper( );
	virtual void AddToEntity( u64 uHandleID ,FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY(EditAnywhere, Category = SimEcs)
		FExplosion Value;
};


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Formation             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////
//
//UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
//class ENTITASFRAME_API USimEcs_FormationComponentWrapper : public UActorComponent, public IComponentWrapper {
//	GENERATED_BODY( )
//protected:
//	// Called when the Entity Generate successful
//	virtual void BeginPlay( ) {};
//	virtual void BeginDestory( ) {};
//public:
//	USimEcs_FormationComponentWrapper( );
//	virtual void AddToEntity( u64 uHandleID, FString& jsonValue );
//	void ParseJson( FString& jsonValue );
//	UPROPERTY( EditAnywhere, Category = SimEcs )
//		FFormation Value;
//};



UCLASS( ClassGroup = (SimEcs), meta = (BlueprintSpawnableComponent) )
class ENTITASFRAME_API USimEcs_FormationComponentWrapper : public UActorComponent, public IComponentWrapper {
	GENERATED_BODY( )
protected:
	// Called when the Entity Generate successful
	virtual void BeginPlay( ) {};
	virtual void BeginDestory( ) {};
public:
	USimEcs_FormationComponentWrapper( );
	virtual void AddToEntity( u64 uHandleID, FString& jsonValue );
	void ParseJson( FString& jsonValue );
	UPROPERTY( EditAnywhere, Category = SimEcs )
		FFormation Value;
};