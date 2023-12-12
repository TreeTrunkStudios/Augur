// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class AugurTarget : TargetRules
{
	public AugurTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bLegacyParentIncludePaths = false;
		CppStandard = CppStandardVersion.Default;
		WindowsPlatform.bStrictConformanceMode = true;
		ExtraModuleNames.Add("EquippableSystem");
		ExtraModuleNames.Add("GameSettingsSubsystem");
		ExtraModuleNames.Add("GenericInteractionSystem");
		ExtraModuleNames.Add("GenericInventorySystem");
		ExtraModuleNames.Add("GlobalSystem");
		ExtraModuleNames.Add("InputImageSubsystem");
		ExtraModuleNames.Add("MovementSystem");
		ExtraModuleNames.Add("SaveGameSubsystem");
		ExtraModuleNames.Add("SubtitleSystem");
	}
}
