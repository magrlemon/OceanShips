// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "GameModeInterface.h"

// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACannon::AdjustFire_Implementation()
{
	//TargetLocation = pos;

	SortDistances();
	AActor* boat = m_BoatDistances.begin().Key();
	FVector fForward = RootComponent->GetForwardVector().GetSafeNormal();
	float angleValue = fForward.CosineAngle2D((boat->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	//旋转炮台	
	float leftOrRight = (FVector::CrossProduct(fForward, boat->GetActorLocation() - GetActorLocation()).Z > 0) ? -1.0f : 1.0f;
	newRot = GetActorRotation();
	newRot.Yaw += leftOrRight * angleValue;
	//调远近
	float farHit = FVector::Dist2D(GetActorLocation(), LastHitPos) > FVector::Dist2D(GetActorLocation(), boat->GetActorLocation())?1:-1;
	newRot.Pitch += farHit * 0.1;	
}

void ACannon::SortDistances()
{
	m_BoatDistances.Empty();
	int num = IGameModeInterface::Execute_GetBoatNum(UGameplayStatics::GetGameMode(this));
	for (int i = 0; i < num; ++i)
	{
		AActor* boat = IGameModeInterface::Execute_GetBoat(UGameplayStatics::GetGameMode(this), i);
		float dis = FVector::Dist2D(GetActorLocation(), boat->GetActorLocation());
		m_BoatDistances.Add(boat, dis);
	}
	m_BoatDistances.ValueSort(TGreater<float>());
}