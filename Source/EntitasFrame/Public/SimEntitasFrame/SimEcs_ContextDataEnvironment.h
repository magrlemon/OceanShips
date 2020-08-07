// Fill out your copyright notice in the Description page of Project Settings.
/**
* Copyright 2019 北京伯睿科技有限公司.
*
* @File ContextDataXXXEnvironment.h
* @Description 想定数据初始化组件
*
* @Author magi
* @Date 2020年4月9日
* @Version 1.0
*/

#pragma once

#include "CoreMinimal.h"
#include "SimEcs_Engine.h"
#include "Engine.h"
#include "SimEcs_Platform.h"
#include "GameFramework/Actor.h"
#include "UObject/UObjectGlobals.h"
#include "SimEcs_ComponentWrapper.h"
#include "SimDataStructure.h"
#include "SimEcs_ContextDataEnvironment.generated.h"


using TMapScenario = const  TMap<FString, FString>;


UCLASS( )
class  ASimEnvironment : public AActor {
	GENERATED_BODY( )
public:
	
	ASimEnvironment( const FObjectInitializer& ObjectInitializer );

	virtual ASimEnvironment* Clone( ) const {
		return nullptr;  
	};

	void* Get( ) const {
		return nullptr;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return nullptr;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) {};
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData );
};


/* Tank */
UCLASS( )
class  ASimContextDataTankEnvironment : public ASimEnvironment{
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

	auto Get() const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay( ) override;

private:
	FSTankPropertyData  m_STPropData;
	FString  m_JsonValue;
};

/* Charactor */
UCLASS( )
class  ASimContextDataCharactorEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	//GENERATED_COMPONENT_BODY( ContextDataCharactorEnvironment );

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};
protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCharactorPropertyData m_STPropData;
	FString  m_JsonValue;
};

/*AirCraft */

UCLASS( )
class  ASimContextDataAirCraftEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties

	//GENERATED_COMPONENT_BODY( ContextDataAirCraftEnvironment );

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};
protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSAirCraftPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* Helicopter */
UCLASS( )
class  ASimContextDataHelicopterEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties

	//GENERATED_COMPONENT_BODY( ContextDataHelicopterEnvironment );

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};
protected:
	// Called when the game starts
	void BeginPlay( );

private:
	
	FSHelicopterPropertyData m_STPropData;
	FString  m_JsonValue;
};



/* */
UCLASS()
class  ASimContextDataDroneBoatEnvironment : public ASimEnvironment
{
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	virtual void CreateEntity(TWeakPtr<SimEcs_Engine> pSimEcs_Engine) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

	auto Get() const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType() const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay();

private:
	FSDroneBoatPropertyData m_STPropData;
	FString  m_JsonValue;
};

/* */
UCLASS( )
class  ASimContextDataNavalCraftEnvironment : public ASimEnvironment{
	GENERATED_UCLASS_BODY( )
public:

	//GENERATED_COMPONENT_BODY( ContextDataNavalCraftEnvironment );

	virtual void  CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSNavalCraftPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* */
UCLASS( )
class  ASimContextDataVehicle4WEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure(TMapScenario & refScenarixData) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSVehicle4WPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* */
UCLASS( )
class  ASimContextDataPatrolBoatEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSPatrolBoatPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* */
UCLASS( )
class  ASimContextDataTrafficBoatEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSTrafficBoatPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* 交通艇 */
UCLASS( )
class  ASimContextDataEngineeringBoatEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:
	// Sets default values for this component's properties
	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSEngineeringBoatPropertyData m_STPropData;
	FString  m_JsonValue;
};


//////////////////////////////////////////////////////////////////////////
//	Ocean Barrier /* GTZ  HLS SJZ */
//////////////////////////////////////////////////////////////////////////
UCLASS( )
class  ASimContextDataGTZEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefGTZPropertyData m_STPropData;
	FString  m_JsonValue;
};

/* GTZ  HLS SJZ */
UCLASS( )
class  ASimContextDataSJZEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefSJZPropertyData m_STPropData;
	FString  m_JsonValue;
};


/* GTZ  HLS SJZ */
UCLASS( )
class  ASimContextDataHLSEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefHLSPropertyData m_STPropData;
	FString  m_JsonValue;
};


UCLASS( )
class  ASimContextDataGCWEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefGCWPropertyData m_STPropData;
	FString  m_JsonValue;
};

/* TSW_SF 蛇腹铁丝网 */
UCLASS( )
class  ASimContextDataTSWSFEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefTSWSFPropertyData m_STPropData;
	FString  m_JsonValue;
};
 

/* TSW_YL */
UCLASS( )
class  ASimContextDataTSWYLEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefTSWYLPropertyData m_STPropData;
	FString  m_JsonValue;
};



/* //组隔墙		 1007 */
UCLASS( )
class  ASimContextDataZGQEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefZGQPropertyData m_STPropData;
	FString  m_JsonValue;
};



//混合雷场		 1008
UCLASS( )
class  ASimContextDataHHLCEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefHHLCPropertyData m_STPropData;
	FString  m_JsonValue;
};




//玻璃刀山		 1009
UCLASS( )
class  ASimContextDataBLDSEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefBLDSPropertyData m_STPropData;
	FString  m_JsonValue;
};


//水迹轨条砦	 1010
UCLASS( )
class  ASimContextDataGTZSUIJIEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefGTZSUIJIPropertyData m_STPropData;
	FString  m_JsonValue;
};



//阻绝壕		 1011
UCLASS( )
class  ASimContextDataZJHEnvironment : public ASimEnvironment {
	GENERATED_UCLASS_BODY( )
public:

	/* get dataStructure */
	auto Get( ) const {
		return m_STPropData;
	};

	/*return Entity Type { Entity Enum } */
	auto GetEntityType( ) const {
		return m_STPropData.entType;
	};

	virtual void CreateEntity( TWeakPtr<SimEcs_Engine> pSimEcs_Engine ) override;
	/* serialize data form scenario xml*/
	virtual void SerializeStructure( TMapScenario & refScenarixData ) override;

protected:
	// Called when the game starts
	void BeginPlay( );

private:
	FSCoastDefZJHPropertyData m_STPropData;
	FString  m_JsonValue;
};

