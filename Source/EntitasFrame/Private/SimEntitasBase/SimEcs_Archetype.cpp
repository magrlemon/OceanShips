// Fill out your copyright notice in the Description page of Project Settings.

#include "SimEcs_Archetype.h"
#include "SimEcs_ComponentWrapper.h"
#include "Classes\AdvancedBuoyancyComponent\AdvancedBuoyancyComponent.h"

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

float ASimEcs_Archetype::GetHealthRange()
{
	return FMath::Clamp<float>((Health - TotalDamage) / Health,0.0,1.0);
}

void ASimEcs_Archetype::HitDamge(float damage)
{
	TotalDamage += damage;
}
void ASimEcs_Archetype::RecordParticle(UParticleSystemComponent* particle)
{
	if (!mParticles.Find(particle))
		mParticles.Add(particle, particle->GetComponentScale());
}

void ASimEcs_Archetype::ScaleParticleSize(UParticleSystemComponent* particle, float scale)
{
	if (particle == NULL)
		return;
	FVector* scaleVec = mParticles.Find(particle);
	if(scaleVec)
	{
		particle->SetWorldScale3D((*scaleVec) * scale);
	}
}

bool ASimEcs_Archetype::IsPreparedAttack()
{
	return FVector::Dist2D(AttackPos,GetActorLocation()) <= AttackDistance;
}

void ASimEcs_Archetype::EnableWaveForce_Implementation(bool enable)
{
	
}

void ASimEcs_Archetype::EnableBoatEffect_Implementation(bool enable)
{

}
void ASimEcs_Archetype::StartFire_Implementation()
{
	//GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, "boat->Get( )->StartFire( );" );
}

void ASimEcs_Archetype::ResetParticelSize_Implementation(float scale)
{

}

void ASimEcs_Archetype::GrapBuoyancyComponent_Initialize( int32 ActorType ) {

	//TArray<UActorComponent*> allComponents = owner->GetComponents( ).Array( );
	///*for (int i = 0; i < allComponents.Num( ); i++) {
	//	if (allComponents[i] == NULL) {
	//		continue;
	//	}
	//	if (allComponents[i]->IsA<UBuoyancyForceComponent>( )) {
	//		PhysicsComponent = (UPrimitiveComponent*)allComponents[i];
	//		break;
	//	}
	//}*/
	UAdvancedBuoyancyComponent* pUBuoyancyComponent = (UAdvancedBuoyancyComponent*)(this->GetComponentByClass( UAdvancedBuoyancyComponent::StaticClass( ) ));
	if(pUBuoyancyComponent)
		pUBuoyancyComponent->AddLocalOffset(FVector::ZeroVector );

}

void ASimEcs_Archetype::Avoid()
{
	AVoided = true;
	GetWorldTimerManager().SetTimer(AvoidHandler, this, &ASimEcs_Archetype::Active_AvoidStop, AvoidDelay);
}

void ASimEcs_Archetype::Active_AvoidStop()
{
	AVoided = false;
}

void ASimEcs_Archetype::RecordMissionTime()
{
	if(m_bStartMission)
	{
		MissionStartTime = UGameplayStatics::GetRealTimeSeconds(GWorld);
		m_bStartMission = false;
	}
	else
	{
		MissionEndTime = UGameplayStatics::GetRealTimeSeconds(GWorld);
	}
}

TSharedPtr<UStaticMeshComponent> ASimEcs_Archetype::GetSubUStaticMeshComponentByName( FString& strComponentName )
{
	if (strComponentName.IsEmpty( ))return nullptr;
	TSet<UActorComponent*> arrSpot = GetComponents( );
	for (auto part : arrSpot) {
		if (!part || part->GetName( ).Compare( strComponentName ) != 0) {
			continue;
		}
		return MakeShareable(Cast< UStaticMeshComponent >( part ));
	}

	return nullptr;
}