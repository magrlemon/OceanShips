// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
//
//
//DECLARE_STATS_GROUP(TEXT("SimEcs"), STATGROUP_ECS, STATCAT_Advanced);
//
//
//DECLARE_LOG_CATEGORY_EXTERN(LogFlying, Log, All);


// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h" 


class FSimEntitasFrameModule : public IModuleInterface {
public:

	/** IModuleInterface implementation */
	virtual void StartupModule( ) override;
	virtual void ShutdownModule( ) override;
};


DECLARE_STATS_GROUP( TEXT( "SimEcs" ), STATGROUP_ECS, STATCAT_Advanced );


DECLARE_LOG_CATEGORY_EXTERN( LogFlying, Log, All );
