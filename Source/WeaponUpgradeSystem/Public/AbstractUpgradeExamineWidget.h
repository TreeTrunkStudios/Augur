// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractUpgradeSpecWidget.h"
#include "Blueprint/UserWidget.h"
#include "AbstractUpgradeExamineWidget.generated.h"


// 
UCLASS()
class WEAPONUPGRADESYSTEM_API UAbstractUpgradeExamineWidget : public UUserWidget {
	GENERATED_BODY()

//
protected:

	// Soft object pointer (meaning not loaded immediately) containing the database to all supported upgradable items by FName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault, RequiredAssetDataTags="RowStructure=/Script/WeaponUpgradeSystem.WeaponUpgradeData"))
	TSoftObjectPtr<UDataTable> UpgradeItemDataTable;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget * SpecificationInfoContainer;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> EmptySlotTexture;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> FullSlotTexture;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAbstractUpgradeSpecWidget> UpgradeSpecWidgetClass = UAbstractUpgradeSpecWidget::StaticClass();


//
public:

	//
	UFUNCTION(BlueprintCallable)
	bool Setup(const FName & ItemName, const int64 & UpgradeBitmask, const int32 & CurrentStackCount);


//
protected:

	// Simple function which collects every row's name from a given data table and converts them into an array of strings
#if WITH_EDITORONLY_DATA
	UFUNCTION()
	TArray<FString> GetValidInventoryItemNames() const;
#endif
};
