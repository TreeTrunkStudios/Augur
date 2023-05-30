// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsWidget.h"
#include "AbstractIntegerSettingsWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractIntegerSettingsWidget : public UAbstractSettingsWidget {
	GENERATED_BODY()
	

// Public section for handling all inline functions (which is all of them due to them all being rather simple in nature)
public:

	// Overridden final version of collecting this, and all of its children's, data type, fixed to be an integer
	virtual const USettingDataTypeEnum GetDataType() const override final {
		return USettingDataTypeEnum::Integer;
	};


	// Function for initializing the current state of this widget based on its given settings
	virtual void InitializeSetting_Implementation(const FSettingsOptionsStruct & SettingsOptions) {

		// Start by calling its parent's initialization
		Super::InitializeSetting_Implementation(SettingsOptions);

		// Set the current data type's value to be whatever the global game settings have collected from the config file, crashing otherwise
		DataContainer.SetSubtype<int32>(CurrentGameSettings->GetSettingData_Internal(ConfigSectionName, ConfigVariableName).GetSubtype<int32>());

		// If we are handling a console variable, then update it as well, ensuring everything went smoothly
		if (CurrentConsoleVariable) {
			CurrentConsoleVariable->SetWithCurrentPriority(DataContainer.GetSubtype<int32>());
			ensureAlways(DataContainer.GetSubtype<int32>() == CurrentConsoleVariable->GetInt());
		}

		// 
		// 
	}


	// Function for overridding the current value of this config variable
	UFUNCTION(BlueprintCallable)
	void SetConfigVariable(int32 NewValue) {

		// Ensure that the data container is an integer and that nothing has broke
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 1);

		// Set the current data type to be the newly wanted value
		DataContainer.SetSubtype<int32>(NewValue);

		// Save the global settings map to the newly wanted value as well
		CurrentGameSettings->SetSettingData_Internal<int32>(ConfigSectionName, ConfigVariableName, NewValue);

		// Lastly, if we are handling a console variable, then update it to the newly wanted valueand ensure everything worked perfectly
		if (CurrentConsoleVariable) {
			CurrentConsoleVariable->SetWithCurrentPriority(NewValue);
			ensureAlways(DataContainer.GetSubtype<int32>() == CurrentConsoleVariable->GetInt());
		}
	}


	// Function for collecting the current value of this config variable
	UFUNCTION(BlueprintCallable)
	int32 GetConfigVariable() {

		// Ensure that the data container is an integer and that nothing has broken
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 1);

		// If we are handling a console variable, then ensure that its value matches the current value, else something has broke
		if (CurrentConsoleVariable) {
			ensureAlways(DataContainer.GetSubtype<int32>() == CurrentConsoleVariable->GetInt());
		}

		// Get the current data type's value
		return DataContainer.GetSubtype<int32>();
	}
};
