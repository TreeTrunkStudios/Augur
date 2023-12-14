// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include <InputAction.h>
#include <InputMappingContext.h>
#include <Components/TextBlock.h>
#include "Blueprint/UserWidget.h"
#include "Components/InputKeySelector.h"
#include "AbstractKeyBindingOptionWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UAbstractKeyBindingOptionWidget : public UUserWidget {
	GENERATED_BODY()

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * NameText;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTextBlock * DescriptionTextWidget;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DescriptionText;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<UInputKeySelector*> ArrayOfValidInputKeySelectors;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputMappingContext> TargetInputMappingContext;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> TargetInputAction;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TSoftObjectPtr<UInputModifier>> TargetInputModifiers;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FKey> TargetInvalidKeys;


// Protected section for handling all C++ functions
protected:

	// 
	virtual bool Initialize() override;

	//
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;
};
