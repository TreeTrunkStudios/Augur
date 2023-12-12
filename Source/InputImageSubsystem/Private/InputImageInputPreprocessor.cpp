// TreeTrunkStudios (c) 2023


// 
#include "InputImageInputPreprocessor.h"
#include "InputImageSubsystem.h"


// 
#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"

#include "Engine/Engine.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "Framework/Application/SlateApplication.h"
#include "Framework/Application/SlateUser.h"
#include "Misc/ConfigCacheIni.h"
#include "HAL/PlatformStackWalk.h"
#include "Widgets/SViewport.h"

#if WITH_EDITOR
#include "Settings/LevelEditorPlaySettings.h"
#endif


//
DEFINE_LOG_CATEGORY(LogInputImage);


// Default constructor that requires a reference to its owning local player subsystem
FInputImageInputPreprocessor::FInputImageInputPreprocessor(UInputImageSubsystem & GivenOwningLocalPlayerSubsystem) : OwningLocalPlayerSubsystem(GivenOwningLocalPlayerSubsystem) {}


// An empty tick function, as we do not need to tick
void FInputImageInputPreprocessor::Tick(const float DeltaTime, FSlateApplication & SlateApp, TSharedRef<ICursor> Cursor) {}


// 
bool FInputImageInputPreprocessor::HandleKeyDownEvent(FSlateApplication & SlateApp, const FKeyEvent & InKeyEvent) {
	const EBaseInputTypes InputType = GetInputType(InKeyEvent.GetKey());
	if (IsRelevantInput(SlateApp, InKeyEvent, InputType))
	{
		RefreshCurrentInputMethod(InputType);
	}
	return false;
}


// 
bool FInputImageInputPreprocessor::HandleAnalogInputEvent(FSlateApplication & SlateApp, const FAnalogInputEvent & InAnalogInputEvent) {
	const EBaseInputTypes InputType = GetInputType(InAnalogInputEvent.GetKey());
	return !IsRelevantInput(SlateApp, InAnalogInputEvent, InputType);
}


// 
bool FInputImageInputPreprocessor::HandleMouseMoveEvent(FSlateApplication & SlateApp, const FPointerEvent & InPointerEvent) {
	const EBaseInputTypes InputType = GetInputType(InPointerEvent);
	if (IsRelevantInput(SlateApp, InPointerEvent, InputType))
	{
		if (!InPointerEvent.GetCursorDelta().IsNearlyZero())
		{
			RefreshCurrentInputMethod(InputType);
		}
	}
	
	return false;
}


// 
bool FInputImageInputPreprocessor::HandleMouseButtonDownEvent(FSlateApplication & SlateApp, const FPointerEvent & InPointerEvent) {

	// 
	if (const EBaseInputTypes InputType = GetInputType(InPointerEvent); IsRelevantInput(SlateApp, InPointerEvent, InputType))
		RefreshCurrentInputMethod(InputType);

	// 
	return false;
}


// 
bool FInputImageInputPreprocessor::HandleMouseButtonDoubleClickEvent(FSlateApplication & SlateApp, const FPointerEvent & InPointerEvent) {

	// 
	if (const EBaseInputTypes InputType = GetInputType(InPointerEvent); IsRelevantInput(SlateApp, InPointerEvent, InputType))
		RefreshCurrentInputMethod(InputType);

	// 
	return false;
}


// 
bool FInputImageInputPreprocessor::IsRelevantInput(const FSlateApplication & SlateApp, const FInputEvent & InputEvent, const EBaseInputTypes DesiredInputType) const {

	// If we're stopped at a breakpoint we need for this input preprocessor to just ignore all incoming input
	// because we're now doing stuff outside the game loop in the editor and it needs to not block all that.
	// This can happen if you suspend input while spawning a dialog and then hit another breakpoint and then
	// try and use the editor, you can suddenly be unable to do anything.
#if WITH_EDITOR
	if (GIntraFrameDebuggingGameThread)
		return false;
#endif
	
	if (SlateApp.IsActive() || SlateApp.GetHandleDeviceInputWhenApplicationNotActive()) {
		int32 ControllerId = OwningLocalPlayerSubsystem.GetLocalPlayerChecked()->GetControllerId();

		// PIE is a very special flower, especially when running two clients - we have two LocalPlayers with ControllerId 0
		// The editor has existing shenanigans for handling this scenario, so we're using those for now
		// Ultimately this would ideally be something the editor resolves at the SlateApplication level with a custom ISlateInputMapping or something
#if WITH_EDITOR
		GEngine->RemapGamepadControllerIdForPIE(OwningLocalPlayerSubsystem.GetLocalPlayerChecked()->ViewportClient, ControllerId);
#endif

		// Finally, return whether our current controller ID equals the input event's user index
		return ControllerId == InputEvent.GetUserIndex();
	}

	// If anything goes wrong, then default to returning false
	return false;
}


// 
void FInputImageInputPreprocessor::RefreshCurrentInputMethod(const EBaseInputTypes InputMethod) {
	
#if WITH_EDITOR
	// Lots of special-case fun for PIE - there are special scenarios wherein we want to ignore the update attempt
	const ULocalPlayer & LocalPlayer = *OwningLocalPlayerSubsystem.GetLocalPlayerChecked();
	bool bCanApplyInputMethodUpdate = false;
	if (LocalPlayer.ViewportClient)
	{
		if (const TSharedPtr<FSlateUser> SlateUser = FSlateApplication::Get().GetUser(LocalPlayer.GetControllerId()); ensure(SlateUser))
		{
			bool bRoutingGamepadToNextPlayer = false;
			if (!GetDefault<ULevelEditorPlaySettings>()->GetRouteGamepadToSecondWindow(bRoutingGamepadToNextPlayer))
			{
				// This looks strange - it's because the getter will set the output param based on the value of the setting, but return
				//	whether the setting actually matters. So we're making sure that even if the setting is true, we revert to false when it's irrelevant.
				bRoutingGamepadToNextPlayer = false;
			}

			if (SlateUser->IsWidgetInFocusPath(LocalPlayer.ViewportClient->GetGameViewportWidget()))
			{
				// Our owner's game viewport is in the focus path, which in a PIE scenario means we shouldn't
				// acknowledge gamepad input if it's being routed to another PIE client
				if (InputMethod != EBaseInputTypes::Gamepad || !bRoutingGamepadToNextPlayer)
				{
					bCanApplyInputMethodUpdate = true;
				}
			}
			else if (InputMethod == EBaseInputTypes::Gamepad)
			{
				bCanApplyInputMethodUpdate = bRoutingGamepadToNextPlayer;
			}
		}
	}
	if (!bCanApplyInputMethodUpdate)
	{
		return;
	}
#endif

	OwningLocalPlayerSubsystem.SetCurrentInputType(InputMethod);

	// Try to auto-detect the type of gamepad that the user just switched to
	if (InputMethod == EBaseInputTypes::Gamepad) {

		// 
		if (const FInputDeviceScope * DeviceScope = FInputDeviceScope::GetCurrent()) {

			// 
			if ((DeviceScope->InputDeviceName != LastSeenGamepadInputDeviceName) || (DeviceScope->HardwareDeviceIdentifier != LastSeenGamepadHardwareDeviceIdentifier)) {
				
				// 
				LastSeenGamepadInputDeviceName = DeviceScope->InputDeviceName;
				LastSeenGamepadHardwareDeviceIdentifier = DeviceScope->HardwareDeviceIdentifier;

				//
				const FName GamepadInputType = OwningLocalPlayerSubsystem.GetCurrentGamepadName();

				//
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Black, DeviceScope->InputDeviceName.ToString());
					GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Black, DeviceScope->HardwareDeviceIdentifier);
					GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Black, FString("From: ") + GamepadInputType.ToString());
				}

				//
				const FName BestGamepadType = DeviceScope->InputDeviceName;

				// 
				UE_LOG(LogInputImage, Log, TEXT("FInputImageInputPreprocessor: Autodetect changed GamepadInputType to %s"), *BestGamepadType.ToString());
				OwningLocalPlayerSubsystem.SetGamepadInputType(BestGamepadType);
				OnGamepadChangeDetected.Broadcast(BestGamepadType);
			}
		}
	}
}


// 
EBaseInputTypes FInputImageInputPreprocessor::GetInputType(const FKey & Key) {

	// If the key is one of the physical touch keys, then return touch
	for (const FKey & CurrentTouchKey : EKeys::TouchKeys)
		if (CurrentTouchKey == Key)
			return EBaseInputTypes::Touch;

	// Else, if it was just a touch input, then return touch
	if (Key.IsTouch())
		return EBaseInputTypes::Touch;
	
	// Else, if the key was a gamepad key, then return gamepad
	if (Key.IsGamepadKey())
		return EBaseInputTypes::Gamepad;

	// Else, finally, default to just keyboard and mouse
	return EBaseInputTypes::KeyboardAndMouse;
}


// 
EBaseInputTypes FInputImageInputPreprocessor::GetInputType(const FPointerEvent & PointerEvent) {

	// If this is a touch event, then return touch
	if (PointerEvent.IsTouchEvent())
		return EBaseInputTypes::Touch;

	// Else, default to keyboard and mouse, as a pointer event cannot be a gamepad event
	return EBaseInputTypes::KeyboardAndMouse;
}
