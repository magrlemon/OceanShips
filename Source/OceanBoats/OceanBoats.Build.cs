// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class OceanBoats : ModuleRules
{
    private string ModulePath
    {
        get
        {
            return ModuleDirectory;
        }
    }
    private string BehaviacPath
    {
        get
        {
            return ModulePath + "/../../Source/Behaviac/";
        }
    }
    public OceanBoats(ReadOnlyTargetRules Target) : base(Target)
	{
        PrivatePCHHeaderFile = "Public/OceanBoats.h";
        
        PrivateIncludePaths.AddRange(
            new string[] {
                "OceanBoats/Private",
                "OceanBoats/Private/UI",
                "OceanBoats/Private/UI/Menu",
                "OceanBoats/Private/UI/Style",
                "OceanBoats/Private/UI/Widgets"
            }
        );

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "HeadMountedDisplay",
            "OnlineSubsystem",
            "OnlineSubsystemUtils",
            "AssetRegistry",
            "EntitasFrame",
            "AIModule",
            "GameplayTasks",
            "NavigationSystem",
            "ArmyLoadingScreen",
            "PhysX",
            "PhysXVehicles",
            "PhysXVehicleLib"
        });


        PrivateDependencyModuleNames.AddRange(
           new string[] {
                "InputCore",
                "Slate",
                "SlateCore",
                "Json",
                "ApplicationCore",
                "ReplicationGraph",
                "PakFile",
                "RHI",
                "OceanPlugin"
           }
       );
        DynamicallyLoadedModuleNames.AddRange(
            new string[] {
                "OnlineSubsystemNull",
                "NetworkReplayStreaming",
                "NullNetworkReplayStreaming",
                "HttpNetworkReplayStreaming",
                "LocalFileNetworkReplayStreaming"
            }
        );
        PublicIncludePaths.Add(BehaviacPath + "include/");
        PublicAdditionalLibraries.Add(BehaviacPath + "lib/behaviac_releasestatic_win64_vs2017.lib");
        PublicAdditionalLibraries.Add(BehaviacPath + "lib/LibAiAgent.lib");
        bEnableUndefinedIdentifierWarnings = false;
    }
}
