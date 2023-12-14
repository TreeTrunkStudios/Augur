// TreeTrunkStudios (c) 2023


// 
#include "AbstractSettingsOptionWidget.h"
#include "GameSettingsSubsystem.h"


// 
bool UAbstractSettingsOptionWidget::Initialize() {

	//
	SettingsOptionConsoleVariable = IConsoleManager::Get().FindConsoleVariable(ConsoleCommand.ToString().GetCharArray().GetData());

	//
#if WITH_EDITOR

	//
	ensureAlways(SettingsOptionConsoleVariable != nullptr);

	//
	UGameSettingsSubsystem * GameSettingsSubsystem = GEngine->GetEngineSubsystem<UGameSettingsSubsystem>();

	//
	ensureAlways(GameSettingsSubsystem != nullptr && GameSettingsSubsystem->DoesSettingExist(SectionName, ConsoleCommand));

	//
#endif
	
	// 
	return Super::Initialize();
}


// 
void UAbstractSettingsOptionWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) {

	//
	DescriptionTextWidget->SetText(DescriptionText);

	// 
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}
