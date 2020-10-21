// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OceanCommonTypes.h"
#include "ExplodeInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UExplodeInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OCEANBOATS_API IExplodeInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SwitchColor(UStaticMeshComponent* root, FColor baseColor, bool left);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void SetKiller(AActor* killer);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		void KillEvent(AActor* victim);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
		bool IsKilled();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Cannon")
		void SetHitPos(FVector hitPos);

};
