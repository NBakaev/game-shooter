// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MagicManNet : ModuleRules
{
    public string ProjectRoot
    {
        get
        {
            return System.IO.Path.GetFullPath(
                System.IO.Path.Combine(ModuleDirectory, "../../")
            );
        }
    }

    public MagicManNet(TargetInfo Target)
    {
        bEnableExceptions = true;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG" });
	    PrivateDependencyModuleNames.AddRange(new string[] {"Slate", "SlateCore"});

        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/gen_hpack_tables.lib");
        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/gpr.lib");
        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/grpc_unsecure.lib");
        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/grpc++_unsecure.lib");
        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/libprotobuf.lib");
        PublicAdditionalLibraries.Add(ProjectRoot + "/Source/ThirdParty/Libraries/zlib.lib");

        PublicIncludePaths.Add(ProjectRoot + "/Source/ThirdParty/Includes");
    }
}
