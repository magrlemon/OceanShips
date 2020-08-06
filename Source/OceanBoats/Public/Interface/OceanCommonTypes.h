#pragma once

UENUM()
enum OceanMoveMode
{
	EMoveMode_On,
	EMoveMode_Back,
	EMoveMode_Fire,/*
	EMoveMode_SpeedUp,
	EMoveMode_SpeedDown,*/
	EMoveMode_Idle
};

UENUM()
enum ObstacleType
{
	Obstacle_GTZ,
	Obstacle_STZ,
	Obstacle_HLS,
	Obstacle_Invalid
};

UENUM()
enum ExplodeType
{
	Explode_Normal,
	Explode_TBD
};

struct OceanObstacleCls
{
public:
	ObstacleType type = Obstacle_Invalid;
	FString name = "";
	int total = 0;
	int destroyed = 0;
	uint32 killerUID = -1;

	bool operator==(const OceanObstacleCls& other) const
	{
		return (type == other.type);
	}
};
