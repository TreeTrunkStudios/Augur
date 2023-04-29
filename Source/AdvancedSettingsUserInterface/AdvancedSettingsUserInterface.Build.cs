
// 
using UnrealBuildTool;


// 
public class AdvancedSettingsUserInterface : ModuleRules {

	// 
	public AdvancedSettingsUserInterface(ReadOnlyTargetRules Target) : base(Target) {
		PrivateDependencyModuleNames.AddRange(new string[]{"Core", "CoreUObject", "Engine"});
	}
}
