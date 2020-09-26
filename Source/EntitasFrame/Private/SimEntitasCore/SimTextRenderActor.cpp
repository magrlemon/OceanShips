
#include "SimTextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ASimTextRenderActor::ASimTextRenderActor(const FObjectInitializer & ObjectInitializer) : Super (ObjectInitializer)
{
	//������ַ�����Ϊÿ֡����Tick()���������Ҫ�����Թرմ�ѡ����������ܡ�
	PrimaryActorTick.bCanEverTick = true;
}

void ASimTextRenderActor::BeginPlay()
{
	Super::BeginPlay();
}

void ASimTextRenderActor::PostActorCreated()
{
	Super::PostActorCreated();

	FVector Location = GetActorLocation();
	FRotator LookAtRot = GetActorRotation();
	SetActorRotation(LookAtRot + FRotator(0, 90, 0));
	GetTextRender()->SetHorizontalAlignment(EHorizTextAligment::EHTA_Center);
	GetTextRender()->SetWorldSize(14);
}

void ASimTextRenderActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASimTextRenderActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	Timer += DeltaSeconds;
	if (Timer >= ShowTextMaxTime) {
		Destroy();
	}
}

void ASimTextRenderActor::SetColor(FColor textColor) {
	GetTextRender()->SetTextRenderColor(textColor);
}

void ASimTextRenderActor::SetText(const FString & TextString)
{
	GetTextRender()->SetText(FText::FromString(TextString));
}
 
void ASimTextRenderActor::SetTextRelativeLocation(FVector& relativeLoc) {
	SetActorRelativeLocation(relativeLoc);
}
