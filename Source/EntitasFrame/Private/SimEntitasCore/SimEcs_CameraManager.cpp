
#include "SimTextRenderActor.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "SimEcs_CameraManager.h"
#include "UnrealMathUtility.h"



ASimEcs_PlayerCameraManager::ASimEcs_PlayerCameraManager( const FObjectInitializer& ObjectInitializer ) : Super( ObjectInitializer )
{
	NormalFOV = 90.0f;
	TargetingFOV = 60.0f;
	ViewPitchMin = -87.0f;
	ViewPitchMax = 87.0f;
	bAlwaysApplyModifiers = true;
}

void ASimEcs_PlayerCameraManager::UpdateCamera( float DeltaTime )
{
	/*ASoldierCharacter* MyPawn = PCOwner ? Cast<ASoldierCharacter>( PCOwner->GetPawn( ) ) : NULL;
	if (MyPawn && MyPawn->IsFirstPerson( )) {
		const float TargetFOV = MyPawn->IsTargeting( ) ? TargetingFOV : NormalFOV;
		DefaultFOV = FMath::FInterpTo( DefaultFOV, TargetFOV, DeltaTime, 20.0f );
	}*/

	Super::UpdateCamera( DeltaTime );

}


// Sets default values
SimEcs_CameraPointActor::SimEcs_CameraPointActor( )
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.

}

// Called when the game starts or when spawned
void SimEcs_CameraPointActor::BeginPlay( )
{
	

}


// 通过将Points给出的坐标组投影到Normal轴上获得最小的一个结果（考虑到它，我们也将偏移大小）。
float SimEcs_CameraPointActor::GetMinProjection( FVector Normal, const TArray<FVector> &Points, const TArray<float> &Sizes )
{
	float Retval = MAX_FLT;

	for (int i = 0; i < Points.Num( ); ++i) {
		Retval = FMath::Min( Retval, FVector::DotProduct( Points[i], Normal ) - Sizes[i] );
	}

	return Retval;
}

// 根据目标位置/大小和摄像机方向确定最佳摄像机位置
FVector SimEcs_CameraPointActor::CalcCameraLocationByTargetPoints(
	const TArray<FVector> &Points, const TArray<float> &Sizes,const FRotator CameraRotation, const float FovXDegrees,
	const float AspectRatio, const float FovLeftMargin,const float FovRightMargin, 
	const float FovBottomMargin,const float FovTopMargin )
{
	const float HalfFov_X = FMath::DegreesToRadians( FovXDegrees*0.5f );

	const float HalfTanX = FMath::Tan( HalfFov_X );
	const float HalfTanY = HalfTanX / AspectRatio;

	//支持不对称的安全框架
	//获取上/下/左/右安全框的视角
	const float HalfFovLeft = FMath::Atan( HalfTanX * (1.0f - FovLeftMargin * 2.0f) );
	const float HalfFovRight = FMath::Atan( HalfTanX * (1.0f - FovRightMargin * 2.0f) );
	const float HalfFovBottom = FMath::Atan( HalfTanY * (1.0f - FovBottomMargin * 2.0f) );
	const float HalfFovTop = FMath::Atan( HalfTanY * (1.0f - FovTopMargin * 2.0f) );

	const float HalfFovLeftDeg = FMath::RadiansToDegrees( HalfFovLeft );
	const float HalfFovRightDeg = FMath::RadiansToDegrees( HalfFovRight );
	const float HalfFovBottomDeg = FMath::RadiansToDegrees( HalfFovBottom );
	const float HalfFovTopDeg = FMath::RadiansToDegrees( HalfFovTop );

	//通过使坐标轴平行于摄像机的坐标轴来简化计算。
	//原样调整原点，仅旋转
	//请注意，相机的坐标不是原点。
	TArray<FVector> PointsCS;
	PointsCS.SetNum( Points.Num( ) );

	for (int i = 0; i < Points.Num( ); ++i) {
		PointsCS[i] = CameraRotation.UnrotateVector( Points[i] );
	}

	// 在此处存储计算结果
	FVector CameraLocationCS;


	// 左右面的约束
	float XbyLR;
	float Dydx;
	{
		// 在与摄像机平行的坐标系中，可以在XY平面上计算
		const FVector LeftBoundNormalCS = FVector::RightVector.RotateAngleAxis( -HalfFovLeftDeg, FVector::UpVector );
		const float LeftBoundPosCS = GetMinProjection( LeftBoundNormalCS, PointsCS, Sizes );

		const FVector RightBoundNormalCS = -FVector::RightVector.RotateAngleAxis( HalfFovRightDeg, FVector::UpVector );
		const float RightBoundPosCS = GetMinProjection( RightBoundNormalCS, PointsCS, Sizes );

		//求解联立方程。
		// Q是投影在XY平面上的左右面的交点
		// dot(Q, LeftBoundNormalCS) = LeftBoundPosCS
		// dot(Q, RightBoundNormalCS) = RightBoundPosCS
		// 
		// 组织并​​获得以下公式
		const float QX = (RightBoundNormalCS.Y*LeftBoundPosCS - LeftBoundNormalCS.Y*RightBoundPosCS) / (LeftBoundNormalCS.X*RightBoundNormalCS.Y - LeftBoundNormalCS.Y*RightBoundNormalCS.X);
		const float QY = (LeftBoundPosCS - QX * LeftBoundNormalCS.X) / LeftBoundNormalCS.Y;

		//我们会暂时存储解决方案。
		// X是临时值，有必要将其与从上下两个条件获得的值进行比较。
		// Y由此决定。
		//Z在检查上下表面的状况之前不理解。
		XbyLR = QX;
		CameraLocationCS.Y = QY;

		// 保存Y相对于X的变化量（处理安全帧不对称）
		Dydx = HalfTanX * (FovLeftMargin - FovRightMargin);
	}

	// 上下面的约束
	float XbyTB;
	float Dzdx;
	{
		// 在XZ平面上计算
		const FVector BottomBoundNormalCS = FVector::UpVector.RotateAngleAxis( -HalfFovBottomDeg, -FVector::RightVector );
		const float BottomBoundPosCS = GetMinProjection( BottomBoundNormalCS, PointsCS, Sizes );

		const FVector TopBoundNormalCS = -FVector::UpVector.RotateAngleAxis( HalfFovTopDeg, -FVector::RightVector );
		const float TopBoundPosCS = GetMinProjection( TopBoundNormalCS, PointsCS, Sizes );

		// 与左/右侧相同。
		const float QX = (TopBoundNormalCS.Z*BottomBoundPosCS - BottomBoundNormalCS.Z*TopBoundPosCS) / (BottomBoundNormalCS.X*TopBoundNormalCS.Z - BottomBoundNormalCS.Z*TopBoundNormalCS.X);
		const float QZ = (BottomBoundPosCS - QX * BottomBoundNormalCS.X) / BottomBoundNormalCS.Z;

		// 从左/右，上/下条件（向后拉）获得的X中较小的X是获得的值。
		XbyTB = QX;
		CameraLocationCS.Z = QZ;

		// 保存Z相对于X的变化量（处理安全帧不对称）
		Dzdx = HalfTanY * (FovBottomMargin - FovTopMargin);
	}

	CameraLocationCS.X = FMath::Min( XbyLR, XbyTB );
	CameraLocationCS.Y += (Dydx * (CameraLocationCS.X - XbyLR));
	CameraLocationCS.Z += (Dzdx * (CameraLocationCS.X - XbyTB));

	// 恢复到世界坐标并返回
	return CameraRotation.RotateVector( CameraLocationCS );
}