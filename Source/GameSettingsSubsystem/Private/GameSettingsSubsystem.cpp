// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingsSubsystem.h"

void UGameSettingsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
}

void UGameSettingsSubsystem::Deinitialize()
{
}

void UGameSettingsSubsystem::ForceSave()
{
}

template<typename ReturnType>
inline ReturnType UGameSettingsSubsystem::GetControlSetting(const ControlSettingsEnum WantedControlSetting, const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetGameplaySetting(const GameplaySettingsEnum WantedGameplaySetting, const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetLanguageSetting(const LanguageSettingsEnum WantedLanguageSetting, const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetAccessibilitySetting(const AccessibilitySettingsEnum WantedAccessibilitySetting, const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetDebugSetting(const DebugSettingsEnum WantedDebugSetting)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetAudioSetting(const AudioSettingsEnum WantedAudioSetting, const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetDisplaySetting(const DisplaySettingsEnum WantedDisplaySetting)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType>
ReturnType UGameSettingsSubsystem::GetGraphicsSetting(const GraphicsSettingsEnum WantedGraphicsSetting)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, ControlSettingsEnum WantedControlSetting>
ReturnType UGameSettingsSubsystem::GetControlSetting(const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, GameplaySettingsEnum WantedGameplaySetting>
ReturnType UGameSettingsSubsystem::GetGameplaySetting(const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, LanguageSettingsEnum WantedLanguageSetting>
ReturnType UGameSettingsSubsystem::GetLanguageSetting(const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, AccessibilitySettingsEnum WantedAccessibilitySetting>
ReturnType UGameSettingsSubsystem::GetAccessibilitySetting(const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, DebugSettingsEnum WantedDebugSetting>
ReturnType UGameSettingsSubsystem::GetDebugSetting()
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, AudioSettingsEnum WantedAudioSetting>
ReturnType UGameSettingsSubsystem::GetAudioSetting(const uint8 SaveIndex)
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, DisplaySettingsEnum WantedDisplaySetting>
ReturnType UGameSettingsSubsystem::GetDisplaySetting()
{
	return static_cast<uint8>(0);
}

template<typename ReturnType, GraphicsSettingsEnum WantedGraphicsSetting>
ReturnType UGameSettingsSubsystem::GetGraphicsSetting()
{
	return static_cast<uint8>(0);
}
