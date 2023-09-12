
// 
using UnrealBuildTool;


// 
public class InputImageSubsystem : ModuleRules {

	// 
	public InputImageSubsystem(ReadOnlyTargetRules Target) : base(Target) {
		PrivateDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine"});
	}
}
