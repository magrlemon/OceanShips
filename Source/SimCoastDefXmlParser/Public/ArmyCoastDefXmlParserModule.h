// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once


#include "ModuleInterface.h"
#include "GenericApplication.h"

/** Module interface for this game's loading screens */
class ICoastDefXmlParserModule : public IModuleInterface          
{
public:
	/** Kicks off the loading screen for in game loading (not startup) */
	virtual void StartCoastDefXmlParser() abstract;
	virtual void GetCoastDefXmlParserInfo( TArray<TMap<FString, FString>>& refContain ) abstract;

};


