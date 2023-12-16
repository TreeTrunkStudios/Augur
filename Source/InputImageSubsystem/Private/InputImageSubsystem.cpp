// TreeTrunkStudios (c) 2023


// 
#include "InputImageSubsystem.h"
#include "EnhancedInputSubsystems.h"


// 
void UInputImageSubsystem::BroadcastInputMethodChanged() {
	OnControllerTypeChanged.Broadcast(this);
}


// 
bool UInputImageSubsystem::Tick(float DeltaTime) {

	QUICK_SCOPE_CYCLE_COUNTER(STAT_UInputImageSubsystem_Tick);

	// Keep the InputImageInputPreprocessor on top. Input swap and input filtering (e.g. "Ignore Gamepad Input")
	// both start to break down if narrow game preprocessors temporarily get in front of it.
	// This is a workaround to avoid a bigger intervention in the SlateApplication API for managing preprocessors.
	if (InputImageInputPreprocessor.IsValid() && FSlateApplication::IsInitialized())
	{
		if (FSlateApplication & SlateApplication = FSlateApplication::Get(); SlateApplication.FindInputPreProcessor(InputImageInputPreprocessor) != 0) {
			SlateApplication.UnregisterInputPreProcessor(InputImageInputPreprocessor);
			SlateApplication.RegisterInputPreProcessor(InputImageInputPreprocessor, 0);
		}
	}
	
	return true; //repeat ticking
}


// 
uint8 UInputImageSubsystem::CollectTargetInputType() const {
	
	//
	const int32 GivenInputTypeOverride = CVarGamepadInputTypeOverride.GetValueOnGameThread();
	
	// Create a local boolean to see if an input type reset was necessary, defaulting it to whether the auto deduce value is set
	bool IsAutoDetected = (GivenInputTypeOverride < 0);

	// If our current type override is far too great of a number, then reset it back to -1 (auto type finding)
	if (GivenInputTypeOverride >= static_cast<int32>(EInputType::Num)) {
		CVarGamepadInputTypeOverride->SetWithCurrentPriority(-1);
		IsAutoDetected = true;
	}

	// If our override is less than zero, then we need to auto deduce the input type, in which case...
	if (IsAutoDetected == true) {

		// Swap on what our current input type is assumed to be, and...
		switch (CurrentInputType) {

			// If this is an unsupported input type, then we are defaulting to keyboard and mouse as to prevent crashing
			case EBaseInputTypes::Touch:
			default:
				[[fallthrough]];

			// If we are utilizing keyboard and mouse, then...
			case EBaseInputTypes::KeyboardAndMouse:

				// Assume the default of a dark keyboard type and continue
				return static_cast<uint8>(EInputType::KeyboardAndMouse);

			// If we are utilizing a gamepad, then we need to collect on what type of gamepad it is
			case EBaseInputTypes::Gamepad:

				// Automatically deduce our current gamepad type (or default) and return it
				return static_cast<uint8>(DeduceGamepadType());
		}
	}

	// Else, our given override is a valid override amount and must be returned
	return static_cast<uint8>(GivenInputTypeOverride);
}


// 
TObjectPtr<UTexture2D> UInputImageSubsystem::GetExactInputImage(const FKey & GivenKey) const {

	//
	if (GivenKey.IsValid() == false) {
		return InvalidInputImage.LoadSynchronous();
	}

	// If the given key type is a touch input, then...
	else if (GivenKey.IsTouch()) {

		// Simply return an invalid input image currently, as we do not support this yet
		return InvalidInputImage.LoadSynchronous();
	}

	// Else, if the current key type is either a gamepad key or analog, then...
	else if (GivenKey.IsGamepadKey() || GivenKey.IsAnalog()) {

		// Deduce our current gamepad type and return
		return GlobalInputImageMapArray[static_cast<uint8>(DeduceGamepadType())].FindChecked(GivenKey).LoadSynchronous();
	}

	// Else, we assume to be utilizing keyboard and mouse, in which case we just collect the relevant key and load it
	else {
		return GlobalInputImageMapArray[static_cast<uint8>(EInputType::KeyboardAndMouse)].FindChecked(GivenKey).LoadSynchronous();
	}
}


// 
TObjectPtr<UTexture2D> UInputImageSubsystem::GetFirstInputImage(const UInputMappingContext * GivenMapping, const UInputAction * GivenAction) const {

	// Locally store a constant reference to the local player (makes it so that we only cast once)
	const ULocalPlayer * CurrentLocalPlayer = GetLocalPlayer();

	// Ensure that the current local player is valid, as it always should be (only for testing/debug builds)
	check(IsValid(CurrentLocalPlayer));
	
	// Loop through all action key mappings for the given input mapping, and...
	for(const FEnhancedActionKeyMapping & Element : GivenMapping->GetMappings()) {

		// If the given action equals the current action key mapping's action, then...
		if (Element.Action == GivenAction) {

			// Switch our functionality ever so slightly depending on the type of input the current player is utilizing
			switch (CurrentInputType) {

				// If the current player is utilizing keyboard and mouse, then...
				case EBaseInputTypes::KeyboardAndMouse:

					// Skip the current key if it is a gamepad key
						if (Element.Key.IsGamepadKey() == true)
							continue;
				break;

				// Else, if the current player is utilizing a gamepad, then...
				case EBaseInputTypes::Gamepad:

					// Skip the current key if it is NOT a gamepad key
						if (Element.Key.IsGamepadKey() == false)
							continue;
				break;

				// Else, we do not support this type of input yet, so please skip it. Apologies.
				case EBaseInputTypes::Touch:
					[[fallthrough]];
				default:
					continue;
			}

			// Finally, collect our target soft reference to the wanted texture and load it into memory, returning the new UTexture2D object pointer
			// NOTE: LoadSynchronous is the same as .Get(), but will also synchronously load the texture if it is not fully loaded yet
			return GlobalInputImageMapArray[CollectTargetInputType()].FindChecked(Element.Key).LoadSynchronous();
		}
	}

	// If we failed to find anything that we desired, then return an empty texture rather than crashing the game
	return InvalidInputImage.LoadSynchronous();
}


// 
TArray<TObjectPtr<UTexture2D>> UInputImageSubsystem::GetAllInputImages(const UInputMappingContext * GivenMapping, const UInputAction * GivenAction) const {

	// Locally store a constant reference to the local player (makes it so that we only cast once)
	const ULocalPlayer * CurrentLocalPlayer = GetLocalPlayer();

	// Ensure that the current local player is valid, as it always should be (only for testing/debug builds)
	check(IsValid(CurrentLocalPlayer));

	// Create a local array of object pointers to store all found input images into
	TArray<TObjectPtr<UTexture2D>> LocalTexturePointerArray;

	// Loop through all action key mappings for the given input mapping, and...
	for(const FEnhancedActionKeyMapping & Element : GivenMapping->GetMappings()) {

		// If the given action equals the current action key mapping's action, then...
		if (Element.Action == GivenAction) {

			// Switch our functionality ever so slightly depending on the type of input the current player is utilizing
			switch (CurrentInputType) {

				// If the current player is utilizing keyboard and mouse, then...
				case EBaseInputTypes::KeyboardAndMouse:

					// Skip the current key if it is a gamepad key
					if (Element.Key.IsGamepadKey() == true)
						continue;
					break;

				// Else, if the current player is utilizing a gamepad, then...
				case EBaseInputTypes::Gamepad:

					// Skip the current key if it is NOT a gamepad key
					if (Element.Key.IsGamepadKey() == false)
						continue;
					break;

				// Else, we do not support this type of input yet, so please skip it. Apologies.
				case EBaseInputTypes::Touch:
					[[fallthrough]];
				default:
					continue;
			}

			// Finally, collect our target soft reference to the wanted texture and load it into memory, returning the new UTexture2D object pointer
			// NOTE: LoadSynchronous is the same as .Get(), but will also synchronously load the texture if it is not fully loaded yet
			LocalTexturePointerArray.Push(GlobalInputImageMapArray[CollectTargetInputType()].FindChecked(Element.Key).LoadSynchronous());
		}
	}

	// If we found nothing, then give the array in invalid image so that it informs the user of there being an error visually
	if (LocalTexturePointerArray.IsEmpty() == true)
		LocalTexturePointerArray.Add(InvalidInputImage.LoadSynchronous());

	// If we failed to find anything that we desired, then return an empty texture rather than crashing the game
	return LocalTexturePointerArray;
}


// Define the same loctext namespace utilized when creating FKeys
#define LOCTEXT_NAMESPACE "InputKeys"


// 
void UInputImageSubsystem::Initialize(FSubsystemCollectionBase & Collection) {

	//
	InputImageInputPreprocessor = MakeShared<FInputImageInputPreprocessor>(*this);
	FSlateApplication::Get().RegisterInputPreProcessor(InputImageInputPreprocessor, 0);

	//
	TickHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UInputImageSubsystem::Tick), 0.1f);

	//
	if (UEnhancedInputLocalPlayerSubsystem * EnhancedInputLocalPlayerSubsystem = GetLocalPlayerChecked()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
	{
		BroadcastInputMethodChangedEvent.BindUFunction(this, GET_FUNCTION_NAME_CHECKED(UInputImageSubsystem, BroadcastInputMethodChanged));
		EnhancedInputLocalPlayerSubsystem->ControlMappingsRebuiltDelegate.AddUnique(BroadcastInputMethodChangedEvent);
	}

	// Ensure that this section only initializes once throughout everywhere
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
void UInputImageSubsystem::Deinitialize() {

	// 
	Super::Deinitialize();

	//
	if (FSlateApplication::IsInitialized()) {
		FSlateApplication::Get().UnregisterInputPreProcessor(InputImageInputPreprocessor);
	}

	// 
	InputImageInputPreprocessor.Reset();

	// 
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetLocalPlayerChecked()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
		EnhancedInputLocalPlayerSubsystem->ControlMappingsRebuiltDelegate.Remove(BroadcastInputMethodChangedEvent);
		BroadcastInputMethodChangedEvent.Unbind();
	}

	//
	FTSTicker::GetCoreTicker().RemoveTicker(TickHandle);
}


// 
void UInputImageSubsystem::SetCurrentInputType(const EBaseInputTypes GivenInputType) {

	// If the given input type is actually different from our current one, then...
	if (CurrentInputType != GivenInputType) {
	
		// Set our current input type to the newly given one
		CurrentInputType = GivenInputType;

		//
		if (CurrentInputType == EBaseInputTypes::KeyboardAndMouse) {

			// 
			FSlateApplication & SlateApplication = FSlateApplication::Get();

			// 
			if (ULocalPlayer * LocalPlayer = GetLocalPlayerChecked(); (LocalPlayer && LocalPlayer->GetSlateUser() == SlateApplication.GetCursorUser()))
				SlateApplication.UsePlatformCursorForCursorUser(true);
		}

		// If we are using the editor, then write to log what our new input base type is
#if WITH_EDITOR
		switch (CurrentInputType) {

			// If our value has been corrupted or become invalid, then...
			default:

				// Reset our current input type to keyboard and mouse and assume that
				CurrentInputType = EBaseInputTypes::KeyboardAndMouse;
				[[fallthrough]];

			// If our input device is keyboard and mouse, then...
			case EBaseInputTypes::KeyboardAndMouse:
				
				// 
				UE_LOG(LogInputImage, Log, TEXT("UInputImageSubsystem::SetCurrentInputType(): Using Mouse"));
				break;
			
			// 
			case EBaseInputTypes::Gamepad:

				// 
				UE_LOG(LogInputImage, Log, TEXT("UInputImageSubsystem::SetCurrentInputType(): Using Gamepad"));
				break;

			// 
			case EBaseInputTypes::Touch:

				// 
				UE_LOG(LogInputImage, Log, TEXT("UInputImageSubsystem::SetCurrentInputType(): Using Touch"));
				break;
		}
#endif

		// Finally, send out notifications so we update our buttons
		BroadcastInputMethodChanged();
	}
}


// 
FName UInputImageSubsystem::GetCurrentGamepadName() const {
	return GamepadInputType;
}


// 
void UInputImageSubsystem::SetGamepadInputType(const FName GivenInputTypeName) {

	// If the gamepad type is actually different, then...
	if (GamepadInputType != GivenInputTypeName) {

		// Update to the given input type name
		GamepadInputType = GivenInputTypeName;

		// Send out notifications so we update our buttons
		BroadcastInputMethodChanged();
	}
}


// 
EInputType UInputImageSubsystem::DeduceGamepadType() const {

	//
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Purple, FString("Gamepad Name: ") + GamepadInputType.ToString());

	// 
	return EInputType::XboxSeriesController;
}


// Undefine the loctext namespace, as it is no longer needed
#undef LOCTEXT_NAMESPACE
