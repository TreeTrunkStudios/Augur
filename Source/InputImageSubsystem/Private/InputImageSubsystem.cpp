// TreeTrunkStudios (c) 2023


// 
#include "InputImageSubsystem.h"
#include "EnhancedInputSubsystems.h"


// TODO: Implement all console commands for settings system
TAutoConsoleVariable<int32> CVarGamepadInputTypeOverride(
	TEXT("Input.Image.GamepadTypeOverride"),
	-1,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
);


// 
const FKey ECustomKeys::F13("F13");
const FKey ECustomKeys::F14("F14");
const FKey ECustomKeys::F15("F15");
const FKey ECustomKeys::F16("F16");
const FKey ECustomKeys::F17("F17");
const FKey ECustomKeys::F18("F18");
const FKey ECustomKeys::F19("F19");
const FKey ECustomKeys::F20("F20");
const FKey ECustomKeys::F21("F21");
const FKey ECustomKeys::F22("F22");
const FKey ECustomKeys::F23("F23");
const FKey ECustomKeys::F24("F24");

const FKey ECustomKeys::U_AccentGrave("U_AccentGrave");


// 
const uint8 UInputImageSubsystem::CollectTargetInputType(const int32 && GivenInputTypeOverride, const ECommonInputType & CurrentInputType) const {

	// TESTING PURPOSES ONLY
	if (GEngine) {
		const FName CurrentGamepadName = GetLocalPlayer()->GetSubsystem<UCommonInputSubsystem>()->GetCurrentGamepadName();
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Purple, CurrentGamepadName.ToString());
	}
	
	// Create a local boolean to see if an input type reset was necessary, defaulting it to whether the auto deduce value is set
	bool WasReset = (GivenInputTypeOverride < 0);

	// If our current type override is far too great of a number, then reset it back to -1 (auto type finding)
	if (GivenInputTypeOverride >= static_cast<int32>(EInputType::Num)) {
		CVarGamepadInputTypeOverride->Set(-1);
		WasReset = true;
	}

	// If our override is less than zero, then we need to auto deduce the input type, in which case...
	if (WasReset == true) {

		// Swap on what our current input type is assumed to be, and...
		switch (CurrentInputType) {

			// If this is an unsupported input type, then we are defaulting to keyboard and mouse as to prevent crashing
			case ECommonInputType::Touch:
			default:
				[[fallthrough]];

			// If we are utilizing keyboard and mouse, then...
			case ECommonInputType::MouseAndKeyboard:

				// Assume the default of a dark keyboard type and continue
				return static_cast<uint8>(EInputType::KeyboardAndMouse);

			// If we are utilizing a gamepad, then we need to collect on what type of gamepad it is
			case ECommonInputType::Gamepad:

				// TODO: Implement auto controller type deduction here so that we do not just default to a generic controller
				return static_cast<uint8>(EInputType::XboxSeriesController);
		}
	}

	// Else, our given override is a valid override amount and must be 
	else {
		return static_cast<uint8>(GivenInputTypeOverride);
	}

	// 
	return 0;
}


// 
TObjectPtr<class UTexture2D> UInputImageSubsystem::GetFirstInputImage(UInputAction * GivenAction) {

	// Locally store a constant reference to the local player (makes it so that we only cast once)
	const ULocalPlayer * CurrentLocalPlayer = GetLocalPlayer();

	// Ensure that the current local player is valid, as it always should be (only for testing/debug builds)
	check(IsValid(CurrentLocalPlayer));

	// Locally store a constant copy to the local player's common input subsystem's current input type
	const ECommonInputType CurrentInputType = CurrentLocalPlayer->GetSubsystem<UCommonInputSubsystem>()->GetCurrentInputType();


	// Loop through all keys associated with the given action collected through the local player's enhanced input subsystem, and...
	for (const FKey & CurrentKey : CurrentLocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()->QueryKeysMappedToAction(GivenAction)) {

		// Switch our functionality ever so slightly depending on the type of input the current player is utilizing
		switch (CurrentInputType) {

			// If the current player is utilizing keyboard and mouse, then...
			case ECommonInputType::MouseAndKeyboard:

				// Skip the current key if it is a gamepad key
				if (CurrentKey.IsGamepadKey() == true) {
					continue;
				}
				break;

			// Else, if the current player is utilizing a gamepad, then...
			case ECommonInputType::Gamepad:

				// Skip the current key if it is NOT a gamepad key
				if (CurrentKey.IsGamepadKey() == false) {
					continue;
				}
				break;

			// Else, we do not support this type of input yet, so please skip it. Apologies.
			case ECommonInputType::Touch:
				[[fallthrough]];
			default:
				return TObjectPtr<class UTexture2D>(nullptr);
		}

		// Finally, collect our target soft reference to the wanted texture and load it into memory, returning the new UTexture2D object pointer
		// NOTE: LoadSynchronous is the same as .Get(), but will also synchronously load the texture if it is not fully loaded yet
		return GlobalInputImageMapArray[CollectTargetInputType(CVarGamepadInputTypeOverride.GetValueOnGameThread(), CurrentInputType)].FindChecked(CurrentKey).LoadSynchronous();
	}

	// If we failed to find anything that we desired, then return an empty texture rather than crashing the game
	return TObjectPtr<class UTexture2D>(nullptr);
}


// 
TArray<TObjectPtr<class UTexture2D>> UInputImageSubsystem::GetAllInputImages(UInputAction * GivenAction) {
	return TArray<TObjectPtr<class UTexture2D>>();
}


// 
#define LOCTEXT_NAMESPACE "InputKeys"


// 
void UInputImageSubsystem::Initialize(FSubsystemCollectionBase & Collection) {

	// 
	if (GEngine) {
		const FName CurrentGamepadName = GetLocalPlayer()->GetSubsystem<UCommonInputSubsystem>()->GetCurrentGamepadName();
		GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Purple, CurrentGamepadName.ToString());
	}

	// 
	static bool IsInitialized = false;
	if (IsInitialized == true) return;
	IsInitialized = true;

	// 
	EKeys::AddKey(FKeyDetails(ECustomKeys::F13, LOCTEXT("F13", "F13")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F14, LOCTEXT("F14", "F14")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F15, LOCTEXT("F15", "F15")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F16, LOCTEXT("F16", "F16")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F17, LOCTEXT("F17", "F17")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F18, LOCTEXT("F18", "F18")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F19, LOCTEXT("F19", "F19")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F20, LOCTEXT("F20", "F20")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F21, LOCTEXT("F21", "F21")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F22, LOCTEXT("F22", "F22")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F23, LOCTEXT("F23", "F23")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::F24, LOCTEXT("F24", "F24")));
	EKeys::AddKey(FKeyDetails(ECustomKeys::U_AccentGrave, FText::FromString(FString::Chr(249))));
}


// 
#undef LOCTEXT_NAMESPACE
