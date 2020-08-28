
// Fill out your copyright notice in the Description page of Project Settings.
/**
* Copyright 2019 北京伯睿科技有限公司.
*
* @File Data Struct.h
* @Description Root Actor
*
* @Author magi
* @Date 2020年3月20日
* @Version 1.0
*/

#pragma once
#include "Engine/EngineTypes.h"
#include "SimEcs_Platform.h"
#include "SimEcs_BattleComponentsStructData.generated.h"

UENUM()
enum BoatMoveMode
{
	EBoatMoveMode_On,
	EBoatMoveMode_Back,
	EBoatMoveMode_Fire,/*
	EMoveMode_SpeedUp,
	EMoveMode_SpeedDown,*/
	EBoatMoveMode_Idle
};


//代理生成坐标点
USTRUCT( BlueprintType )
struct FPrePosition {

	GENERATED_BODY( )

		FPrePosition( ) : pos( FVector::ZeroVector ) {}
	FPrePosition( FVector _pos ) :pos( _pos ) {  };

	void Add( FVector offset ) { pos += offset; }

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FVector pos;
};



USTRUCT( BlueprintType )
struct FPosition {

	GENERATED_BODY( )

		FPosition( ) : pos( FVector::ZeroVector ) {}
	FPosition( FVector _pos ) :pos( _pos ) {  };

	void Add( FVector offset ) { pos += offset; }

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FVector pos;
};

USTRUCT( BlueprintType )
struct FScale {
	GENERATED_BODY( )

		FScale( ) : scale( FVector( 1.0, 1.0, 1.0 ) ) {}
	FScale( FVector _scale ) :scale( _scale ) {};

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FVector scale;
};



class UStaticMesh;
class UMaterial;

USTRUCT( BlueprintType )
struct FMovement {

	GENERATED_BODY( )

		UPROPERTY( EditAnywhere, Category = SimEcs )
		float GravityStrenght;
};

USTRUCT( BlueprintType )
struct FInstancedStaticMesh {
	GENERATED_BODY( )

		FInstancedStaticMesh( UStaticMesh* _mesh = nullptr, UMaterial* _material = nullptr ) : mesh( _mesh ), material( _material ) {}

	UPROPERTY( EditAnywhere, Category = SimEcs )
		UStaticMesh* mesh;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		UMaterial * material;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		FTransform RelativeTransform;

	FTransform RenderTransform;


	int instanceIndex{ -1 };
};

USTRUCT( BlueprintType )
struct FEntForce {
	GENERATED_BODY( )


	UPROPERTY( EditAnywhere, Category = SimEcs )
	FVector Direction;

	// Application point of the force
	UPROPERTY( EditAnywhere, Category = SimEcs )
	FVector Point;

	FEntForce( ) : Direction( FVector::ZeroVector ), Point( FVector::ZeroVector ) {}
	FEntForce( const FVector& dir, const FVector& point ) : Direction( dir ), Point( point )
	{
	}
	void ChangeForce( FVector dir ) { Direction = dir; }

	void ChangeForce( FVector dir, FVector point ) {
		Direction = dir; Point = point;
	}

};

USTRUCT( BlueprintType )
struct FVelocity {

	GENERATED_BODY( )

		FVelocity( ) : vel( FVector::ZeroVector ) {}

	FVelocity( FVector _vel ) : vel( _vel ) { };

	void Add( FVector _vel ) { vel += _vel; }

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FVector vel;
};

USTRUCT( BlueprintType )
struct FLastPosition {

	GENERATED_BODY( )

		FLastPosition( ) : pos( FVector::ZeroVector ) {}

	FLastPosition( FVector _pos ) : pos( _pos ) { };

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FVector pos;
};

USTRUCT( BlueprintType )
struct FMovementRaycast {

	GENERATED_BODY( )

		FMovementRaycast( ) {};
	FMovementRaycast( TEnumAsByte<ECollisionChannel> _trace ) : RayChannel( _trace ) { };

	UPROPERTY( EditAnywhere, Category = SimEcs )
		TEnumAsByte<ECollisionChannel> RayChannel;
};

struct FRaycastResult {


	FRaycastResult( FTraceHandle _handle ) :handle( _handle ) { };

	FTraceHandle handle;

};

USTRUCT(BlueprintType)
struct FBarrierFixedRaycastResult {

	GENERATED_BODY()
	FBarrierFixedRaycastResult() {};
	FBarrierFixedRaycastResult(TEnumAsByte<ECollisionChannel> _trace) : RayChannel(_trace), Distance(100000.0f){ };

	UPROPERTY(EditAnywhere, Category = SimEcs)
		TEnumAsByte<ECollisionChannel> RayChannel;


	UPROPERTY( EditAnywhere, Category = SimEcs )
		float Distance;

};

struct FGridMap {
	FIntVector GridLocation;
};

USTRUCT( BlueprintType )
struct FRotationComponent {
	GENERATED_BODY( )

		FRotationComponent( ) { rot = FQuat( ); }
	FRotationComponent( FQuat _rot ) : rot( _rot ) {}
	FRotationComponent( FRotator _rot ) { rot = _rot.Quaternion( ); }

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FQuat rot;
};

USTRUCT( BlueprintType )
struct FRandomArcSpawn {
	GENERATED_BODY( )

		UPROPERTY( EditAnywhere, Category = SimEcs )
		float MinAngle;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		float MaxAngle;

	UPROPERTY( EditAnywhere, Category = SimEcs )
		float MinVelocity;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		float MaxVelocity;
};


USTRUCT( BlueprintType )
struct FDebugSphere {
	GENERATED_BODY( )

		UPROPERTY( EditAnywhere, Category = SimEcs )
		float radius;

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FColor color;
};



USTRUCT( BlueprintType )
struct FActorReference {
	GENERATED_BODY( )
		FActorReference( AActor * Owner = nullptr ) { ptr = Owner; }

	TWeakObjectPtr<AActor> ptr;

};
USTRUCT( BlueprintType )
struct FLifetime {
	GENERATED_BODY( )
		FLifetime( ) :LifeLeft( 0.0 ) {}

	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		float LifeLeft;
};

USTRUCT( BlueprintType )
struct FSimulatePhysical {
	GENERATED_BODY( )
		FSimulatePhysical( ) :LifeLeft( 0.0f ), SecLifeLeft(0.0f),bSimulatePhysical( false), bJumpOne(false){}

	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		float LifeLeft;
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		float SecLifeLeft;
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		bool bSimulatePhysical;
	UPROPERTY( EditAnywhere, Category = "SimEcs" )
		bool bJumpOne;
};


struct FDestroy {

};

USTRUCT( BlueprintType )
struct FCopyTransformToECS {
	GENERATED_BODY( )
		FCopyTransformToECS( ) : bWorldSpace( true ) {};

	UPROPERTY( EditAnywhere, Category = SimEcs )
		bool bWorldSpace;
};
USTRUCT( BlueprintType )
struct FActorTransform {
	GENERATED_BODY( )
		FActorTransform( ) {};
	FActorTransform( FTransform trns ) : transform( trns ) {};

	UPROPERTY( EditAnywhere, Category = SimEcs )
		FTransform transform;
};
USTRUCT( BlueprintType )
struct FCopyTransformToActor {
	GENERATED_BODY( )

		FCopyTransformToActor( ) : bWorldSpace( true ), bSweep( false ) {};

	UPROPERTY( EditAnywhere, Category = SimEcs )
		bool bWorldSpace;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		bool bSweep;
};
class ASimEcs_Archetype;
USTRUCT( BlueprintType )
struct FArchetypeSpawner {
	GENERATED_BODY( )

		UPROPERTY( EditAnywhere, Category = SimEcs )
		TSubclassOf<ASimEcs_Archetype> ArchetypeClass;

	UPROPERTY( EditAnywhere, Category = SimEcs )
		float TimeUntilSpawn;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		float SpawnRate;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		bool bLoopSpawn;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		int32 ActorType;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		bool isLeader;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		FName Name;
	UPROPERTY( EditAnywhere, Category = SimEcs )
		FString GroupName;

	using EntityHandleId = uint64_t;
	EntityHandleId   entHandleId;
	

};


UENUM(BlueprintType)
enum class EFaction :uint8 {
	Red,
	Blue,
	Neutral
};

USTRUCT(BlueprintType)
struct FFaction {
	GENERATED_BODY()

		FFaction(EFaction _fact) : faction(_fact) {};
		FFaction() : faction(EFaction::Neutral){};

	
	UPROPERTY(EditAnywhere, Category = SimEcs)
		EFaction faction;
};

USTRUCT(BlueprintType)
struct FOceanShip {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float AvoidanceStrenght;
	UPROPERTY(EditAnywhere)
		float MaxVelocity;

	UPROPERTY(EditAnywhere)
	FVector TargetMoveLocation;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float BoatMass = 5000.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float StepSpeed = 0.01;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float SteeringSpeed = 200;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float ForwardSpeed = 1200;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float TurnStep = 3;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float AttackDelay = 1;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float Damage = 120;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float DamageRadius = 1200;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float HitRadialImpulse = 150;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float ProjectileVelocity = 1200;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float CannonRecoilImpulse = 5;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float LaserTime = 2;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float FireStepTime = 0.02;

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float MoveSpeed = 0.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float MoveDistance = 0.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float AccSpeedDownDistance = 0.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float AccRollbakDistance = 0.0f;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		bool isLeader  = false;
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		FVector UnderwaterMax = FVector(75.0, 200.0,75.0f);
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		FVector UnderwaterMin = FVector( -75.0, -200.0f, -75.0f );
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = "OceanShip" )
		float UnderwaterBoxLength = 450.0f;
		
	BoatMoveMode MoveMode = BoatMoveMode::EBoatMoveMode_Idle;
	UStaticMeshComponent* MainMeshComponent;
	float ForwardAxisValue;
	float RightAxisValue;
	float CurrentSpeed = 0;
	float ExpectSpeed = 0;
	bool FireEnd = false;
	int FireIndex = 0;
	float FireTime;
	int TurnIndex = 0;
	bool ReverseMoveEnd = false;
	bool bSpeedDown = false;
	bool bRollBack = false;
	FName Name;
	FVector LastPos;
	FVector MoveOnPos;
	FVector MoveBackPos;
	FVector AttackPos;
	FTimerHandle DelayHandler;
};

USTRUCT(BlueprintType)
struct FProjectile {
	GENERATED_BODY()

		UPROPERTY(EditAnywhere)
		float HeatSeekStrenght;
	UPROPERTY(EditAnywhere)
		float MaxVelocity;
	UPROPERTY(EditAnywhere, Category = SimEcs)
		TSubclassOf<ASimEcs_Archetype> ExplosionArchetypeClass;
};

USTRUCT(BlueprintType)
struct FExplosion {
	GENERATED_BODY()

		float LiveTime;

		UPROPERTY(EditAnywhere)
		float Duration;
	UPROPERTY(EditAnywhere)
		float MaxScale;
};


USTRUCT(BlueprintType)
struct FHealth {
	GENERATED_BODY()

		
	FHealth(float _h = 0.f) : Health(_h) {};

	UPROPERTY(EditAnywhere)
		float Health;
};

USTRUCT( BlueprintType )
struct FFormation {
	GENERATED_BODY( )

	UPROPERTY( EditAnywhere )
		int32 FormationValue = 1;
	UPROPERTY(EditAnywhere)
		FString GroupName;
};


USTRUCT( BlueprintType )
struct FAnimation {
	GENERATED_BODY( )

	UPROPERTY( EditAnywhere )
		int32 Time = 10;
	UPROPERTY( EditAnywhere )
		FString AnimName;   //动画名称
	UPROPERTY( EditAnywhere )
		FString PartName;   //太阳帆板

	    
};

