// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Containers/Union.h"
#include "GlobalSettingsConsoleVariables.h"
#include "GameSettingsSubsystem.generated.h"


// 
typedef TUnion<bool, int32, float, FString> SettingsDataUnion;


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UGameSettingsSubsystem final : public UEngineSubsystem {
	GENERATED_BODY()
	
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
	bool DoesSettingExist(FName SectionName, FName VariableName) {
		return (SettingsMap.Contains(SectionName) && SettingsMap.FindChecked(SectionName).Contains(VariableName));
	}

	// 
	SettingsDataUnion GetSettingData_Internal(FName SectionName, FName VariableName) {
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
