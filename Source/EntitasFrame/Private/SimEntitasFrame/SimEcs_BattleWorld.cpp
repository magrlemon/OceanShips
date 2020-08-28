// Fill out your copyright notice in the Description page of Project Settings.

#include "SimEcs_BattleWorld.h"
#include "SimEcs_Core.h"
#include "SimEcs_BaseSystems.h"
#include "SimEcs_BattleSystems.h"


// Sets default values
ASimEcs_WorldActor::ASimEcs_WorldActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASimEcs_WorldActor::BeginPlay()
{
	Super::BeginPlay();

		ECSWorld = MakeUnique<SimEcs_World>();

		//ECSWorld->CreateAndRegisterSystem<CopyTransformToECSSystem>();
		//ECSWorld->CreateAndRegisterSystem<BoidSystem>();
		ECSWorld->CreateAndRegisterSystem<MovementSystem>();
		ECSWorld->CreateAndRegisterSystem<BoatsMovementSystem>( );
		ECSWorld->CreateAndRegisterSystem<ExplosionSystem>();
		ECSWorld->CreateAndRegisterSystem<OceanShipSystem>();
		ECSWorld->CreateAndRegisterSystem<AvoidObstacleSystem>( );
		ECSWorld->CreateAndRegisterSystem<BoatFormationSystem>( );
		ECSWorld->CreateAndRegisterSystem<RaycastSystem>();
		ECSWorld->CreateAndRegisterSystem<BarrierFixedRaycastSystem>();
		ECSWorld->CreateAndRegisterSystem<LifetimeSystem>();
		ECSWorld->CreateAndRegisterSystem<SwitchSimulatePhysicalSystem>( );
		ECSWorld->CreateAndRegisterSystem<StaticMeshDrawSystem>();
		//ECSWorld->CreateAndRegisterSystem<DebugDrawSystem>();
		//ECSWorld->CreateAndRegisterSystem<CopyTransformToActorSystem>();
		ECSWorld->CreateAndRegisterSystem<ArchetypeSpawnerSystem>();		
		ECSWorld->InitializeSystems(this);

}


// Called every frame
void ASimEcs_WorldActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ECSWorld->UpdateSystems(DeltaTime);

}

