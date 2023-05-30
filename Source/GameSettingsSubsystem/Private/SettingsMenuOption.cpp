// TreeTrunkStudios (c) 2023


// 
#include "SettingsMenuOption.h"
#include "Blueprint/WidgetTree.h"


// 
//USettingsMenuOption::USettingsMenuOption(const FObjectInitializer & ObjectInitializer) : UUserWidget(ObjectInitializer) {}


// 
bool USettingsMenuOption::Initialize() {

	// Call the parent initialize function and, if it succeeded, then...
	if (Super::Initialize()) {

		// If the given class type is valid, then...
		if (IsValid(Type.Get())) {

			//// Create our newly wanted settings widget
			// Construct the wanted widget and store it within the exposed UPROPERTY
			SettingsWidget = WidgetTree->ConstructWidget<UAbstractSettingsWidget>(Type.Get(), TEXT("SettingsWidgetType"));

			// Ensure that the widget is always successfully created and stored
			ensureAlways(IsValid(SettingsWidget));

			// Let the type of settings menu that we are now utilizing decide on the utilized Data Type
			SettingsOptions.SettingDataType = SettingsWidget->GetDataType();

			// 
			if (SettingsOptions.SettingDataType != USettingDataTypeEnum::Integer) {
				SettingsOptions.SettingLevelLabels.Empty();
			}


			//// Initialize our new settings widget
			// Initialize the newly created widget with the wanted settings options
			SettingsWidget->InitializeSetting(SettingsOptions);

			// If we have a root widget already, then just add as a child widget
			if (IsValid(Cast<UPanelWidget>(WidgetTree->RootWidget))) {
				Cast<UPanelWidget>(WidgetTree->RootWidget)->AddChild(SettingsWidget);
			}

			// Else, we can make the new widget as the root, which is much better
			else if (SettingsWidget->IsSafeForRootSet()) {
				WidgetTree->RootWidget = SettingsWidget;
			}

			// Else, return, crash, and burn, as we have no root that we need
			else {
				return false;
			}


			//// Handle our wanted rely setting, if we have one at all
			// 
			if (IsValid(OptionalRelySetting)) {

				// 
				RelySettingDataType = OptionalRelySetting->SettingsOptions.SettingDataType;

				// 
				switch (RelySettingDataType) {

					// If the relying setting's data type is a boolean, then...
					case USettingDataTypeEnum::Boolean:

						// If the states match, then make this widget visible, else collapse it
						this->SetVisibility((OptionalRelySetting->CollectDataContainer().GetSubtype<bool>() == RelySettingVisibleBoolState) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
						break;

					// Else, if the relying setting's data type is an integer, then...
					case USettingDataTypeEnum::Integer:

						// If the current integer value is one of the visible values, then make this widget visible
						if (RelySettingVisibleIntegerStates.Contains(OptionalRelySetting->CollectDataContainer().GetSubtype<int32>())) {
							this->SetVisibility(ESlateVisibility::Visible);
						}

						// Else, collapse this widget
						else {
							this->SetVisibility(ESlateVisibility::Collapsed);
						}
						break;

					// Else, if the relying setting's data type is a float, then...
					case USettingDataTypeEnum::Float:

						// If the float is inclusively between the min and max range, then make this widget visible
						if (OptionalRelySetting->CollectDataContainer().GetSubtype<float>() >= RelySettingVisibleFloatMinRange &&
							OptionalRelySetting->CollectDataContainer().GetSubtype<float>() <= RelySettingVisibleFloatMaxRange) {
							this->SetVisibility(ESlateVisibility::Visible);
						}

						// Else, collapse this widget
						else {
							this->SetVisibility(ESlateVisibility::Collapsed);
						}
						break;

					// Else, if the relying setting's data type is a string, then...
					case USettingDataTypeEnum::String:

						// If the current string value is one of the visible values, then make this widget visible
						if (RelySettingVisibleStringStates.Contains(OptionalRelySetting->CollectDataContainer().GetSubtype<FString>())) {
							this->SetVisibility(ESlateVisibility::Visible);
						}

						// Else, collapse this widget
						else {
							this->SetVisibility(ESlateVisibility::Collapsed);
						}
						break;

					// Else, the target's data type is an error, so collapse this widget, as it is invalid, technically
					case USettingDataTypeEnum::ERROR:
					default:
						this->SetVisibility(ESlateVisibility::Collapsed);
						break;
				}
			}
		}

		// 
		else {

			// Ensure that the root widget is reset if the previous settings widget was its parent
			if (WidgetTree->RootWidget == SettingsWidget) {
				WidgetTree->RootWidget = nullptr;
			}

			// Ensure that the settings widget is no more (if it ever was anything)
			SettingsWidget = nullptr;

			// Ensure that the setting's data type is an error for safety reasons
			SettingsOptions.SettingDataType = USettingDataTypeEnum::ERROR;

			// Ensure that the setting's level labels are all empty and hidden, as they are rarely necessary
			SettingsOptions.SettingLevelLabels.Empty(0);
		}

		// Finally, return true, as we have successfully initialized
		return true;
	}

	// Else, we have failed to initialize in some way and need to return false
	return false;
}


// 
const TUnion<bool, int32, float, FString> & USettingsMenuOption::CollectDataContainer() const {
	ensureAlways(IsValid(SettingsWidget));
	return SettingsWidget->GetDataContainer();
}


// 
void USettingsMenuOption::NativeConstruct() {

	// Call the parent constructor function first
	Super::NativeConstruct();

	// If we have not selected a type by game start, then throw an error!
	if (!IsValid(Type.Get())) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: No valid Type given to USettingsMenuOption!!")));
	}
}
