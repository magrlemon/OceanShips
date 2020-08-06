// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/SimEcs_OceanCommonTypes.h"
#include "Interface/SimEcs_BoatInterface.h"
#include "SimEntitasBase/SimEcs_Archetype.h"
#include "SimEcs_BoatArchetype.generated.h"

/**
 * 
 */
UCLASS()
class ENTITASFRAME_API ASimEcs_BoatArchetype : public ASimEcs_Archetype, public ISimEcs_BoatInterface
{
	GENERATED_BODY()
public:
	ASimEcs_BoatArchetype();
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

public:
	///IBoatInerface
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetMoveMode( ESimOceanMoveMode mode);
	virtual void SetMoveMode_Implementation( ESimOceanMoveMode mode);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedUp();
	virtual void SpeedUp_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SpeedDown();
	virtual void SpeedDown_Implementation();
	/*UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void FireImpl(int killerUID, FTransform trans, ExplodeType type);
	virtual void FireImpl_Implementation(int killerUID, FTransform trans, ESimExplodeType type);*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		int GetDestroyNum(ESimObstacleType type);
	virtual int GetDestroyNum_Implementation(ESimObstacleType type);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void RecordDestroy(ESimObstacleType type);
	virtual void RecordDestroy_Implementation(ESimObstacleType type);

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
	///
	UFUNCTION(BlueprintCallable, Category = "Game")
		void TickMove(FVector loc, UPrimitiveComponent* root);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void MainLoopLogic();
	UFUNCTION(BlueprintCallable, Category = "Game")
		ESimOceanMoveMode GetMoveMode();
	UFUNCTION(BlueprintCallable, Category = "Game")
		void SetMainMesh(UStaticMeshComponent* mainMesh);
	UFUNCTION(BlueprintCallable, Category = "Game")
		void RuntimeStateLoop();

protected:
	void CheckSpeedUp();
	void MoveLoopImpl(ATargetPoint* target);
public:
	void Active_MoveOn(FVector targetPos = FVector::ZeroVector);
	void Active_Attack();
	void Active_Turn();
	void Active_MoveBack(FVector targetPos = FVector::ZeroVector);
	bool IsMoveing() { return Moveing; }
	

protected:
	float ForwardAxisValue;
	float RightAxisValue;
	float CurrentSpeed;
	float ExpectSpeed;
	bool FireEnd = false;
	int FireIndex = 0;
	float FireTime;
	int TurnIndex = 0;
	bool Moveing = false;
	bool ReverseMoveEnd = false;
	TMap<ESimObstacleType, int> DestroyResult;
	TSharedPtr<UPrimitiveComponent> RootCom;
	FVector LastPos;
	bool bSpeedDown = false;
	bool bRollBack = false;

	FVector MoveOnPos;
	FVector MoveBackPos;
	FVector AttackPos;

	ESimOceanMoveMode MoveMode = ESimOceanMoveMode::ESimMoveMode_Idle;
	FTimerHandle DelayHandler;
	UStaticMeshComponent* MainMeshComponent;
};
