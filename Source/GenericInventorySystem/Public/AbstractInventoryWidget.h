// TreeTrunkStudios (c) 2023


// 
#pragma once


//
#include "CoreMinimal.h"
#include <Components/TextBlock.h>
#include "AbstractInventoryItemWidget.h"
#include "AbstractItemUseSubMenuWidget.h"
#include "Blueprint/UserWidget.h"
#include "InventoryDataTypes.h"
#include "Components/GridPanel.h"
#include "Components/Button.h"
#include "BasicInventoryComponent.h"
#include "AbstractInventoryWidget.generated.h"


// 
UCLASS(Abstract)
class GENERICINVENTORYSYSTEM_API UAbstractInventoryWidget : public UUserWidget {
	GENERATED_BODY()

// Section containing all protected UPROPERTY variables
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UGridPanel * InventoryGridSystem;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * ItemNameText;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * ItemDescriptionText;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * SelectButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * MoveButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * UnselectButton;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UAbstractItemUseSubMenuWidget * ItemUseSubMenuWidget;

	
	// 
	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation * RevealWindow;


	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	TSubclassOf<UAbstractInventoryItemWidget> PerInventoryItemClass = UAbstractInventoryItemWidget::StaticClass();

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FName> InvalidToFocusWidgetTypes;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> ForceCloseInterfaceInputAction;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> UnselectInputAction;


	//
	UPROPERTY(BlueprintReadOnly)
	UBasicInventoryComponent * CurrentInventoryComponent;

	//
	UPROPERTY(BlueprintReadOnly)
	UAbstractInventoryItemWidget * CurrentTempItemReference;

	//
	UPROPERTY(BlueprintReadOnly)
	UAbstractInventoryItemWidget * CurrentItemWidget;


// Section for containing protected C++ variables
protected:

	//
	FTempInventoryItem TempDataContainer;

	//
	TSharedPtr<SWidget> CachedFocusedWidget;


// Section for containing public C++ variables
public:

	//
	FToggleUserInterfaceDelegate CloseUserInterfaceDelegate;
	

// Section for containing public C++ functions
public:
	
	//
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;


// Section for containing public UFUNCTION functions
public:

	//
	UFUNCTION(BlueprintCallable)
	void ForceCloseUserInterface();

	//
	UFUNCTION(BlueprintCallable)
	void SetFocusWidget();

	//
	TSharedPtr<SWidget> GetFocusWidget();

	//
	UFUNCTION(BlueprintCallable)
	void Setup(UBasicInventoryComponent * GivenInventoryComponent);

	//
	UFUNCTION(BlueprintCallable)
	void SetupWithNewItem(UBasicInventoryComponent * GivenInventoryComponent, const FTempInventoryItem GivenNewItem);

	//
	UFUNCTION(BlueprintCallable)
	void MakeVisible();

	//
	UFUNCTION(BlueprintCallable)
	void MakeInvisible(bool IsInternal = false);

	//
	UFUNCTION(BlueprintCallable)
	void UpdateItemInformation(UAbstractInventoryItemWidget * GivenFocusedWidget, const FText & ItemName, const FText & ItemDescription);

	//
	UFUNCTION(BlueprintCallable)
	bool DoesContainItem(const FName & GivenItemName);

	//
	UFUNCTION(BlueprintCallable)
	UAbstractInventoryItemWidget * FindLastItem(const FName & GivenItemName);

	//
	UFUNCTION(BlueprintCallable)
	void AddItem(FBasicInventoryItem & GivenItemType, const int32 ItemIndex, const bool ShouldCheckForEmptyWidgets = true);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * MoveItem(UAbstractInventoryItemWidget * Item);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * RemoveItem(UAbstractInventoryItemWidget * Item);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * CombineItem(UAbstractInventoryItemWidget * Item);

	//
	UFUNCTION(BlueprintCallable)
	void SwapItem();
	
	//
	UFUNCTION(BlueprintCallable)
	void MakeTempItem(const FTempInventoryItem GivenNewTempItem);

	//
	UFUNCTION(BlueprintCallable)
	void ReplaceTempItem();


	//
	UFUNCTION(BlueprintCallable)
	void SelectItem();
	
	//
	UFUNCTION(BlueprintCallable)
	void ExitInventory();


	//
	UFUNCTION(BlueprintCallable)
	void RotateTempItem();

	//
	UFUNCTION(BlueprintImplementableEvent)
	void HandleShowAllText();

	//
	UFUNCTION(BlueprintImplementableEvent)
	void HandleHideAllText();

	//
	void ActivateSubMenu(UPanelWidget * GivenAttachment, UAbstractInventoryItemWidget * GivenInventoryItem);


//
protected:

	//
	void SetWidgetFocus(const TSharedPtr<SWidget> & GivenWidget);
	

//
protected:

	//
	friend UAbstractInventoryItemWidget;
};
