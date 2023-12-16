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
class GAMESETTINGSSUBSYSTEM_API UAbstractBooleanOptionWidget : public UAbstractSettingsOptionWidget {
	GENERATED_BODY()
	
// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCheckBox * CheckBox;

	// 
	UPROPERTY(EditInstanceOnly, meta=(ArraySizeEnum="ESettingsLevel"))
	uint8 SettingsLevelValues[static_cast<uint8>(ESettingsLevel::Num)];


//
protected:

	//
	bool Initialize() override;


//
protected:
	
	//
	UFUNCTION()
	void HandleStateChange(bool NewState);


//
public:	

	//
	virtual void UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) override;
};
