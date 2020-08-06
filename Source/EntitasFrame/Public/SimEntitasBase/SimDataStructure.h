
#pragma once

#include "EntitasFramePCH.h"
#include "SimEcs_Platform.h"


using TMapScenario = const  TMap<FString, FString>;

class  FSPropertyDataBase
{

public:
	/** Default constructor. */
	FSPropertyDataBase() {
		entType = 0;			// entity number
		entDirection = 0.0f;
		entID = "";				// entity serial number
		entName = "";
		entPos = FVector::ZeroVector;
	};
	~FSPropertyDataBase() {}

	int32		 entType;			// entity Type
	int32		 entSubType;		// entity sub Type
	float		 entDirection;
	FName		 entID;				// entity serial number
	FString		 entName;
	FVector		 entPos;
};


class  FSTankPropertyData : public FSPropertyDataBase
{
public:
	FSTankPropertyData() {
		maxSpeed = 100.0f;
		launchSpeed = 10.0f;
	};

	FSTankPropertyData& operator = (TMapScenario & refScenaData);

	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_TANK_TYPE;

	float maxSpeed;
	float launchSpeed;

};


class  FSCharactorPropertyData : public FSPropertyDataBase
{
public:
	FSCharactorPropertyData() {
		maxSpeed = 100.0f;
		launchSpeed = 10.0f;
	};

	FSCharactorPropertyData& operator = (TMapScenario & refScenaData);
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_CHARACTOR_TYPE;
	float maxSpeed;
	float launchSpeed;
};


class  FSAirCraftPropertyData : public FSPropertyDataBase
{
public:
	FSAirCraftPropertyData() {
		maxSpeed = 100.0f;
		launchSpeed = 10.0f;
	};

	FSAirCraftPropertyData& operator = (TMapScenario & refScenaData);
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_AIRCRAFT_TYPE;
	float maxSpeed;
	float launchSpeed;
};


class  FSHelicopterPropertyData : public FSPropertyDataBase
{
public:

	FSHelicopterPropertyData() {
		maxYawSpeed = 1.0f;
		maxSpeed = 100.0f;
		launchSpeed;
		acceleration = 150.0f;
		maxSpeedUpDown = 10.0f;
		accelerationStrength = 1.0f;
		maxSpeedForward = 25.0f;
		maxSpeedBackward = 25.0f;
		maxSpeedLeftRight = 15.0f;
	}

	FSHelicopterPropertyData& operator = (TMapScenario& refScenaData);


	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_HELICOPTER_TYPE;

	float maxYawSpeed = 1;
	float maxSpeed = 100;
	float launchSpeed;
	float acceleration = 150;
	float maxSpeedUpDown = 10;
	float accelerationStrength = 1;
	float maxSpeedForward = 25;
	float maxSpeedBackward = 25;
	float maxSpeedLeftRight = 15;
};

/* */

class   FSDroneBoatPropertyData : public FSPropertyDataBase
{
public:
	FSDroneBoatPropertyData() {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};

	FSDroneBoatPropertyData& operator = (TMapScenario& refScenaData);

	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_DRONEBOAT_TYPE;


	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};

//////////////////////////////////////////////////////////////////////////
class   FSNavalCraftPropertyData : public FSPropertyDataBase
{
public:
	FSNavalCraftPropertyData() {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};

	FSNavalCraftPropertyData& operator = (TMapScenario& refScenaData);
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_NAVALCRAFT_TYPE;
	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};

class   FSVehicle4WPropertyData : public FSPropertyDataBase
{
public:

	FSVehicle4WPropertyData() {
		maxSpeed = 1000.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 9.8f;
	};

	FSVehicle4WPropertyData& operator = (TMapScenario& refScenaData);
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_VEHICLE4W_TYPE;
	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};


/* */

class   FSPatrolBoatPropertyData : public FSPropertyDataBase {
public:
	FSPatrolBoatPropertyData( ) {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};

	FSPatrolBoatPropertyData& operator = ( TMapScenario& refScenaData );

	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_PATROLBOAT_TYPE;

	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};


/* */

class   FSTrafficBoatPropertyData : public FSPropertyDataBase {
public:
	FSTrafficBoatPropertyData( ) {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};

	FSTrafficBoatPropertyData& operator = ( TMapScenario& refScenaData );

	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_TRAFFICBOAT_TYPE;


	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};


/* */

class   FSEngineeringBoatPropertyData : public FSPropertyDataBase {
public:
	FSEngineeringBoatPropertyData( ) {
		maxSpeed = 100.0f;
		launchSpeed = 1.0f;
		minPower = 0.0f;
		maxPower = 1000.0f;
		acceleration = 1.0f;
	};

	FSEngineeringBoatPropertyData& operator = ( TMapScenario& refScenaData );

	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_ENGINEERINBOAT_TYPE;

	float maxSpeed;
	float launchSpeed;
	float minPower;
	float maxPower;
	float acceleration;
};



//////////////////////////////////////////////////////////////////////////
/// ’œ∞≠ŒÔ
//////////////////////////////////////////////////////////////////////////


class   FSCoastDefGTZPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefGTZPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefGTZPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_GTZ_TYPE;
	float heath;
};

class   FSCoastDefSJZPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefSJZPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefSJZPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_SJZ_TYPE;
	float heath;
};

class   FSCoastDefHLSPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefHLSPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefHLSPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};



class   FSCoastDefGCWPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefGCWPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefGCWPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_GTZ_TYPE;
	float heath;
};

class   FSCoastDefTSWSFPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefTSWSFPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefTSWSFPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_SJZ_TYPE;
	float heath;
};

class   FSCoastDefTSWYLPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefTSWYLPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefTSWYLPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};

class   FSCoastDefZGQPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefZGQPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefZGQPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};

class   FSCoastDefHHLCPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefHHLCPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefHHLCPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};


class   FSCoastDefBLDSPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefBLDSPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefBLDSPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};


class   FSCoastDefGTZSUIJIPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefGTZSUIJIPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefGTZSUIJIPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};


class   FSCoastDefZJHPropertyData : public FSPropertyDataBase {
public:

	FSCoastDefZJHPropertyData( ) {
		heath = 100.0f;
	};

	FSCoastDefZJHPropertyData& operator = ( TMapScenario& refScenaData );
	static const ComponentTypeId STATIC_ENTITY_TYPE_ID = EEntityEquips::EEE_COASTDEF_HLS_TYPE;
	float heath;
};






