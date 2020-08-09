#include "SimEcs_BaseSystems.h"
#include "SimEcs_BattleComponents.h"
#include "DestructibleComponent.h"

DECLARE_CYCLE_STAT(TEXT("SimEcs: Instance Mesh Prepare"), STAT_InstancedMeshPrepare, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("SimEcs: Instance Mesh Draw"), STAT_InstancedMeshDraw, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("SimEcs: Instance Mesh Clean"), STAT_InstancedMeshClean, STATGROUP_ECS);
void StaticMeshDrawSystem::update(SimEcs_Registry &registry, float dt)
{
	//reset instances once per second
	Elapsed -= dt;
	if (Elapsed < 0)
	{
		Elapsed = 1;
		for (auto i : MeshMap)
		{
			i.Value.ISM->ClearInstances();
		}			
	}	



	{
		SCOPE_CYCLE_COUNTER(STAT_InstancedMeshPrepare);
		//copy transforms into the ISM RenderTransform
		registry.view<FInstancedStaticMesh, FPosition>().each([&, dt](auto entity, FInstancedStaticMesh & ism, FPosition & pos) {
			ism.RenderTransform.SetLocation(pos.pos);
		});
		registry.view<FInstancedStaticMesh, FRotationComponent>().each([&, dt](auto entity, FInstancedStaticMesh & ism, FRotationComponent & rot) {
			ism.RenderTransform.SetRotation(rot.rot);
		});
		registry.view<FInstancedStaticMesh, FScale>().each([&, dt](auto entity, FInstancedStaticMesh & ism, FScale & sc) {
			ism.RenderTransform.SetScale3D(sc.scale);
		});

	}
	for (auto &i : MeshMap)
	{
		i.Value.rendered = 0;
	}

	{
		SCOPE_CYCLE_COUNTER(STAT_InstancedMeshDraw);
	
		auto view = registry.view<FInstancedStaticMesh>();

		for (auto entity : view) {
			
			FInstancedStaticMesh &mesh = view.get(entity);

			auto MeshData = GetInstancedMeshForMesh(mesh.mesh);
			auto RenderMesh = MeshData->ISM;

			const FTransform &RenderTransform = mesh.RenderTransform;// mesh.RelativeTransform.GetRelativeTransform(mesh.RenderTransform);

			if (RenderMesh->GetInstanceCount() < MeshData->rendered)
			{
				RenderMesh->AddInstanceWorldSpace(RenderTransform);
			}
			else
			{
				RenderMesh->UpdateInstanceTransform(MeshData->rendered, RenderTransform, true, false);
			}
			MeshData->rendered++;
		}

		
	}
	
	for (auto &i : MeshMap)
	{	
		SCOPE_CYCLE_COUNTER(STAT_InstancedMeshClean)

		FTransform nulltransform;
		nulltransform.SetScale3D(FVector(0.0, 0.0, 0.0));

		//if we have more instances than renderables, set the instances to null transform so they dont draw.
		//they will get cleanup once a second.
		if (IsValid(i.Value.ISM))
		{
			while (i.Value.rendered < i.Value.ISM->GetInstanceCount())
			{

				i.Value.rendered++;
				i.Value.ISM->UpdateInstanceTransform(i.Value.rendered, nulltransform, true, false);

			}
			i.Value.ISM->MarkRenderStateDirty();
		}
	}
}

void ArchetypeSpawnerSystem::update(SimEcs_Registry &registry, float dt)
{
	assert(OwnerActor);

	SCOPE_CYCLE_COUNTER(STAT_ECSSpawn);

	//exclusively update timing
	auto SpawnerView = registry.view<FArchetypeSpawner>();
	for (auto e : SpawnerView)
	{
		SpawnerView.get(e).TimeUntilSpawn -= dt;
	}

	//spawn from arc and actortransform
	auto SpawnerArcView = registry.view<FArchetypeSpawner, FRandomArcSpawn, FActorTransform>();
	for (auto e : SpawnerArcView)
	{
		const FTransform &ActorTransform = SpawnerArcView.get<FActorTransform>(e).transform;
		FArchetypeSpawner& spawner = SpawnerArcView.get<FArchetypeSpawner>(e);
		const FRandomArcSpawn& arc = SpawnerArcView.get<FRandomArcSpawn>(e);

		if (spawner.TimeUntilSpawn < 0)
		{
			if (spawner.ArchetypeClass)
			{
				SpawnFromArchetype(registry, spawner.entHandleId, spawner.ArchetypeClass, ActorTransform.GetLocation( ) );
				
				registry.accommodate<FPosition>( spawner.entHandleId, ActorTransform.GetLocation());

				if (registry.has<FFaction>(e))
				{
					registry.accommodate<FFaction>( spawner.entHandleId, registry.get<FFaction>(e));
				}

				FVelocity vel;
				const float VelMagnitude = World->rng.FRandRange(arc.MinVelocity, arc.MaxVelocity);
				const float Arc = FMath::DegreesToRadians(World->rng.FRandRange(arc.MinAngle, arc.MaxAngle));


				FVector ArcVel = World->rng.VRandCone(FVector(1.0, 0.0, 0.0), Arc) *VelMagnitude;

				FVector BulletVelocity = ActorTransform.GetRotation().RotateVector(ArcVel);
				registry.accommodate<FVelocity>( spawner.entHandleId, BulletVelocity);
			}

			if (spawner.bLoopSpawn)
			{
				spawner.TimeUntilSpawn = spawner.SpawnRate;
			}
			else
			{
				registry.remove<FArchetypeSpawner>(e);
			}
		}
	}

	//Spawn with basic position
	auto SpawnerPositionView = registry.view<FArchetypeSpawner, FPrePosition, FRotationComponent>();
	for (auto e : SpawnerPositionView)
	{
		const FVector &SpawnPosition = SpawnerPositionView.get<FPrePosition>(e).pos;
		const FQuat&  quatRot = SpawnerPositionView.get<FRotationComponent>( e ).rot;
		FArchetypeSpawner& spawner = SpawnerPositionView.get<FArchetypeSpawner>(e);

		if (spawner.TimeUntilSpawn < 0)
		{
			if (spawner.ArchetypeClass)
			{ 
				ESceneRelevantConv esrc = spawner.ActorType < 1000 ? ESceneRelevantConv::E_SENERAIO_POINT : ESceneRelevantConv::E_BARRIER_POINT;
				FVector relativePos; relativePos.Set( SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z );
				relativePos = USimOceanSceneManager_Singleton::GetInstance( )->GetCovertScenePosition( relativePos , esrc );
				SpawnFromArchetype( registry, spawner.entHandleId, spawner.ArchetypeClass , relativePos, quatRot );
				registry.accommodate<FPosition>( spawner.entHandleId, relativePos );
			}

			if (spawner.bLoopSpawn)
			{
				spawner.TimeUntilSpawn = spawner.SpawnRate;
			}
			else
			{
				registry.remove<FArchetypeSpawner>(e);
			}
		}
	}		
}

void RaycastSystem::update(SimEcs_Registry &registry, float dt)
{
	assert(OwnerActor);
	UWorld * GameWorld = OwnerActor->GetWorld();
	SCOPE_CYCLE_COUNTER(STAT_ECSRaycast);

	//check all the raycast results from the async raycast
	registry.view<FRaycastResult>().each([&, dt](auto entity, FRaycastResult & ray) {
		
		if (GameWorld->IsTraceHandleValid(ray.handle, false))
		{			
			FTraceDatum tdata;
			GameWorld->QueryTraceData(ray.handle, tdata);
			if (tdata.OutHits.IsValidIndex(0))
			{
				//it actually hit
				if (tdata.OutHits[0].bBlockingHit)
				{
					//if its an actor, try to damage it. 
					AActor* act = tdata.OutHits[0].GetActor();
					if (act)
					{
						auto hcmp = act->FindComponentByClass<USimEcs_HealthComponentWrapper>();
						if (hcmp)
						{
							hcmp->OnDamaged.Broadcast(99.0);
						}
					}
					
					//if the entity was a projectile, create explosion and destroy it
					if (registry.has<FProjectile>(entity))
					{
						auto explosionclass = registry.get<FProjectile>(entity).ExplosionArchetypeClass;
						if (explosionclass)
						{
							//create new entity to spawn explosion
							auto h = registry.create();
							registry.assign<FPosition>(h);
							registry.assign<FLifetime>(h);
							registry.assign<FArchetypeSpawner>(h);
							registry.get<FPosition>(h).pos = tdata.OutHits[0].ImpactPoint;
							registry.get<FLifetime>(h).LifeLeft = 0.1;
							FArchetypeSpawner &spawn = registry.get<FArchetypeSpawner>(h);
							spawn.bLoopSpawn = false;
							spawn.ArchetypeClass = explosionclass;
							spawn.SpawnRate = 1;
							spawn.TimeUntilSpawn = 0.0;
						}

						registry.accommodate<FDestroy>(entity);
					}					
				}
			}
		}
	});

	//movement raycast needs a "last position" component
	registry.view<FMovementRaycast, FPosition, FLastPosition>().each([&, dt](auto entity, FMovementRaycast & ray, FPosition & pos, FLastPosition & lastPos) {
		
		if (pos.pos != lastPos.pos)
		{			
			FTraceHandle hit = GameWorld->AsyncLineTraceByChannel(EAsyncTraceType::Single, lastPos.pos, pos.pos, ray.RayChannel);
			
			registry.accommodate<FRaycastResult>(entity, hit);		


		}
	});	
}


void BarrierFixedRaycastSystem::update(SimEcs_Registry &registry, float dt)
{
	assert(OwnerActor);
	UWorld * GameWorld = OwnerActor->GetWorld();
	SCOPE_CYCLE_COUNTER(STAT_ECSBarFixedRaycast);

	//check all the raycast results from the async raycast
	registry.view<FPosition,FBarrierFixedRaycastResult, FHealth>().each([&, dt](auto entity, FPosition& pos, FBarrierFixedRaycastResult & ray, FHealth & health) {
		FHitResult  HitResult(ForceInit);
		FVector fPos, dir;
		auto archtype = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entity );
		if (!archtype)return;

		FCollisionQueryParams cqq(FName(TEXT("Combattrace")), true, NULL);
		cqq.bTraceComplex = true;
		cqq.bReturnPhysicalMaterial = false;
		cqq.AddIgnoredActor( archtype.Get() );
		fPos = archtype.Get( )->GetTransform( ).GetLocation( ); 

		const FRotator Rotation = archtype.Get( )->GetTransform( ).Rotator();
		dir = FVector::UpVector * -1.0f;
		FVector beginPos = fPos;
		FVector posEnd = fPos + dir * 1000;
		UWorld* World = GEngine->GameViewport->GetWorld( );
		World->LineTraceSingleByChannel(HitResult, beginPos, posEnd, ECC_WorldStatic, cqq);

		DrawDebugLine( World, beginPos, posEnd, FColor::Green, true, 5.0f);
		if (HitResult.GetActor())
		{
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitResult.PhysMaterial.Get());
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, HitResult.GetActor()->GetName());
			auto archeType = USimOceanSceneManager_Singleton::GetInstance()->FindArchetype(entity);
			auto  SceneComponent = archeType.Get()->GetRootComponent();
			if (archeType && SceneComponent) {
				pos.pos = posEnd + dir * -800.0f;
				//USimOceanSceneManager_Singleton::GetInstance( )->DebugLogger( pos.pos.ToString());
				SceneComponent->SetWorldLocation( pos.pos );
				archeType.Get( )->SetActorEnableCollision( true );
				
					/*if(UStaticMeshComponent* ChildComponent  = Cast<UStaticMeshComponent>(SceneComponent->GetAttachParent()))
					{
						ChildComponent->SetSimulatePhysics(true);
					}*/
					for (int32 ChildIndex = 0; ChildIndex < SceneComponent->GetNumChildrenComponents(); ++ChildIndex)
					{
						if (UDestructibleComponent* ChildComponent = Cast<UDestructibleComponent>(SceneComponent->GetChildComponent(ChildIndex)))
						{
							//USimOceanSceneManager_Singleton::GetInstance()->DebugLogger(pos.pos.ToString());
							ChildComponent->SetSimulatePhysics(true);
						}
						else if (UStaticMeshComponent* ChildComponent = Cast<UStaticMeshComponent>(SceneComponent->GetChildComponent(ChildIndex)))
						{
							//USimOceanSceneManager_Singleton::GetInstance()->DebugLogger(pos.pos.ToString());
							ChildComponent->SetSimulatePhysics(true);
						}
					}
				//CastChecked<UStaticMeshComponent>( archeType.Get( )->GetRootComponent( )->GetChildComponent() )->SetSimulatePhysics( true );
			}
		}
	});

}