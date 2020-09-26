// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SoldierPawn.generated.h"

class UInputComponent;
class UPawnMovementComponent;
class USphereComponent;
class UStaticMeshComponent;

UCLASS(config = Game, Blueprintable, BlueprintType)
class OCEANBOATS_API ASoldierPawn : public APawn
{
	GENERATED_UCLASS_BODY()

	// Begin Pawn overrides
	virtual UPawnMovementComponent* GetMovementComponent() const override;
	virtual void SetupPlayerInputComponent(UInputComponent* InInputComponent) override;
	virtual void UpdateNavigationRelevance() override;
	// End Pawn overrides

	/**
	 * Input callback to move forward in local space (or backward if Val is negative).
	 * @param Val Amount of movement in the forward direction (or backward if negative).
	 * @see APawn::AddMovementInput()
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
		virtual void MoveForward(float Val);

	/**
	 * Input callback to strafe right in local space (or left if Val is negative).
	 * @param Val Amount of movement in the right direction (or left if negative).
	 * @see APawn::AddMovementInput()
	 */
	UFUNCTION(BlueprintCallable, Category = "Pawn")
		virtual void MoveRight(float Val);

	///**
	// * Called via input to turn at a given rate.
	// * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	// */
	//UFUNCTION(BlueprintCallable, Category = "Pawn")
	//	virtual void TurnAtRate(float Rate);

	///**
	//* Called via input to look up at a given rate (or down if Rate is negative).
	//* @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	//*/
	//UFUNCTION(BlueprintCallable, Category = "Pawn")
	//	virtual void LookUpAtRate(float Rate);

	///** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
	//	float BaseTurnRate;

	///** Base lookup rate, in deg/sec. Other scaling may affect final lookup rate. */
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
	//	float BaseLookUpRate;
public:
	//// Sets default values for this pawn's properties
	//ASoldierPawn();
	/** Name of the MovementComponent.  Use this name if you want to use a different class (with ObjectInitializer.SetDefaultSubobjectClass). */
	FName MovementComponentName;
	/** Name of the CollisionComponent. */
	FName CollisionComponentName;
	/** Name of the MeshComponent. Use this name if you want to prevent creation of the component (with ObjectInitializer.DoNotCreateDefaultSubobject). */
	FName MeshComponentName;

protected:
	/** DefaultPawn movement component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UPawnMovementComponent* MovementComponent;
	/** The mesh associated with this Pawn. */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;
	/** DefaultPawn collision component */
	UPROPERTY(Category = Pawn, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** Returns CollisionComponent subobject **/
	USphereComponent* GetCollisionComponent() const { return CollisionComponent; }
	/** Returns MeshComponent subobject **/
	UStaticMeshComponent* GetMeshComponent() const { return MeshComponent; }
};
