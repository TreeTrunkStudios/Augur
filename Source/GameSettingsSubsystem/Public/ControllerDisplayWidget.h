// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InputAction.h"
#include "ControllerDisplayWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UControllerDisplayWidget : public UUserWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic * LeftStickMaterial;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UMaterialInstanceDynamic * RightStickMaterial;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText LeftStickText;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText RightStickText;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FText AimDownSightsText;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool DoesAimingDownSightsMatter;

	// 
	UPROPERTY()
	UMaterial * DefaultStickMaterial;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * RawMovementInputAction;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * RawLookingInputAction;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * MovementInputAction;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * LookingInputAction;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * AimDownSightsInputAction;

	// 
	UControllerDisplayWidget(const FObjectInitializer & ObjectInitializer);

	//
	virtual bool Initialize() override;

	// 
	virtual void NativeConstruct() override;

	// 
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;
};
