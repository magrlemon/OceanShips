// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class EntitasFrame : ModuleRules
{
	public EntitasFrame(ReadOnlyTargetRules Target) : base(Target)
	{
        //PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        // UEBuildConfiguration.bForceEnableExceptions = true;
        PrivatePCHHeaderFile = "Public/EntitasFramePCH.h";

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore" ,
            "CommonPlugin",
            "HeadMountedDisplay",
            "AssetRegistry",
            "PhysX",
            "APEX",
            "ApexDestruction",
            "SimScenarioXmlParser",
            "SimCoastDefXmlParser",
            "Json",
            "JsonUtilities"
        });
        bEnableExceptions = true;
        PublicIncludePaths.AddRange(
        new string[] {
                    "EntitasFrame",
                    "EntitasFrame/Public",
                    "EntitasFrame/Public/ThirdParty",
                    "EntitasFrame/Public/SimEntitasBase",
                    "EntitasFrame/Public/SimEntitasFrame",
                    "EntitasFrame/Public/SimEntitasCore"
                    
            // ... add public include paths required here ...
        }
        );

        PrivateIncludePaths.AddRange(
            new string[] {
                 "EntitasFrame",
                    "EntitasFrame/Private",
                    "EntitasFrame/Private/SimEntitasBase",
                    "EntitasFrame/Private/SimEntitasFrame",
                    "EntitasFrame/Private/SimEntitasCore"
				// ... add other private include paths required here ...
			}
            );
    }
}
