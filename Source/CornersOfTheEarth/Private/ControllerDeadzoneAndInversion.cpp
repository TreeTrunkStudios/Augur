// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#include "ControllerDeadzoneAndInversion.h"
#include "EnhancedPlayerInput.h"


// 
UControllerDeadzoneAndInversion::UControllerDeadzoneAndInversion() {

	// 
	AimDownSightsInputAction = ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/InputActionsCharacter/AimDownSights.AimDownSights'")).Object;

	// 
	CacheDeadzoneValues(nullptr);

	// 
	CVarMovementStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarMovementStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarViewStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarViewStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarAdsStickDeadzoneX.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
	CVarAdsStickDeadzoneY.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateUObject(this, &UControllerDeadzoneAndInversion::CacheDeadzoneValues));
}


// 
void UControllerDeadzoneAndInversion::CacheDeadzoneValues(IConsoleVariable*) {
	MovementStickDeadzone = FVector2D((1.0f / (1.0f - CVarMovementStickDeadzoneX.GetValueOnGameThread())), (1.0f / (1.0f - CVarMovementStickDeadzoneY.GetValueOnGameThread())));
	ViewStickDeadzone = FVector2D((1.0f / (1.0f - CVarViewStickDeadzoneX.GetValueOnGameThread())), (1.0f / (1.0f - CVarViewStickDeadzoneY.GetValueOnGameThread())));
	AdsStickDeadzone = FVector2D((1.0f / (1.0f - CVarAdsStickDeadzoneX.GetValueOnGameThread())), (1.0f / (1.0f - CVarAdsStickDeadzoneY.GetValueOnGameThread())));
}


// 
FInputActionValue UControllerDeadzoneAndInversion::ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) {

	// Always ensure that the player input is valid, else everything should probably break
	ensureAlways(IsValid(PlayerInput));

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
		// XOR the negative handling booleans based on the movement stick's wanted inversion
		const int32 LocalMovementStickInversion = CVarMovementStickInversion.GetValueOnGameThread();
		IsNegativeX ^= ((LocalMovementStickInversion == 1) || (LocalMovementStickInversion == 3));
		IsNegativeY ^= ((LocalMovementStickInversion == 2) || (LocalMovementStickInversion == 3));

		// Ensure that, if there is an inversion requested, that it functioned properly
		ensureAlways(((LocalMovementStickInversion == 1) || (LocalMovementStickInversion == 3)) ? IsNegativeX != FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X) : IsNegativeX == FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X));
		ensureAlways(((LocalMovementStickInversion == 2) || (LocalMovementStickInversion == 3)) ? IsNegativeY != FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y) : IsNegativeY == FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y));


		//// Handle "no-diagonals" masking of the joystick now, as it should be handled prior to deadzones in order to properly handle them
		// Calculate if the x axis is currently contained within a "no-diagonals" zone and, if so, then we set x to be 0.0f
		float LocalFloat = CVarMovementStickNoDiagonalStartX.GetValueOnGameThread();
		LocalFloat = (((CVarMovementStickNoDiagonalEndX.GetValueOnGameThread() - LocalFloat) * LocalCurrentValue.Y) + LocalFloat);
		LocalCurrentValue.X = ((LocalCurrentValue.X < LocalFloat) ? 0.0f : LocalCurrentValue.X);

		// Calculate if the y axis is currently contained within a "no-diagonals" zone and, if so, then we set y to be 0.0f
		LocalFloat = CVarMovementStickNoDiagonalStartY.GetValueOnGameThread();
		LocalFloat = (((CVarMovementStickNoDiagonalEndY.GetValueOnGameThread() - LocalFloat) * LocalCurrentValue.X) + LocalFloat);
		LocalCurrentValue.Y = ((LocalCurrentValue.Y < LocalFloat) ? 0.0f : LocalCurrentValue.Y);


		//// Next, handle deadzones, as they simply ensure that the joystick ignores a certain percentage of its inputs, then scales the rest so that the user still has a 0.0 .. 1.0 input range
		// Change functionality based on if we wish to invert any axes on this joystick or not
		switch (CVarMovementStickDeadzoneType.GetValueOnGameThread()) {

			// If we want a square deadzone, then...
			case 1:

				// Calculate each axis independently in order to produce a square deadzone for the current joystick
				LocalCurrentValue.X = FMath::Clamp((LocalCurrentValue.X - CVarMovementStickDeadzoneX.GetValueOnGameThread()) * MovementStickDeadzone.X, 0.0, 1.0);
				LocalCurrentValue.Y = FMath::Clamp((LocalCurrentValue.Y - CVarMovementStickDeadzoneY.GetValueOnGameThread()) * MovementStickDeadzone.Y, 0.0, 1.0);

				// Exit, as we are now done applying a square deadzone
				break;

			// If we want a rounded square deadzone, then...
			case 2:
				break;

			// If we want a cross deadzone, then...
			case 3:

				// If the values are less than their target amount, then simply set them to zero and forget about them, ergo you have a cross deadzone
				LocalCurrentValue.X = ((LocalCurrentValue.X <= CVarMovementStickDeadzoneX.GetValueOnGameThread()) ? 0.0 : LocalCurrentValue.X);
				LocalCurrentValue.Y = ((LocalCurrentValue.Y <= CVarMovementStickDeadzoneY.GetValueOnGameThread()) ? 0.0 : LocalCurrentValue.Y);

				// 
				break;

			// Else, we default to a circular deadzone
			default:

				//



				float LocalMagnitude = LocalCurrentValue.Size();

				// 
				float LocalDeadzoneX = CVarMovementStickDeadzoneX.GetValueOnGameThread();
				//(LocalCurrentValue.X * LocalCurrentValue.X) / (LocalDeadzoneX * LocalDeadzoneX);

				// 
				break;
		}


		//// Finally, we want to ensure that the final result always has a maximum magnitude of 1.0, and we want to ensure that negatives are reintroduced if need be
		// Normalize the current value to ensure that it never breaks a magnitude of 1.0f
		if (LocalCurrentValue.SizeSquared() > 1.0f) LocalCurrentValue.Normalize();

		// Ensure that, if we want a negative value, then we convert it to be such prior to use
		LocalCurrentValue.X = (IsNegativeX ? -LocalCurrentValue.X : LocalCurrentValue.X);
		LocalCurrentValue.Y = (IsNegativeY ? -LocalCurrentValue.Y : LocalCurrentValue.Y);
	}

	// Else, we are NOT a movement stick and also need to check for ADS special controls
	else {

		// Check if we have different settings for when ADS and, if so, then...
		if (CVarAdsHasDifferentControls.GetValueOnGameThread() && PlayerInput->GetActionValue(AimDownSightsInputAction).Get<bool>()) {

			// 
			// 
		}

		// Else, we utilize the same view stick settings and that's all
		else {

			// 
			//
		}

		// 
		// 
	}

	// Return the final CurrentValue that contains the targetted new value
	return FInputActionValue{LocalCurrentValue};
}
