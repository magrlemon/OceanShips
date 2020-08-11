#pragma once

#include "SimEcs_Core.h"
#include "SimEcs_BaseComponents.h"
#include "SimEcs_Archetype.h"
#include "SimEcs_BattleComponents.h"
#include "DrawDebugHelpers.h"
#include "Sim_OceanSceneManager.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "ParallelFor.h"


DECLARE_CYCLE_STAT(TEXT("SimEcs: DebugDraw"), STAT_DebugDraw, STATGROUP_ECS);
using EntityHandleId = uint64_t;
struct DebugDrawSystem :public SystemT {

	const float UpdateRate = 0.1;

	float elapsed = 0;

	void update(SimEcs_Registry &registry, float dt) override
	{
		assert(OwnerActor);
		elapsed -= dt;
		if (elapsed > 0)
		{
			return;
		}

		elapsed = UpdateRate;

		registry.view<FDebugSphere, FPosition>().each([&,dt](auto entity, FDebugSphere & ds, FPosition & pos) {
			SCOPE_CYCLE_COUNTER(STAT_DebugDraw);
			DrawDebugSphere(OwnerActor->GetWorld(),pos.pos,ds.radius,12,ds.color,true,UpdateRate);
		});
	}
};

DECLARE_CYCLE_STAT(TEXT("SimEcs: Movement Update"), STAT_Movement, STATGROUP_ECS);
struct MovementSystem :public SystemT {

	
	void update(SimEcs_Registry &registry, float dt) override
	{		
		SCOPE_CYCLE_COUNTER(STAT_Movement);

		//movement raycast gets a "last position" component
		registry.view<FMovementRaycast,FPosition>().each([&, dt](auto entity,FMovementRaycast & ray, FPosition & pos) {
			registry.accommodate<FLastPosition>(entity, pos.pos);

		});

		//add gravity and basic movement from velocity
		registry.view<FMovement, FPosition, FVelocity>().each([&, dt](auto entity, FMovement & m, FPosition & pos, FVelocity & vel) {

			//gravity
			const FVector gravity = FVector(0.f, 0.f, -980) * m.GravityStrenght;
			vel.Add(gravity*dt);

			pos.Add( vel.vel * dt);			
		});
	}
};

// Bosts Movement System

DECLARE_CYCLE_STAT( TEXT( "SimEcs: Boats Movement Update" ), STAT_Boats_Movement, STATGROUP_ECS );
struct BoatsMovementSystem :public SystemT {


	void update( SimEcs_Registry &registry, float dt ) override
	{
		SCOPE_CYCLE_COUNTER( STAT_Movement );

		//movement raycast gets a "last position" component
		//registry.view<FMovementRaycast, FPosition, FEntForce>( ).each( [&, dt]( auto entity, FMovementRaycast & ray, FPosition & pos, FEntForce & entForce ) {
		//	//registry.accommodate<FLastPosition>( entity, pos.pos );
		//	FString strInfor = TEXT( "FMovementRaycast");
		//	USimOceanSceneManager_Singleton::GetInstance( )->DebugLogger( strInfor );
		//} );

		////add gravity and basic movement from velocity
		registry.view<FMovement, FPosition, FEntForce>( ).each( [&, dt]( auto entity, FMovement & m, FPosition & pos, FEntForce & entForce ) {


		} );
	}
};

DECLARE_CYCLE_STAT(TEXT("SimEcs: Copy Transform To SimEcs"), STAT_CopyTransformECS, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("SimEcs: Unpack Actor Transform"), STAT_UnpackActorTransform, STATGROUP_ECS);
struct CopyTransformToECSSystem :public SystemT {	

	void update(SimEcs_Registry &registry, float dt) override
	{
		assert(OwnerActor);		

		//copy transforms from actor into FActorTransform
		auto ActorTransformView = registry.view<FCopyTransformToECS, FActorReference>();
		for (auto e : ActorTransformView)
		{
			SCOPE_CYCLE_COUNTER(STAT_CopyTransformECS);
			FActorReference & actor = ActorTransformView.get<FActorReference>(e);
			if (actor.ptr.IsValid())
			{
				const FTransform & ActorTransform = actor.ptr->GetActorTransform();
				registry.accommodate<FActorTransform>(e, ActorTransform);
			}
		}		
		{

			SCOPE_CYCLE_COUNTER(STAT_UnpackActorTransform);
			//unpack from ActorTransform into the separate transform components, only if the entity does have that component
			registry.view<FActorTransform, FPosition>().each([&, dt](auto entity, FActorTransform & transform, FPosition & pos) {

			
				pos.pos = transform.transform.GetLocation();
			});
			registry.view<FActorTransform, FRotationComponent>().each([&, dt](auto entity, FActorTransform & transform, FRotationComponent & rot) {

				rot.rot = transform.transform.GetRotation();
			});
			registry.view<FActorTransform, FScale>().each([&, dt](auto entity, FActorTransform & transform, FScale & sc) {

				sc.scale = transform.transform.GetScale3D();
			});
		}
	}
};
DECLARE_CYCLE_STAT(TEXT("SimEcs: Copy Transform To Actor"), STAT_CopyTransformActor, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("SimEcs: Pack actor transform"), STAT_PackActorTransform, STATGROUP_ECS);

struct CopyTransformToActorSystem :public SystemT {

	void update(SimEcs_Registry &registry, float dt) override
	{
		
		assert(OwnerActor);

		{
			SCOPE_CYCLE_COUNTER(STAT_PackActorTransform);
			//fill ActorTransform from separate components		
			registry.view<FActorTransform, FPosition>().each([&, dt](auto entity, FActorTransform & transform, FPosition & pos) {
				transform.transform.SetLocation(pos.pos);
			});
			registry.view<FActorTransform, FRotationComponent>().each([&, dt](auto entity, FActorTransform & transform, FRotationComponent & rot) {
				transform.transform.SetRotation(rot.rot);
			});
			registry.view<FActorTransform, FScale>().each([&, dt](auto entity, FActorTransform & transform, FScale & sc) {

				transform.transform.SetScale3D(sc.scale);
			});
		}
		SCOPE_CYCLE_COUNTER(STAT_CopyTransformActor);
		//copy transforms from actor into FActorTransform	
		auto TransformView = registry.view<FCopyTransformToActor, FActorReference, FActorTransform>();
		for (auto e : TransformView)
		{			
			const FTransform&transform = TransformView.get<FActorTransform>(e).transform;
			FActorReference&actor = TransformView.get<FActorReference>(e);

			if (actor.ptr.IsValid())
			{
				actor.ptr->SetActorTransform(transform);
			}
		}		
	}
};
using EntityHandleId = uint64_t;
DECLARE_CYCLE_STAT(TEXT("SimEcs: Spanwer System"), STAT_ECSSpawn, STATGROUP_ECS);
struct ArchetypeSpawnerSystem :public SystemT {


	void SpawnFromArchetype(SimEcs_Registry & registry, EntityHandleId handleID,TSubclassOf<ASimEcs_Archetype> &ArchetypeClass, const FVector &SpawnPosition ,const FQuat  quatRot = FQuat::Identity )
	{
		//try to find the spawn archetype in the map, spawn a new one if not found, use it to initialize entity
		//

		auto Found = USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes.Find( handleID );
		TSharedPtr<ASimEcs_Archetype> FoundArchetype = nullptr;
		if (!Found)
		{
			FoundArchetype = MakeShareable( OwnerActor->GetWorld()->SpawnActor<ASimEcs_Archetype>(ArchetypeClass, SpawnPosition , quatRot.Rotator()));
			if (!FoundArchetype.IsValid( ))return;
			FoundArchetype->SetActorLabel( *GetNameSafe( FoundArchetype.Get() ) );
			UE_LOG(LogFlying, Warning, TEXT("Spawned archetype: %s"), *GetNameSafe( FoundArchetype.Get( ) ));
			if (!FoundArchetype)
			{   
				UE_LOG(LogFlying, Warning, TEXT("Error when spawning archetype: %s"), *GetNameSafe(ArchetypeClass));
			}
			else
			{
				//FString strInfor = TEXT( "m_mapArchetypes pawnerSystem" );
				//USimOceanSceneManager_Singleton::GetInstance( )->DebugLogger( strInfor );
				USimOceanSceneManager_Singleton::GetInstance( )->m_MapArchetypes.Add( handleID, FoundArchetype);
			}
		}
		else
		{
			FoundArchetype = *Found;
			FoundArchetype->SetActorLocation( SpawnPosition );
		}
		//if (FoundArchetype) {
		//	return FoundArchetype->CreateNewEntityFromThis( handleID );
		//}
		//else {
		//	UE_LOG( LogFlying, Warning, TEXT( "Failed new Entity: %s" ), *GetNameSafe( ArchetypeClass ) );
		//}

	}

	TSharedPtr<ASimEcs_Archetype> GetArcheTypeByHandleID( EntityHandleId handleID ) {
		return USimOceanSceneManager_Singleton::GetInstance( )->m_MapArchetypes[handleID];
	}

	void update(SimEcs_Registry &registry, float dt) override;
};
 
class StaticMeshDrawSystem :public SystemT {
public:

	struct ISMData {
		UInstancedStaticMeshComponent* ISM;
		int rendered;
	};

	UInstancedStaticMeshComponent * ISM;
	TMap<UStaticMesh*, ISMData> MeshMap;
	

	int render = 0;
	float Elapsed{5.f};	

	ISMData * GetInstancedMeshForMesh(UStaticMesh * mesh)
	{
		auto find = MeshMap.Find(mesh);
		if (find)
		{
			return find;
		}
		else
		{		
			UInstancedStaticMeshComponent* NewComp = NewObject<UInstancedStaticMeshComponent>(OwnerActor, UInstancedStaticMeshComponent::StaticClass());
			if (!NewComp)
			{
				return NULL;
			}

			NewComp->RegisterComponent();
			NewComp->SetStaticMesh(mesh);
			NewComp->SetWorldLocation(FVector(0.0, 0.0, 0.0));
			NewComp->SetCastShadow(false);
			NewComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			NewComp->SetWorldScale3D(FVector(1.0, 1.0, 1.0));
			NewComp->SetCanEverAffectNavigation(false);

			ISMData NewData;
			NewData.ISM = NewComp;
			NewData.rendered = 0;
			auto &d = MeshMap.Add(mesh, NewData);
			
			return &d;
		}				

	}

	void initialize(AActor * _Owner, SimEcs_World * _World) override{
		
		OwnerActor = _Owner;	
		World = _World;
		ISM = nullptr;		
	}	

	void update(SimEcs_Registry &registry,float dt) override;
};

DECLARE_CYCLE_STAT(TEXT("SimEcs: Raycast System"), STAT_ECSRaycast, STATGROUP_ECS);
struct RaycastSystem :public SystemT {

	void update(SimEcs_Registry &registry, float dt) override;
};

DECLARE_CYCLE_STAT(TEXT("SimEcs: BarrierFixed Raycast System"), STAT_ECSBarFixedRaycast, STATGROUP_ECS);
struct BarrierFixedRaycastSystem :public SystemT {
	void update(SimEcs_Registry &registry, float dt) override;
};

DECLARE_CYCLE_STAT(TEXT("SimEcs: Lifetime System"), STAT_Lifetime, STATGROUP_ECS);
struct LifetimeSystem :public SystemT {

	void update(SimEcs_Registry &registry, float dt) override
	{
		assert(OwnerActor);
		
		SCOPE_CYCLE_COUNTER(STAT_Lifetime);

		//tick the lifetime timers
		auto LifetimeView = registry.view<FLifetime>();
		for (auto e : LifetimeView)
		{
			auto &Deleter = LifetimeView.get(e);

			Deleter.LifeLeft -= dt;
			if (Deleter.LifeLeft < 0)
			{
				//add a Destroy component so it gets deleted
				registry.accommodate<FDestroy>(e);				
			}
		}

		//logic can be done here for custom deleters, nothing right now

		 
		//delete everything with a FDestroy component
		auto DeleteView = registry.view<FDestroy>();
		for (auto e : DeleteView)
		{			
			registry.destroy(e);			
		}		
	}
};



DECLARE_CYCLE_STAT( TEXT( "SimEcs: SimulatePhysical System" ), STAT_SimulatePhysical, STATGROUP_ECS );
struct SwitchSimulatePhysicalSystem :public SystemT {

	void update( SimEcs_Registry &registry, float dt ) override;

};

