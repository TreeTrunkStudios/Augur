// TreeTrunkStudios (c) 2023

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "SettingsOptionsStruct.h"
#include "GameSettingsSubsystem.generated.h"


// 
typedef TUnion<bool, int32, float, FString> SettingsDataUnion;


// 
typedef TMulticastDelegate<void(const TMap<FName, SettingsDataUnion>&)> FSettingsUpdate;


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UGameSettingsSubsystem final : public UEngineSubsystem
{
	GENERATED_BODY()


// 
public:

	// 
	FSettingsUpdate ControlSettingsUpdate;

	// 
	FSettingsUpdate GameplaySettingsUpdate;

	// 
	FSettingsUpdate LanguageSettingsUpdate;

	// 
	FSettingsUpdate AccessibilitySettingsUpdate;

	// 
	FSettingsUpdate DebugSettingsUpdate;

	// 
	FSettingsUpdate AudioSettingsUpdate;

	// 
	FSettingsUpdate DisplaySettingsUpdate;

	// 
	FSettingsUpdate GraphicsSettingsUpdate;

	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	virtual void Deinitialize() override;

	// Function which forces saving the data to a local file
	UFUNCTION(BlueprintCallable)
	void ForceSave();

	// Function which forces saving the data to a local file
	UFUNCTION(BlueprintCallable)
	void ForceLoad();


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	void SetCurrentGameID(const FString & NewGameID);

	// 
	UFUNCTION(BlueprintCallable)
	const FUnionDataStruct GetSettingData(FName SectionName, FName VariableName);

	// 
	UFUNCTION(BlueprintCallable)
	void SetSettingData(FName SectionName, FName VariableName, FUnionDataStruct VariableData);

	// 
	const bool DoesSettingExist(FName SectionName, FName VariableName) {
		return (SettingsMap.Contains(SectionName) && SettingsMap.FindChecked(SectionName).Contains(VariableName));
	}

	// 
	const SettingsDataUnion GetSettingData_Internal(FName SectionName, FName VariableName) {
		return SettingsMap.FindChecked(SectionName).FindChecked(VariableName);
	}

	// 
	template<typename Type>
	void SetSettingData_Internal(FName SectionName, FName VariableName, Type NewData) {
		SettingsMap.FindOrAdd(SectionName).FindOrAdd(VariableName).SetSubtype<Type>(NewData);
	}


// 
private:

	// 
	FString CurrentSaveGameID;

	// 
	FString CurrentIniFileName;

	// 
	TMap<FName, TMap<FName, SettingsDataUnion>> SettingsMap;
};
