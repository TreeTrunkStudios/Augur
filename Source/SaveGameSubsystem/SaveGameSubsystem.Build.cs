
// 
using UnrealBuildTool;


// 
public class SaveGameSubsystem: ModuleRules {

	// 
	public SaveGameSubsystem(ReadOnlyTargetRules Target) : base(Target) {
		PrivateDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine"});
	}
}
