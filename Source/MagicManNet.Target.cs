// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MagicManNetTarget : TargetRules
{
	public MagicManNetTarget(TargetInfo Target) : base (Target)
	{
		Type = TargetType.Game;
		ExtraModuleNames.Add("MagicManNet");
	}

	//
	// TargetRules interface.
	//

	// public override void SetupBinaries(
	// 	TargetInfo Target,
	// 	ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
	// 	ref List<string> OutExtraModuleNames
	// 	)
	// {
	// 	OutExtraModuleNames.Add("MagicManNet");
	// }
}
