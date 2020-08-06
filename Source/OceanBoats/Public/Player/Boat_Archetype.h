// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OceanCommonTypes.h"
#include "BoatInterface.h"
#include "SimEntitasBase/SimEcs_Archetype.h"
#include "Boat_Archetype.generated.h"

/**
 * 
 */
UCLASS()
class OCEANBOATS_API ABoat_Archetype : public ASimEcs_Archetype, public IBoatInterface
{
	GENERATED_BODY()
public:
	ABoat_Archetype();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		TArray<UArrowComponent*> Cannons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
		TArray<UArrowComponent*> CannonsTD;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float StepSpeed = 0.01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float SteeringSpeed = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float ForwardSpeed = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float TurnStep = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		float BackMoveTime = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		ATargetPoint* MoveTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		ATargetPoint* BackTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Move")
		ATargetPoint* AttackTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float AttackDelay = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float Damage = 120;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float DamageRadius = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float HitRadialImpulse = 150;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float ProjectileVelocity = 1200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float CannonRecoilImpulse = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float LaserTime = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
		float FireStepTime = 0.02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
		float MoveSpeed = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
		float MoveDistance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
		float AccSpeedDownDistance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
		float AccRollbakDistance = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
	float ForwardAxisValue;

public:
	///IBoatInerface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetMoveMode(OceanMoveMode mode);
	virtual void SetMoveMode_Implementation(OceanMoveMode mode);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void FireImpl(int killerUID, FTransform trans, ExplodeType type);
	virtual void FireImpl_Implementation(int killerUID, FTransform trans, ExplodeType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetDestroyNum(ObstacleType type);
	virtual int GetDestroyNum_Implementation(ObstacleType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void RecordDestroy(ObstacleType type);
	virtual void RecordDestroy_Implementation(ObstacleType type);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSpeed();
	virtual float GetSpeed_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSailDistance();
	virtual float GetSailDistance_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSailAngle();
	virtual float GetSailAngle_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetSpeedDownDistance();
	virtual float GetSpeedDownDistance_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetHorizontalDistance();
	virtual float GetHorizontalDistance_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetRollbackDistance();
	virtual float GetRollbackDistance_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		float GetRollbackAngle();
	virtual float GetRollbackAngle_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_MoveOn(FVector targetPos = FVector::ZeroVector);
	virtual void Active_MoveOn_Implementation(FVector targetPos = FVector::ZeroVector);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_MoveBack(FVector targetPos = FVector::ZeroVector);
	virtual void Active_MoveBack_Implementation(FVector targetPos = FVector::ZeroVector);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void Active_HoldOn(FVector nextTargetPos = FVector::ZeroVector);
	virtual void Active_HoldOn_Implementation(FVector nextTargetPos = FVector::ZeroVector);
	///
	UFUNCTION(BlueprintCallable, Category = "Game")
		void TickMove(FVector loc, UPrimitiveComponent* root);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void MainLoopLogic();
	UFUNCTION(BlueprintCallable, Category = "Game")
		OceanMoveMode GetMoveMode();
	UFUNCTION(BlueprintCallable, Category = "Game")
		void SetMainMesh(UStaticMeshComponent* mainMesh);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void RuntimeStateLoop();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableBuoyancyForce(bool bEnable);
	virtual void EnableBuoyancyForce_Implementation(bool bEnable);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableEffect(bool bEnable);
	virtual void EnableEffect_Implementation(bool bEnable);

protected:
	void CheckSpeedUp();
	void MoveLoopImpl(/*ATargetPoint* target*/);
	void SpeedUp();
	void SpeedDown();

	float FindLookAtRotation_Yaw( const FVector& Start, const FVector& Target );
public:
	
	void Active_Attack();
	void Active_Turn();	

protected:
	FRotator OldLookAtRotation;
	float RightAxisValue;
	float CurrentSpeed;
	float ExpectSpeed;
	bool FireEnd = false;
	int FireIndex = 0;
	float FireTime;
	int TurnIndex = 0;
	bool ReverseMoveEnd = false;
	TMap<ObstacleType, int> DestroyResult;
	TSharedPtr<UPrimitiveComponent> RootCom;
	FVector LastPos;
	bool bSpeedDown = false;
	bool bRollBack = false;

	FVector MoveOnPos;
	FVector MoveBackPos;
	FVector AttackPos;

	FVector2D tempV1;
	FVector2D tempV2;

	OceanMoveMode MoveMode = OceanMoveMode::EMoveMode_Idle;
	FTimerHandle DelayHandler;
	UStaticMeshComponent* MainMeshComponent;
};
