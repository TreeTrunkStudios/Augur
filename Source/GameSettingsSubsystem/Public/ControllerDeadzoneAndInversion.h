// Sidney Towers
// TreeTrunkStudios (c) 2023


// Include guard section
#pragma once


// Include section
#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputAction.h"
#include "ControllerDeadzoneAndInversion.generated.h"


// Custom console variables for handling global gamepad functionality
// Console variable for storing the global controller state (none, left stick moves, or right stick moves)
static TAutoConsoleVariable<int32> CVarControllerState(
    TEXT("controls.gamepad.ControllerState"),
    1,
    TEXT("0 == no controller support, 1 == left stick movement stick, 2 == right stick movement stick"),
	ECVF_Set_NoSinkCall_Unsafe);


//// Custom console variables that are only made to handle the movement stick functionality
// Console variable for checking what kind of inversion the user wishes for
static TAutoConsoleVariable<int32> CVarMovementStickInversion(
    TEXT("controls.gamepad.MovementStickInversion"),
    0,
    TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's deadzone type
static TAutoConsoleVariable<int32> CVarMovementStickDeadzoneType(
	TEXT("controls.gamepad.MovementStickDeadzoneType"),
	4,
	TEXT("0 == ellipse, 1 == square, 2 == rounded square, 3 == cross, else none"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's deadzone percentage on the x axis
static TAutoConsoleVariable<float> CVarMovementStickDeadzoneX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's deadzone percentage on the y axis
static TAutoConsoleVariable<float> CVarMovementStickDeadzoneY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's no-diagonal zone percentage on the x axis (when at zero)
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalStartX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's no-diagonal zone percentage on the y axis (when at zero)
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalStartY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's no-diagonal zone percentage on the x axis (when at full/one)
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalEndX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the movement stick's no-diagonal zone percentage on the y axis (when at full/one)
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalEndY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


//// Custom console variables that are only made to handle the view stick functionality
// Console variable for 
static TAutoConsoleVariable<int32> CVarViewStickInversion(
	TEXT("controls.gamepad.ViewStickInversion"),
	0,
	TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<int32> CVarViewStickDeadzoneType(
	TEXT("controls.gamepad.ViewStickDeadzoneType"),
	4,
	TEXT("0 == ellipse, 1 == square, 2 == rounded square"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarViewStickDeadzoneX(
	TEXT("controls.gamepad.ViewStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarViewStickDeadzoneY(
	TEXT("controls.gamepad.ViewStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the view stick's no-diagonal zone percentage on the x axis (when at zero)
static TAutoConsoleVariable<float> CVarViewStickNoDiagonalStartX(
	TEXT("controls.gamepad.ViewStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the view stick's no-diagonal zone percentage on the y axis (when at zero)
static TAutoConsoleVariable<float> CVarViewStickNoDiagonalStartY(
	TEXT("controls.gamepad.ViewStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the view stick's no-diagonal zone percentage on the x axis (when at full/one)
static TAutoConsoleVariable<float> CVarViewStickNoDiagonalEndX(
	TEXT("controls.gamepad.ViewStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the view stick's no-diagonal zone percentage on the y axis (when at full/one)
static TAutoConsoleVariable<float> CVarViewStickNoDiagonalEndY(
	TEXT("controls.gamepad.ViewStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarViewStickSensitivityX(
	TEXT("controls.gamepad.ViewStickSensitivityX"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarViewStickSensitivityY(
	TEXT("controls.gamepad.ViewStickSensitivityY"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


//// Custom console variables that are only made to handle the view stick functionality when aiming down sights specifically
// Console variable for 
static TAutoConsoleVariable<bool> CVarAdsHasDifferentControls(
	TEXT("controls.gamepad.AdsHasDifferentControls"),
	false,
	TEXT("1 == aim down sights utilizes unique settings, 0 == aim down sights utilizes the same settings as its owning stick"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<int32> CVarAdsStickInversion(
	TEXT("controls.gamepad.AdsStickInversion"),
	0,
	TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<int32> CVarAdsStickDeadzoneType(
	TEXT("controls.gamepad.AdsStickDeadzoneType"),
	0,
	TEXT("0 == ellipse, 1 == square, 2 == rounded square"),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarAdsStickDeadzoneX(
	TEXT("controls.gamepad.AdsStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarAdsStickDeadzoneY(
	TEXT("controls.gamepad.AdsStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the aiming down sights stick's no-diagonal zone percentage on the x axis (when at zero)
static TAutoConsoleVariable<float> CVarAdsStickNoDiagonalStartX(
	TEXT("controls.gamepad.AdsStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the aiming down sights stick's no-diagonal zone percentage on the y axis (when at zero)
static TAutoConsoleVariable<float> CVarAdsStickNoDiagonalStartY(
	TEXT("controls.gamepad.AdsStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the aiming down sights stick's no-diagonal zone percentage on the x axis (when at full/one)
static TAutoConsoleVariable<float> CVarAdsStickNoDiagonalEndX(
	TEXT("controls.gamepad.AdsStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for handling the aiming down sights stick's no-diagonal zone percentage on the y axis (when at full/one)
static TAutoConsoleVariable<float> CVarAdsStickNoDiagonalEndY(
	TEXT("controls.gamepad.AdsStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarAdsStickSensitivityX(
	TEXT("controls.gamepad.AdsStickSensitivityX"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// Console variable for 
static TAutoConsoleVariable<float> CVarAdsStickSensitivityY(
	TEXT("controls.gamepad.AdsStickSensitivityY"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


// Input modifier class to handle all gamepad (and keyboard) inputs in regards to movement,
//     looking around, and looking around while aiming down sights
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UControllerDeadzoneAndInversion : public UInputModifier {
	GENERATED_BODY()


// Public UPROPERTY variable section
public:

	// Uproperty "static" variable for checking whether this is for the left stick or the right stick
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	bool IsLeftStick;


// Protected variable section
protected:

	// Local pointer to the aiming down sights input action for checking against the current input controller
	UInputAction * AimDownSightsInputAction;

	// Local 2D vectors to cache the calculated deadzones for each stick type
	FVector2D MovementStickDeadzone;
	FVector2D ViewStickDeadzone;
	FVector2D AdsStickDeadzone;

	// Local 2D vectors to cache the calculated inverse squared deadzones for each stick type
	FVector2D InverseSquaredMovementDeadzone;
	FVector2D InverseSquaredViewDeadzone;
	FVector2D InverseSquaredAdsDeadzone;


// Protected function section
protected:

	// Constructor for this class
	UControllerDeadzoneAndInversion();

	// Function which handles caching the wanted deadzone values whenever an associated console variable changes in value
	void CacheDeadzoneValues(IConsoleVariable*);

	// The function which handles modifying the inputs to handle deadzone, no-diagonal zones, and sensitivity
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
