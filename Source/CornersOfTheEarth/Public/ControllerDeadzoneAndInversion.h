// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InputModifiers.h"
#include "ControllerDeadzoneAndInversion.generated.h"


// 
UCLASS()
class CORNERSOFTHEEARTH_API UControllerDeadzoneAndInversion : public UInputModifier {
	GENERATED_BODY()


// 
public:

	// 
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = Settings)
	bool IsMovementStick = true;


// 
protected:

	// 
	virtual FInputActionValue ModifyRaw_Implementation(const UEnhancedPlayerInput * PlayerInput, FInputActionValue CurrentValue, float DeltaTime) override;
};
