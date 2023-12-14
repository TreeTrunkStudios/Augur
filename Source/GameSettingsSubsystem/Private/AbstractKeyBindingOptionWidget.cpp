// TreeTrunkStudios (c) 2023


// 
#include "AbstractKeyBindingOptionWidget.h"


//
bool UAbstractKeyBindingOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	//
	//

	// 
	return ReturnValue;
}


// 
void UAbstractKeyBindingOptionWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) {

	//
	DescriptionTextWidget->SetText(DescriptionText);

	// 
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}
