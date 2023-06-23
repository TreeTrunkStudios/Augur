// TreeTrunkStudios (c) 2023


// 
#include "MouseSettingsModifier.h"


// 
FInputActionValue UMouseSettingsModifier::ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) {

	//// Handle the inversion of the mouse input
	// Locally check and store whether each axis is negative or not
	bool IsNegativeX = FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().X);
	bool IsNegativeY = FMath::IsNegativeOrNegativeZero(CurrentValue.Get<FVector>().Y);

	// Locally invert the x and y axes respectively based on the given mouse inversion console variable (setting)
	const uint32 LocalMouseInversion = CVarMouseInversion.GetValueOnGameThread();
	IsNegativeX ^= ((LocalMouseInversion == 1) || (LocalMouseInversion == 3));
	IsNegativeY ^= ((LocalMouseInversion == 2) || (LocalMouseInversion == 3));

	// Finally, multiply the absolute value of the given mouse input by the sensitivity settings which hold the negatives of each axis as well
	return FInputActionValue(CurrentValue.Get<FVector2D>().GetAbs() *
		FVector2D(IsNegativeX ? -CVarMouseSensitivityX.GetValueOnGameThread() : CVarMouseSensitivityX.GetValueOnGameThread(),
			IsNegativeY ? -CVarMouseSensitivityY.GetValueOnGameThread() : CVarMouseSensitivityY.GetValueOnGameThread()));
}
