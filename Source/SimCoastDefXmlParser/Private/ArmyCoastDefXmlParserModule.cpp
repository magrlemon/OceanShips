// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArmyCoastDefXmlParserModule.h"
#include "GenericApplication.h"
#include "GenericApplicationMessageHandler.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "ArmyCoastDefLoader.h"


class FArmyCoastDefXmlParserModule : public ICoastDefXmlParserModule
{
public:
	virtual void StartupModule() override
	{	
		
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void GetCoastDefXmlParserInfo(   TArray<TMap<FString, FString>>& refContain )  {
		//using TArrScenario = const  TArray<TMap<FString, FString>>;
		refContain = ArmyCoastDefLoader::GetInstance( ).GetScenarioData( );

	}

	virtual void StartCoastDefXmlParser() override
	{
		FString strPath = FPaths::ProjectDir( ) + TEXT( "Config/CoastDefenceXML/CoastDefenceScena.xml" );
		ArmyCoastDefLoader::GetInstance( ).Init( strPath );
	}
};

IMPLEMENT_GAME_MODULE( FArmyCoastDefXmlParserModule, SimCoastDefXmlParser);
