// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SimEcs_Core.h"
#include "Engine.h"
#include "util/Handle.h"
#include "SimEcs_Platform.h"
#include "util/FamilyTypeID.h"
#include "SimEcs_ComponentWrapper.generated.h"


using ComponentId = ObjectID;
using ComponentTypeId = TypeID;
using EntityWrapperId = TypeID;
static const ComponentId INVALID_COMPONENT_ID = INVALID_OBJECT_ID;


class WrapperDataInitializer {
public:
	WrapperDataInitializer( ) {
		uHandleID = 0; jsonValue = "";
	}
	using EntityHandleId = u64;

	template<class T, class ...ARGS>
	void GrapData( T handleID, ARGS&&... args ) {
		uHandleID = handleID;
		GrapData( std::forward<ARGS>( args )...);
	}

	void GrapData( FString& strValue );
	void GrapData( ) {};
public:
	EntityHandleId uHandleID;
	FString jsonValue;
};

UINTERFACE( )
class UComponentWrapper : public UInterface {
	GENERATED_BODY( )
};


class IComponentWrapper  {
	GENERATED_BODY()
	// Called when the game starts
public:

	IComponentWrapper( );
	virtual void AddToEntity( u64 uHandleID, FString& jsonValue ) abstract;

	virtual void BeginPlay( ) abstract;
	static ComponentTypeId GetStaticComponentTypeID( ) 
	{
		return STATIC_COMPONENT_TYPE_ID;
	}

	static  ComponentTypeId STATIC_COMPONENT_TYPE_ID;

	ComponentId m_ComponentId;
	EntityWrapperId m_SimOwner;
	ComponentId m_SimHashValue;
	bool			m_Enabled;
};