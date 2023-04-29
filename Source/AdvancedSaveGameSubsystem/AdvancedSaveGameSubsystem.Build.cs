
// 
using UnrealBuildTool;


// 
public class AdvancedSaveGameSubsystem: ModuleRules {

	// 
	public AdvancedSaveGameSubsystem(ReadOnlyTargetRules Target) : base(Target) {
		PrivateDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine"});
	}
}
