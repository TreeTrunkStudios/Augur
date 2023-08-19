// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class CornersOfTheEarthEditorTarget : TargetRules
{
	public CornersOfTheEarthEditorTarget( TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("AdvancedMovementSystem");
		ExtraModuleNames.Add("AdvancedSaveGameSubsystem");
		ExtraModuleNames.Add("AdvancedSubtitleSystem");
		ExtraModuleNames.Add("EquippableSystem");
		ExtraModuleNames.Add("GameSettingsSubsystem");
		ExtraModuleNames.Add("GenericInventorySystem");
		ExtraModuleNames.Add("SimpleGridBasedInventorySystem");
	}
}
