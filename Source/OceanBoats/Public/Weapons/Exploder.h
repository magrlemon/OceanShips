// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplodeInterface.h"
#include "Exploder.generated.h"

UCLASS()
class OCEANBOATS_API AExploder : public AActor, public IExplodeInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExploder();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	UMaterialParameterCollection* CollectionLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game")
	UMaterialParameterCollection* CollectionRight;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SwitchColor(UStaticMeshComponent* root, FColor baseColor, bool left);
	virtual void SwitchColor_Implementation(UStaticMeshComponent* root, FColor baseColor, bool left);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetKiller(AActor* killer);
	virtual void SetKiller_Implementation(AActor* killer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void KillEvent(AActor* victim);
	virtual void KillEvent_Implementation(AActor* victim);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		bool IsKilled();
	virtual bool IsKilled_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cannon")
		void SetHitPos(FVector hitPos);
	virtual void SetHitPos_Implementation(FVector hitPos);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void ChangeColor();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	FTimerHandle m_SwitchHandler;
	FColor m_baseColor;
	UStaticMeshComponent* mainRoot;
	bool m_left;
};
