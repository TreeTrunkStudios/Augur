// TreeTrunkStudios (c) 2023


// 
#include "AbstractSettingsOptionWidget.h"
#include "GameSettingsSubsystem.h"


// 
bool UAbstractSettingsOptionWidget::Initialize() {

	//
	SettingsOptionConsoleVariable = IConsoleManager::Get().FindConsoleVariable(ConsoleCommand.ToString().GetCharArray().GetData());

	//
	if (IsValid(NameText))
		NameText->SetText(VariableNameText);
	
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
