// TreeTrunkStudios (c) 2023


// 
#include "AbstractGlobalOptionWidget.h"


//
FText SettingsLevelToText(const ESettingsLevel & GivenSettingsLevel) {

	//
	switch(GivenSettingsLevel) {

		//
		case ESettingsLevel::Low:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelLow", "Low");

		//
		case ESettingsLevel::Medium:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelMedium", "Medium");

		//
		case ESettingsLevel::High:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelHigh", "High");

		//
		case ESettingsLevel::Ultra:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelUltra", "Ultra");

		//
		case ESettingsLevel::Perfection:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelPerfection", "Perfection");

		//
		case ESettingsLevel::Custom:
			return NSLOCTEXT("SettingsInfo", "SettingsLevelCustom", "Custom");

		//
		default:
			return FText::FromString("ERROR");
	}
}


//
bool UAbstractGlobalOptionWidget::Initialize() {

	// If our combo box is valid, then...
	if (IsValid(ComboBox) && SettingsOptionConsoleVariable != nullptr) {

		// Remove all user input options
		ComboBox->ClearOptions();

		// Loop through and automatically add all settings levels as text to the combo box instead
		for (uint8 CurrentIndex = 0; CurrentIndex < static_cast<uint8>(ESettingsLevel::Num); ++CurrentIndex) {
			ComboBox->AddOption(SettingsLevelToText(static_cast<ESettingsLevel>(CurrentIndex)));
		}
	}

	// Loop through all of our connected settings option widgets and attach our console variable change handler to it
	for (const UAbstractSettingsOptionWidget * CurrentSettingsOptionWidget : SettingsToAdjustWhenChanged) {
		CurrentSettingsOptionWidget->GetSettingsOptionConsoleVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UAbstractGlobalOptionWidget::HandleCheckForCustomState));
	}

	// Finally, return our parent function's return value
	return Super::Initialize();
}


// 
void UAbstractGlobalOptionWidget::HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType) {

	//
	DidStateChange = true;

	// Handle the parent functionality now (updates visuals and the associated console variable
	Super::HandleStateChange(SelectedItem, SelectionType);

	// If the newly selected index is a valid settings level, then update our current targets to the same level
	if (const int32 SelectedIndex = ComboBox->GetSelectedIndex(); SelectedIndex >= 0 && SelectedIndex < static_cast<uint8>(ESettingsLevel::Custom))
		for (UAbstractSettingsOptionWidget * CurrentSettingsOptionWidget : SettingsToAdjustWhenChanged)
			CurrentSettingsOptionWidget->UpdateToTargetLevel(static_cast<ESettingsLevel>(SelectedIndex));

	//
	DidStateChange = false;
}


// 
void UAbstractGlobalOptionWidget::HandleCheckForCustomState(IConsoleVariable * GivenConsoleVariable) {
	if (DidStateChange == false)
		UpdateToTargetLevel(ESettingsLevel::Custom);
}


// 
void UAbstractGlobalOptionWidget::UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {
	ComboBox->SetSelectedIndex(static_cast<uint8>(GivenSettingsLevel));
}
