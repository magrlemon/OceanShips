// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cannon.generated.h"

UCLASS()
class OCEANBOATS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACannon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void AdjustFire(/*FVector pos*/);
	virtual void AdjustFire_Implementation(/*FVector pos*/);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SortDistances();
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cannon)
		FVector LastHitPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Cannon)
	FRotator newRot;

protected:
	TMap<AActor*, float> m_BoatDistances;
};
