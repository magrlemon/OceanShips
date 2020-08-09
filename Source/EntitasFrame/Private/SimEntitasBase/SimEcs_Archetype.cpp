// Fill out your copyright notice in the Description page of Project Settings.

#include "SimEcs_Archetype.h"
#include "SimEcs_ComponentWrapper.h"


// Sets default values
ASimEcs_Archetype::ASimEcs_Archetype()//( const FObjectInitializer& ObjectInitializer )
	//: Super( ObjectInitializer ) 
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

//void ASimEcs_Archetype::MoveOn(FVector targetPos)
//{
//
//}
//
//void ASimEcs_Archetype::HoldOn(FVector nextTargetPos)
//{
//
//}
//
//void ASimEcs_Archetype::MoveBack(FVector targetPos)
//{
//
//}

void ASimEcs_Archetype::EnableWaveForce_Implementation(bool bEnable)
{

}

void ASimEcs_Archetype::EnableBoatEffect_Implementation(bool bEnable)
{

}

