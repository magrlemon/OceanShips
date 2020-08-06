// Fill out your copyright notice in the Description page of Project Settings.


#include "Boat_Archetype.h"

FRotator MakeRotFromX(const FVector& X)
{
	return FRotationMatrix::MakeFromX(X).Rotator();
}

FRotator FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	const FVector fLerpValue = FMath::Lerp( Target, Start,  0.001);
	FVector fValue = Target - fLerpValue;
	fValue.Z = 0.0;
	float dist = FVector::Dist( Target,Start );
	if (dist  < 5000.0f) {
		MakeRotFromX( fValue );
	}
	return MakeRotFromX( fValue );
}

///
ABoat_Archetype::ABoat_Archetype()
{

}
void ABoat_Archetype::SetMainMesh(UStaticMeshComponent* mainMesh)
{
	MainMeshComponent = mainMesh;
	//RootComponent = MainMeshComponent;
}
void ABoat_Archetype::SpeedUp()
{
	ExpectSpeed = 1.0;
}

void ABoat_Archetype::SpeedDown()
{
	ExpectSpeed = 0;
}


float ABoat_Archetype::FindLookAtRotation_Yaw( const FVector& Start, const FVector& Target )
{
	const FVector fLerpValue = FMath::Lerp( Start, Target, 0.0001f );
	FVector fValue = Target - fLerpValue;
	fValue.Z = 0.0;
	float dist = FVector::Dist( Target, Start );
	if (dist < 2200.0f) {
		SetMoveMode( EMoveMode_Idle );
		return OldLookAtRotation.Yaw;
	}
	OldLookAtRotation = MakeRotFromX( fValue );
	return OldLookAtRotation.Yaw;
}

void ABoat_Archetype::TickMove(FVector loc, UPrimitiveComponent* root)
{
	if (MoveMode == EMoveMode_Idle)
		return;

	//BoatRoot = root;
	
	if (Tags.Num( ) > 0) {
		if (Tags[0].Compare( TEXT( "Sim_BoatArchetype" ) ) == 0) {
		
			FVector curPos = MainMeshComponent->GetComponentLocation( );
			DVector2 vecLatLon =  USimOceanSceneManager_Singleton::GetInstance()->ConvertWorldPositonToLatLon( curPos );
			int id = 0;
			FString fName = GetName( );
			if (GetName( ).Compare( "BP_Boats_XLT5_2" ) == 0) {
				id = 1;
			}
			else if (GetName( ).Compare( "BP_Boats_XLT_EX4_5" ) == 0)
			{
				id = 2;
			}
			else if (GetName( ).Compare( "BP_Boats_GCZYJT_2" ) == 0) {
				id = 3;
			}
			else if (GetName( ).Compare( "BP_Boats_XLT_EX_2" ) == 0) {
				id = 4;
			}
			else if (GetName( ).Compare( "BP_Boats_XLT_EX2_5" ) == 0) {
				id = 5;
			}
			
		//	GEngine->AddOnScreenDebugMessage( -1, 8.f, FColor::Red, vecLatLon.ToString() );
			time_t now = FDateTime::Now().GetTicks() / ETimespan::TicksPerMillisecond;
			static time_t _last_beat = 0;
			static time_t run_time = 0;
			static time_t max_time = 100;
			run_time += now - _last_beat;
			if (run_time>max_time) 
			{
				FString  jsonstr;
				TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&jsonstr);
				Writer->WriteObjectStart();
				Writer->WriteValue("Num", id);
				Writer->WriteValue("J", vecLatLon.X);
				Writer->WriteValue("W", vecLatLon.Y);
				Writer->WriteValue("Speed", 0);
				Writer->WriteObjectEnd();
				Writer->Close();
				NetWork::SendMsg(TCHAR_TO_UTF8(*jsonstr));
				run_time = 0;
			}

			_last_beat = now;
			
		}
	}

	root->AddForceAtLocation(root->GetRightVector() * RightAxisValue * root->GetMass() * SteeringSpeed, loc);
	root->AddForce(root->GetForwardVector() * root->GetMass() * ForwardAxisValue * ForwardSpeed);
}

void ABoat_Archetype::MainLoopLogic()
{
	//if (MoveOnPos == FVector::ZeroVector)
	//{
	//	SetMoveMode(EMoveMode_Idle);
	//	return;
	//}

	ForwardAxisValue = CurrentSpeed;
	CheckSpeedUp();

	if (MoveMode == EMoveMode_On)
	{	
		bRollBack = false;
		
		
		FRotator rot;
		rot.Yaw = FindLookAtRotation_Yaw(MainMeshComponent->GetComponentLocation()/*GetActorLocation()*/, MoveOnPos/* MoveTarget->GetActorLocation()*/);
		//SetActorRotation(rot);

		if (MainMeshComponent != NULL)
		{
			MainMeshComponent->SetWorldRotation(rot);
		}
	}
	//else if (MoveMode == EMoveMode_Idle)
	//{
	//	FRotator rot;
	//	rot = FindLookAtRotation(MoveOnPos, MainMeshComponent->GetComponentLocation());
	//	tempV1.X = MainMeshComponent->GetForwardVector().X;
	//	tempV1.Y = MainMeshComponent->GetForwardVector().Y;
	//	//FVector2D v1 = FVector2D(MainMeshComponent->GetForwardVector().X, MainMeshComponent->GetForwardVector().Y);
	//	tempV2.X = (MoveOnPos - MainMeshComponent->GetComponentLocation()).X;
	//	tempV2.Y = (MoveOnPos - MainMeshComponent->GetComponentLocation()).Y;
	//	//FVector2D v2 = FVector2D((MoveOnPos - MainMeshComponent->GetComponentLocation()).X, (MoveOnPos - MainMeshComponent->GetComponentLocation()).Y);

	//	float lastAngle = FMath::Clamp(FVector2D::DotProduct(tempV1, tempV2), 0.0f,1.0f);
	//	
	//	float newYaw = FindLookAtRotation(MoveOnPos, MainMeshComponent->GetComponentLocation()).Yaw;
	//	float deg = FMath::RadiansToDegrees(lastAngle);
	//	if (deg > 3.0f)//逐渐转向目标点
	//	{
	//		rot.Yaw = newYaw + TurnIndex * TurnStep;
	//		//SetActorRotation(rot);
	//		if (MainMeshComponent != NULL)
	//		{
	//			MainMeshComponent->SetWorldRotation(rot);
	//		}
	//		TurnIndex++;
	//	}
	//	else
	//	{
	//		SetMoveMode(EMoveMode_On);
	//		TurnIndex = 0;
	//	}
	//}
}
void ABoat_Archetype::CheckSpeedUp()
{
	if (MoveMode == EMoveMode_On || MoveMode == EMoveMode_Back)
	{
		if (abs(CurrentSpeed - ExpectSpeed) > StepSpeed*0.99)
		{
			if (CurrentSpeed > ExpectSpeed)
			{
				bSpeedDown = true;
				CurrentSpeed = (CurrentSpeed - StepSpeed) > 0 ? (CurrentSpeed - StepSpeed) : 0;
			}
			else
			{
				bSpeedDown = false;
				CurrentSpeed = (CurrentSpeed + StepSpeed) < ExpectSpeed ? (CurrentSpeed + StepSpeed) : ExpectSpeed;
			}
		}
		else//fire
		{
			if (MoveMode == EMoveMode_On && (abs(CurrentSpeed) < StepSpeed*0.99))
			{
				SetMoveMode(EMoveMode_Idle);
				/*MoveMode = EMoveMode_Fire;
				EnableBuoyancyForce(false);
				GetWorldTimerManager().SetTimer(DelayHandler, this, &ABoat_Archetype::Active_Attack, AttackDelay);*/
			}
		}
	}
}

void ABoat_Archetype::RuntimeStateLoop()
{
	//record Move Speed
	MoveSpeed = MainMeshComponent ? MainMeshComponent->GetPhysicsLinearVelocity().Size() : 0.0f;

	//record move distance		
	if (LastPos.Equals(FVector::ZeroVector))
		LastPos = GetActorLocation(); 
	else
	{
		float move = (LastPos - MainMeshComponent->GetComponentLocation()/*GetActorLocation()*/).Size2D();

		if(MoveMode == EMoveMode_On)
			MoveDistance += move;
		if (bSpeedDown)
			AccSpeedDownDistance += move;
		if (bRollBack)
			AccRollbakDistance += move;

		LastPos = MainMeshComponent->GetComponentLocation();// GetActorLocation();
	}	
}

void ABoat_Archetype::MoveLoopImpl(/*ATargetPoint* target*/)
{
	ForwardAxisValue = CurrentSpeed;
	CheckSpeedUp();
	//FVector currentPos = GetActorLocation();
	//FVector targetPos = MoveOnPos;
	//if (MoveOnPos == FVector::ZeroVector)
	//	targetPos = target->GetActorLocation();

	if(MoveMode == EMoveMode_On)
	{
		if (MoveOnPos == FVector::ZeroVector)
		{
			SetMoveMode(EMoveMode_Idle);
			return;
		}

		bRollBack = false;

		FRotator rot;
		rot.Yaw = FindLookAtRotation(MainMeshComponent->GetComponentLocation()/*GetActorLocation()*/, MoveOnPos/* MoveTarget->GetActorLocation()*/).Yaw;
		//SetActorRotation(rot);
		if(MainMeshComponent != NULL)
		{
			MainMeshComponent->SetWorldRotation(rot);
		}
	}	
	//else if(MoveMode == EMoveMode_Back)
	//{
	//	bRollBack = true;

	//	if (!ReverseMoveEnd)//径直后退
	//	{
	//		ForwardAxisValue = -CurrentSpeed;
	//		FRotator rot = FindLookAtRotation(targetPos, currentPos);
	//		//SetActorRotation(rot);
	//		if (MainMeshComponent != NULL)
	//		{
	//			MainMeshComponent->SetWorldRotation(rot);
	//		}
	//	}
	//	else
	//	{
	//		FRotator rot;
	//		float newYaw = FindLookAtRotation(targetPos, currentPos).Yaw;
	//		if (TurnIndex * TurnStep < 180)//逐渐转向180度
	//		{
	//			rot.Yaw =  newYaw + TurnIndex * TurnStep;
	//			//SetActorRotation(rot);
	//			if (MainMeshComponent != NULL)
	//			{
	//				MainMeshComponent->SetWorldRotation(rot);
	//			}
	//			TurnIndex++;
	//		}
	//		else //径直转向180度
	//		{
	//			EnableBuoyancyForce(true);
	//			EnableEffect(true);
	//			rot.Yaw = newYaw + 180;
	//			//SetActorRotation(rot);
	//			if (MainMeshComponent != NULL)
	//			{
	//				MainMeshComponent->SetWorldRotation(rot);
	//			}
	//		}
	//	}
	//}
}

void ABoat_Archetype::Active_Turn()
{
	ReverseMoveEnd = true;
	EnableBuoyancyForce(false);
}

void ABoat_Archetype::Active_MoveOn_Implementation(FVector targetPos)
{
	if(MoveTarget)
		MoveOnPos = MoveTarget->GetActorLocation();	
	if (targetPos != FVector::ZeroVector)
		MoveOnPos = targetPos;/*
	else
		MoveOnPos = MoveTarget->GetActorLocation();*/

	SetMoveMode(OceanMoveMode::EMoveMode_On);
	SpeedUp();	
	MainMeshComponent->SetSimulatePhysics( true );
	EnableBuoyancyForce(true);
	EnableEffect(true);
}

void ABoat_Archetype::Active_MoveBack_Implementation(FVector targetPos)
{
	if(BackTarget)
		MoveBackPos = BackTarget->GetActorLocation();
	if (targetPos != FVector::ZeroVector)
		MoveBackPos = targetPos;/*
	else
		MoveBackPos = BackTarget->GetActorLocation();*/

	SetMoveMode(OceanMoveMode::EMoveMode_Back);
	SpeedUp();
	
	GetWorldTimerManager().SetTimer(DelayHandler, this, &ABoat_Archetype::Active_Turn, BackMoveTime);
	EnableEffect(false);
}

void ABoat_Archetype::Active_HoldOn_Implementation(FVector nextTargetPos)
{
	SetMoveMode(EMoveMode_Idle);
	MoveOnPos = nextTargetPos;
}

void ABoat_Archetype::Active_Attack()
{
	AttackPos = AttackTarget->GetActorLocation();

	FRotator rot;
	rot.Yaw = /*GetActorRotation().Yaw + */FindLookAtRotation(GetActorLocation(), AttackPos).Yaw;
	SetActorRotation(rot);

	if(!FireEnd)
	{
		for (int i = 0; i < Cannons.Num(); ++i)
		{
			FireImpl(this->GetUniqueID(), Cannons[i]->GetComponentTransform(),Explode_Normal);
		}
		for (int k = 0; k < CannonsTD.Num(); ++k)
		{
			FireImpl(this->GetUniqueID(), CannonsTD[k]->GetComponentTransform(),Explode_TBD);
		}
		FireEnd = true;
	}
	//打完炮弹开始后退
	Active_MoveBack();
}

OceanMoveMode ABoat_Archetype::GetMoveMode()
{
	return MoveMode;
}

void ABoat_Archetype::SetMoveMode_Implementation(OceanMoveMode mode)
{
	MoveMode = mode;
}




void ABoat_Archetype::FireImpl_Implementation(int killerUID, FTransform trans, ExplodeType type)
{

}

void ABoat_Archetype::RecordDestroy_Implementation(ObstacleType type)
{
	for (TMap<ObstacleType, int>::TIterator iter(DestroyResult); iter; ++iter)
	{
		if (iter->Key == type)
		{
			iter->Value++;
			return;
		}
	}
	DestroyResult.Add(type, 1);
}

void ABoat_Archetype::EnableBuoyancyForce_Implementation(bool bEnable)
{

}
void ABoat_Archetype::EnableEffect_Implementation(bool bEnable)
{

}

int ABoat_Archetype::GetDestroyNum_Implementation(ObstacleType type)
{
	return *DestroyResult.Find(type);
}
float ABoat_Archetype::GetSpeed_Implementation()
{	
	return MoveSpeed;
}
float ABoat_Archetype::GetSailDistance_Implementation()
{
	return MoveDistance;
}
float ABoat_Archetype::GetSailAngle_Implementation()
{
	return 0.0f;
}
float ABoat_Archetype::GetSpeedDownDistance_Implementation()
{
	return AccSpeedDownDistance;
}
float ABoat_Archetype::GetHorizontalDistance_Implementation()
{
	return 0.0f;
}
float ABoat_Archetype::GetRollbackDistance_Implementation()
{
	return AccRollbakDistance;
}
float ABoat_Archetype::GetRollbackAngle_Implementation()
{
	return 0.0f;
}