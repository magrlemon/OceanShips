// Fill out your copyright notice in the Description page of Project Settings.

#include "SimEcs_Archetype.h"
#include "SimEcs_ComponentWrapper.h"


// Sets default values
ASimEcs_Archetype::ASimEcs_Archetype( /*const FObjectInitializer& ObjectInitializer*/ )
	//: Super( ObjectInitializer ) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ASimEcs_Archetype::GrapMesh(UStaticMeshComponent* mainMesh) {
	MainStaticMesh = mainMesh;
}

void ASimEcs_Archetype::CreateNewEntityFromThis( uint64  handleID)
{
	for (auto c : GetComponents( )) {
		auto wr = Cast<IComponentWrapper>( c );
		if (wr) {
		//	wr->AddToEntity( handleID );
		}
	}
}

int ASimEcs_Archetype::GetDamageResult(int type)
{
	int* result = DamageResults.Find(type);
	if (result)
	{
		return *result;
	}
	return 0;
}
int ASimEcs_Archetype::GetTotalDamage()
{
	int total = 0;
	for (auto result : DamageResults)
	{
		total += result.Value;
	}
	return total;
}
void ASimEcs_Archetype::AddResult(int type)
{
	DamageResults.FindOrAdd(type)++;
}

void ASimEcs_Archetype::EnableWaveForce_Implementation(bool enable)
{
	
}

void ASimEcs_Archetype::EnableBoatEffect_Implementation(bool enable)
{

}
void ASimEcs_Archetype::StartFire_Implementation()
{

}
