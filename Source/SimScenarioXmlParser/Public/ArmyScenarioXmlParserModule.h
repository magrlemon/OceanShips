// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#ifndef _ARMY_SCENARIOXMLPARSER_H__
#define _ARMY_SCENARIOXMLPARSER_H__

#include "ModuleInterface.h"
#include "GenericApplication.h"

/** Module interface for this game's loading screens */
class IScenarioXmlParserModule : public IModuleInterface
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartScenarioXmlParser() abstract;
	virtual void GetScenarioXmlParserInfo( TArray<TMap<FString, FString>>& refContain ) abstract;

};

#endif // _ARMY_SCENARIOXMLPARSER_H__
