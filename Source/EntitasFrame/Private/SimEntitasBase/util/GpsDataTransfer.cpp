
#include "util/GpsDataTransfer.h"
#include "Engine.h"

GpsDataTransfer::GpsDataTransfer( ) {

}

void GpsDataTransfer::InitData( FVector UeScenePos, DVector2 JwdPos ) {

	m_UESceneCenterPosition = UeScenePos;
	m_UESceneTopRightPosition = m_UESceneCenterPosition + FVector( 5000.0f, 5000.0f, 0.0f  );//长宽500m
	m_UESceneBottomLeftPosition = m_UESceneCenterPosition + FVector( -5000.0f, -5000.0f ,0.0f);//长宽500m

	m_JWDCenterPosition = JwdPos;
	m_JWDTopRightPoint = m_JWDCenterPosition + DVector2( 0.00001141 * 5000, 0.00000899 * 5000 );
	m_JWDBottomLeftPoint = m_JWDCenterPosition + DVector2( -0.00001141 * 5000, -0.00000899 * 5000 );

	z_offset = m_JWDBottomLeftPoint.Y - m_JWDTopRightPoint.Y;
	x_offset = m_JWDBottomLeftPoint.X - m_JWDTopRightPoint.X;
	z_w_offset = (m_UESceneBottomLeftPosition.Y - m_UESceneTopRightPosition.Y);
	x_w_offset = (m_UESceneBottomLeftPosition.X - m_UESceneTopRightPosition.X);
}

double GpsDataTransfer::Abs( double x, double y )
{
	double v = x - y;
	if (v < 0.0)
		return v * -1.0;
	return v;
}



FVector GpsDataTransfer::ChangeLatLngSpaceToUESceneSpace( double Posx, double Posy )
{
	GetWorldPoint( Posx, Posy );
	return  FVector( (float)(m_dConverX), 0.0f, (float)(m_dConverY) );
}

FVector GpsDataTransfer::ChangeLatLngSpaceToUESceneSpace( FString strPosx, FString strPosy )
{
	GetWorldPoint( FCString::Atod( *strPosx ), FCString::Atod( *strPosy ) );

	return  FVector( (float)(m_dConverX), 0.0f, (float)(m_dConverY) );
}



 void GpsDataTransfer::GetWorldPoint( double ltx, double lgy ) {
	if (m_dOffsetLon_x == 0 || m_dOffsetLati_z == 0)
		return;

	double tempX = ltx - m_JWDTopRightPoint.X;       //经线
	double tempZ = lgy - m_JWDBottomLeftPoint.Y;   //维线   

	double cx = (tempX * m_dXWoffsetPoint / m_dOffsetLon_x + m_JWDTopRightPoint.X);//经度
	double cz = (tempZ * m_dZWoffsetPoint / m_dOffsetLati_z + m_JWDBottomLeftPoint.Y);    //维度

	DVector3 tempPos =  DVector3( (float)cx, 0.0f, (float)cz );


	m_dZWoffsetPoint = tempPos.X;
	m_dConverY = tempPos.Y;
}

/// <summary>
/// 通用获取坐标
/// </summary>
/// <param name="se"></param>
/// <returns></returns>
FVector GpsDataTransfer::GetWorldPoint( DVector3 se )
{
	double tempX = se.X - m_JWDTopRightPoint.X;
	double tempZ = se.Y - m_JWDBottomLeftPoint.Y;
	double _tempX = (tempX * x_w_offset / x_offset + m_JWDTopRightPoint.X);
	double _tempZ = (tempZ * z_w_offset / z_offset + m_UESceneBottomLeftPosition.Y);

	return  FVector( (float)_tempX, 0, (float)_tempZ );
}

DVector2 GpsDataTransfer::GetLatLon( FVector currentPoint )
{
	double _x_offset = (currentPoint.X/100.0f - m_UESceneBottomLeftPosition.X) * x_offset / x_w_offset;
	double _z_offset = (currentPoint.Y/100.0f - m_UESceneTopRightPosition.Y) * z_offset / z_w_offset;
	double resultX = _x_offset + m_JWDBottomLeftPoint.X;
	double resultY = _z_offset + m_JWDTopRightPoint.Y;
	return  DVector2( resultX, resultY );
}


