// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "GameSettingsSubsystem.generated.h"


// 
#define DEFAULT_SAVE_INDEX UINT8_MAX


// Enum for dividing out the wanted control settings data to collect, that way we are not just constantly passing enums around
UENUM()
enum struct ControlSettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct GameplaySettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct LanguageSettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct AccessibilitySettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct DebugSettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct AudioSettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct DisplaySettingsEnum {
	MAX_SIZE
};

// 
UENUM()
enum struct GraphicsSettingsEnum {
	MAX_SIZE
};


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UGameSettingsSubsystem final : public UEngineSubsystem
{
	GENERATED_BODY()
	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	virtual void Deinitialize() override;

	// Function which forces saving the data to a local file
	void ForceSave();


// 
public:

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetControlSetting(const ControlSettingsEnum WantedControlSetting, const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetGameplaySetting(const GameplaySettingsEnum WantedGameplaySetting, const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetLanguageSetting(const LanguageSettingsEnum WantedLanguageSetting, const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetAccessibilitySetting(const AccessibilitySettingsEnum WantedAccessibilitySetting, const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetDebugSetting(const DebugSettingsEnum WantedDebugSetting);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetAudioSetting(const AudioSettingsEnum WantedAudioSetting, const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetDisplaySetting(const DisplaySettingsEnum WantedDisplaySetting);

	// 
	//UFUNCTION()
	template<typename ReturnType>
	ReturnType GetGraphicsSetting(const GraphicsSettingsEnum WantedGraphicsSetting);


	// 
public:

	// 
	template<typename ReturnType, const ControlSettingsEnum WantedControlSetting>
	ReturnType GetControlSetting(const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	template<typename ReturnType, const GameplaySettingsEnum WantedGameplaySetting>
	ReturnType GetGameplaySetting(const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	template<typename ReturnType, const LanguageSettingsEnum WantedLanguageSetting>
	ReturnType GetLanguageSetting(const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	template<typename ReturnType, const AccessibilitySettingsEnum WantedAccessibilitySetting>
	ReturnType GetAccessibilitySetting(const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	template<typename ReturnType, const DebugSettingsEnum WantedDebugSetting>
	ReturnType GetDebugSetting();

	// 
	template<typename ReturnType, const AudioSettingsEnum WantedAudioSetting>
	ReturnType GetAudioSetting(const uint8 SaveIndex = DEFAULT_SAVE_INDEX);

	// 
	template<typename ReturnType, const DisplaySettingsEnum WantedDisplaySetting>
	ReturnType GetDisplaySetting();

	// 
	template<typename ReturnType, const GraphicsSettingsEnum WantedGraphicsSetting>
	ReturnType GetGraphicsSetting();


// 
private:

	// 
	struct ControlSettingsStruct {

	} ControlSettings;

	// 
	struct GameplaySettingsStruct {

	} GameplaySettings;

	// 
	struct LanguageSettingsStruct {

	} LanguageSettings;

	// 
	struct AccessibilitySettingsStruct {

	} AccessibilitySettings;

	// 
	struct DebugSettingsStruct {

	} DebugSettings;

	// 
	struct AudioSettingsStruct {

	} AudioSettings;

	// 
	struct DisplaySettingsStruct {

	} DisplaySettings;

	// 
	struct GraphicsSettingsStruct {

	} GraphicsSettings;


// 
private:

	// 
	TArray<ControlSettingsStruct> PerSaveControlSettings;

	// 
	TArray<GameplaySettingsStruct> PerSaveGameplaySettings;

	// 
	TArray<LanguageSettingsStruct> PerSaveLanguageSettings;

	// 
	TArray<AccessibilitySettingsStruct> PerSaveAccessibilitySettings;

	// 
	TArray<AudioSettingsStruct> PerSaveAudioSettings;
};
