// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include <InputAction.h>
#include <InputMappingContext.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "Blueprint/UserWidget.h"
#include "Components/InputKeySelector.h"
#include "AbstractKeyBindingOptionWidget.generated.h"


//
USTRUCT(BlueprintType, Blueprintable)
struct FSingleKeySelectorData {
	GENERATED_BODY()

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName InputKeySelectorName;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName OptionalClearMappingButtonName;

	// 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName OptionalKeyImageName;

	// 
	FKey CurrentKey;
	TWeakObjectPtr<UInputKeySelector> InputKeySelector;
	TWeakObjectPtr<UButton> OptionalClearMappingButton;
	TWeakObjectPtr<UImage> OptionalKeyImage;

	//
	void UpdateKeyData(ULocalPlayer * OwningLocalPlayer) const;
};


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UAbstractKeyBindingOptionWidget : public UUserWidget {
	GENERATED_BODY()

// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * NameText;
	
	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * ClearAllMappingsButton;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UTextBlock * DescriptionTextWidget;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FText SettingNameText;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UInputMappingContext * TargetInputMappingContext;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UInputAction * TargetInputAction;

	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	TArray<FSingleKeySelectorData> KeySelectorDataArray;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UInputModifier*> TargetInputModifiers;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FKey> TargetEscapeKeys;


// Protected section for handling all C++ functions
protected:

	// 
	virtual bool Initialize() override;

	//
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;

	//
	UFUNCTION()
	void HandleStateChange(FInputChord SelectedKey);

	//
	UFUNCTION()
	void HandleRemoveMappingButton();

	//
	UFUNCTION()
	UWidget * HandleRemoveMapping(EUINavigation GivenNavigation);

	//
	UFUNCTION()
	void HandleRemoveAllMappingsButton();

	//
	UFUNCTION()
	UWidget * HandleRemoveAllMappings(EUINavigation GivenNavigation);
};
