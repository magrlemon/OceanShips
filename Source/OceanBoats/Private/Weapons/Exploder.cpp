// Fill out your copyright notice in the Description page of Project Settings.


#include "Exploder.h"

// Sets default values
AExploder::AExploder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AExploder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExploder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExploder::SwitchColor_Implementation(UStaticMeshComponent* root, FColor baseColor)
{
	m_baseColor = baseColor;
	m_mat = root->GetMaterial(0);
	GetWorldTimerManager().SetTimer(m_SwitchHandler, this, &AExploder::ChangeColor,0.5f);
}
bool AExploder::IsKilled_Implementation()
{
	return false;
}
void AExploder::SetHitPos_Implementation(FVector hitPos)
{
}
void AExploder::SetKiller_Implementation(AActor* killer)
{

}
void AExploder::KillEvent_Implementation(AActor* victim)
{

}
void AExploder::ChangeColor()
{		
	FName paramName("SwitchColor");
	FLinearColor color;
	FMaterialParameterInfo info;
	info.Name = paramName;
	m_mat->GetVectorParameterValue(info, color);
	FColor paramValue;
	if (color.GetMax() <= 0)
		paramValue = m_baseColor;		
	else
		paramValue = FColor(0, 0, 0, 0);

	UMaterialInstanceDynamic* matDyn = Cast<UMaterialInstanceDynamic>(m_mat);
	if(matDyn)
	{
		matDyn->SetVectorParameterValue(paramName, paramValue);
	}
	
	GetWorldTimerManager().SetTimer(m_SwitchHandler, this, &AExploder::ChangeColor, 0.5f);
	
}