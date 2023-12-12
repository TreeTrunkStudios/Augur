// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include <InputAction.h>
#include <Components/Button.h>

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Image.h>
#include "Components/PanelWidget.h"
#include "InventoryDataTypes.h"
#include "RenderInventoryItemExamineMesh.h"
#include "AbstractExamineMenuWidget.generated.h"


//
static TAutoConsoleVariable<float> CVarInventoryExamineRotationSensitivity{
	TEXT("UI.Inventory.Examine.RotationSensitivity"),
	10.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
};


//
static TAutoConsoleVariable<float> CVarInventoryExamineZoomSensitivity{
	TEXT("UI.Inventory.Examine.ZoomSensitivity"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
};


// 
UCLASS(Abstract)
class GENERICINVENTORYSYSTEM_API UAbstractExamineMenuWidget : public UUserWidget {
	GENERATED_BODY()

//
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * SelectButton;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * UnselectButton;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage * ExamineMeshImage;

	//
	UPROPERTY(BlueprintReadOnly)
	ARenderInventoryItemExamineMesh * ExamineMeshActor;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> TargetDynamicMaterialClass;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> RotationInputAction;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> ZoomInputAction;


	//
	TWeakObjectPtr<UWidget> WidgetThatCreatedThis;


//
public:

	//
	void Setup(UWidget * CreatingWidget, UStaticMesh * GivenStaticMesh, int32 OptionalOffsetY = 0);

	//
	UFUNCTION(BlueprintImplementableEvent)
	bool GiveExtraInformation(const FName ItemName, const int64 UpgradeBitmask, const int32 CurrentStackCount);

	//
	UWidget * GetWidgetToFocus() const;

	//
	void ForceClose();


//
protected:

	//
	UFUNCTION()
	void HandleRotationInput(const FInputActionValue & GivenInputActionValue);

	//
	UFUNCTION()
	void HandleZoomInput(const FInputActionValue & GivenInputActionValue);

	//
	UFUNCTION()
	UWidget * HandleSelect(EUINavigation GivenNavigation);

	//
	UFUNCTION()
	UWidget * HandleUnselect(EUINavigation GivenNavigation);

	//
	UFUNCTION()
	void HandleSelectButton();

	//
	UFUNCTION()
	void HandleUnselectButton();
};
