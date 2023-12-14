// TreeTrunkStudios (c) 2023


// 
#include "..\Public\AbstractBooleanOptionWidget.h"


//
bool UAbstractBooleanSettingsOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	//
	CheckBox->SetCheckedState(SettingsOptionConsoleVariable->GetBool() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);

	//
	CheckBox->OnCheckStateChanged.AddDynamic(this, &UAbstractBooleanSettingsOptionWidget::HandleStateChange);

	// 
	return ReturnValue;
}


// 
void UAbstractBooleanSettingsOptionWidget::HandleStateChange(bool NewState) {
	SettingsOptionConsoleVariable->SetWithCurrentPriority(NewState);
}
