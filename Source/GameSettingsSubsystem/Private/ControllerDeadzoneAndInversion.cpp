// Sidney Towers
// TreeTrunkStudios (c) 2023


// Include section
#include "ControllerDeadzoneAndInversion.h"
#include "EnhancedPlayerInput.h"


// Calculate whether the sticks should be inverted (if so, negate their current IsNegative statuses)
void CalculateInversions(bool & IsNegativeX, bool & IsNegativeY, const int32 LocalMovementStickInversion
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	, const FInputActionValue & CurrentValue
#endif
) {

	// XOR the negative handling booleans based on the movement stick's wanted inversion
	//const int32 LocalMovementStickInversion = CVarMovementStickInversion.GetValueOnGameThread();
	IsNegativeX ^= ((LocalMovementStickInversion == 1) || (LocalMovementStickInversion == 3));
	IsNegativeY ^= ((LocalMovementStickInversion == 2) || (LocalMovementStickInversion == 3));

#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	// Ensure that, if there is an inversion requested, that it functioned properly (only on debug builds)
	ensureAlways(((LocalMovementStickInversion == 1) || (LocalMovementStickInversion == 3)) ?
		IsNegativeX != FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X) :
		IsNegativeX == FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X));
	ensureAlways(((LocalMovementStickInversion == 2) || (LocalMovementStickInversion == 3)) ?
		IsNegativeY != FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y) :
		IsNegativeY == FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y));
#endif
}


// Handle "no-diagonals" masking of the joystick
void CalculateNoDiagonal(FVector2D & TargetInput, float NoDiagonalStartX, float NoDiagonalStartY, const float NoDiagonalEndX, const float NoDiagonalEndY) {

	// Calculate if the x axis is currently contained within a "no-diagonals" zone and, if so, then we set x to be 0.0
	NoDiagonalStartX = (((NoDiagonalEndX - NoDiagonalStartX) * TargetInput.Y) + NoDiagonalStartX);
	TargetInput.X = ((TargetInput.X <= NoDiagonalStartX) ? 0.0 : FMath::Clamp(1.0 - ((TargetInput.X - 1.0) / (NoDiagonalStartX - 1.0)), 0.0, 1.0));

	// Calculate if the y axis is currently contained within a "no-diagonals" zone and, if so, then we set y to be 0.0
	NoDiagonalStartY = (((NoDiagonalEndY - NoDiagonalStartY) * TargetInput.X) + NoDiagonalStartY);
	TargetInput.Y = ((TargetInput.Y <= NoDiagonalStartY) ? 0.0 : FMath::Clamp(1.0 - ((TargetInput.Y - 1.0) / (NoDiagonalStartY - 1.0)), 0.0, 1.0));
}


// Calculate deadzones, which simply ensures that the joystick ignores a certain percentage of its inputs,
//     then scales the rest so that the user still has a 0.0 .. 1.0 input range
void CalculateDeadzone(FVector2D & TargetInput, const FVector2D & TargetDeadzone, const FVector2D & InverseSquaredDeadzone, const int32 DeadzoneType, const float DeadzoneX, const float DeadzoneY) {

	// 
	double LocalDouble = 0.0;

	// Change functionality based on if we wish to invert any axes on this joystick or not
	switch (DeadzoneType) {

		// If we want an ellipse deadzone, then...
		case 0:

			// Calculate where the target input is in relation to the deadzone ellipse
			LocalDouble = FMath::InvSqrt((TargetInput.X * TargetInput.X * InverseSquaredDeadzone.X) +
				(TargetInput.Y * TargetInput.Y * InverseSquaredDeadzone.Y));

			// Scale the target input by the inverse ellipse overhang and get its size, so that we now have the scaled magnitude
			LocalDouble = ((LocalDouble >= 1.0) ? TargetInput.Size() : (TargetInput * LocalDouble).Size());

			// Finally, calculate the new scaled input and apply it to the given target (ensuring that it stays within the range of 0.0 .. 1.0)
			TargetInput = (TargetInput.GetSafeNormal() * FMath::Clamp(((TargetInput.Size() - LocalDouble) / (1.0 - LocalDouble)), 0.0, 1.0));

			// Exit, as we are now done applying an ellipse deadzone
			break;

		// If we want a square deadzone, then...
		case 1:

			// Calculate each axis independently in order to produce a square deadzone for the current joystick
			TargetInput.X = FMath::Clamp((TargetInput.X - DeadzoneX) * TargetDeadzone.X, 0.0, 1.0);
			TargetInput.Y = FMath::Clamp((TargetInput.Y - DeadzoneY) * TargetDeadzone.Y, 0.0, 1.0);

			// Exit, as we are now done applying a square deadzone
			break;

		// If we want a rounded square deadzone, then...
		case 2:
			break;

		// If we want a cross deadzone, then...
		case 3:

			// If the values are less than their target amount, then simply set them to zero and forget about them, ergo you have a cross deadzone
			TargetInput.X = ((TargetInput.X <= DeadzoneX) ? 0.0 : FMath::Clamp((TargetInput.X - DeadzoneX) * TargetDeadzone.X, 0.0, 1.0));
			TargetInput.Y = ((TargetInput.Y <= DeadzoneY) ? 0.0 : FMath::Clamp((TargetInput.Y - DeadzoneY) * TargetDeadzone.Y, 0.0, 1.0));

			// Exit, as we are now done applying a cross deadzone
			break;

		// Else, we default to no deadzone, so exit
		default:
			break;
	}
}


// Constructor for this class
UControllerDeadzoneAndInversion::UControllerDeadzoneAndInversion() {

	// Collect the wanted aiming down sights input action and locally cache it now
	AimDownSightsInputAction = ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/InputActionsCharacter/AimDownSights.AimDownSights'")).Object;

	// Call the function to cache the wanted deadzone values on initialization
	CacheDeadzoneValues(nullptr);

	// Attach the deadzone caching functionality to each and every relevant console variable
	CVarMovementStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarMovementStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarViewStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarViewStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarAdsStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarAdsStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
}


// Function which handles caching the wanted deadzone values whenever an associated console variable changes in value
void UControllerDeadzoneAndInversion::CacheDeadzoneValues(IConsoleVariable*) {
	MovementStickDeadzone = FVector2D((1.0 / (1.0 - CVarMovementStickDeadzoneX.GetValueOnGameThread())), (1.0 / (1.0 - CVarMovementStickDeadzoneY.GetValueOnGameThread())));
	ViewStickDeadzone = FVector2D((1.0 / (1.0 - CVarViewStickDeadzoneX.GetValueOnGameThread())), (1.0 / (1.0 - CVarViewStickDeadzoneY.GetValueOnGameThread())));
	AdsStickDeadzone = FVector2D((1.0 / (1.0 - CVarAdsStickDeadzoneX.GetValueOnGameThread())), (1.0 / (1.0 - CVarAdsStickDeadzoneY.GetValueOnGameThread())));

	InverseSquaredMovementDeadzone = FVector2D(CVarMovementStickDeadzoneX.GetValueOnGameThread(), CVarMovementStickDeadzoneY.GetValueOnGameThread());
	InverseSquaredMovementDeadzone = (FVector2D(1.0, 1.0) / (InverseSquaredMovementDeadzone * InverseSquaredMovementDeadzone));

	InverseSquaredViewDeadzone = FVector2D(CVarViewStickDeadzoneX.GetValueOnGameThread(), CVarViewStickDeadzoneY.GetValueOnGameThread());
	InverseSquaredViewDeadzone = (FVector2D(1.0, 1.0) / (InverseSquaredViewDeadzone * InverseSquaredViewDeadzone));

	InverseSquaredAdsDeadzone = FVector2D(CVarAdsStickDeadzoneX.GetValueOnGameThread(), CVarAdsStickDeadzoneY.GetValueOnGameThread());
	InverseSquaredAdsDeadzone = (FVector2D(1.0, 1.0) / (InverseSquaredAdsDeadzone * InverseSquaredAdsDeadzone));
}


// The function which handles modifying the inputs to handle deadzone, no-diagonal zones, and sensitivity
FInputActionValue UControllerDeadzoneAndInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) {

	// Always ensure that the player input is valid, else everything should definitely break
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
	ensureAlways(IsValid(PlayerInput));
#endif

	// Locally store whether each axis is negative or not (treats the FVector2D as an FVector for better performance)
	bool IsNegativeX = FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X);
	bool IsNegativeY = FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y);

	// Locally store the current values into their absolute values for more simplistic future mathematical handling
	FVector2D LocalCurrentValue = CurrentValue.Get<FVector2D>().GetAbs();

	// Create a local boolean to keep track of whether the current input is from the movement stick or the view stick
	bool IsThisMovementStick = (CVarControllerState.GetValueOnGameThread() == 2 ? (!IsLeftStick) : IsLeftStick);

	// If we are a movement stick, then...
	if (IsThisMovementStick) {

		//// Handle joystick inversion now, as it is simply just inverting the values of two booleans, potentially
		CalculateInversions(IsNegativeX, IsNegativeY, CVarMovementStickInversion.GetValueOnGameThread()
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
			, CurrentValue
#endif
		);
		

		//// Handle "no-diagonals" masking of the joystick now, as it should be handled prior to deadzones in order to properly handle them
		CalculateNoDiagonal(LocalCurrentValue, CVarMovementStickNoDiagonalStartX.GetValueOnGameThread(), CVarMovementStickNoDiagonalStartY.GetValueOnGameThread(),
			CVarMovementStickNoDiagonalEndX.GetValueOnGameThread(), CVarMovementStickNoDiagonalEndY.GetValueOnGameThread());


		//// Next, handle deadzones, as they simply ensure that the joystick ignores a certain percentage of its inputs,
		////     then scales the rest so that the user still has a 0.0 .. 1.0 input range
		CalculateDeadzone(LocalCurrentValue, MovementStickDeadzone, InverseSquaredMovementDeadzone, CVarMovementStickDeadzoneType.GetValueOnGameThread(),
			CVarMovementStickDeadzoneX.GetValueOnGameThread(), CVarMovementStickDeadzoneY.GetValueOnGameThread());


		//// Finally, we want to ensure that the final result always has a maximum magnitude of 1.0, and we want to ensure that negatives are reintroduced if need be
		// Normalize the current value to ensure that it never breaks a magnitude of 1.0
		if (LocalCurrentValue.SizeSquared() > 1.0) LocalCurrentValue.Normalize();
	}

	// Else, we are NOT a movement stick and also need to check for ADS special controls, then...
	else {

		// Check if we have different settings for when ADS and, if so, then...
		if (CVarAdsHasDifferentControls.GetValueOnGameThread() && PlayerInput->GetActionValue(AimDownSightsInputAction).Get<bool>()) {

			//// Handle joystick inversion now, as it is simply just inverting the values of two booleans, potentially
			CalculateInversions(IsNegativeX, IsNegativeY, CVarAdsStickInversion.GetValueOnGameThread()
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
				, CurrentValue
#endif
			);


			//// Handle "no-diagonals" masking of the joystick now, as it should be handled prior to deadzones in order to properly handle them
			CalculateNoDiagonal(LocalCurrentValue, CVarAdsStickNoDiagonalStartX.GetValueOnGameThread(), CVarAdsStickNoDiagonalStartY.GetValueOnGameThread(),
				CVarAdsStickNoDiagonalEndX.GetValueOnGameThread(), CVarAdsStickNoDiagonalEndY.GetValueOnGameThread());


			//// Next, handle deadzones, as they simply ensure that the joystick ignores a certain percentage of its inputs,
			////     then scales the rest so that the user still has a 0.0 .. 1.0 input range
			CalculateDeadzone(LocalCurrentValue, AdsStickDeadzone, InverseSquaredAdsDeadzone, CVarAdsStickDeadzoneType.GetValueOnGameThread(),
				CVarAdsStickDeadzoneX.GetValueOnGameThread(), CVarAdsStickDeadzoneY.GetValueOnGameThread());


			//// Lastly, because this is an aiming down sights stick, we want to apply a sensitivity multiplier to allow the user to adjust how much a full stick rotates by
			LocalCurrentValue.X *= CVarAdsStickSensitivityX.GetValueOnGameThread();
			LocalCurrentValue.Y *= CVarAdsStickSensitivityY.GetValueOnGameThread();
		}

		// Else, we utilize the same view stick settings and that's all
		else {

			//// Handle joystick inversion now, as it is simply just inverting the values of two booleans, potentially
			CalculateInversions(IsNegativeX, IsNegativeY, CVarViewStickInversion.GetValueOnGameThread()
#if UE_BUILD_DEBUG || UE_BUILD_DEVELOPMENT
				, CurrentValue
#endif
			);


			//// Handle "no-diagonals" masking of the joystick now, as it should be handled prior to deadzones in order to properly handle them
			CalculateNoDiagonal(LocalCurrentValue, CVarViewStickNoDiagonalStartX.GetValueOnGameThread(), CVarViewStickNoDiagonalStartY.GetValueOnGameThread(),
				CVarViewStickNoDiagonalEndX.GetValueOnGameThread(), CVarViewStickNoDiagonalEndY.GetValueOnGameThread());


			//// Next, handle deadzones, as they simply ensure that the joystick ignores a certain percentage of its inputs,
			////     then scales the rest so that the user still has a 0.0 .. 1.0 input range
			CalculateDeadzone(LocalCurrentValue, ViewStickDeadzone, InverseSquaredViewDeadzone, CVarViewStickDeadzoneType.GetValueOnGameThread(),
				CVarViewStickDeadzoneX.GetValueOnGameThread(), CVarViewStickDeadzoneY.GetValueOnGameThread());

			
			//// Lastly, because this is a view stick, we want to apply a sensitivity multiplier to allow the user to adjust how much a full stick rotates by
			LocalCurrentValue.X *= CVarViewStickSensitivityX.GetValueOnGameThread();
			LocalCurrentValue.Y *= CVarViewStickSensitivityY.GetValueOnGameThread();
		}
	}

	// Ensure that, if we want a negative value, then we convert it to be such prior to use
	LocalCurrentValue.X = (IsNegativeX ? -LocalCurrentValue.X : LocalCurrentValue.X);
	LocalCurrentValue.Y = (IsNegativeY ? -LocalCurrentValue.Y : LocalCurrentValue.Y);

	// Return the final CurrentValue that contains the targetted new value
	return FInputActionValue{LocalCurrentValue};
}
