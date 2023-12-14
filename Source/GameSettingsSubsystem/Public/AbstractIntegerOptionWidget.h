// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsOptionWidget.h"
#include "ComboBoxText.h"
#include <Components/UniformGridPanel.h>
#include "AbstractIntegerOptionWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractIntegerSettingsOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UComboBoxText * ComboBox;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel * StateUniformGrid;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SelectedIndexTexture;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> UnselectedIndexTexture;


//
protected:

	//
	bool Initialize() override;


//
protected:
	
	//
	UFUNCTION()
	void HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType);
};
