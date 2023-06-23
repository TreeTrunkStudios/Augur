// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "MouseSettingsModifier.generated.h"


//// Custom console variables that are only made to handle the view stick functionality
// Console variable for 
static TAutoConsoleVariable<int32> CVarMouseInversion(
	TEXT("controls.mouse.Inversion"),
	0,
	TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarMouseSensitivityX(
	TEXT("controls.mouse.SensitivityX"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarMouseSensitivityY(
	TEXT("controls.mouse.SensitivityY"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UMouseSettingsModifier : public UInputModifier {
	GENERATED_BODY()
	

// 
protected:

	// The function which handles modifying the inputs to handle deadzone, no-diagonal zones, and sensitivity
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
