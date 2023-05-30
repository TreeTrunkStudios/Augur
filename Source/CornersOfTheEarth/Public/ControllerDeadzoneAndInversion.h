// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "InputAction.h"
#include "ControllerDeadzoneAndInversion.generated.h"


// Create a console variable for storing the global controller state
static TAutoConsoleVariable<int32> CVarControllerState(
    TEXT("controls.gamepad.ControllerState"),
    1,
    TEXT("0 == no controller support, 1 == left stick movement stick, 2 == right stick movement stick"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<bool> CVarAreControllerJoysticksSimilar(
	TEXT("controls.gamepad.AreJoysticksSimilar"),
	true,
	TEXT("1 == both sticks use the same settings, 0 == each stick has its own settings"),
	ECVF_Set_NoSinkCall_Unsafe);


//// 
// 
static TAutoConsoleVariable<int32> CVarMovementStickInversion(
    TEXT("controls.gamepad.MovementStickInversion"),
    0,
    TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<int32> CVarMovementStickDeadzoneType(
	TEXT("controls.gamepad.MovementStickDeadzoneType"),
	0,
	TEXT("0 == circular, 1 == square, 2 == rounded square, 3 == cross"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickDeadzoneX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickDeadzoneY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalStartX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalStartY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalEndX(
	TEXT("controls.gamepad.MovementStickDeadzoneX"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarMovementStickNoDiagonalEndY(
	TEXT("controls.gamepad.MovementStickDeadzoneY"),
	0.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


////
// 
static TAutoConsoleVariable<int32> CVarViewStickInversion(
	TEXT("controls.gamepad.ViewStickInversion"),
	0,
	TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<int32> CVarViewStickDeadzoneType(
	TEXT("controls.gamepad.ViewStickDeadzoneType"),
	0,
	TEXT("0 == circular, 1 == square, 2 == rounded square"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarViewStickDeadzoneX(
	TEXT("controls.gamepad.ViewStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarViewStickDeadzoneY(
	TEXT("controls.gamepad.ViewStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarViewStickSensitivityX(
	TEXT("controls.gamepad.ViewStickSensitivityX"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarViewStickSensitivityY(
	TEXT("controls.gamepad.ViewStickSensitivityY"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


//// 
// 
static TAutoConsoleVariable<bool> CVarAdsHasDifferentControls(
	TEXT("controls.gamepad.AdsHasDifferentControls"),
	false,
	TEXT("1 == aim down sights utilizes unique settings, 0 == aim down sights utilizes the same settings as its owning stick"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<int32> CVarAdsStickInversion(
	TEXT("controls.gamepad.AdsStickInversion"),
	0,
	TEXT("0 == no inversion, 1 == invert x-axis only, 2 == invert y-axis only, 3 == invert both axes"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<int32> CVarAdsStickDeadzoneType(
	TEXT("controls.gamepad.AdsStickDeadzoneType"),
	0,
	TEXT("0 == circular, 1 == square, 2 == rounded square"),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarAdsStickDeadzoneX(
	TEXT("controls.gamepad.AdsStickDeadzoneX"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarAdsStickDeadzoneY(
	TEXT("controls.gamepad.AdsStickDeadzoneY"),
	0.1f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarAdsStickSensitivityX(
	TEXT("controls.gamepad.AdsStickSensitivityX"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);

// 
static TAutoConsoleVariable<float> CVarAdsStickSensitivityY(
	TEXT("controls.gamepad.AdsStickSensitivityY"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);


// 
UCLASS()
class CORNERSOFTHEEARTH_API UControllerDeadzoneAndInversion : public UInputModifier {
	GENERATED_BODY()


// 
public:

	// 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	bool IsLeftStick;


// 
protected:

	//
	UInputAction * AimDownSightsInputAction;

	// 
	FVector2D MovementStickDeadzone;
	FVector2D ViewStickDeadzone;
	FVector2D AdsStickDeadzone;


// 
protected:

	// 
	UControllerDeadzoneAndInversion();

	// 
	void CacheDeadzoneValues(IConsoleVariable*);

	// 
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
