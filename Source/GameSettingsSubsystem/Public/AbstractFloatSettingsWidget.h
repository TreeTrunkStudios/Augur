// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsWidget.h"
#include "AbstractFloatSettingsWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractFloatSettingsWidget : public UAbstractSettingsWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	virtual const USettingDataTypeEnum GetDataType() const override final { return USettingDataTypeEnum::Float; };

	// 
	UFUNCTION(BlueprintCallable)
	void SetConsoleVariable(float NewValue) {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 2);

		// 
		DataContainer.SetSubtype<float>(NewValue);

		// 
		if (CurrentConsoleVariable) {
			CurrentConsoleVariable->SetWithCurrentPriority(NewValue);
			ensureAlways(DataContainer.GetSubtype<float>() == CurrentConsoleVariable->GetFloat());
		}
	}

	// 
	UFUNCTION(BlueprintCallable)
	float GetConsoleVariable() {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 2);

		// 
		if (CurrentConsoleVariable) {
			ensureAlways(DataContainer.GetSubtype<float>() == CurrentConsoleVariable->GetFloat());
		}

		// 
		return DataContainer.GetSubtype<float>();
	}
};
