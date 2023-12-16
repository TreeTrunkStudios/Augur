
// 
using UnrealBuildTool;


// 
public class GameSettingsSubsystem: ModuleRules {

	// 
	public GameSettingsSubsystem(ReadOnlyTargetRules Target) : base(Target) {
		PrivateDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine", "InputImageSubsystem"});
		PublicDependencyModuleNames.AddRange(new string[]{"Slate", "SlateCore"});
	}
}
