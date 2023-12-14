// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "AbstractSettingsOptionWidget.generated.h"


// 
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, meta=(DontUseGenericSpawnObject="True", DisableNativeTick))
class GAMESETTINGSSUBSYSTEM_API UAbstractSettingsOptionWidget : public UUserWidget {
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
	FName ConsoleCommand;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName SectionName;


//
protected:

	//
	IConsoleVariable * SettingsOptionConsoleVariable;


// Protected section for handling all C++ functions
protected:

	// 
	virtual bool Initialize() override;

	//
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;
};
