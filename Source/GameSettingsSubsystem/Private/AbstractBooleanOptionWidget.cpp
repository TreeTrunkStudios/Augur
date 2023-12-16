// TreeTrunkStudios (c) 2023


// 
#include "AbstractBooleanOptionWidget.h"


//
bool UAbstractBooleanOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
#if WITH_EDITOR
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
#endif

	//
	CheckBox->SetIsChecked(SettingsOptionConsoleVariable->GetBool());

	//
	CheckBox->OnCheckStateChanged.AddDynamic(this, &UAbstractBooleanOptionWidget::HandleStateChange);

	// 
	return ReturnValue;
}


// 
void UAbstractBooleanOptionWidget::HandleStateChange(bool NewState) {
	SettingsOptionConsoleVariable->SetWithCurrentPriority(NewState);
}


// 
void UAbstractBooleanOptionWidget::UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {
	CheckBox->SetIsChecked(SettingsLevelValues[static_cast<uint8>(GivenSettingsLevel)] != 0);
}
