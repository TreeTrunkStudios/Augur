// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "WeaponUpgradeTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include <Components/Image.h>
#include "Components/UniformGridPanel.h"
#include "AbstractUpgradeSpecWidget.generated.h"


// 
UCLASS()
class WEAPONUPGRADESYSTEM_API UAbstractUpgradeSpecWidget : public UUserWidget {
	GENERATED_BODY()

//
protected:

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * SpecNameText;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * SpecLevelText;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage * SpecImage;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel * SpecVisualContainer;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * SpecValueText;


//
public:

	//
	UFUNCTION(BlueprintCallable)
	void Setup(const EUpgradeTypes & CurrentUpgradeType, const int64 & CurrentBitmaskValue, const FPerUpgradeData & CurrentUpgradeData, const TSoftObjectPtr<UTexture2D> & EmptySlotTexture, const TSoftObjectPtr<UTexture2D> & FullSlotTexture);
};
