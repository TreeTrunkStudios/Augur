// TreeTrunkStudios (c) 2023


// 
#include "GameSettingsSubsystem.h"


// 
void UGameSettingsSubsystem::Initialize(FSubsystemCollectionBase & Collection) {

	//
	//

	// 
	ForceLoad();
}


// 
void UGameSettingsSubsystem::Deinitialize() {

	// 
	//

	// 
	ForceSave();
}


// 
void UGameSettingsSubsystem::ForceSave() {

	// Loop through all settings menu sections, and...
	for (const auto & CurrentSection : SettingsMap) {
		
		// Loop through this section's variables, and...
		for (const auto & CurrentVariable : CurrentSection.Value) {

			// We need to figure out what type of data this is...
			switch (CurrentVariable.Value.GetCurrentSubtypeIndex()) {

				// It is a bool, so write it to the config file as such
				case 0:
					GConfig->SetBool(*CurrentSection.Key.ToString(), *CurrentVariable.Key.ToString(), CurrentVariable.Value.GetSubtype<bool>(), CurrentIniFileName);
					break;

				// It is an integer, so write it to the config file as such
				case 1:
					GConfig->SetInt(*CurrentSection.Key.ToString(), *CurrentVariable.Key.ToString(), CurrentVariable.Value.GetSubtype<int32>(), CurrentIniFileName);
					break;

				// It is a float, so write it to the config file as such
				case 2:
					GConfig->SetFloat(*CurrentSection.Key.ToString(), *CurrentVariable.Key.ToString(), CurrentVariable.Value.GetSubtype<float>(), CurrentIniFileName);
					break;

				// It is a string, so write it to the config file as such
				case 3:
					GConfig->SetString(*CurrentSection.Key.ToString(), *CurrentVariable.Key.ToString(), *CurrentVariable.Value.GetSubtype<FString>(), CurrentIniFileName);
					break;

				// Else, skip this one, as it is broken and bad
				default:
					break;
			}
		}
	}
}


// 
const SettingsDataUnion CalculateSettingDataTypeFromString(const FString & DataString) {

	// Locally collect the FCoreTexts singleton for boolean checking
	const FCoreTexts & CoreTexts = FCoreTexts::Get();

#if PLATFORM_TCHAR_IS_UTF8CHAR

	// Locally store the FString as a pointer to a UTF8CHAR
	const UTF8CHAR * LocalString = StringCast<UTF8CHAR>(*DataString).Get();

	// If the given string is any form of boolean as true, then return that data
	if (FCStringUtf8::Stricmp(LocalString, TEXT("True")) == 0 ||
		FCStringUtf8::Stricmp(LocalString, TEXT("Yes")) == 0 ||
		FCStringUtf8::Stricmp(LocalString, TEXT("On")) == 0 ||
		FCStringUtf8::Stricmp(LocalString, *(CoreTexts.True.ToString())) == 0 ||
		FCStringUtf8::Stricmp(LocalString, *(CoreTexts.Yes.ToString())) == 0) {
		return SettingsDataUnion{static_cast<bool>(true)};
	}

	// Else, if the given string is any form of boolean as false, then return that data
	if (FCStringUtf8::Stricmp(LocalString, TEXT("False")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, TEXT("No")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, TEXT("Off")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, *(CoreTexts.False.ToString())) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, *(CoreTexts.No.ToString())) == 0) {
		return SettingsDataUnion{static_cast<bool>(false)};
	}
#else

	// Locally store the FString as a pointer to a WIDECHAR
	const WIDECHAR * LocalString = *DataString;

	// If the given string is any form of boolean as true, then return that data
	if (FCStringWide::Stricmp(LocalString, TEXT("True")) == 0 ||
	    FCStringWide::Stricmp(LocalString, TEXT("Yes")) == 0 ||
	    FCStringWide::Stricmp(LocalString, TEXT("On")) == 0 ||
	    FCStringWide::Stricmp(LocalString, *(CoreTexts.True.ToString())) == 0 ||
	    FCStringWide::Stricmp(LocalString, *(CoreTexts.Yes.ToString())) == 0) {
		return SettingsDataUnion{static_cast<bool>(true)};
	}

	// Else, if the given string is any form of boolean as false, then return that data
	if (FCStringWide::Stricmp(LocalString, TEXT("False")) == 0 ||
		     FCStringWide::Stricmp(LocalString, TEXT("No")) == 0 ||
		     FCStringWide::Stricmp(LocalString, TEXT("Off")) == 0 ||
		     FCStringWide::Stricmp(LocalString, *(CoreTexts.False.ToString())) == 0 ||
		     FCStringWide::Stricmp(LocalString, *(CoreTexts.No.ToString())) == 0) {
		return SettingsDataUnion{static_cast<bool>(false)};
	}
#endif

	//// Else, we are not a boolean and need to continue checking
	// Check for if the current string is numeric, and, if so...
	if (DataString.IsNumeric()) {

		// Check if the data contains a period, in which case it is a float, so return the data as a float
		if (DataString.Contains(TEXT("."))) {
#if PLATFORM_TCHAR_IS_UTF8CHAR
			return SettingsDataUnion{FCStringUtf8::Atof(StringCast<UTF8CHAR>(*DataString).Get())};
#else
			return SettingsDataUnion{FCStringWide::Atof(*DataString)};
#endif
		}

		// Else, we are dealing with an integer of some sorts, so return the data as an integer
		else {
#if PLATFORM_TCHAR_IS_UTF8CHAR
			return SettingsDataUnion{FCStringUtf8::Atoi(StringCast<UTF8CHAR>(*DataString).Get())};
#else
			return SettingsDataUnion{FCStringWide::Atoi(*DataString)};
#endif
		}
	}

	// Else, just assume it's a string and move on
	return SettingsDataUnion{DataString};
}


// 
void UGameSettingsSubsystem::ForceLoad() {

	// Create a local array of strings to store any and all found sections
	TArray<FString> SectionNames;

	// Generate the wanted global ini file and locally store the string referencing it
	ensureAlways(GConfig->LoadGlobalIniFile(CurrentIniFileName, *(TEXT("SettingsOptions") + CurrentSaveGameID), NULL, false, true));

	// Collect any and all section names from our target file and store them locally
	ensureAlways(GConfig->GetSectionNames(CurrentIniFileName, SectionNames));

	// Loop through all found sections in the target config file, and...
	for (const FString & CurrentSection : SectionNames) {

		// Locally create a delegate callback function to handle checking each and every variable of the current section
		FKeyValueSink CallbackFunction;
		CallbackFunction.BindLambda([&](const TCHAR * VariableName, const TCHAR * VariableValue) {

			// Collect or create a mapping within the SettingsMap to store the current section and variable data within a union
			SettingsDataUnion & CurrentSettingsOption = SettingsMap.FindOrAdd(FName(CurrentSection)).FindOrAdd(FName(VariableName));

			// Attempt to collect the important console variable if this variable is associated with one
			IConsoleVariable * CurrentConsoleVariable = IConsoleManager::Get().FindConsoleVariable(VariableName);

			// Calculate what the data type actually is
			const SettingsDataUnion LocalUnionDataStruct = CalculateSettingDataTypeFromString(FString(VariableValue));

			// Switch on the type of data this contains
			switch (LocalUnionDataStruct.GetCurrentSubtypeIndex()) {

				// If boolean, then set is as boolean data and apply it to the console variable, if valid
				case 0:
					CurrentSettingsOption.SetSubtype<bool>(LocalUnionDataStruct.GetSubtype<bool>());
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.GetSubtype<bool>(), ECVF_SetByConsole);
					GConfig->SetBool(*CurrentSection, VariableName, LocalUnionDataStruct.GetSubtype<bool>(), CurrentIniFileName);
					break;

				// If integer, then set is as integer data and apply it to the console variable, if valid
				case 1:
					CurrentSettingsOption.SetSubtype<int32>(LocalUnionDataStruct.GetSubtype<int32>());
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.GetSubtype<int32>(), ECVF_SetByConsole);
					GConfig->SetInt(*CurrentSection, VariableName, LocalUnionDataStruct.GetSubtype<int32>(), CurrentIniFileName);
					break;

				// If float, then set is as float data and apply it to the console variable, if valid
				case 2:
					CurrentSettingsOption.SetSubtype<float>(LocalUnionDataStruct.GetSubtype<float>());
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.GetSubtype<float>(), ECVF_SetByConsole);
					GConfig->SetFloat(*CurrentSection, VariableName, LocalUnionDataStruct.GetSubtype<float>(), CurrentIniFileName);
					break;

				// If string, then set is as string data and apply it to the console variable, if valid
				case 3:
					CurrentSettingsOption.SetSubtype<FString>(LocalUnionDataStruct.GetSubtype<FString>());
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(*LocalUnionDataStruct.GetSubtype<FString>(), ECVF_SetByConsole);
					GConfig->SetString(*CurrentSection, VariableName, *LocalUnionDataStruct.GetSubtype<FString>(), CurrentIniFileName);
					break;

				// Else, ignore it, as something has gone horribly wrong
				default:
					break;
			}
		});

		// Finally, call the above defined lambda on each and every variable defined in the current section of the config file
		GConfig->ForEachEntry(CallbackFunction, *CurrentSection, CurrentIniFileName);
	}
}


// 
void UGameSettingsSubsystem::SetCurrentGameID(const FString & NewGameID) {

	// Unload the current config file, as it is no longer the important one
	GConfig->UnloadFile(CurrentIniFileName);

	// Reset the settings menu map, as these settings are no longer valid
	SettingsMap.Reset();

	// Update the current save game ID
	CurrentSaveGameID = NewGameID;

	// Finally, force loading the new settings menu save game information
	this->ForceLoad();
}
