// Fill out your copyright notice in the Description page of Project Settings.


#include "SimEcs_BoatArchetype.h"

FRotator MakeRotFromX(const FVector& X)
{
	return FRotationMatrix::MakeFromX(X).Rotator();
}
FRotator FindLookAtRotation(const FVector& Start, const FVector& Target)
{
	return MakeRotFromX(Target - Start);
}


///
ASimEcs_BoatArchetype::ASimEcs_BoatArchetype()
{

}
void ASimEcs_BoatArchetype::SetMainMesh(UStaticMeshComponent* mainMesh)
{
	MainMeshComponent = mainMesh;
	//RootComponent = MainMeshComponent;
}

void ASimEcs_BoatArchetype::TickMove(FVector loc, UPrimitiveComponent* root)
{
	//BoatRoot = root;
	
	root->AddForceAtLocation(root->GetRightVector() * RightAxisValue * root->GetMass() * SteeringSpeed, loc);
	root->AddForce(root->GetForwardVector() * root->GetMass() * ForwardAxisValue * ForwardSpeed);
}

void ASimEcs_BoatArchetype::MainLoopLogic()
{
	if(MoveMode == ESimMoveMode_On)
	{
		MoveLoopImpl(MoveTarget);
	}
	else if(MoveMode == ESimMoveMode_Back)
	{
		MoveLoopImpl(BackTarget);
	}
}
void ASimEcs_BoatArchetype::CheckSpeedUp()
{
	if (MoveMode == ESimMoveMode_On || MoveMode == ESimMoveMode_Back)
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
			if (MoveMode == ESimMoveMode_On && (abs(CurrentSpeed) < StepSpeed*0.99))
			{
				MoveMode = ESimMoveMode_Fire;
				GetWorldTimerManager().SetTimer(DelayHandler, this, &ASimEcs_BoatArchetype::Active_Attack, AttackDelay);
			}
		}
	}
}

void ASimEcs_BoatArchetype::RuntimeStateLoop()
{
	//record Move Speed
	MoveSpeed = MainMeshComponent ? MainMeshComponent->GetPhysicsLinearVelocity().Size() : 0.0f;

	//record move distance		
	if (LastPos.Equals(FVector::ZeroVector))
		LastPos = GetActorLocation(); 
	else
	{
		float move = (LastPos - MainMeshComponent->GetComponentLocation()/*GetActorLocation()*/).Size2D();

		if(MoveMode == ESimMoveMode_On)
			MoveDistance += move;
		if (bSpeedDown)
			AccSpeedDownDistance += move;
		if (bRollBack)
			AccRollbakDistance += move;

		LastPos = MainMeshComponent->GetComponentLocation();// GetActorLocation();
	}	
}

void ASimEcs_BoatArchetype::MoveLoopImpl(ATargetPoint* target)
{
	ForwardAxisValue = CurrentSpeed;
	CheckSpeedUp();
	if(MoveMode == ESimMoveMode_On)
	{
		bRollBack = false;

		FRotator rot;
		rot.Yaw = FindLookAtRotation(GetActorLocation(), target->GetActorLocation()).Yaw;
		//SetActorRotation(rot);
		if(MainMeshComponent != NULL)
		{
			MainMeshComponent->SetWorldRotation(rot);
		}
	}
	else if(MoveMode == ESimMoveMode_Back)
	{
		bRollBack = true;

		if (!ReverseMoveEnd)//径直后退
		{
			ForwardAxisValue = -CurrentSpeed;
			FRotator rot = FindLookAtRotation(target->GetActorLocation(), GetActorLocation());
			//SetActorRotation(rot);
			if (MainMeshComponent != NULL)
			{
				MainMeshComponent->SetWorldRotation(rot);
			}
		}
		else
		{
			FRotator rot;
			float newYaw = FindLookAtRotation(target->GetActorLocation(), GetActorLocation()).Yaw;
			if (TurnIndex * TurnStep < 180)//逐渐转向180度
			{
				rot.Yaw =  newYaw + TurnIndex * TurnStep;
				//SetActorRotation(rot);
				if (MainMeshComponent != NULL)
				{
					MainMeshComponent->SetWorldRotation(rot);
				}
				TurnIndex++;
			}
			else //径直转向180度
			{
				rot.Yaw = newYaw + 180;
				//SetActorRotation(rot);
				if (MainMeshComponent != NULL)
				{
					MainMeshComponent->SetWorldRotation(rot);
				}
			}
		}
	}
}

void ASimEcs_BoatArchetype::Active_Turn()
{
	ReverseMoveEnd = true;
}

void ASimEcs_BoatArchetype::Active_MoveOn(FVector targetPos)
{
	if (targetPos != FVector::ZeroVector)
		MoveOnPos = targetPos;
	else
		MoveOnPos = MoveTarget->GetActorLocation();

	SetMoveMode( ESimOceanMoveMode::ESimMoveMode_On);
	SpeedUp();
}

void ASimEcs_BoatArchetype::Active_MoveBack(FVector targetPos)
{
	if (targetPos != FVector::ZeroVector)
		MoveBackPos = targetPos;
	else
		MoveBackPos = BackTarget->GetActorLocation();

	SetMoveMode( ESimOceanMoveMode::ESimMoveMode_Back);
	SpeedUp();
	GetWorldTimerManager().SetTimer(DelayHandler, this, &ASimEcs_BoatArchetype::Active_Turn, BackMoveTime);
}

void ASimEcs_BoatArchetype::Active_Attack()
{
	AttackPos = AttackTarget->GetActorLocation();

	FRotator rot;
	rot.Yaw = /*GetActorRotation().Yaw + */FindLookAtRotation(GetActorLocation(), AttackPos).Yaw;
	SetActorRotation(rot);

	/*if(!FireEnd)
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
	}*/
	//打完炮弹开始后退
	Active_MoveBack();
}

ESimOceanMoveMode ASimEcs_BoatArchetype::GetMoveMode()
{
	return MoveMode;
}

void ASimEcs_BoatArchetype::SetMoveMode_Implementation( ESimOceanMoveMode mode)
{
	MoveMode = mode;
}

void ASimEcs_BoatArchetype::SpeedUp_Implementation()
{
	ExpectSpeed = 1.0;
}

void ASimEcs_BoatArchetype::SpeedDown_Implementation()
{
	ExpectSpeed = 0;
}

//
//void ASimEcs_BoatArchetype::FireImpl_Implementation(int killerUID, FTransform trans, ExplodeType type)
//{
//
//}

void ASimEcs_BoatArchetype::RecordDestroy_Implementation(ESimObstacleType type)
{
	for (TMap<ESimObstacleType, int>::TIterator iter(DestroyResult); iter; ++iter)
	{
		if (iter->Key == type)
		{
			iter->Value++;
			return;
		}
	}
	DestroyResult.Add(type, 1);
}

int ASimEcs_BoatArchetype::GetDestroyNum_Implementation(ESimObstacleType type)
{
	return *DestroyResult.Find(type);
}
float ASimEcs_BoatArchetype::GetSpeed_Implementation()
{	
	return MoveSpeed;
}
float ASimEcs_BoatArchetype::GetSailDistance_Implementation()
{
	return MoveDistance;
}
float ASimEcs_BoatArchetype::GetSailAngle_Implementation()
{
	return 0.0f;
}
float ASimEcs_BoatArchetype::GetSpeedDownDistance_Implementation()
{
	return AccSpeedDownDistance;
}
float ASimEcs_BoatArchetype::GetHorizontalDistance_Implementation()
{
	return 0.0f;
}
float ASimEcs_BoatArchetype::GetRollbackDistance_Implementation()
{
	return AccRollbakDistance;
}
float ASimEcs_BoatArchetype::GetRollbackAngle_Implementation()
{
	return 0.0f;
}