// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "GameModeInterface.h"
#include "Components/ArrowComponent.h"
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

void ACannon::AdjustFire_Implementation(UArrowComponent* arrow)
{
	USceneComponent* root = GetRootComponent();
	FRotator meshRot = GetRootComponent()->GetComponentRotation();
	SortDistances();
	if (m_BoatDistances.Num() <= 0)
		return;
	AActor* boat = m_BoatDistances.begin().Key();
	if (boat == NULL)
		return;

	FVector fForward = root->GetForwardVector().GetSafeNormal();
	float angleValue = fForward.CosineAngle2D((boat->GetActorLocation() - GetActorLocation()).GetSafeNormal());
	//旋转炮台	
	float leftOrRight = (FVector::CrossProduct(fForward, boat->GetActorLocation() - GetActorLocation()).Z > 0) ? -1.0f : 1.0f;
	meshRot.Yaw += leftOrRight * angleValue;
	root->SetWorldRotation(meshRot);
	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Red, FString::SanitizeFloat(leftOrRight * angleValue));
	//GetRootComponent()->GetComponentRotation().Add(0, 45/*leftOrRight * angleValue * 10*/, 0);
	//调远近
	float farHit = FVector::Dist2D(GetActorLocation(), LastHitPos) > FVector::Dist2D(GetActorLocation(), boat->GetActorLocation())?1:-1;
	FRotator arrowRot = arrow->GetComponentRotation();
	arrowRot.Pitch += farHit * 0.1;
	arrow->SetWorldRotation(arrowRot);
	//arrow->GetComponentRotation().Add(farHit * 0.1, 0, 0);
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