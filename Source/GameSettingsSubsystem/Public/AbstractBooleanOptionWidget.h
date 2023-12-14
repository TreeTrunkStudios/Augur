// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractSettingsOptionWidget.h"
#include "Components/CheckBox.h"
#include "AbstractBooleanOptionWidget.generated.h"


// 
UCLASS(Abstract, Blueprintable, BlueprintType)
class GAMESETTINGSSUBSYSTEM_API UAbstractBooleanSettingsOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UCheckBox * CheckBox;


//
protected:

	//
	bool Initialize() override;


//
protected:
	
	//
	UFUNCTION()
	void HandleStateChange(bool NewState);
};
