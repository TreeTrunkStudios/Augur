// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsWidget.h"
#include "AbstractBooleanSettingsWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractBooleanSettingsWidget : public UAbstractSettingsWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	virtual const USettingDataTypeEnum GetDataType() const override final { return USettingDataTypeEnum::Boolean; };

	// 
	virtual void InitializeSetting_Implementation(const FSettingsOptionsStruct & SettingsOptions) {

		// 
		Super::InitializeSetting_Implementation(SettingsOptions);

		// 
		if (CurrentConsoleVariable) {
			ensureAlways(CurrentConsoleVariable->IsVariableBool());
			DataContainer.SetSubtype<bool>(CurrentConsoleVariable->GetBool());
		}

		// 
		else {
			DataContainer.SetSubtype<bool>(false);
		}

		// 
		// 
	}

	// 
	UFUNCTION(BlueprintCallable)
	void SetConsoleVariable(bool NewValue) {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 0);

		// 
		DataContainer.SetSubtype<bool>(NewValue);

		// 
		if (CurrentConsoleVariable) {
			CurrentConsoleVariable->SetWithCurrentPriority(NewValue);
			ensureAlways(DataContainer.GetSubtype<bool>() == CurrentConsoleVariable->GetBool());
		}
	}

	// 
	UFUNCTION(BlueprintCallable)
	bool GetConsoleVariable() {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 0);

		// 
		if (CurrentConsoleVariable) {
			ensureAlways(DataContainer.GetSubtype<bool>() == CurrentConsoleVariable->GetBool());
		}

		// 
		return DataContainer.GetSubtype<bool>();
	}
};
