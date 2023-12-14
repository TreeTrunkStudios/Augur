// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Components/SpinBox.h"
#include "Components/ProgressBar.h"
#include "AbstractSettingsOptionWidget.h"
#include "AbstractFloatOptionWidget.generated.h"


//
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UAbstractFloatOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	USpinBox * SpinBox;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UProgressBar * ProgressBar;


//
protected:

	//
	bool Initialize() override;


//
protected:

	//
	UFUNCTION()
	void HandleStateUpdate(float NewState);
	
	//
	UFUNCTION()
	void HandleStateChange(float NewState, ETextCommit::Type CommitType);	
};
