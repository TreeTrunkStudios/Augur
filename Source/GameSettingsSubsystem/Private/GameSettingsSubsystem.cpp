// Fill out your copyright notice in the Description page of Project Settings.


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

	// 
	//
}


// 
void UGameSettingsSubsystem::ForceLoad() {

	// 
	//
}


// 
void UGameSettingsSubsystem::SetCurrentGameID(const FString & NewGameID) {
	CurrentSaveGameID = NewGameID;
}
