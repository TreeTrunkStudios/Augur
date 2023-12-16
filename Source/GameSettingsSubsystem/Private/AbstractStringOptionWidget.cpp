// TreeTrunkStudios (c) 2023


// 
#include "AbstractStringOptionWidget.h"


// 
bool UAbstractStringOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
#if WITH_EDITOR
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
#endif

	//
	EditableTextBox->SetText(FText::FromString(SettingsOptionConsoleVariable->GetString()));

	//
	EditableTextBox->OnTextCommitted.AddDynamic(this, &UAbstractStringOptionWidget::HandleStateChange);

	// 
	return ReturnValue;
}


// 
void UAbstractStringOptionWidget::HandleStateChange(const FText & NewState, ETextCommit::Type CommitMethod) {
	SettingsOptionConsoleVariable->SetWithCurrentPriority(NewState.ToString().GetCharArray().GetData());
}


// 
void UAbstractStringOptionWidget::UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {
	EditableTextBox->SetText(FText::FromString(SettingsLevelValues[static_cast<uint8>(GivenSettingsLevel)]));
}
