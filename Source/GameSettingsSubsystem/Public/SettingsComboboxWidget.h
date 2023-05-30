// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractIntegerSettingsWidget.h"
#include "SettingsComboboxWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API USettingsComboboxWidget : public UAbstractIntegerSettingsWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	virtual void InitializeSetting_Implementation(const FSettingsOptionsStruct & SettingsOptions) override;
};
