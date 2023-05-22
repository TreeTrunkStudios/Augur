// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "GameSettingsSubsystem.generated.h"


// 
struct ControlSettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FControlSettingsUpdate, const ControlSettingsStruct&);


// 
struct GameplaySettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FGameplaySettingsUpdate, const GameplaySettingsStruct&);


// 
struct LanguageSettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FLanguageSettingsUpdate, const LanguageSettingsStruct&);


// 
struct AccessibilitySettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FAccessibilitySettingsUpdate, const AccessibilitySettingsStruct&);


// 
struct DebugSettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FDebugSettingsUpdate, const DebugSettingsStruct&);


// 
struct AudioSettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FAudioSettingsUpdate, const AudioSettingsStruct&);


// 
struct DisplaySettingsStruct {

	// 
	//
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FDisplaySettingsUpdate, const DisplaySettingsStruct&);


// 
struct GraphicsSettingsStruct {

	// 
	// 
};


// 
DECLARE_MULTICAST_DELEGATE_OneParam(FGraphicsSettingsUpdate, const GraphicsSettingsStruct&);


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UGameSettingsSubsystem final : public UEngineSubsystem
{
	GENERATED_BODY()
	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	virtual void Deinitialize() override;

	// Function which forces saving the data to a local file
	void ForceSave();

	// Function which forces saving the data to a local file
	void ForceLoad();


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	void SetCurrentGameID(const FString & NewGameID);


// 
private:

	// 
	FString CurrentSaveGameID{TEXT("_DEFAULT")};

	// 
	ControlSettingsStruct ControlSettings;

	// 
	GameplaySettingsStruct GameplaySettings;

	// 
	LanguageSettingsStruct LanguageSettings;

	// 
	AccessibilitySettingsStruct AccessibilitySettings;

	// 
	DebugSettingsStruct DebugSettings;

	// 
	AudioSettingsStruct AudioSettings;

	// 
	DisplaySettingsStruct DisplaySettings;

	// 
	GraphicsSettingsStruct GraphicsSettings;
};
