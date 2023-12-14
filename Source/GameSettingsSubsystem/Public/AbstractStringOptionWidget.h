// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Components/EditableTextBox.h"
#include "AbstractSettingsOptionWidget.h"
#include "AbstractStringOptionWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UAbstractStringOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UEditableTextBox * EditableTextBox;


//
protected:

	//
	bool Initialize() override;


//
protected:
	
	//
	UFUNCTION()
	void HandleStateChange(const FText & NewState, ETextCommit::Type CommitMethod);
};
