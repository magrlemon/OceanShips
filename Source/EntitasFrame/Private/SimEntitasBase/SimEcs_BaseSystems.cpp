#include "SimEcs_BaseSystems.h"
#include "SimEcs_BattleComponents.h"
#include "DestructibleComponent.h"

DECLARE_CYCLE_STAT( TEXT( "SimEcs: Instance Mesh Prepare" ), STAT_InstancedMeshPrepare, STATGROUP_ECS );
DECLARE_CYCLE_STAT( TEXT( "SimEcs: Instance Mesh Draw" ), STAT_InstancedMeshDraw, STATGROUP_ECS );
DECLARE_CYCLE_STAT( TEXT( "SimEcs: Instance Mesh Clean" ), STAT_InstancedMeshClean, STATGROUP_ECS );
void StaticMeshDrawSystem::update( SimEcs_Registry &registry, float dt )
{
	//reset instances once per second
	Elapsed -= dt;
	if (Elapsed < 0) {
		Elapsed = 1;
		for (auto i : MeshMap) {
			i.Value.ISM->ClearInstances( );
		}
	}



	{
		SCOPE_CYCLE_COUNTER( STAT_InstancedMeshPrepare );
		//copy transforms into the ISM RenderTransform
		registry.view<FInstancedStaticMesh, FPosition>( ).each( [&, dt]( auto entity, FInstancedStaticMesh & ism, FPosition & pos ) {
			ism.RenderTransform.SetLocation( pos.pos );
		} );
		registry.view<FInstancedStaticMesh, FRotationComponent>( ).each( [&, dt]( auto entity, FInstancedStaticMesh & ism, FRotationComponent & rot ) {
			ism.RenderTransform.SetRotation( rot.rot );
		} );
		registry.view<FInstancedStaticMesh, FScale>( ).each( [&, dt]( auto entity, FInstancedStaticMesh & ism, FScale & sc ) {
			ism.RenderTransform.SetScale3D( sc.scale );
		} );

	}
	for (auto &i : MeshMap) {
		i.Value.rendered = 0;
	}

	{
		SCOPE_CYCLE_COUNTER( STAT_InstancedMeshDraw );

		auto view = registry.view<FInstancedStaticMesh>( );

		for (auto entity : view) {

			FInstancedStaticMesh &mesh = view.get( entity );

			auto MeshData = GetInstancedMeshForMesh( mesh.mesh );
			auto RenderMesh = MeshData->ISM;

			const FTransform &RenderTransform = mesh.RenderTransform;// mesh.RelativeTransform.GetRelativeTransform(mesh.RenderTransform);

			if (RenderMesh->GetInstanceCount( ) < MeshData->rendered) {
				RenderMesh->AddInstanceWorldSpace( RenderTransform );
			}
			else {
				RenderMesh->UpdateInstanceTransform( MeshData->rendered, RenderTransform, true, false );
			}
			MeshData->rendered++;
		}
	}

	for (auto &i : MeshMap) {
		SCOPE_CYCLE_COUNTER( STAT_InstancedMeshClean )

			FTransform nulltransform;
		nulltransform.SetScale3D( FVector( 0.0, 0.0, 0.0 ) );

		//if we have more instances than renderables, set the instances to null transform so they dont draw.
		//they will get cleanup once a second.
		if (IsValid( i.Value.ISM )) {
			while (i.Value.rendered < i.Value.ISM->GetInstanceCount( )) {

				i.Value.rendered++;
				i.Value.ISM->UpdateInstanceTransform( i.Value.rendered, nulltransform, true, false );

			}
			i.Value.ISM->MarkRenderStateDirty( );
		}
	}
}

void ArchetypePreSpawnerSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );

	SCOPE_CYCLE_COUNTER( STAT_ECSPreSpawn );

	//exclusively update timing
	auto SpawnerView = registry.view<FArchetypeSpawner>( );
	for (auto e : SpawnerView) {
		SpawnerView.get( e ).TimeUntilSpawn -= dt;
	}

		
	//Spawn with basic position
	auto SpawnerPositionView = registry.view<FArchetypeSpawner, FPrePosition, FRotationComponent>( );
	for (auto e : SpawnerPositionView) {
		const FVector &SpawnPosition = SpawnerPositionView.get<FPrePosition>( e ).pos;
		const FQuat&  quatRot = SpawnerPositionView.get<FRotationComponent>( e ).rot;
		FArchetypeSpawner& spawner = SpawnerPositionView.get<FArchetypeSpawner>( e );

		if (spawner.TimeUntilSpawn < 0) {
			if (spawner.ArchetypeClass) {
				ESceneRelevantConv esrc = spawner.ActorType < 1000 ? ESceneRelevantConv::E_SENERAIO_POINT : ESceneRelevantConv::E_BARRIER_POINT;
				FVector relativePos; relativePos.Set( SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z );
				relativePos = USimOceanSceneManager_Singleton::GetInstance( )->GetCovertScenePosition( relativePos, esrc );
				PreSpawnFromArchetype( registry, spawner , relativePos, quatRot );
				registry.accommodate<FPosition>( spawner.entHandleId, relativePos );
			}

			if (spawner.bLoopSpawn) {
				spawner.TimeUntilSpawn = spawner.SpawnRate;
			}
			else {
				registry.remove<FArchetypeSpawner>( e );
			}
		}
	}
}



void ArchetypeSpawnerSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );

	SCOPE_CYCLE_COUNTER( STAT_ECSSpawn );

	//exclusively update timing
	auto SpawnerView = registry.view<FArchetypeSpawner>( );
	for (auto e : SpawnerView) {
		SpawnerView.get( e ).TimeUntilSpawn -= dt;
	}


	//spawn from arc and actortransform
	auto SpawnerArcView = registry.view<FArchetypeSpawner, FRandomArcSpawn, FActorTransform>( );
	for (auto e : SpawnerArcView) {
		const FTransform &ActorTransform = SpawnerArcView.get<FActorTransform>( e ).transform;
		FArchetypeSpawner& spawner = SpawnerArcView.get<FArchetypeSpawner>( e );
		const FRandomArcSpawn& arc = SpawnerArcView.get<FRandomArcSpawn>( e );

		if (spawner.TimeUntilSpawn < 0) {
			if (spawner.ArchetypeClass) {
				SpawnFromArchetype( registry, spawner.entHandleId, spawner.ActorType, spawner.ArchetypeClass, ActorTransform.GetLocation( ) );

				registry.accommodate<FPosition>( spawner.entHandleId, ActorTransform.GetLocation( ) );

				if (registry.has<FFaction>( e )) {
					registry.accommodate<FFaction>( spawner.entHandleId, registry.get<FFaction>( e ) );
				}

				FVelocity vel;
				const float VelMagnitude = World->rng.FRandRange( arc.MinVelocity, arc.MaxVelocity );
				const float Arc = FMath::DegreesToRadians( World->rng.FRandRange( arc.MinAngle, arc.MaxAngle ) );


				FVector ArcVel = World->rng.VRandCone( FVector( 1.0, 0.0, 0.0 ), Arc ) *VelMagnitude;

				FVector BulletVelocity = ActorTransform.GetRotation( ).RotateVector( ArcVel );
				registry.accommodate<FVelocity>( spawner.entHandleId, BulletVelocity );
			}

			if (spawner.bLoopSpawn) {
				spawner.TimeUntilSpawn = spawner.SpawnRate;
			}
			else {
				registry.remove<FArchetypeSpawner>( e );
			}
		}
	}

	//Spawn with basic position
	auto SpawnerPositionView = registry.view<FArchetypeSpawner, FPrePosition, FRotationComponent>( );
	for (auto e : SpawnerPositionView) {
		const FVector &SpawnPosition = SpawnerPositionView.get<FPrePosition>( e ).pos;
		const FQuat&  quatRot = SpawnerPositionView.get<FRotationComponent>( e ).rot;
		FArchetypeSpawner& spawner = SpawnerPositionView.get<FArchetypeSpawner>( e );

		if (spawner.TimeUntilSpawn < 0) {
			if (spawner.ArchetypeClass) {
				ESceneRelevantConv esrc = spawner.ActorType < 1000 ? ESceneRelevantConv::E_SENERAIO_POINT : ESceneRelevantConv::E_BARRIER_POINT;
				FVector relativePos; relativePos.Set( SpawnPosition.X, SpawnPosition.Y, SpawnPosition.Z );
				relativePos = USimOceanSceneManager_Singleton::GetInstance( )->GetCovertScenePosition( relativePos, esrc );
				SpawnFromArchetype( registry, spawner, relativePos, quatRot );
				registry.accommodate<FPosition>( spawner.entHandleId, relativePos );
			}

			if (spawner.bLoopSpawn) {
				spawner.TimeUntilSpawn = spawner.SpawnRate;
			}
			else {
				registry.remove<FArchetypeSpawner>( e );
			}
		}
	}
}
//////////////////////////////////////////////////////////////////////////
void RaycastSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );
	UWorld * GameWorld = OwnerActor->GetWorld( );
	SCOPE_CYCLE_COUNTER( STAT_ECSRaycast );

	//check all the raycast results from the async raycast
	registry.view<FRaycastResult>( ).each( [&, dt]( auto entity, FRaycastResult & ray ) {

		if (GameWorld->IsTraceHandleValid( ray.handle, false )) {
			FTraceDatum tdata;
			GameWorld->QueryTraceData( ray.handle, tdata );
			if (tdata.OutHits.IsValidIndex( 0 )) {
				//it actually hit
				if (tdata.OutHits[0].bBlockingHit) {
					//if its an actor, try to damage it. 
					AActor* act = tdata.OutHits[0].GetActor( );
					if (act) {
						auto hcmp = act->FindComponentByClass<USimEcs_HealthComponentWrapper>( );
						if (hcmp) {
							hcmp->OnDamaged.Broadcast( 99.0 );
						}
					}

					//if the entity was a projectile, create explosion and destroy it
					if (registry.has<FProjectile>( entity )) {
						auto explosionclass = registry.get<FProjectile>( entity ).ExplosionArchetypeClass;
						if (explosionclass) {
							//create new entity to spawn explosion
							auto h = registry.create( );
							registry.assign<FPosition>( h );
							registry.assign<FLifetime>( h );
							registry.assign<FArchetypeSpawner>( h );
							registry.get<FPosition>( h ).pos = tdata.OutHits[0].ImpactPoint;
							registry.get<FLifetime>( h ).LifeLeft = 0.1;
							FArchetypeSpawner &spawn = registry.get<FArchetypeSpawner>( h );
							spawn.bLoopSpawn = false;
							spawn.ArchetypeClass = explosionclass;
							spawn.SpawnRate = 1;
							spawn.TimeUntilSpawn = 0.0;
						}

						registry.accommodate<FDestroy>( entity );
					}
				}
			}
		}
	} );

	//movement raycast needs a "last position" component
	registry.view<FMovementRaycast, FPosition, FLastPosition>( ).each( [&, dt]( auto entity, FMovementRaycast & ray, FPosition & pos, FLastPosition & lastPos ) {

		if (pos.pos != lastPos.pos) {
			FTraceHandle hit = GameWorld->AsyncLineTraceByChannel( EAsyncTraceType::Single, lastPos.pos, pos.pos, ray.RayChannel );

			//registry.accommodate<FRaycastResult>( entity, hit );


		}
	} );
}


void BarrierFixedRaycastSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );
	UWorld * GameWorld = OwnerActor->GetWorld( );
	SCOPE_CYCLE_COUNTER( STAT_ECSBarFixedRaycast );
	
	//check all the raycast results from the async raycast
	registry.view<FPosition, FBarrierFixedRaycastResult, FHealth>( ).each( [&, dt]( auto entity, FPosition& pos, FBarrierFixedRaycastResult & ray, FHealth & health ) {
		FHitResult  HitResult( ForceInit );
		FVector fPos, dir;
		auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entity );
		if (!archeType)return;

		FCollisionQueryParams Line( FName( TEXT( "sim_ocean" ) ), true, NULL );
		Line.bTraceComplex = true;
		Line.bReturnPhysicalMaterial = false;
		Line.AddIgnoredActor( archeType.Get( ) );
		fPos = archeType.Get( )->GetTransform( ).GetLocation( );

		const FRotator Rotation = archeType.Get( )->GetTransform( ).Rotator( );
		dir = FVector::UpVector * -1.0f;
		FVector actorLocation = fPos;
		FVector forwardVector = fPos + dir * ECS_ABOUVE_GROUD_HEIGHT;
		UWorld* World = GEngine->GameViewport->GetWorld( );
		if (!World)return;

		TArray<struct FHitResult> OutHitResults;
		bool const bHadBlockingHit = World->LineTraceMultiByChannel( OutHitResults, actorLocation, forwardVector, ECC_WorldStatic, Line );
		if (!bHadBlockingHit)return;
		int32 findHitIdx = -1;
		bool bHitLandScape = false;
		for (int32 HitIdx = 0; HitIdx < OutHitResults.Num( ); HitIdx++) {
			const FHitResult& Check = OutHitResults[HitIdx];
			if (Check.GetActor( ) && Check.GetActor( )->Tags.Num()>0 &&  Check.GetActor( )->Tags[0].Compare( "Landscape" ) == 0) {
				findHitIdx = HitIdx;
				bHitLandScape = true;
				break;
			}
			else if (Check.GetActor() && Check.GetActor()->Tags.Num() > 0 && Check.GetActor()->Tags[0].Compare("sim_ocean") == 0) {
				findHitIdx = HitIdx;
				break;
			}
			/*else {
				findHitIdx = HitIdx;
			}*/
		}
		if (findHitIdx < 0)return;
		if (OutHitResults[findHitIdx].GetActor( )) {
			EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType( OutHitResults[findHitIdx].PhysMaterial.Get( ) );

			auto  SceneComponent = archeType.Get( )->GetRootComponent( );
			if (archeType && SceneComponent) {
				pos.pos = OutHitResults[findHitIdx].Location + dir * -10.0f;
				if (!bHitLandScape)
				{
					if (World->LineTraceMultiByChannel(OutHitResults, pos.pos, forwardVector, ECC_WorldStatic, Line))
					{
						pos.pos = OutHitResults[0].Location + dir * -10.0f;
					}
				}				

				if (ray.Distance < 15.0f) {
					FSimulatePhysical fsp;
					fsp.LifeLeft = 0.5f;
					fsp.SecLifeLeft = 2.0f;
					fsp.bSimulatePhysical = true;
					fsp.bJumpOne = true;
					registry.remove<FBarrierFixedRaycastResult>( entity );
					registry.accommodate<FSimulatePhysical>( entity, fsp );
					return;
				}
				//DrawDebugLine( World, archeType->GetTransform( ).GetTranslation( ), pos.pos, FColor::Green, true, 5.0f );
				//DrawDebugPoint(World, pos.pos, 10, FColor::Red, false, 1);
				//archeType.Get( )->SetActorEnableCollision( true );
				
				//archeType.Get()->SetActorLocation(pos.pos);

				for (int32 ChildIndex = 0; ChildIndex < SceneComponent->GetNumChildrenComponents(); ++ChildIndex) {
					if (UDestructibleComponent* ChildComponent = Cast<UDestructibleComponent>(SceneComponent->GetChildComponent(ChildIndex))) {
						ChildComponent->SetWorldLocation(pos.pos);
						FVector fCurPos = ChildComponent->GetComponentLocation();
						ray.Distance = FVector::Dist(fCurPos, pos.pos);
					}
				}
			}
		}
	} );
}


void SwitchSimulatePhysicalSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );

	SCOPE_CYCLE_COUNTER( STAT_SimulatePhysical );

	//tick the lifetime timers
	auto SimulatePhysicalView = registry.view<FSimulatePhysical>( );
	for (auto e : SimulatePhysicalView) {
		auto &simPhy = SimulatePhysicalView.get( e );
		if(simPhy.LifeLeft> 0.0f)
			simPhy.LifeLeft -= dt;
		if (simPhy.LifeLeft < 0) {
			auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( e );
			auto  SceneComponent = archeType.Get( )->GetRootComponent( );
			if (!archeType)return;
			//add a Destroy component so it gets deleted
			bool bRemoveSimCom = false;
			for (int32 ChildIndex = 0; ChildIndex < SceneComponent->GetNumChildrenComponents( ); ++ChildIndex) {
				if (UDestructibleComponent* ChildComponent = Cast<UDestructibleComponent>( SceneComponent->GetChildComponent( ChildIndex ) )) {
					ChildComponent->SetSimulatePhysics( simPhy.bSimulatePhysical );
					bRemoveSimCom = true;
				}
			}
			if (bRemoveSimCom) {
				if (simPhy.bJumpOne) {
					simPhy.LifeLeft = simPhy.SecLifeLeft;
					simPhy.SecLifeLeft = 0.0f;
					simPhy.bJumpOne = false;
					simPhy.bSimulatePhysical = false;
				}
				else {
					registry.remove<FSimulatePhysical>( e );
				}
			}
		}
	}

	//logic can be done here for custom deleters, nothing right now
}




/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Formation             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////  

#define COLLISION_TRACE ECC_GameTraceChannel4
using  BoatFormationType = USimOceanSceneManager_Singleton::BoatFormationStruct;
FVector  AvoidObstacleSystem::AvoidObstacle( FOceanShip & ex, BoatFormationType * pBFType, FVector& fCurrentPosition )
{
	if (!pBFType || !ex.MainMeshComponent)return FVector::ZeroVector;
	FVector actorLocation = fCurrentPosition + FVector::UpVector * 100.0f;
	FVector forwardVector = (ex.MainMeshComponent->GetComponentRotation( ).Vector( ).GetSafeNormal() * pBFType->AvoidanceDistance) + actorLocation;

	FHitResult OutHitResult;
	FCollisionQueryParams Line( FName( "Collision param" ), true );
	UWorld* World = GEngine->GameViewport->GetWorld( );
	if (!World)	return 	FVector::ZeroVector;
	TArray<struct FHitResult> OutHitResults;
	//DrawDebugLine( World, actorLocation, forwardVector, FColor::Green, true, 5.0f );
	bool const bHadBlockingHit = World->LineTraceMultiByChannel( OutHitResults, actorLocation, forwardVector, COLLISION_TRACE, Line );
	int32 findHitIdx = -1;
	for (int32 HitIdx = 0; HitIdx < OutHitResults.Num( ); HitIdx++) {
		const FHitResult& Check = OutHitResults[HitIdx];
		if (Check.GetActor( ) && Check.GetActor( )->Tags.Num( ) > 0 && Check.GetActor( )->Tags[0].Compare( "bp_oceanboat" ) == 0) {
			findHitIdx = HitIdx;
			GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "find AvoidObstacle %s", *(Check.GetActor( )->GetName()) );
		}
	}
	if (findHitIdx < 0) return FVector::ZeroVector;
	FVector returnVector = FVector( 0, 0, 0 );
	float distanceToBound = distanceToBound = (fCurrentPosition - OutHitResults[findHitIdx].ImpactPoint).Size( );
	if (bHadBlockingHit && findHitIdx>=0) {

		if (OutHitResults[findHitIdx].ImpactPoint.X > fCurrentPosition.X) {
			returnVector.X += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * -1;
		}
		else if (OutHitResults[findHitIdx].ImpactPoint.X < fCurrentPosition.X) {

			returnVector.X += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * 1;
		}

		if (OutHitResults[findHitIdx].ImpactPoint.Y > fCurrentPosition.Y) {
			returnVector.Y += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * -1;
		}
		else if (OutHitResults[findHitIdx].ImpactPoint.Y < fCurrentPosition.Y) {

			returnVector.Y += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * 1;
		}

		returnVector.Normalize( );
		FVector avoidance = returnVector * pBFType->AvoidanceForce;
		GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, avoidance.ToString() );
		return avoidance;
	}
	return FVector( 0, 0, 0 );
}


void AvoidObstacleSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );

	SCOPE_CYCLE_COUNTER( STAT_AvoidObstacle );

	//tick the lifetime timers
	registry.view<FOceanShip, FFormation, FPosition>( ).each( [&, dt]( auto entity, FOceanShip & ex, FFormation & formation, FPosition& pos ) {
		if (formation.GroupName.IsEmpty( ))return;
		auto boatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo.Find( formation.GroupName );
		if (boatFormationInfo) {

			USimOceanSceneManager_Singleton::BoatFormationStruct* itemLafe = boatFormationInfo->Find( entity );
			if (ex.MainMeshComponent) {
				ex.MainMeshComponent->GetComponentLocation( );
				itemLafe->ExpectAvoidOffect = AvoidObstacle( ex, itemLafe, pos.pos );
				itemLafe->ExpectAvoidOffect.Z = 0.0f;
			}
		}
	} );
}




void FSMAnimationSystem::update( SimEcs_Registry &registry, float dt )
{
	assert( OwnerActor );

	SCOPE_CYCLE_COUNTER( STAT_FSMAnimationSystem );

	//tick the FSMAnimation timers
	registry.view<FOceanShip, FFSMAnimation, FPosition>( ).each( [&, dt]( auto entity, FOceanShip & ex, FFSMAnimation & fsmAnimation, FPosition& pos ) {

		auto GInstance = USimOceanSceneManager_Singleton::GetInstance( );
		auto archeType = USimOceanSceneManager_Singleton::GetInstance( )->FindArchetype( entity );
		if (!archeType)return;
		if (fsmAnimation.bFsmAnim && GInstance->GetFsmManager( )->GetFsm( *(archeType->GetName( )) )->GetName() == "IdleZJAnimationState") {

		}
		/*auto boatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo.Find( formation.GroupName );
		if (boatFormationInfo) {
			USimOceanSceneManager_Singleton::BoatFormationStruct* itemLafe = boatFormationInfo->Find( entity );
			if (ex.MainMeshComponent) {

			}
		}*/
	} );
}
