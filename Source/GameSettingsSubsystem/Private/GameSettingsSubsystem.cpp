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
const FUnionDataStruct CalculateSettingDataTypeFromString(const FString & DataString) {

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
		return FUnionDataStruct{true};
	}

	// Else, if the given string is any form of boolean as false, then return that data
	if (FCStringUtf8::Stricmp(LocalString, TEXT("False")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, TEXT("No")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, TEXT("Off")) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, *(CoreTexts.False.ToString())) == 0 ||
			 FCStringUtf8::Stricmp(LocalString, *(CoreTexts.No.ToString())) == 0) {
		return FUnionDataStruct{false};
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
		return FUnionDataStruct{static_cast<bool>(true)};
	}

	// Else, if the given string is any form of boolean as false, then return that data
	if (FCStringWide::Stricmp(LocalString, TEXT("False")) == 0 ||
		     FCStringWide::Stricmp(LocalString, TEXT("No")) == 0 ||
		     FCStringWide::Stricmp(LocalString, TEXT("Off")) == 0 ||
		     FCStringWide::Stricmp(LocalString, *(CoreTexts.False.ToString())) == 0 ||
		     FCStringWide::Stricmp(LocalString, *(CoreTexts.No.ToString())) == 0) {
		return FUnionDataStruct{static_cast<bool>(false)};
	}
#endif

	//// Else, we are not a boolean and need to continue checking
	// Check for if the current string is numeric, and, if so...
	if (DataString.IsNumeric()) {

		// Check if the data contains a period, in which case it is a float, so return the data as a float
		if (DataString.Contains(TEXT("."))) {
#if PLATFORM_TCHAR_IS_UTF8CHAR
			return FUnionDataStruct{FCStringUtf8::Atof(StringCast<UTF8CHAR>(*DataString).Get())};
#else
			return FUnionDataStruct{FCStringWide::Atof(*DataString)};
#endif
		}

		// Else, we are dealing with an integer of some sorts, so return the data as an integer
		else {
#if PLATFORM_TCHAR_IS_UTF8CHAR
			return FUnionDataStruct{FCStringUtf8::Atoi(StringCast<UTF8CHAR>(*DataString).Get())};
#else
			return FUnionDataStruct{FCStringWide::Atoi(*DataString)};
#endif
		}
	}

	// Else, just assume it's a string and move on
	return FUnionDataStruct{DataString};
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
			const FUnionDataStruct LocalUnionDataStruct = CalculateSettingDataTypeFromString(FString(VariableValue));

			// Switch on the type of data this contains
			switch (LocalUnionDataStruct.DataType) {

				// If boolean, then set is as boolean data and apply it to the console variable, if valid
				case USettingDataTypeEnum::Boolean:
					CurrentSettingsOption.SetSubtype<bool>(LocalUnionDataStruct.BooleanData);
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.BooleanData, ECVF_SetByConsole);
					GConfig->SetBool(*CurrentSection, VariableName, LocalUnionDataStruct.BooleanData, CurrentIniFileName);
					break;

				// If integer, then set is as integer data and apply it to the console variable, if valid
				case USettingDataTypeEnum::Integer:
					CurrentSettingsOption.SetSubtype<int32>(LocalUnionDataStruct.IntegerData);
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.IntegerData, ECVF_SetByConsole);
					GConfig->SetInt(*CurrentSection, VariableName, LocalUnionDataStruct.IntegerData, CurrentIniFileName);
					break;

				// If float, then set is as float data and apply it to the console variable, if valid
				case USettingDataTypeEnum::Float:
					CurrentSettingsOption.SetSubtype<float>(LocalUnionDataStruct.FloatData);
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(LocalUnionDataStruct.FloatData, ECVF_SetByConsole);
					GConfig->SetFloat(*CurrentSection, VariableName, LocalUnionDataStruct.FloatData, CurrentIniFileName);
					break;

				// If string, then set is as string data and apply it to the console variable, if valid
				case USettingDataTypeEnum::String:
					CurrentSettingsOption.SetSubtype<FString>(LocalUnionDataStruct.StringData);
					if (CurrentConsoleVariable) CurrentConsoleVariable->Set(*LocalUnionDataStruct.StringData, ECVF_SetByConsole);
					GConfig->SetString(*CurrentSection, VariableName, *LocalUnionDataStruct.StringData, CurrentIniFileName);
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


// 
const FUnionDataStruct UGameSettingsSubsystem::GetSettingData(FName SectionName, FName VariableName) {

	// Internally collect the settings data properly
	const SettingsDataUnion & InnerData = GetSettingData_Internal(SectionName, VariableName);

	// We need to figure out what type of data this is...
	switch (InnerData.GetCurrentSubtypeIndex()) {

		// It is a bool, so return such
		case 0:
			return FUnionDataStruct{ InnerData.GetSubtype<bool>() };
			break;

		// It is an integer, so return such
		case 1:
			return FUnionDataStruct{ InnerData.GetSubtype<int32>() };
			break;

		// It is a float, so return such
		case 2:
			return FUnionDataStruct{ InnerData.GetSubtype<float>() };
			break;

		// It is a string, so return such
		case 3:
			return FUnionDataStruct{ InnerData.GetSubtype<FString>() };
			break;

		// Else, go to the leftover return
		default:
			break;
	}

	// 
	return FUnionDataStruct{};
}


// 
void UGameSettingsSubsystem::SetSettingData(FName SectionName, FName VariableName, FUnionDataStruct VariableData) {

	// Change functionality based off what type of data we are dealing with
	switch (VariableData.DataType) {
	
		// If the relying setting's data type is a boolean, then set it
		case USettingDataTypeEnum::Boolean:
			SetSettingData_Internal<bool>(SectionName, VariableName, VariableData.BooleanData);
			break;

		// Else, if the relying setting's data type is an integer, then set it
		case USettingDataTypeEnum::Integer:
			SetSettingData_Internal<int32>(SectionName, VariableName, VariableData.IntegerData);
			break;

		// Else, if the relying setting's data type is a float, then set it
		case USettingDataTypeEnum::Float:
			SetSettingData_Internal<float>(SectionName, VariableName, VariableData.FloatData);
			break;

		// Else, if the relying setting's data type is a string, then set it
		case USettingDataTypeEnum::String:
			SetSettingData_Internal<FString>(SectionName, VariableName, VariableData.StringData);
			break;

		// Else, the target's data type is an error, so collapse this widget, as it is invalid, technically
		case USettingDataTypeEnum::ERROR:
		default:
			break;
	}
}
