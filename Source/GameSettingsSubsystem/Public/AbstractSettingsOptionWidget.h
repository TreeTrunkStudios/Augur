// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/TextBlock.h>
#include "AbstractSettingsOptionWidget.generated.h"


//
UENUM(BlueprintType, Blueprintable)
enum class ESettingsLevel : uint8 {
	Low,
	Medium,
	High,
	Ultra,
	Perfection,
	Custom,
	Num UMETA(Hidden)
};


// 
UCLASS(Abstract, EditInlineNew, DefaultToInstanced, meta=(DontUseGenericSpawnObject="True", DisableNativeTick))
class GAMESETTINGSSUBSYSTEM_API UAbstractSettingsOptionWidget : public UUserWidget {
	GENERATED_BODY()
	
// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * NameText;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	UTextBlock * DescriptionTextWidget;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FText VariableNameText;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FText DescriptionText;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName ConsoleCommand;

	//
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FName SectionName;


//
protected:

	//
	IConsoleVariable * SettingsOptionConsoleVariable;


//
public:

	//
	FORCEINLINE IConsoleVariable * GetSettingsOptionConsoleVariable() const {
		return SettingsOptionConsoleVariable;
	}


// Protected section for handling all C++ functions
protected:

	// 
	virtual bool Initialize() override;

	//
	virtual void NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) override;


//
public:

	//
	virtual void UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {};
};
