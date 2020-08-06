// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

// This module must be loaded "PreLoadingScreen" in the .uproject file, otherwise it will not hook in time!

public class SimCoastDefXmlParser : ModuleRules
{
    public SimCoastDefXmlParser(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivatePCHHeaderFile = "Public/ArmyCoastDefXmlParserModule.h";

        //PCHUsage = PCHUsageMode.UseSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "MoviePlayer",
                "Slate",
                "SlateCore",
                "InputCore",
                "XmlParser"
            }
        );
    }
}
