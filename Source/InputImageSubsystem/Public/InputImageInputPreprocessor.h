// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "Subsystems/LocalPlayerSubsystem.h"
#include "Containers/Ticker.h"
#include "Framework/Application/IInputProcessor.h"
#include "Framework/Application/SlateApplication.h"


// 
#if UE_ENABLE_INCLUDE_ORDER_DEPRECATED_IN_5_2
#include "Framework/Application/SlateApplication.h"
#include "Styling/SlateTypes.h"
#endif


//
DECLARE_LOG_CATEGORY_EXTERN(LogInputImage, Log, All);


//
UENUM()
enum class EBaseInputTypes {
	KeyboardAndMouse,
	Gamepad,
	Touch,
	Num UMETA(Hidden)
};


//
DECLARE_EVENT_OneParam(FInputImageInputPreprocessor, FGamepadChangeDetectedEvent, FName);


//
class UInputImageSubsystem;


// Helper class that is designed to fire before any UI has a chance to process input so that
//     we can properly set the current input type of the application before anything else
class INPUTIMAGESUBSYSTEM_API FInputImageInputPreprocessor final : public IInputProcessor {

// Section containing public C++ style functions
public:

	// 
	FInputImageInputPreprocessor(UInputImageSubsystem & GivenOwningLocalPlayerSubsystem);

	//~ Begin IInputProcessor Interface
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override;
	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override;
	virtual bool HandleAnalogInputEvent(FSlateApplication& SlateApp, const FAnalogInputEvent& InAnalogInputEvent) override;
	virtual bool HandleMouseMoveEvent(FSlateApplication& SlateApp, const FPointerEvent& InPointerEvent) override;
	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& InPointerEvent) override;
	virtual bool HandleMouseButtonDoubleClickEvent(FSlateApplication& SlateApp, const FPointerEvent& InPointerEvent) override;
	virtual const TCHAR* GetDebugName() const override { return TEXT("InputImage"); }


// Section containing public C++ style variables
public:

	//
	FGamepadChangeDetectedEvent OnGamepadChangeDetected;
	

// Section containing protected C++ style functions
protected:

	// 
	bool IsRelevantInput(const FSlateApplication & SlateApp, const FInputEvent & InputEvent, const EBaseInputTypes DesiredInputType) const;

	// 
	void RefreshCurrentInputMethod(EBaseInputTypes InputMethod);

	//
static EBaseInputTypes GetInputType(const FKey & Key);

	//
static EBaseInputTypes GetInputType(const FPointerEvent & PointerEvent);

	
// Section containing protected C++ style variables
protected:

	//
	UInputImageSubsystem & OwningLocalPlayerSubsystem;

	// 
	FName LastSeenGamepadInputDeviceName;
	FString LastSeenGamepadHardwareDeviceIdentifier;
};
