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

void AExploder::SwitchColor_Implementation(UStaticMeshComponent* root, FColor baseColor, bool left)
{
	m_baseColor = baseColor;	
	mainRoot = root;
	m_left = left;
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

	UMaterialParameterCollectionInstance* Instance = NULL;
	if (m_left)
		Instance = GWorld->GetParameterCollectionInstance(CollectionLeft);
	else
		Instance = GWorld->GetParameterCollectionInstance(CollectionRight);
	if (Instance)
	{
		Instance->GetVectorParameterValue(paramName, color);
		FColor paramValue;
		if (color.GetMax() <= 0)
		{
			paramValue = m_baseColor;
			//paramValue.R = m_baseColor.R;
			//paramValue.G = m_baseColor.G;
			//paramValue.A = m_baseColor.A;
		}	//
		else
			paramValue = FColor(0, 0, 0, 0);

		const bool bFoundParameter = Instance->SetVectorParameterValue(paramName, paramValue);		
	}

	GetWorldTimerManager().SetTimer(m_SwitchHandler, this, &AExploder::ChangeColor, 0.5f);
	
}