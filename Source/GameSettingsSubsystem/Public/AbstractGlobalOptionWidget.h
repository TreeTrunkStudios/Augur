// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractIntegerOptionWidget.h"
#include "AbstractSettingsOptionWidget.h"
#include "AbstractGlobalOptionWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UAbstractGlobalOptionWidget : public UAbstractIntegerOptionWidget {
	GENERATED_BODY()

// 
protected:

	// 
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<UAbstractSettingsOptionWidget*> SettingsToAdjustWhenChanged;

	//
	bool DidStateChange = false;


//
protected:

	//
	bool Initialize() override;


//
protected:
	
	//
	void HandleStateChange(FText SelectedItem, ESelectInfo::Type SelectionType) override;

	//
	void HandleCheckForCustomState(IConsoleVariable * GivenConsoleVariable);


//
public:
	
	//
	virtual void UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) override;	
};
