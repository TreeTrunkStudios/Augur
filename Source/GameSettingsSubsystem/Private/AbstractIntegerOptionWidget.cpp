// TreeTrunkStudios (c) 2023


// 
#include "AbstractIntegerOptionWidget.h"
#include <Blueprint/WidgetTree.h>
#include <Components/Image.h>


//
bool UAbstractIntegerSettingsOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	//
	ComboBox->SetSelectedIndex(SettingsOptionConsoleVariable->GetInt());

	//
	ComboBox->OnSelectionChanged.AddDynamic(this, &UAbstractIntegerSettingsOptionWidget::HandleStateChange);

	//
	const int32 SelectedIndex = ComboBox->GetSelectedIndex();

	//
	for (int32 CurrentOptionIndex = 0, MaxOptionCount = ComboBox->GetOptionCount(); CurrentOptionIndex < MaxOptionCount; ++CurrentOptionIndex) {

		//
		UImage * NewImage = WidgetTree->ConstructWidget<UImage>();

		//
		NewImage->SetBrushFromSoftTexture((CurrentOptionIndex == SelectedIndex) ? SelectedIndexTexture : UnselectedIndexTexture);

		//
		StateUniformGrid->AddChildToUniformGrid(NewImage, 0, CurrentOptionIndex);
	}

	// 
	return ReturnValue;
}


// 
void UAbstractIntegerSettingsOptionWidget::HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType) {

	//
	const int32 SelectedIndex = ComboBox->GetSelectedIndex();

	// 
	SettingsOptionConsoleVariable->SetWithCurrentPriority(SelectedIndex);

	//
	const TArray<UPanelSlot*> & CurrentSlots = StateUniformGrid->GetSlots();
	for (uint8 CurrentOptionIndex = 0; CurrentOptionIndex < CurrentSlots.Num(); ++CurrentOptionIndex) {
		Cast<UImage>(CurrentSlots[CurrentOptionIndex]->Content.Get())->SetBrushFromSoftTexture((CurrentOptionIndex == SelectedIndex) ? SelectedIndexTexture : UnselectedIndexTexture);
	}
}
