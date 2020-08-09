
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

DECLARE_CYCLE_STAT(TEXT("SimEcs: Explosion System"), STAT_Explosion, STATGROUP_ECS);

struct ExplosionSystem :public SystemT {

	const float UpdateRate = 0.1;

	float elapsed = 0;

	void update(SimEcs_Registry &registry, float dt) override
	{
		assert(OwnerActor);
		SCOPE_CYCLE_COUNTER(STAT_Explosion);

		auto AllExplosionsView = registry.view<FExplosion>();
		for (auto e : AllExplosionsView) {
			FExplosion & ex = AllExplosionsView.get(e);

			ex.LiveTime += dt;
			if (ex.LiveTime > ex.Duration)
			{
				registry.assign<FDestroy>(e);
			}
		}


		registry.view<FExplosion, FScale>().each([&, dt](auto entity, FExplosion & ex, FScale & scale) {
			

			scale.scale = FVector( (ex.LiveTime / ex.Duration)*ex.MaxScale);		
			
		});


	}
};

struct OceanShipSystem :public SystemT {
	
	FRotator MakeRotFromX(const FVector& X)
	{
		return FRotationMatrix::MakeFromX(X).Rotator();
	}

	FRotator FindLookAtRotation(const FVector& Start, const FVector& Target)
	{
		const FVector fLerpValue = FMath::Lerp(Target, Start, 0.001);
		FVector fValue = Target - fLerpValue;
		fValue.Z = 0.0;
		float dist = FVector::Dist(Target, Start);
		if (dist < 5000.0f) {
			MakeRotFromX(fValue);
		}
		return MakeRotFromX(fValue);
	}

	//void TickMove(FOceanShip& ship, FVector loc, UPrimitiveComponent* root)
	//{
	//	if (ship.MoveMode == EBoatMoveMode_Idle)
	//		return;

	//	root->AddForceAtLocation(root->GetRightVector() * ship.RightAxisValue * root->GetMass() * ship.SteeringSpeed, loc);
	//	root->AddForce(root->GetForwardVector() * root->GetMass() * ship.ForwardAxisValue * ship.ForwardSpeed);
	//}

	void CheckSpeedUp(FOceanShip& ship)
	{
		if (ship.MoveMode == EBoatMoveMode_On || ship.MoveMode == EBoatMoveMode_Back)
		{
			if (abs(ship.CurrentSpeed - ship.ExpectSpeed) > ship.StepSpeed*0.99)
			{
				if (ship.CurrentSpeed > ship.ExpectSpeed)
				{
					ship.bSpeedDown = true;
					ship.CurrentSpeed = (ship.CurrentSpeed - ship.StepSpeed) > 0 ? (ship.CurrentSpeed - ship.StepSpeed) : 0;
				}
				else
				{
					ship.bSpeedDown = false;
					ship.CurrentSpeed = (ship.CurrentSpeed + ship.StepSpeed) < ship.ExpectSpeed ? (ship.CurrentSpeed + ship.StepSpeed) : ship.ExpectSpeed;
				}
			}
			else//fire
			{
				//if (MoveMode == EMoveMode_On && (abs(CurrentSpeed) < StepSpeed*0.99))
				//{
				//	SetMoveMode(EMoveMode_Idle);
				//}
			}
		}
	}

	void MainLoopLogic(FOceanShip& ship, FVector forceLocation, UStaticMeshComponent* root)
	{
		ship.MainMeshComponent = root;
		if (ship.MoveMode == EBoatMoveMode_Idle)
			return;

		root->AddForceAtLocation(root->GetRightVector() * ship.RightAxisValue * root->GetMass() * ship.SteeringSpeed, forceLocation);
		root->AddForce(root->GetForwardVector() * root->GetMass() * ship.ForwardAxisValue * ship.ForwardSpeed);

		ship.ForwardAxisValue = ship.CurrentSpeed;
		CheckSpeedUp(ship);

		if (ship.MoveMode == EBoatMoveMode_On)
		{
			ship.bRollBack = false;
			FRotator rot;
			rot.Yaw = FindLookAtRotation(ship.MainMeshComponent->GetComponentLocation(), ship.MoveOnPos).Yaw;

			if (ship.MainMeshComponent != NULL)
			{
				ship.MainMeshComponent->SetWorldRotation(rot);
			}
		}
	}

	void CatchBoat(FOceanShip& ship)
	{
		GEngine->AddOnScreenDebugMessage
		(
			0,
			2, 			//	显示的时间/秒
			FColor::Yellow, 	//	显示的颜色
			FString("No BoatAcheTypep---") + FString::FromInt(ship.MoveMode)	//	显示的信息
		);
	}

	void Active_MoveOn(FOceanShip& ship, FVector targetPos = FVector::ZeroVector)
	{
		ship.MoveOnPos = targetPos;
		ship.MoveMode = BoatMoveMode::EBoatMoveMode_On;
		ship.ExpectSpeed = 1.0f;
		ship.MainMeshComponent->SetSimulatePhysics(true);
		//EnableBuoyancyForce(true);
		//EnableEffect(true);
	}

	void  HoldOn(FVector nextTargetPos = FVector::ZeroVector)
	{

	}

	void update(SimEcs_Registry &registry, float dt) override
	{
		assert(OwnerActor);
		SCOPE_CYCLE_COUNTER(STAT_Explosion);

		registry.view<FOceanShip, FRotationComponent,FVelocity>().each([&, dt](auto entity, FOceanShip & ship, FRotationComponent & rotation, FVelocity&vel) {
			ArchetypeSpawnerSystem* SpawnerSystem= static_cast<ArchetypeSpawnerSystem*>( World->GetArchetypeSpawnerSystem( ) );
		FVector moveOnPos(-180180 + 5000, -370000 + 5000, -6600);
		//rotation.rot = vel.vel.Rotation().Quaternion();
		TSharedPtr<ASimEcs_Archetype>* boat = USimOceanSceneManager_Singleton::GetInstance()->m_MapArchetypes.Find(entity);
		if (boat)
		{			
			boat->Get()->EntityId = entity;
			MainLoopLogic(ship, boat->Get()->ForceLocation, Cast<UStaticMeshComponent>(boat->Get()->GetRootComponent()));
			
			if(ship.MoveMode == EBoatMoveMode_Idle)
			{
				Active_MoveOn(ship, moveOnPos);
			}
			if(ship.MoveMode == EBoatMoveMode_On)
			{
				boat->Get()->EnableWaveForce(true);
				boat->Get()->EnableBoatEffect(true);
			}

			GEngine->AddOnScreenDebugMessage
			(
				0,
				2, 			//	显示的时间/秒
				FColor::Yellow, 	//	显示的颜色
				FString("----Ship Mode-----")+FString::FromInt(ship.MoveMode)	//	显示的信息
			);
		}
		else
		{
			CatchBoat(ship);
		}
			
		});


	}
};


DECLARE_CYCLE_STAT(TEXT("SimEcs: Boids Update"), STAT_Boids, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("SimEcs: Gridmap Update"), STAT_GridmapUpdate, STATGROUP_ECS);

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

	void AddToGridmap(EntityHandle ent, FPosition&pos)
	{
		const FIntVector GridLoc = FIntVector(pos.pos / GRID_DIMENSION);
		auto SearchGrid = GridMap.Find(GridLoc);

		GridItem item;
		item.ID = ent;
		item.Position = pos.pos;

		if (World->GetRegistry()->has<FFaction>(ent.handle))
		{
			item.Faction = World->GetRegistry()->get<FFaction>(ent.handle).faction;
		}
		else
		{
			item.Faction = EFaction::Neutral;
		}


		if (!SearchGrid)
		{
			TArray<GridItem> NewGrid;

			NewGrid.Reserve(10);
			NewGrid.Add(item);

			GridMap.Emplace(GridLoc, std::move(NewGrid));

		}
		else
		{
			SearchGrid->Add(item);
		}
	}
	void Foreach_EntitiesInRadius(float radius, FVector origin, TFunctionRef<void(GridItem&)> Body)
	{
		const float radSquared = radius * radius;
		const FVector RadVector(radius, radius, radius);
		const FIntVector GridLoc = FIntVector(origin / GRID_DIMENSION);
		const FIntVector MinGrid = FIntVector((origin - RadVector) / GRID_DIMENSION);
		const FIntVector MaxGrid = FIntVector((origin + RadVector) / GRID_DIMENSION);


		for (int x = MinGrid.X; x <= MaxGrid.X; x++) {
			for (int y = MinGrid.Y; y <= MaxGrid.Y; y++) {
				for (int z = MinGrid.Z; z <= MaxGrid.Z; z++) {
					const FIntVector SearchLoc(x, y, z);
					const auto SearchGrid = GridMap.Find(SearchLoc);
					if (SearchGrid)
					{
						for (auto e : *SearchGrid)
						{
							if (FVector::DistSquared(e.Position, origin) < radSquared)
							{
								Body(e);

							}
						}
					}
				}
			}
		}
	}

	void update(SimEcs_Registry &registry, float dt) override
	{

		//add everything to the gridmap
		GridMap.Empty(50);

		auto GridView = registry.view<FGridMap, FPosition>();

		{
			SCOPE_CYCLE_COUNTER(STAT_GridmapUpdate);
			for (auto e : GridView)
			{
				EntityHandle handle(e);

				AddToGridmap(handle, GridView.get<FPosition>(e));
			}
		}
		

		{
			SCOPE_CYCLE_COUNTER(STAT_Boids);

			TypedLinearMemory<ProjectileData> ProjArray(World->ScratchPad);

			auto ProjectileView = registry.view<FProjectile, FPosition, FVelocity, FFaction>(entt::persistent_t{});
			//ProjArray.Reset();
			unsigned int NumProjectiles = 0;
			//copy projectile data into array so we can do a parallel update later
			for (auto e : ProjectileView)
			{
				ProjectileData Projectile;
				Projectile.faction = ProjectileView.get<FFaction>(e);
				Projectile.pos = ProjectileView.get<FPosition>(e);
				Projectile.proj = ProjectileView.get<FProjectile>(e);

				Projectile.vel = &ProjectileView.get<FVelocity>(e);
				ProjArray.push_back(Projectile);
				NumProjectiles++;
				//ProjArray.Add(Projectile);
			}


			ParallelFor(NumProjectiles, [&](int32 Index)
			{
				ProjectileData data = ProjArray[Index];

				//unpack projectile data
				const FVector ProjPosition = data.pos.pos;
				const EFaction ProjFaction = data.faction.faction;
				const float ProjSeekStrenght = data.proj.HeatSeekStrenght;
				const float ProjMaxVelocity = data.proj.MaxVelocity;
				FVector & ProjVelocity = data.vel->vel;


				const float ProjCheckRadius = 1000;
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
		//its not good to have both spaceship and projectile logic here, they should be on their own systems
		{
			SCOPE_CYCLE_COUNTER(STAT_Boids);


			//auto spaceshipEnd = SpaceshipView.size()
			
			//int nShips = registry.view<FOceanShip>().size();
			auto SpaceshipView = registry.view<FOceanShip, FPosition, FVelocity, FFaction>(entt::persistent_t{});

			TypedLinearMemory<SpaceshipData> SpaceshipArray(World->ScratchPad);
			unsigned int NumShips = 0;
			//SpaceshipArray.Reset();// Spaces//nShips);
			//copy spaceship data into array so we can do a paralle update later
			for (auto e : SpaceshipView)
			{
				SpaceshipData Ship;
				Ship.faction = SpaceshipView.get<FFaction>(e);
				Ship.pos = SpaceshipView.get<FPosition>(e);
				Ship.ship = SpaceshipView.get<FOceanShip>(e);

				Ship.vel = &SpaceshipView.get<FVelocity>(e);
				NumShips++;
				SpaceshipArray.push_back(Ship);
				//SpaceshipArray.Add(Ship);
			}

			ParallelFor(NumShips, [&](int32 Index)
			{
				SpaceshipData data = SpaceshipArray[Index];

				//unpack ship variables from the array
				const FVector ShipPosition = data.pos.pos;
				const EFaction ShipFaction = data.faction.faction;
				const float ShipAvoidanceStrenght = data.ship.AvoidanceStrenght;
				const float ShipMaxVelocity = data.ship.MaxVelocity;
				FVector & ShipVelocity = data.vel->vel;
				const FVector ShipTarget = data.ship.TargetMoveLocation;

				const float shipCheckRadius = 1000;
				Foreach_EntitiesInRadius(shipCheckRadius, ShipPosition, [&](GridItem& item) {

					if (item.Faction == ShipFaction)
					{
						const FVector TestPosition = item.Position;

						const float DistSquared = FVector::DistSquared(TestPosition, ShipPosition);

						const float AvoidanceDistance = shipCheckRadius * shipCheckRadius;
						const float DistStrenght = FMath::Clamp(1.0 - (DistSquared / (AvoidanceDistance)), 0.1, 1.0) * dt;
						const FVector AvoidanceDirection = ShipPosition - TestPosition;

						ShipVelocity += AvoidanceDirection.GetSafeNormal() * ShipAvoidanceStrenght*DistStrenght;
					}
				});

				FVector ToTarget = ShipTarget - ShipPosition;
				ToTarget.Normalize();

				ShipVelocity += (ToTarget * 500 * dt);
				ShipVelocity = ShipVelocity.GetClampedToMaxSize(ShipMaxVelocity);
			});

			
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
		if (bDebugGridMap)
		{
			const float MaxUnits = 5.0;
			for (auto& g : GridMap)
			{
				const FVector BoxMin = FVector(g.Key) * GRID_DIMENSION;
				const FVector BoxMax = BoxMin + FVector(GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION);
				const FVector BoxCenter = BoxMin + FVector(GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION) / 2.0;
				const FColor BoxColor = FColor::MakeRedToGreenColorFromScalar(g.Value.Num() / MaxUnits);
				DrawDebugBox(OwnerActor->GetWorld(), BoxCenter, FVector(GRID_DIMENSION, GRID_DIMENSION, GRID_DIMENSION) / 2.0, BoxColor);
			}
		}

	}
};