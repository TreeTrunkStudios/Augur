// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOptionsStruct.h"
#include "GameSettingsSubsystem.h"
#include "AbstractSettingsWidget.generated.h"


// 
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, meta=(DontUseGenericSpawnObject="True", DisableNativeTick))
class GAMESETTINGSSUBSYSTEM_API UAbstractSettingsWidget : public UUserWidget {
	GENERATED_BODY()


// 
protected:

	// Local data container for simplistic access to all supported data types
	SettingsDataUnion DataContainer;

	// Local pointer to hold the current conole variable, if any
	IConsoleVariable * CurrentConsoleVariable;

	// Local storage for the governing game settings subsystem
	TWeakObjectPtr<UGameSettingsSubsystem> CurrentGameSettings;

	// Local storage for the current widget's section name
	FName ConfigSectionName;

	// Local storage for the current widget's variable name
	FName ConfigVariableName;


// Public section for handling all UFUNCTION functions
public:

	// Calls InitializeSetting_Implementation(const FSettingsOptionsStruct & SettingsOptions) unless overridden in a child blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(ForceAsFunction))
	void InitializeSetting(const FSettingsOptionsStruct & SettingsOptions);
	

// Public section for handling all inline UFUNCTION functions (which is all of them due to them all being rather simple in nature)
public:

	// Default by converting the DataContainer's sub type index to the USettingDataTypeEnum
	UFUNCTION(BlueprintCallable)
	virtual const USettingDataTypeEnum GetDataType() const {
		return USettingDataTypeEnum::ERROR;
	};


// Public section for handling all inline functions (which is all of them due to them all being rather simple in nature)
public:

	// Simple function which returns the current object's data container
	const SettingsDataUnion & GetDataContainer() const {
		return DataContainer;
	};


	// Collect and store the wanted console variable for simplistic future use
	virtual void InitializeSetting_Implementation(const FSettingsOptionsStruct & SettingsOptions) {

		// Attempt to find a console variable that shares the same name as the given config variable name
		CurrentConsoleVariable = IConsoleManager::Get().FindConsoleVariable(*SettingsOptions.ConfigVariableName.ToString());

		// Collect the global game settings engine subsystem and ensure that it functioned perfectly, else crash
		CurrentGameSettings = GEngine->GetEngineSubsystem<UGameSettingsSubsystem>();
		ensureAlways(CurrentGameSettings.IsValid());

		// Lastly, store the config section and variable names locally for child widgets to utilize directly
		ConfigSectionName = SettingsOptions.ConfigSectionName;
		ConfigVariableName = SettingsOptions.ConfigVariableName;
	};
};
