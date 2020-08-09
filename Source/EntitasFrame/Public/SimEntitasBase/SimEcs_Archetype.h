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
#include "SimEcs_Archetype.generated.h"



using EntityHandleId = uint64_t;
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

	//UFUNCTION(/*BlueprintNativeEvent, */BlueprintCallable, Category = "Game")
	//	void MoveOn(FVector targetPos = FVector::ZeroVector);
	////virtual void MoveOn_Implementation(FVector targetPos = FVector::ZeroVector);
	//UFUNCTION(/*BlueprintNativeEvent,*/ BlueprintCallable, Category = "Game")
	//	void  HoldOn(FVector nextTargetPos = FVector::ZeroVector);
	////virtual void HoldOn_Implementation(FVector nextTargetPos = FVector::ZeroVector);
	//UFUNCTION(/*BlueprintNativeEvent,*/ BlueprintCallable, Category = "Game")
	//	void  MoveBack(FVector targetPos = FVector::ZeroVector);
	////virtual void MoveBack_Implementation(FVector targetPos = FVector::ZeroVector);


	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableWaveForce(bool bEnable);
	virtual void EnableWaveForce_Implementation(bool bEnable);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void EnableBoatEffect(bool bEnable);
	virtual void EnableBoatEffect_Implementation(bool bEnable);

	virtual void CreateNewEntityFromThis( uint64  handleID );
	
public:
	EntityHandleId EntityId;
};
