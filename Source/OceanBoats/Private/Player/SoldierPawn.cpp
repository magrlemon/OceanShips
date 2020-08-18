// Fill out your copyright notice in the Description page of Project Settings.


#include "SoldierPawn.h"
#include "Engine/StaticMesh.h"
#include "Components/SphereComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/PlayerInput.h"
#include "Engine/CollisionProfile.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ASoldierPawn::ASoldierPawn(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponentName = TEXT("MovementComponent");
	CollisionComponentName = TEXT("CollisionComponent");
	MeshComponentName = TEXT("MeshComponent");

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(CollisionComponentName);
	CollisionComponent->InitSphereRadius(35.0f);
	CollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

	CollisionComponent->CanCharacterStepUpOn = ECB_No;
	CollisionComponent->SetShouldUpdatePhysicsVolume(true);
	CollisionComponent->SetCanEverAffectNavigation(false);
	CollisionComponent->bDynamicObstacle = true;

	RootComponent = CollisionComponent;

	MovementComponent = CreateDefaultSubobject<UPawnMovementComponent, UFloatingPawnMovement>(MovementComponentName);
	MovementComponent->UpdatedComponent = CollisionComponent;
	Cast<UFloatingPawnMovement>(MovementComponent)->MaxSpeed = 5000;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh;
		FConstructorStatics()
			: SphereMesh(TEXT("/Engine/EngineMeshes/Sphere")) {}
	};

	static FConstructorStatics ConstructorStatics;

	MeshComponent = CreateOptionalDefaultSubobject<UStaticMeshComponent>(MeshComponentName);
	if (MeshComponent)
	{
		MeshComponent->SetStaticMesh(ConstructorStatics.SphereMesh.Object);
		MeshComponent->AlwaysLoadOnClient = true;
		MeshComponent->AlwaysLoadOnServer = true;
		MeshComponent->bOwnerNoSee = true;
		MeshComponent->bCastDynamicShadow = true;
		MeshComponent->bAffectDynamicIndirectLighting = false;
		MeshComponent->bAffectDistanceFieldLighting = false;
		MeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
		MeshComponent->SetupAttachment(RootComponent);
		MeshComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
		const float Scale = CollisionComponent->GetUnscaledSphereRadius() / 160.f; // @TODO: hardcoding known size of EngineMeshes.Sphere. Should use a unit sphere instead.
		MeshComponent->SetRelativeScale3D(FVector(Scale));
		MeshComponent->SetGenerateOverlapEvents(false);
		MeshComponent->SetCanEverAffectNavigation(false);
	}
	//BaseTurnRate = 45.f;
	//BaseLookUpRate = 45.f;
}

// Called when the game starts or when spawned
void ASoldierPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoldierPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASoldierPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//if (bAddDefaultMovementBindings)
	{
		//InitializeDefaultPawnInputBindings();

		PlayerInputComponent->BindAxis("MoveForward", this, &ASoldierPawn::MoveForward);
		PlayerInputComponent->BindAxis("MoveRight", this, &ASoldierPawn::MoveRight);
		PlayerInputComponent->BindAxis("Turn", this, &ASoldierPawn::AddControllerYawInput);
		PlayerInputComponent->BindAxis("LookUp", this, &ASoldierPawn::AddControllerPitchInput);
	}
}
void ASoldierPawn::MoveForward(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
		}
	}
}

void ASoldierPawn::MoveRight(float Val)
{
	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);
		}
	}
}
//void ASoldierPawn::TurnAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
//}
//
//void ASoldierPawn::LookUpAtRate(float Rate)
//{
//	// calculate delta for this frame from the rate information
//	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
//}
void ASoldierPawn::UpdateNavigationRelevance()
{
	if (CollisionComponent)
	{
		CollisionComponent->SetCanEverAffectNavigation(bCanAffectNavigationGeneration);
	}
}

UPawnMovementComponent* ASoldierPawn::GetMovementComponent() const
{
	return MovementComponent;
}