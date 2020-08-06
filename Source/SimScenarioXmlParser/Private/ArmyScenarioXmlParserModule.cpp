// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ArmyScenarioXmlParserModule.h"
#include "GenericApplication.h"
#include "GenericApplicationMessageHandler.h"
#include "SlateBasics.h"
#include "SlateExtras.h"
#include "ArmyScenarioLoader.h"


class FScenarioXmlParserModule : public IScenarioXmlParserModule
{
public:
	virtual void StartupModule() override
	{	
		
	}
	
	virtual bool IsGameModule() const override
	{
		return true;
	}

	virtual void GetScenarioXmlParserInfo(   TArray<TMap<FString, FString>>& refContain )  {
		//using TArrScenario = const  TArray<TMap<FString, FString>>;
		refContain = ArmyScenarioLoader::GetInstance( ).GetScenarioData( );

	}

	virtual void StartScenarioXmlParser() override
	{
		FString strPath = FPaths::ProjectDir( ) + TEXT( "Config/ScenarioXML/Scenario002.xml" );
		ArmyScenarioLoader::GetInstance( ).Init( strPath );
	}
};

IMPLEMENT_GAME_MODULE(FScenarioXmlParserModule, SimScenarioXmlParser);
