// TreeTrunkStudios (c) 2023


// 
#include "AbstractIntegerOptionWidget.h"
#include <Blueprint/WidgetTree.h>
#include <Components/Image.h>


//
bool UAbstractIntegerOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
#if WITH_EDITOR
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
#endif

	//
	ComboBox->SetSelectedIndex(SettingsOptionConsoleVariable->GetInt());

	//
	ComboBox->OnSelectionChanged.AddDynamic(this, &UAbstractIntegerOptionWidget::HandleStateChange);

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
	IncrementButton->OnClicked.AddDynamic(this, &UAbstractIntegerOptionWidget::HandleIncrementButton);
	DecrementButton->OnClicked.AddDynamic(this, &UAbstractIntegerOptionWidget::HandleDecrementButton);

	// 
	return ReturnValue;
}


// 
void UAbstractIntegerOptionWidget::HandleIncrementButton() {
	HandleIncrement(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractIntegerOptionWidget::HandleDecrementButton() {
	HandleIncrement(EUINavigation::Next)->SetFocus();
}


// 
UWidget * UAbstractIntegerOptionWidget::HandleIncrement(EUINavigation GivenNavigation) {
	ComboBox->SetSelectedIndex(ComboBox->GetSelectedIndex() + 1);
	return IncrementButton;
}


// 
UWidget * UAbstractIntegerOptionWidget::HandleDecrement(EUINavigation GivenNavigation) {
	ComboBox->SetSelectedIndex(ComboBox->GetSelectedIndex() - 1);
	return DecrementButton;
}


// 
void UAbstractIntegerOptionWidget::HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType) {

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


// 
void UAbstractIntegerOptionWidget::UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {
	ComboBox->SetSelectedIndex(SettingsLevelValues[static_cast<uint8>(GivenSettingsLevel)]);
}
