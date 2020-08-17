/**
* Copyright 2019 北京伯睿科技有限公司.
* All Rights Reserved.
* http://www.BoRui.com
*
* @File SimTextRenderActor.h
* @Description 3D 字体
*
* @Author LiuKX
* @Date 2019年7月3日
* @Version 1.0
*/
#pragma once

#include "CoreMinimal.h"
#include "Engine/TextRenderActor.h"
#include "SimEcs_CameraManager.generated.h"

/**
 * 
 */


class  SimEcs_CameraPointActor {

public:
	// Sets default values for this actor's properties
	SimEcs_CameraPointActor( );
	~SimEcs_CameraPointActor( ) {};
protected:
	// Called when the game starts or when spawned
	void BeginPlay( );

public:
	// Called every frame

	float GetMinProjection( FVector Normal, const TArray<FVector> &Points, const TArray<float> &Sizes );

	FVector CalcCameraLocationByTargetPoints( const TArray<FVector> &Points, const TArray<float> &Sizes,
		const FRotator CameraRotation, const float FovXDegrees, const float AspectRatio, const float FovLeftMargin,
		const float FovRightMargin, const float FovBottomMargin, const float FovTopMargin );
};

UCLASS( )
class ASimEcs_PlayerCameraManager : public APlayerCameraManager {
	GENERATED_UCLASS_BODY( )

public:

	/** After updating camera, inform pawn to update 1p mesh to match camera's location&rotation */
	virtual void UpdateCamera( float DeltaTime ) override;

	/** normal FOV */
	float NormalFOV;

	/** targeting FOV */
	float TargetingFOV;

	SimEcs_CameraPointActor m_SimCameraPointActor;

};

