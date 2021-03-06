/** Copyright 2019 北京伯睿科技有限公司.
*	Author :	magi
	Date   :	11th July, 2019
	File   :	SimEcs_Archetype.h
	Summary:	Declares the  assest resource.
	All Rights Reserved.
	*http://www.borui.com	(c) Copyright 2019.
*/
// Fill out your copyright notice in the Description page of Project Settings.


#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Info.h"
#include "SimEcs_Core.h"
#include "Sim_FsmStateDataBase.h"
#include "SimApi.h"
#include "SimEcs_Archetype.generated.h"


class SimEcs_World;

//base class for pure entity archetypes. This class cant be used directly, but can be spawned from an SimEcs spawner type system
UCLASS(Blueprintable)
class ENTITASFRAME_API ASimEcs_Archetype :  public AActor
{
	GENERATED_BODY()
public:	
	ASimEcs_Archetype();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OceanShip")
		FVector ForceLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Normal")
		bool AVoided = false;

	UFUNCTION(BlueprintCallable, Category = "Game")
		void Avoid();

	UFUNCTION(BlueprintCallable, Category = "Game")
		void GrapMesh(UStaticMeshComponent* mainMesh);
	virtual void CreateNewEntityFromThis( uint64  handleID );
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableWaveForce(bool enable);
	virtual void EnableWaveForce_Implementation(bool enable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableBoatEffect(bool enable);
	virtual void EnableBoatEffect_Implementation(bool enable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void StartFire();
	virtual void StartFire_Implementation( );

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void ResetParticelSize(float scale);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void HitDamge(float damage);
	UFUNCTION(BlueprintCallable, Category = "Game")
		float GetHealthRange();
	UFUNCTION(BlueprintCallable, Category = "Game")
		void RecordParticle(UParticleSystemComponent* particle);

	UFUNCTION(BlueprintCallable, Category = "Game")
		void ScaleParticleSize(UParticleSystemComponent* particle, float scale);
	UFUNCTION(BlueprintCallable, Category = "Game")
		bool IsPreparedAttack();
	UFUNCTION(BlueprintCallable, Category = "Game")
		float GetRuinedPercent();

<<<<<<< .mine
=======>>>>>>> .theirs	void GrapBuoyancyComponent_Initialize( int32 ActorType);

	TSharedPtr<UStaticMeshComponent> GetSubUStaticMeshComponentByName(FString& strComponentName) ;
public:
	int GetDamageResult(int type);
	int GetTotalDamage();
	void AddResult(int type);
	void Active_AvoidStop();
	void RecordMissionTime();
	float GetMissionTime() { return MissionEndTime - MissionStartTime; }

public:
	int32 ArchType;
	uint64 EntId;
	float Speed;
	float AverageSpeed;
	float SailDistance;
	float SailAngle;
	float SailTime;
	float SpeedDownDistance;
	float HorizontalDistance;
	float RollbackDistance;
	float RollbackAngle;
	FVector AttackPos;
	float PrepareTime;
	float FlashTime;
	float TotalDamage;
	UStaticMeshComponent* MainStaticMesh;
	TMap<int, int> DamageResults;

protected:
	TMap<UParticleSystemComponent*, FVector> mParticles;
<<<<<<< .mine	float Health;    
=======	float Health;    	
>>>>>>> .theirs	float TotalDamage;
	float AttackDistance = 100000;
	AActor* KilledBy;
	float MissionStartTime = 0;
	float MissionEndTime = 0;
	FTimerHandle AvoidHandler;
	float AvoidDelay = 2.0f;	
	bool m_bStartMission=true;
};
