#pragma once

UENUM()
enum ESimOceanMoveMode
{
	ESimMoveMode_On,
	ESimMoveMode_Back,
	ESimMoveMode_Fire,
	ESimMoveMode_SpeedUp,
	ESimMoveMode_SpeedDown,
	ESimMoveMode_Idle
};

UENUM()
enum ESimObstacleType
{
	ESimObstacle_GTZ,
	ESimObstacle_STZ,
	ESimObstacle_HLS,
	ESimObstacle_Invalid
};

UENUM()
enum ESimExplodeType
{
	ESimExplode_Normal,
	ESimExplode_TBD
};

struct ESimOceanObstacleCls
{
public:
	ESimObstacleType type = ESimObstacle_Invalid;
	FString name = "";
	int total = 0;
	int destroyed = 0;
	uint32 killerUID = -1;

	bool operator==(const ESimOceanObstacleCls& other) const
	{
		return (type == other.type);
	}
};
