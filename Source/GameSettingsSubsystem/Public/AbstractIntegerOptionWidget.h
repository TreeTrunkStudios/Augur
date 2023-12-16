// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsOptionWidget.h"
#include "ComboBoxText.h"
#include <Components/Button.h>
#include <Components/UniformGridPanel.h>
#include "AbstractIntegerOptionWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractIntegerOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UComboBoxText * ComboBox;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel * StateUniformGrid;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * IncrementButton;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * DecrementButton;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SelectedIndexTexture;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> UnselectedIndexTexture;

	// 
	UPROPERTY(EditInstanceOnly, meta=(ArraySizeEnum="ESettingsLevel"))
	int32 SettingsLevelValues[static_cast<uint8>(ESettingsLevel::Custom)];


//
protected:

	//
	bool Initialize() override;

	//
	UWidget * HandleIncrement(EUINavigation GivenNavigation);

	//
	UWidget * HandleDecrement(EUINavigation GivenNavigation);


//
protected:

	//
	UFUNCTION()
	void HandleIncrementButton();

	//
	UFUNCTION()
	void HandleDecrementButton();
	
	//
	UFUNCTION()
	virtual void HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType);


//
public:	

	//
	virtual void UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) override;
};
