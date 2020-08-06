
// Fill out your copyright notice in the Description page of Project Settings.
/**
* Copyright 2019 北京伯睿科技有限公司.
*
* @File ASimEcs_WorldActor.h
* @Description Root Actor
*
* @Author magi
* @Date 2020年3月20日
* @Version 1.0
*/

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SimEcs_Core.h"
#include "SimEcs_BattleWorld.generated.h"


UCLASS()
class ENTITASFRAME_API ASimEcs_WorldActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASimEcs_WorldActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TUniquePtr<SimEcs_World> ECSWorld;	
};
