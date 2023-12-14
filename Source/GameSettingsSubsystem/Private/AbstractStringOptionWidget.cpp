// TreeTrunkStudios (c) 2023


// 
#include "AbstractStringOptionWidget.h"


// 
bool UAbstractStringOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	//
	EditableTextBox->OnTextCommitted.AddDynamic(this, &UAbstractStringOptionWidget::HandleStateChange);

	// 
	return ReturnValue;
}


// 
void UAbstractStringOptionWidget::HandleStateChange(const FText & NewState, ETextCommit::Type CommitMethod) {
	SettingsOptionConsoleVariable->SetWithCurrentPriority(NewState.ToString().GetCharArray().GetData());
}
