// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "OceanBoats.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistryModule.h"
#include "IAssetRegistry.h"
#include "ArmySimGameDelegates.h"
#include "ArmySimMenuSoundsWidgetStyle.h"
#include "SoldierMenuWidgetStyle.h"
#include "ArmySimMenuItemWidgetStyle.h"
#include "SoldierOptionsWidgetStyle.h"
#include "SoldierScoreboardWidgetStyle.h"
#include "SoldierChatWidgetStyle.h"

#include "ArmySimStyle.h"

class FOceanBoatsGameModule : public FDefaultGameModuleImpl {

	virtual void StartupModule( ) override
	{
		InitializeArmySimGameDelegates( );
		FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>( TEXT( "AssetRegistry" ) );
		 
		//Hot reload hack
		FSlateStyleRegistry::UnRegisterSlateStyle( FArmySimStyle::GetStyleSetName( ) );
		FArmySimStyle::Initialize( );
	}

	virtual void ShutdownModule( ) override
	{
		FArmySimStyle::Shutdown( );
	}
};


IMPLEMENT_PRIMARY_GAME_MODULE( FOceanBoatsGameModule, OceanBoats, "OceanBoats" );


DEFINE_LOG_CATEGORY( LogShooter )
DEFINE_LOG_CATEGORY( LogShooterWeapon )