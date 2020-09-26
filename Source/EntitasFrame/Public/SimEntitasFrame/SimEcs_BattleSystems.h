
// Fill out your copyright notice in the Description page of Project Settings.
/**
* Copyright 2019 北京伯睿科技有限公司.
*
* @File ExplosionSystem.h
* @Description 爆炸系统
*
* @Author magi
* @Date 2020年3月20日
* @Version 1.0
*/

#pragma once

#include "SimEcs_Core.h"
#include "SimEcs_BaseComponents.h"
#include "SimEcs_Archetype.h"
#include "SimEcs_BattleComponents.h"
#include "Engine/Console.h"
#include "LinearMemory.h"
#include "SimEcs_FormationArithmetic.h"

DECLARE_CYCLE_STAT( TEXT( "SimEcs: Explosion System" ), STAT_Explosion, STATGROUP_ECS );

struct ExplosionSystem :public SystemT {

	const float UpdateRate = 0.1;

	float elapsed = 0;

	void update( SimEcs_Registry &registry, float dt ) override
	{
		assert( OwnerActor );
		SCOPE_CYCLE_COUNTER( STAT_Explosion );

		auto AllExplosionsView = registry.view<FExplosion>( );
		for (auto e : AllExplosionsView) {
			FExplosion & ex = AllExplosionsView.get( e );

			ex.LiveTime += dt;
			if (ex.LiveTime > ex.Duration) {
				registry.assign<FDestroy>( e );
			}
		}


		registry.view<FExplosion, FScale>( ).each( [&, dt]( auto entity, FExplosion & ex, FScale & scale ) {

			scale.scale = FVector( (ex.LiveTime / ex.Duration)*ex.MaxScale );

		} );


	}
};

#define COLLISION_TRACE ECC_GameTraceChannel4
DECLARE_CYCLE_STAT( TEXT( "SimEcs: OceanShip System" ), STAT_OceanShip, STATGROUP_ECS );
struct OceanShipSystem :public SystemT {

	int CoolDownTime = 5;
	int LastTime = 0;

	FRotator MakeRotFromX( const FVector& X )
	{
		return FRotationMatrix::MakeFromX( X ).Rotator( );
	}

	bool GetRotateYaw( FOceanShip& ship, FRotator& rot ) {

		if (ship.MainMeshComponent) {
			FVector fCurrentHeading = ship.MainMeshComponent->GetForwardVector( ).GetSafeNormal( );
			FVector fTargetHeading = (ship.MoveOnPos - ship.MainMeshComponent->GetComponentLocation( )).GetSafeNormal( );
			float fNeedLerpMaxYaw = fCurrentHeading.CosineAngle2D( fTargetHeading );
			float factor = FMath::Clamp( FMath::Abs( FMath::Sin( fNeedLerpMaxYaw ) ), 0.0001f, 0.01f );
			fTargetHeading = FMath::Lerp( fCurrentHeading, fTargetHeading, factor );
			FVector lerpHeading = (fTargetHeading - fCurrentHeading).GetSafeNormal( );
			float fYaw = lerpHeading.CosineAngle2D( fTargetHeading )*0.001f * ship.TurnStep;
			const float leftOrRight = (FVector::CrossProduct( fCurrentHeading, fTargetHeading ).Z > 0) ? -1.0f : 1.0f;
			rot = ship.MainMeshComponent->GetComponentRotation( ).Add( 0.0f, FMath::RadiansToDegrees( fYaw * leftOrRight ), 0.0f );

			if (ship.isLeader) {

				//UWorld* World = GEngine->GameViewport->GetWorld( );
				//if (!World)return false;
				//float dis = FVector::Distance( ship.MainMeshComponent->GetComponentLocation( ), ship.MoveOnPos );
				//GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, FString::SanitizeFloat( fYaw * leftOrRight ) );
				//DrawDebugLine( World, ship.MainMeshComponent->GetComponentLocation( ), ship.MoveOnPos + FVector::UpVector * 100.0f, FColor::Red, true, 5.0f );
				//DrawDebugLine( World, ship.MainMeshComponent->GetComponentLocation( ), ship.MoveOnPos + fTargetHeading * 2000.0f, FColor::Green, true, 5.0f );
			}
			return true;
		}
		return false;
	}

	FRotator FindLookAtRotation( const FVector& Start, const FVector& Target )
	{
		FVector rot = Target - Start;
		//rot.Z = 0;
		return MakeRotFromX( rot );

		const FVector fLerpValue = FMath::Lerp( Start, Target, 0.001f );
		FVector fValue = fLerpValue.GetSafeNormal( );
		fValue.Z = 0.0;
		return MakeRotFromX( fValue );
	}

	void AddForce( FOceanShip& ship, FVector forceLocation, UStaticMeshComponent* root )
	{
		if (ship.MoveMode == EBoatMoveMode_Idle)
			return;

		ship.MainMeshComponent = root;
		FVector moveDir = FVector::ZeroVector;
		if (ship.bAvoid)
			moveDir = root->GetRightVector() + root->GetForwardVector();
		else
			moveDir = root->GetForwardVector();
		/*root->AddForceAtLocation( root->GetRightVector( ) * ship.RightAxisValue * root->GetMass( ) * ship.SteeringSpeed, forceLocation );*/
		root->AddForce( /*root->GetForwardVector( )*/moveDir * root->GetMass( ) * ship.ForwardAxisValue * ship.ForwardSpeed );
	}

	void CheckSpeedUp( FOceanShip& ship )
	{
		//if (ship.MoveMode == EBoatMoveMode_On || ship.MoveMode == EBoatMoveMode_Back) 
		{
			if (abs( ship.CurrentSpeed - ship.ExpectSpeed ) > ship.StepSpeed*0.99) {
				if (ship.CurrentSpeed > ship.ExpectSpeed) {
					ship.bSpeedDown = true;
					ship.CurrentSpeed = (ship.CurrentSpeed - ship.StepSpeed) > 0 ? (ship.CurrentSpeed - ship.StepSpeed) : 0;
				}
				else {
					ship.bSpeedDown = false;
					ship.CurrentSpeed = (ship.CurrentSpeed + ship.StepSpeed) < ship.ExpectSpeed ? (ship.CurrentSpeed + ship.StepSpeed) : ship.ExpectSpeed;
				}
			}
		}
	}


	void FaceRotate( FOceanShip& ship, UStaticMeshComponent* root )
	{
		if (!ship.MainMeshComponent)
			return;
		/*if (ship.MoveMode != EBoatMoveMode_On || ship.MoveMode != EBoatMoveMode_Back)
			return;*/

		FRotator rot;
		FVector currentPos = ship.MainMeshComponent->GetComponentLocation( );
		ship.MoveOnPos.Z = currentPos.Z;

		if (ship.MoveMode == EBoatMoveMode_On || ship.MoveMode == EBoatMoveMode_Fire) {

			if (!GetRotateYaw( ship, rot )) return;

		}
		else if (ship.MoveMode == EBoatMoveMode_Back) {
			ship.bRollBack = true;
			ship.ForwardAxisValue = -ship.CurrentSpeed;
			rot.Yaw = FindLookAtRotation( currentPos, ship.MoveOnPos ).Yaw;
		}
				
		if (ship.MainMeshComponent != NULL) {
			ship.MainMeshComponent->SetWorldRotation( rot );
		}
	}

	void MainLoopLogic( FOceanShip& ship, FVector forceLocation, UStaticMeshComponent* root )
	{
		AddForce( ship, forceLocation, root );

		root->AddForce( root->GetForwardVector( ) * root->GetMass( ) * ship.ForwardAxisValue * ship.ForwardSpeed );

		ship.ForwardAxisValue = ship.CurrentSpeed;

		//check data
		if (ship.StepSpeed <= 0)
			ship.StepSpeed = 0.003;
		if (ship.ForwardSpeed <= 0)
			ship.ForwardSpeed = 1200;
		if (ship.SteeringSpeed <= 0)
			ship.SteeringSpeed = 200;
		if (ship.ProjectileVelocity <= 0)
			ship.ProjectileVelocity = 1200;

		CheckSpeedUp( ship );

		if (ship.MoveMode != EBoatMoveMode_Idle)
			FaceRotate( ship, root );

	}

	void RecordBoatDetail( FOceanShip& ship, ASimEcs_Archetype* boat )
	{
		//record Move Speed
		boat->Speed = boat->MainStaticMesh ? boat->MainStaticMesh->GetPhysicsLinearVelocity( ).Size( ) : 0.0f;

		//record move distance		
		if (ship.LastPos.Equals( FVector::ZeroVector ))
			ship.LastPos = boat->GetActorLocation( );
		else if (boat->MainStaticMesh) {
			float move = (ship.LastPos - boat->MainStaticMesh->GetComponentLocation( )).Size2D( );

			if (ship.MoveMode == EBoatMoveMode_On)
				boat->SailDistance += move;
			if (ship.bSpeedDown)
				boat->SpeedDownDistance += move;
			if (ship.bRollBack)
				boat->RollbackDistance += move;

			ship.LastPos = boat->MainStaticMesh->GetComponentLocation( );
		}
	}

	void CheckState( FOceanShip& ship, ASimEcs_Archetype* boat )
	{
		if (ship.MoveMode == EBoatMoveMode_On || ship.MoveMode == EBoatMoveMode_Back) {
			boat->EnableWaveForce( true );
			boat->EnableBoatEffect( true );
			if (ship.MainMeshComponent)
				ship.MainMeshComponent->SetSimulatePhysics( true );
		}
		else if (ship.MoveMode == EBoatMoveMode_Idle) {
			//boat->EnableBoatEffect( false );
			boat->EnableWaveForce( false );
		}
		else if (ship.MoveMode == EBoatMoveMode_Fire) {
			ship.ExpectSpeed = 0;
			boat->EnableWaveForce( false );
			//boat->EnableBoatEffect( false );
			boat->StartFire( );
			//ship.MoveMode = EBoatMoveMode_Idle;
		}
	}

	void HoldOn( FVector nextTargetPos = FVector::ZeroVector )
	{


	}
	//OceanShipSystem
	void update( SimEcs_Registry &registry, float dt ) override
	{
		assert( OwnerActor );
		SCOPE_CYCLE_COUNTER( STAT_OceanShip );
		int NowPlatformTime = FPlatformTime::Seconds( );
		registry.view<FOceanShip, FRotationComponent, FFormation, FVelocity,FFaction>( ).each( [&, dt]( auto entity,
			FOceanShip & ship, FRotationComponent & rotation, FFormation& formation, FVelocity&vel , FFaction &faction ) {

			auto SimInstance = USimOceanSceneManager_Singleton::GetInstance( );
			bool beJump = false; bool bLeaderIdle = false;
			TSharedPtr<ASimEcs_Archetype>* boat = SimInstance->m_MapArchetypes.Find( entity );
			if (boat) {
				// 获取船艇和父装备关系
				if (!faction.parentDevice.IsEmpty( )) {
					SimInstance->AddParentDeviceMap( faction.parentDevice, entity, ship.DltLocate );
				}

				FVector boatPos = boat->Get( )->GetTransform( ).GetLocation( );
				if (SimInstance->IsLeader( entity )) {
					if (!NowPlatformTime - LastTime > CoolDownTime) {
						FVector leaderPos = (ship.MoveMode == EBoatMoveMode_On) ? ship.MoveOnPos : (ship.MoveMode ==
							EBoatMoveMode_Back ? ship.MoveBackPos : boatPos);
						SimInstance->UpdateLeader( entity, leaderPos );
						LastTime = NowPlatformTime;
					}
				}
				else {
					EntityHandleId handleID = USimOceanSceneManager_Singleton::GetInstance( )->GetGroupLeader( formation.GroupName );
					bLeaderIdle = registry.get<FOceanShip>( handleID ).MoveMode > EBoatMoveMode_Back ? true : false;
					if (FVector::Dist2D( boatPos, ship.MoveOnPos ) < ECS_DISATACE_SHIP_THRESHOLD)
						beJump = true;
				}

				if (beJump && bLeaderIdle) {
					USimOceanSceneManager_Singleton::GetInstance( )->SetIdle( entity, boatPos );
					return;
				}
				RecordBoatDetail( ship, boat->Get( ) );
				ship.bAvoid = boat->Get()->AVoided;
				MainLoopLogic( ship, boat->Get( )->ForceLocation, Cast<UStaticMeshComponent>( boat->Get( )->GetRootComponent( ) ) );

				CheckState( ship, boat->Get( ) );

				boat->Get()->ResetParticelSize(ship.CurrentSpeed);
			}

		} );


	}
};


DECLARE_CYCLE_STAT( TEXT( "SimEcs: Boids Update" ), STAT_Boids, STATGROUP_ECS );
DECLARE_CYCLE_STAT( TEXT( "SimEcs: Gridmap Update" ), STAT_GridmapUpdate, STATGROUP_ECS );

struct BoidSystem :public SystemT {

	const float GRID_DIMENSION = 500.0;

	struct GridItem {
		EntityHandle ID;
		FVector Position;
		EFaction Faction;

	};
	struct ProjectileData {
		//read only data
		FProjectile proj;
		FPosition pos;
		FFaction faction;
		//velocity is a pointer so it can be modified
		FVelocity * vel;
	};
	struct SpaceshipData {

		//read only data
		FOceanShip ship;
		FPosition pos;
		FFaction  faction;

		//velocity is a pointer so it can be modified
		FVelocity * vel;
	};

	TArray<ProjectileData> ProjArray;
	TArray<SpaceshipData> SpaceshipArray;
	TMap<FIntVector, TArray<GridItem>> GridMap;

	void AddToGridmap( EntityHandle ent, FPosition&pos )
	{
		const FIntVector GridLoc = FIntVector( pos.pos / GRID_DIMENSION );
		auto SearchGrid = GridMap.Find( GridLoc );


		GridItem item;
		item.ID = ent;
		item.Position = pos.pos;

		if (World->GetRegistry( )->has<FFaction>( ent.handle )) {
			item.Faction = World->GetRegistry( )->get<FFaction>( ent.handle ).faction;
		}
		else {
			item.Faction = EFaction::Neutral;
		}


		if (!SearchGrid) {
			TArray<GridItem> NewGrid;

			NewGrid.Reserve( 10 );
			NewGrid.Add( item );

			GridMap.Emplace( GridLoc, std::move( NewGrid ) );


		}
		else {
			SearchGrid->Add( item );
		}
	}
	void Foreach_EntitiesInRadius( float radius, FVector origin, TFunctionRef<void( GridItem& )> Body )
	{
		const float radSquared = radius * radius;
		const FVector RadVector( radius, radius, radius );
		const FIntVector GridLoc = FIntVector( origin / GRID_DIMENSION );
		const FIntVector MinGrid = FIntVector( (origin - RadVector) / GRID_DIMENSION );
		const FIntVector MaxGrid = FIntVector( (origin + RadVector) / GRID_DIMENSION );


		for (int x = MinGrid.X; x <= MaxGrid.X; x++) {
			for (int y = MinGrid.Y; y <= MaxGrid.Y; y++) {
				for (int z = MinGrid.Z; z <= MaxGrid.Z; z++) {
					const FIntVector SearchLoc( x, y, z );
					const auto SearchGrid = GridMap.Find( SearchLoc );
					if (SearchGrid) {
						for (auto e : *SearchGrid) {
							if (FVector::DistSquared( e.Position, origin ) < radSquared) {
								Body( e );

							}
						}
					}
				}
			}
		}
	}

	void update( SimEcs_Registry &registry, float dt ) override
	{

		//add everything to the gridmap
		GridMap.Empty( 50 );

		auto GridView = registry.view<FGridMap, FPosition>( );

		{
			SCOPE_CYCLE_COUNTER( STAT_GridmapUpdate );
			for (auto e : GridView) {
				EntityHandle handle( e );

				AddToGridmap( handle, GridView.get<FPosition>( e ) );
			}
		}


		{
			SCOPE_CYCLE_COUNTER( STAT_Boids );

			TypedLinearMemory<ProjectileData> ProjArray( World->ScratchPad );

			auto ProjectileView = registry.view<FProjectile, FPosition, FVelocity, FFaction>( entt::persistent_t{} );
			//ProjArray.Reset();
			unsigned int NumProjectiles = 0;
			//copy projectile data into array so we can do a parallel update later
			for (auto e : ProjectileView) {
				ProjectileData Projectile;
				Projectile.faction = ProjectileView.get<FFaction>( e );
				Projectile.pos = ProjectileView.get<FPosition>( e );
				Projectile.proj = ProjectileView.get<FProjectile>( e );

				Projectile.vel = &ProjectileView.get<FVelocity>( e );
				ProjArray.push_back( Projectile );
				NumProjectiles++;
				//ProjArray.Add(Projectile);
			}


			ParallelFor( NumProjectiles, [&]( int32 Index ) {
				ProjectileData data = ProjArray[Index];

				//unpack projectile data
				const FVector ProjPosition = data.pos.pos;
				const EFaction ProjFaction = data.faction.faction;
				const float ProjSeekStrenght = data.proj.HeatSeekStrenght;
				const float ProjMaxVelocity = data.proj.MaxVelocity;
				FVector & ProjVelocity = data.vel->vel;


				const float ProjCheckRadius = 1000;
				Foreach_EntitiesInRadius( ProjCheckRadius, ProjPosition, [&]( GridItem &item ) {

					if (item.Faction != ProjFaction) {
						const FVector TestPosition = item.Position;

						const float DistSquared = FVector::DistSquared( TestPosition, ProjPosition );

						const float AvoidanceDistance = ProjCheckRadius * ProjCheckRadius;
						const float DistStrenght = FMath::Clamp( 1.0 - (DistSquared / (AvoidanceDistance)), 0.1, 1.0 ) * dt;
						const FVector AvoidanceDirection = TestPosition - ProjPosition;

						ProjVelocity += (AvoidanceDirection.GetSafeNormal( ) * ProjSeekStrenght*DistStrenght);
					}
				} );

				ProjVelocity = ProjVelocity.GetClampedToMaxSize( ProjMaxVelocity );
			} );

		}
		//its not good to have both spaceship and projectile logic here, they should be on their own systems
		{
			SCOPE_CYCLE_COUNTER( STAT_Boids );


			//auto spaceshipEnd = SpaceshipView.size()

			//int nShips = registry.view<FOceanShip>().size();
			auto SpaceshipView = registry.view<FOceanShip, FPosition, FVelocity, FFaction>( entt::persistent_t{} );

			TypedLinearMemory<SpaceshipData> SpaceshipArray( World->ScratchPad );
			unsigned int NumShips = 0;
			//SpaceshipArray.Reset();// Spaces//nShips);
			//copy spaceship data into array so we can do a paralle update later
			for (auto e : SpaceshipView) {
				SpaceshipData Ship;
				Ship.faction = SpaceshipView.get<FFaction>( e );
				Ship.pos = SpaceshipView.get<FPosition>( e );
				Ship.ship = SpaceshipView.get<FOceanShip>( e );

				Ship.vel = &SpaceshipView.get<FVelocity>( e );
				NumShips++;
				SpaceshipArray.push_back( Ship );
				//SpaceshipArray.Add(Ship);
			}

			ParallelFor( NumShips, [&]( int32 Index ) {
				SpaceshipData data = SpaceshipArray[Index];

				//unpack ship variables from the array
				const FVector ShipPosition = data.pos.pos;
				const EFaction ShipFaction = data.faction.faction;
				const float ShipAvoidanceStrenght = data.ship.AvoidanceStrenght;
				const float ShipMaxVelocity = data.ship.MaxVelocity;
				FVector & ShipVelocity = data.vel->vel;
				const FVector ShipTarget = data.ship.TargetMoveLocation;

				const float shipCheckRadius = 1000;
				Foreach_EntitiesInRadius( shipCheckRadius, ShipPosition, [&]( GridItem& item ) {

					if (item.Faction == ShipFaction) {
						const FVector TestPosition = item.Position;

						const float DistSquared = FVector::DistSquared( TestPosition, ShipPosition );

						const float AvoidanceDistance = shipCheckRadius * shipCheckRadius;
						const float DistStrenght = FMath::Clamp( 1.0 - (DistSquared / (AvoidanceDistance)), 0.1, 1.0 ) * dt;
						const FVector AvoidanceDirection = ShipPosition - TestPosition;

						ShipVelocity += AvoidanceDirection.GetSafeNormal( ) * ShipAvoidanceStrenght*DistStrenght;
					}
				} );

				FVector ToTarget = ShipTarget - ShipPosition;
				ToTarget.Normalize( );

				ShipVelocity += (ToTarget * 500 * dt);
				ShipVelocity = ShipVelocity.GetClampedToMaxSize( ShipMaxVelocity );
			} );


		}

		//int nProjectiles = registry.view<FProjectile>().size();


		/*{
			SCOPE_CYCLE_COUNTER(STAT_Boids);


			//auto SpaceshipView = registry.view<FOceanShip, FPosition, FVelocity, FFaction>(entt::persistent_t{});
			//
			//auto SpaceshipBegin = SpaceshipView.begin();
			//int Size = SpaceshipView.size();
			//
			//ProjArray.Reset(nProjectiles);

			auto ProjectileView = registry.view<FProjectile, FPosition, FVelocity, FFaction>(entt::persistent_t{});

			auto ProjectileBegin = ProjectileView.begin();
			int Size = ProjectileView.size();
			//copy projectile data into array so we can do a parallel update later
			//for (auto e : ProjectileView)
			//{


				//ProjArray.Add(Projectile);
			//}

			UE_LOG(LogFlying, Warning, TEXT("Num Projectiles: %s"), *FString::FromInt(Size));
			ParallelFor(Size, [&](int32 Index)
			{
				auto e = ProjectileView.data()[Index];
				//ProjectileData data = ProjArray[Index];

				ProjectileData data;
				data.faction = ProjectileView.get<FFaction>(e);
				data.pos = ProjectileView.get<FPosition>(e);
				data.proj = ProjectileView.get<FProjectile>(e);

				data.vel = &ProjectileView.get<FVelocity>(e);

				//unpack projectile data
				const FVector ProjPosition = data.pos.pos;
				const EFaction ProjFaction = data.faction.faction;
				const float ProjSeekStrenght = 10000;//data.proj.HeatSeekStrenght;
				const float ProjMaxVelocity = data.proj.MaxVelocity;
				FVector & ProjVelocity = data.vel->vel;


				const float ProjCheckRadius = 10000;
				Foreach_EntitiesInRadius(ProjCheckRadius, ProjPosition, [&](GridItem &item) {

					if (item.Faction != ProjFaction)
					{
						const FVector TestPosition = item.Position;

						const float DistSquared = FVector::DistSquared(TestPosition, ProjPosition);

						const float AvoidanceDistance = ProjCheckRadius * ProjCheckRadius;
						const float DistStrenght = FMath::Clamp(1.0 - (DistSquared / (AvoidanceDistance)), 0.1, 1.0) * dt;
						const FVector AvoidanceDirection = TestPosition - ProjPosition;

						ProjVelocity += (AvoidanceDirection.GetSafeNormal() * ProjSeekStrenght*DistStrenght);
					}
				});
  
				ProjVelocity = ProjVelocity.GetClampedToMaxSize(ProjMaxVelocity);
			});

		}
		*/






		//Draw gridmap
		const bool bDebugGridMap = false;
		if (bDebugGridMap) {
			const float MaxUnits = 5.0;
			for (auto& g : GridMap) {
				const FVector BoxMin = FVector( g.Key ) * GRID_DIMENSION;
				const FVector BoxMax = BoxMin + FVector( GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION );
				const FVector BoxCenter = BoxMin + FVector( GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION ) / 2.0;
				const FColor BoxColor = FColor::MakeRedToGreenColorFromScalar( g.Value.Num( ) / MaxUnits );
				DrawDebugBox( OwnerActor->GetWorld( ), BoxCenter, FVector( GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION ) / 2.0, BoxColor );
			}
		}

	}
};


/////////////////////////////////////////////////////////////////////////////
//////////////                                        //////////////////////            
//////////////            Boat Formation             /////////////////////                                 
//////////////                                        ////////////////////              
/////////////////////////////////////////////////////////////////////////

DECLARE_CYCLE_STAT( TEXT( "SimEcs: BoatFormation System" ), STAT_BoatFormation, STATGROUP_ECS );
struct BoatFormationSystem :public SystemT {

	const float UpdateRate = 0.1;
	FVector SamplePoint = FVector::ZeroVector;
	FVector NextLeaderTargetPoint = FVector::ZeroVector;
	float elapsed = 0;
	SimEcs_DelegateFormation m_SimDelegateFormation;
	struct BoatFormationStruct {
		BoatFormationStruct( FVector4 locate, bool isLeader ) : BoatTargetPosition( locate ), IsLeader( isLeader ) {};

		FVector4 BoatTargetPosition = FVector4( 0.0f, 0.0f, 0.0f, 0.0f );
		bool IsLeader = false;
	};

	/*float baseFormationAngle = 30.0f;
	float formationAngle = 5.0f;
	float formationLength = 5000.0f;*/
	float thresholdValue = 100.0f;
	using EntityHandleId = uint64_t;
	int   FormationCoolDownTime = 1.0f;
	int   FormationLastTime = 0.0f;
	int   FormationChildLastTime = 0.0f;
	/*FVector4 CaculateNextFormationLocate( FVector4 leaderPos, float  fNextDistance, float formationAngle ) {
		float a, b, c, t, sqrt_part;
		float x1, x2, y1, y2;
		t = FMath::Tan( formationAngle );
		a = t * t + 1; b = -2.0f* leaderPos.X - 4.0* t * t *  leaderPos.X;
		c = t * t * leaderPos.X* leaderPos.X + leaderPos.X* leaderPos.X - fNextDistance * fNextDistance;
		sqrt_part = FMath::Sqrt( b * b - 4.0f* a * c );
		x1 = (-1.0f* b + sqrt_part) / (2.0*a);
		x2 = (-1.0f* b - sqrt_part) / (2.0*a);
		y1 = t * (x1 - leaderPos.X) + leaderPos.Y;
		y2 = t * (x2 - leaderPos.X) + leaderPos.Y;
		return FVector4( x1, y1, x2, y2 );
	}*/


	void SampleLeaderPoints( const  FVector& fLeaderPos, const FVector& fMoveTargetPos ) {
		FVector fSameplePos = (fMoveTargetPos - fLeaderPos) * 0.05f + fLeaderPos;
		SamplePoint = FVector( fSameplePos.X, fSameplePos.Y, fLeaderPos.Z + 100.0f );

	}

	bool IsArrivingSampleLeaderPoints( FVector fPos ) {
		if (SamplePoint.Equals( FVector::ZeroVector ) == 0) {
			SamplePoint = fPos;
			return true;
		}
		return FVector::DistSquared( fPos, SamplePoint ) > thresholdValue ? false : true;
	}

	using  BoatFormationType = USimOceanSceneManager_Singleton::BoatFormationStruct;
	FVector AvoidObstacle( FOceanShip & ex, BoatFormationType * pBFType, FVector fTargetPosition )
	{
		if (!pBFType)return FVector::ZeroVector;
		FVector actorLocation = fTargetPosition;
		FVector forwardVector = (ex.MainMeshComponent->GetComponentRotation( ).Vector( ) * pBFType->AvoidanceDistance) + actorLocation;

		FHitResult OutHitResult;
		FCollisionQueryParams Line( FName( "Collision param" ), true );
		Line.TraceTag = "Sim_BoatArchetype";
		Line.bTraceComplex = true;
		Line.bReturnFaceIndex = false;
		UWorld* World = GEngine->GameViewport->GetWorld( );
		if (!World)	return 	FVector::ZeroVector;       
		DrawDebugLine( World, actorLocation, forwardVector+ FVector::UpVector * 100.0f, FColor::Red, true, 5.0f );

		bool const bHadBlockingHit = World->LineTraceSingleByChannel( OutHitResult, actorLocation, forwardVector, COLLISION_TRACE, Line );
		FVector returnVector = FVector( 0, 0, 0 );
		float distanceToBound = distanceToBound = (fTargetPosition - OutHitResult.ImpactPoint).Size( );
		if (bHadBlockingHit) {
			GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "AvoidObstacle" );
			if (OutHitResult.ImpactPoint.X > fTargetPosition.X) {
				returnVector.X += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * -1;
			}
			else if (OutHitResult.ImpactPoint.X < fTargetPosition.X) {

				returnVector.X += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * 1;
			}

			if (OutHitResult.ImpactPoint.Y > fTargetPosition.Y) {
				returnVector.Y += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * -1;
			}
			else if (OutHitResult.ImpactPoint.Y < fTargetPosition.Y) {

				returnVector.Y += (1 / (distanceToBound * (1 / pBFType->AvoidForceMultiplier))) * 1;
			}

			returnVector.Normalize( );
			FVector avoidance = returnVector * pBFType->AvoidanceForce;
			return avoidance;
		}
		return FVector( 0, 0, 0 );
	}
	//BoatFormationSystem
	void update( SimEcs_Registry &registry, float dt ) override
	{
		assert( OwnerActor );
		SCOPE_CYCLE_COUNTER( STAT_BoatFormation );
		int NowPlatformTime = FPlatformTime::Seconds( );
		bool bRewriteTime = false;
		registry.view<FOceanShip, FFormation, FPosition>( ).each( [&, dt]( auto entity, FOceanShip & ex, FFormation & formation, FPosition& pos ) {
			if (formation.GroupName.IsEmpty( ))return;
			auto boatFormationInfo = USimOceanSceneManager_Singleton::GetInstance( )->m_TTMapBoatFormationInfo.Find( formation.GroupName );
			if (boatFormationInfo) {
				USimOceanSceneManager_Singleton::BoatFormationStruct* itemLafe = boatFormationInfo->Find( entity );
				if (itemLafe->IsLeader) {
					if (ex.MoveMode <= BoatMoveMode::EBoatMoveMode_Back) {
						NextLeaderTargetPoint = ex.MoveOnPos;
						if (NowPlatformTime - FormationLastTime > FormationCoolDownTime) {
							SampleLeaderPoints( pos.pos, NextLeaderTargetPoint );
							float fLeaderHeadingDegrees = FMath::RadiansToDegrees( itemLafe->ForwardVector.HeadingAngle( ) );
							FTransform leaderTrans( FQuat::MakeFromEuler( FVector( 0.0, 0.0, fLeaderHeadingDegrees ) ), SamplePoint );
							//LeaderFormation( leaderTrans );
							m_SimDelegateFormation.RunFormation( leaderTrans, EBoatFormation( formation.FormationValue ) );//formation.FormationValue
							FormationLastTime = NowPlatformTime;
						}
					}
					return;
				}

				if ((NowPlatformTime - FormationChildLastTime > FormationCoolDownTime)) {
					bRewriteTime = true;
					FVector shipLocation = FVector::ZeroVector;
					if (ex.MainMeshComponent) {
						ex.MainMeshComponent->GetComponentLocation( );
						shipLocation = AvoidObstacle( ex, itemLafe, shipLocation );
						shipLocation.Z = 0.0f;
						UWorld* World = GEngine->GameViewport->GetWorld( );
						if (!World)return;
						FVector fCurrentPosition = ex.MainMeshComponent->GetComponentLocation( );
						//DrawDebugLine( World, fCurrentPosition, ex.MoveOnPos + FVector::UpVector * 100.0f, FColor::Red, true, 5.0f );
					}
					FVector fTargetPosition = FVector( itemLafe->BoatTargetPosition ) + itemLafe->ExpectAvoidOffect;
					USimOceanSceneManager_Singleton::GetInstance( )->MoveEntity( formation.GroupName, entity, fTargetPosition );

				}
			}
		} );
		if (bRewriteTime) {
			FormationChildLastTime = NowPlatformTime;
		}
	}
};


