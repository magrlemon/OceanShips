

#pragma once

#include "CoreMinimal.h"
#include "DVector2.h"
#include "DVector3.h"
#include "SimEcs_Core.h"


class  GpsDataTransfer {

public:
	GpsDataTransfer( );

	void InitData( FVector UeScenePos = FVector(-100.0f, -370.0f,0.0f ), DVector2 JwdPos = DVector2( 115.584444f, 40.273333f) );

	double Abs( double x, double y );

	FVector ChangeLatLngSpaceToUESceneSpace( double Posx, double Posy );

	FVector ChangeLatLngSpaceToUESceneSpace( FString strPosx, FString strPosy );

	/// <summary>
	/// 通用获取坐标
	/// </summary>
	/// <param name="se"></param>
	/// <returns></returns>
	FVector GetWorldPoint( DVector3 se );

	void GetWorldPoint( double ltx, double lgy );

	DVector2 GetLatLon( FVector currentPoint );


private:
	double m_dOffsetLati_z, m_dOffsetLon_x, m_dZWoffsetPoint, m_dXWoffsetPoint;

	double m_dConverX, m_dConverY;

	FVector m_UESceneCenterPosition;// UE Scene 中心点坐标
	FVector m_UESceneTopRightPosition;//右上角
	FVector m_UESceneBottomLeftPosition;

	DVector2 m_JWDCenterPosition;//经纬度中心点
	DVector2 m_JWDTopRightPoint; //经纬度右上角
	DVector2 m_JWDBottomLeftPoint;

	double z_offset, x_offset, z_w_offset, x_w_offset;
};


