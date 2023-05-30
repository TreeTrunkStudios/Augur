// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsWidget.h"
#include "AbstractStringSettingsWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractStringSettingsWidget : public UAbstractSettingsWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	virtual const USettingDataTypeEnum GetDataType() const override final { return USettingDataTypeEnum::String; };

	// 
	UFUNCTION(BlueprintCallable)
	void SetConsoleVariable(FString NewValue) {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 3);

		// 
		DataContainer.SetSubtype<FString>(NewValue);

		// 
		if (CurrentConsoleVariable) {
			CurrentConsoleVariable->SetWithCurrentPriority(*NewValue);
			ensureAlways(DataContainer.GetSubtype<FString>() == CurrentConsoleVariable->GetString());
		}
	}

	// 
	UFUNCTION(BlueprintCallable)
	FString GetConsoleVariable() {

		// 
		ensureAlways(DataContainer.GetCurrentSubtypeIndex() == 3);

		// 
		if (CurrentConsoleVariable) {
			ensureAlways(DataContainer.GetSubtype<FString>() == CurrentConsoleVariable->GetString());
		}

		// 
		return DataContainer.GetSubtype<FString>();
	}
};
