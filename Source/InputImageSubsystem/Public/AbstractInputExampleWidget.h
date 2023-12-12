// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include <InputMappingContext.h>
#include "CoreMinimal.h"
#include "InputAction.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include <Components/UniformGridPanel.h>
#include "InputImageSubsystem.h"
#include "AbstractInputExampleWidget.generated.h"


// 
UCLASS(Abstract)
class INPUTIMAGESUBSYSTEM_API UAbstractInputExampleWidget : public UUserWidget {
	GENERATED_BODY()
	
//
protected:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext * OwningInputMapping = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * OwningInputAction = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxNumberOfShownIcons = 1;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel * InputExampleGrid;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * InputExampleText;


//
public:

	//
	virtual bool Initialize() override;

	//
	virtual void BeginDestroy() override;


//
protected:

	// 
	UFUNCTION(BlueprintCallable)
	void UpdateWithNewInputDeviceType(UInputImageSubsystem * GivenInputImageSubsystem) const;

	//
	virtual void RenderWantedInputs(const UInputImageSubsystem * CurrentInputImageSubsystem) const;
};
