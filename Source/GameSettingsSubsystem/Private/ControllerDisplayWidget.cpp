// TreeTrunkStudios (c) 2023


// 
#include "ControllerDisplayWidget.h"
#include "Blueprint/WidgetTree.h"
#include "InputCoreTypes.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ControllerDeadzoneAndInversion.h"


// 
UControllerDisplayWidget::UControllerDisplayWidget(const FObjectInitializer & ObjectInitializer) : Super(ObjectInitializer) {

	// 
	DefaultStickMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Script/Engine.Material'/Game/UserInterfaces/JoystickDeadzone.JoystickDeadzone'")).Object;
	ensureAlways(DefaultStickMaterial != nullptr);
}


// 
bool UControllerDisplayWidget::Initialize() {

	// 
	ensureAlways(DefaultStickMaterial != nullptr);

	// 
	LeftStickMaterial = UMaterialInstanceDynamic::Create(DefaultStickMaterial, this);
	RightStickMaterial = UMaterialInstanceDynamic::Create(DefaultStickMaterial, this);

	// 
	ensureAlways(LeftStickMaterial != nullptr);
	ensureAlways(RightStickMaterial != nullptr);

	// 
	return Super::Initialize();
}


// 
void UControllerDisplayWidget::NativeConstruct() {

	// 
	ensureAlways(DefaultStickMaterial != nullptr);

	// 
	LeftStickMaterial = UMaterialInstanceDynamic::Create(DefaultStickMaterial, this);
	RightStickMaterial = UMaterialInstanceDynamic::Create(DefaultStickMaterial, this);

	// 
	ensureAlways(LeftStickMaterial != nullptr);
	ensureAlways(RightStickMaterial != nullptr);

	// 
	Super::NativeConstruct();
}


// 
void UControllerDisplayWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime) {

	// 
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 
	ensureAlways(LeftStickMaterial != nullptr);
	ensureAlways(RightStickMaterial != nullptr);

	// Collect and locally store a pointer to the owning player controller's enhanced player input system
	UEnhancedPlayerInput * CurrentPlayerInput = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetOwningPlayer()->GetLocalPlayer())->GetPlayerInput();
	ensureAlways(IsValid(CurrentPlayerInput));

	// If the controller state is less than or equal to one, then we treat it as left stick is movement
	if (CVarControllerState.GetValueOnAnyThread() <= 1) {

		// 
		FVector LocalScaleVector;

		// 
		switch (CVarMovementStickInversion.GetValueOnAnyThread()) {

			// 
			default:
			case 0:
				LocalScaleVector = FVector(0.5, -0.5, 0.0);
				break;

			// 
			case 1:
				LocalScaleVector = FVector(-0.5, -0.5, 0.0);
				break;

			// 
			case 2:
				LocalScaleVector = FVector(0.5, 0.5, 0.0);
				break;

			// 
			case 3:
				LocalScaleVector = FVector(-0.5, 0.5, 0.0);
				break;
		}

		// NOTE: Requires all numbers to be multiplied by 0.5, as the max circular radius is 0.5, meaning
		//     that's how we must treat everything as well sadly; thankfully, it's just a visual representation
		// Left stick as the movement stick
		LeftStickMaterial->SetVectorParameterValue(FName(TEXT("ActualJoystickPosition")),
			CurrentPlayerInput->GetActionValue(RawMovementInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		LeftStickMaterial->SetVectorParameterValue(FName(TEXT("CalculatedJoystickPosition")),
			CurrentPlayerInput->GetActionValue(MovementInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
			CVarMovementStickDeadzoneType.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
			CVarMovementStickDeadzoneX.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
			CVarMovementStickDeadzoneY.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
			CVarMovementStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
			CVarMovementStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
			CVarMovementStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
		LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
			CVarMovementStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);


		// 
		FVector2D x = (CurrentPlayerInput->GetActionValue(RawMovementInputAction).Get<FVector2D>());
		FVector2D y = (CurrentPlayerInput->GetActionValue(MovementInputAction).Get<FVector2D>());
		LeftStickText = FText::FormatOrdered(NSLOCTEXT("ControllerDisplayWidget", "LeftStickText",
			"Raw Position: {0}, {1}\nActual Position : {2}, {3}"), x.X, x.Y, y.X, y.Y);


		// 
		DoesAimingDownSightsMatter = CVarAdsHasDifferentControls.GetValueOnAnyThread();
		AimDownSightsText = FText::FormatOrdered(NSLOCTEXT("ControllerDisplayWidget", "AimDownSightsText",
			"Is Aiming Down Sights: {0}"), CurrentPlayerInput->GetActionValue(AimDownSightsInputAction).Get<bool>());


		// If we are not aiming down sights (and such a difference exists), then use the view stick settings
		if (CVarAdsHasDifferentControls.GetValueOnAnyThread() == true && CurrentPlayerInput->GetActionValue(AimDownSightsInputAction).Get<bool>() == true) {
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
				CVarAdsStickDeadzoneType.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
				CVarAdsStickDeadzoneX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
				CVarAdsStickDeadzoneY.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
				CVarAdsStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
				CVarAdsStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
				CVarAdsStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
				CVarAdsStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);

			// 
			switch (CVarAdsStickInversion.GetValueOnAnyThread()) {

				// 
				default:
				case 0:
					LocalScaleVector = FVector(0.5, -0.5, 0.0);
					break;

				// 
				case 1:
					LocalScaleVector = FVector(-0.5, -0.5, 0.0);
					break;

				// 
				case 2:
					LocalScaleVector = FVector(0.5, 0.5, 0.0);
					break;

				// 
				case 3:
					LocalScaleVector = FVector(-0.5, 0.5, 0.0);
					break;
			}
		}

		// Else, we use the aiming down sights stick settings
		else {
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
				CVarViewStickDeadzoneType.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
				CVarViewStickDeadzoneX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
				CVarViewStickDeadzoneY.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
				CVarViewStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
				CVarViewStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
				CVarViewStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
			RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
				CVarViewStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);

			// 
			switch (CVarViewStickInversion.GetValueOnAnyThread()) {

				// 
				default:
				case 0:
					LocalScaleVector = FVector(0.5, -0.5, 0.0);
					break;

				// 
				case 1:
					LocalScaleVector = FVector(-0.5, -0.5, 0.0);
					break;

				// 
				case 2:
					LocalScaleVector = FVector(0.5, 0.5, 0.0);
					break;

				// 
				case 3:
					LocalScaleVector = FVector(-0.5, 0.5, 0.0);
					break;
			}
		}

		// Right stick as the view stick
		RightStickMaterial->SetVectorParameterValue(FName(TEXT("ActualJoystickPosition")),
			CurrentPlayerInput->GetActionValue(RawLookingInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		RightStickMaterial->SetVectorParameterValue(FName(TEXT("CalculatedJoystickPosition")),
			CurrentPlayerInput->GetActionValue(LookingInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));


		// 
		x = (CurrentPlayerInput->GetActionValue(RawLookingInputAction).Get<FVector2D>());
		y = (CurrentPlayerInput->GetActionValue(LookingInputAction).Get<FVector2D>());
		RightStickText = FText::FormatOrdered(NSLOCTEXT("ControllerDisplayWidget", "RightStickText",
			"Raw Position: {0}, {1}\nActual Position : {2}, {3}"), x.X, x.Y, y.X, y.Y);
	}

	// Else, we treat the right stick as the movement stick
	else {

		// 
		FVector LocalScaleVector;

		// 
		switch (CVarMovementStickInversion.GetValueOnAnyThread()) {

			// 
			default:
			case 0:
				LocalScaleVector = FVector(0.5, -0.5, 0.0);
				break;

			// 
			case 1:
				LocalScaleVector = FVector(-0.5, -0.5, 0.0);
				break;

			// 
			case 2:
				LocalScaleVector = FVector(0.5, 0.5, 0.0);
				break;

			// 
			case 3:
				LocalScaleVector = FVector(-0.5, 0.5, 0.0);
				break;
		}

		// NOTE: Requires all numbers to be multiplied by 0.5, as the max circular radius is 0.5, meaning
		//     that's how we must treat everything as well sadly; thankfully, it's just a visual representation
		// Left stick as the movement stick
		RightStickMaterial->SetVectorParameterValue(FName(TEXT("ActualJoystickPosition")),
			CurrentPlayerInput->GetActionValue(RawMovementInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		RightStickMaterial->SetVectorParameterValue(FName(TEXT("CalculatedJoystickPosition")),
			CurrentPlayerInput->GetActionValue(MovementInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
			CVarMovementStickDeadzoneType.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
			CVarMovementStickDeadzoneX.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
			CVarMovementStickDeadzoneY.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
			CVarMovementStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
			CVarMovementStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
			CVarMovementStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
		RightStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
			CVarMovementStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);


		// If we are not aiming down sights (and such a difference exists), then use the view stick settings
		if (CVarAdsHasDifferentControls.GetValueOnAnyThread() == true && CurrentPlayerInput->GetActionValue(AimDownSightsInputAction).Get<bool>() == true) {
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
				CVarAdsStickDeadzoneType.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
				CVarAdsStickDeadzoneX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
				CVarAdsStickDeadzoneY.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
				CVarAdsStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
				CVarAdsStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
				CVarAdsStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
				CVarAdsStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);

			// 
			switch (CVarAdsStickInversion.GetValueOnAnyThread()) {

				// 
				default:
				case 0:
					LocalScaleVector = FVector(0.5, -0.5, 0.0);
					break;

				// 
				case 1:
					LocalScaleVector = FVector(-0.5, -0.5, 0.0);
					break;

				// 
				case 2:
					LocalScaleVector = FVector(0.5, 0.5, 0.0);
					break;

				// 
				case 3:
					LocalScaleVector = FVector(-0.5, 0.5, 0.0);
					break;
			}
		}

		// Else, we use the aiming down sights stick settings
		else {
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneType")),
				CVarViewStickDeadzoneType.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneX")),
				CVarViewStickDeadzoneX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("DeadzoneY")),
				CVarViewStickDeadzoneY.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartX")),
				CVarViewStickNoDiagonalStartX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalStartY")),
				CVarViewStickNoDiagonalStartY.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndX")),
				CVarViewStickNoDiagonalEndX.GetValueOnAnyThread() * 0.5);
			LeftStickMaterial->SetScalarParameterValue(FName(TEXT("NoDiagonalEndY")),
				CVarViewStickNoDiagonalEndY.GetValueOnAnyThread() * 0.5);

			// 
			switch (CVarViewStickInversion.GetValueOnAnyThread()) {

				// 
				default:
				case 0:
					LocalScaleVector = FVector(0.5, -0.5, 0.0);
					break;

				// 
				case 1:
					LocalScaleVector = FVector(-0.5, -0.5, 0.0);
					break;

				// 
				case 2:
					LocalScaleVector = FVector(0.5, 0.5, 0.0);
					break;

				// 
				case 3:
					LocalScaleVector = FVector(-0.5, 0.5, 0.0);
					break;
			}
		}

		// Right stick as the view stick
		LeftStickMaterial->SetVectorParameterValue(FName(TEXT("ActualJoystickPosition")),
			CurrentPlayerInput->GetActionValue(RawLookingInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
		LeftStickMaterial->SetVectorParameterValue(FName(TEXT("CalculatedJoystickPosition")),
			CurrentPlayerInput->GetActionValue(LookingInputAction).Get<FVector>() * LocalScaleVector + FVector(0.5, 0.5, 0.0));
	}
}
