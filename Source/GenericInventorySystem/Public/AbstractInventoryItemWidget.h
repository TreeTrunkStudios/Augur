// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InventoryDataTypes.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "Components/UniformGridPanel.h"
#include <Components/Image.h>
#include <Components/TextBlock.h>
#include "AbstractInventoryItemWidget.generated.h"


//
static TAutoConsoleVariable<FString> CVarEmptyItemColorHex{
	TEXT("UI.Inventory.EmptyItemColor"),
	FString(TEXT("#FF0000")),
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
};


//
static TAutoConsoleVariable<FString> CVarFullItemColorHex{
	TEXT("UI.Inventory.FullItemColor"),
	FString(TEXT("#FFFFFF")),
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
};


//
class UBasicInventoryComponent;
class UAbstractInventoryWidget;
class UAbstractItemUseSubMenuWidget;


// 
UCLASS(Abstract)
class GENERICINVENTORYSYSTEM_API UAbstractInventoryItemWidget : public UUserWidget {
	GENERATED_BODY()

//
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * ItemButton;
	
	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UUniformGridPanel * BackgroundGrid;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UImage * ItemIcon;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UTextBlock * ItemStackCountText;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget * SubMenuAttachment;
	
	//
	UPROPERTY(BlueprintReadOnly)
	TArray<UWidget*> HighlightWidgets;


	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> TargetBorderMaterial;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> TargetHighlightMaterial;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EmptyItemName = FText::GetEmpty();

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText EmptyItemDescription = FText::GetEmpty();
	

	//
	FBasicInventoryItem * ItemData;

	//
	TWeakObjectPtr<UAbstractInventoryWidget> ParentInventoryWidget;

	//
	FLinearColor FullColor;
	FLinearColor EmptyColor;

	//
	FDelegateHandle NewFullColorHandle;
	FDelegateHandle NewEmptyColorHandle;
	
	//
	int32 CurrentStackCache;

	//
	uint8 ItemPosition;
	

//
public:

	//
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int32 GetItemIndex() const;

	//
	UFUNCTION(BlueprintCallable)
	FBasicInventoryItem GetInventoryItemData() const;

	//
	UFUNCTION(BlueprintCallable)
	FInternalInventoryItemData GetItemInstanceData() const;

	//
	UFUNCTION(BlueprintCallable)
	void Setup(UAbstractInventoryWidget * GivenParentInventoryWidget, FBasicInventoryItem & GivenInventoryItemType, const int32 GivenItemIndex);

	//
	UFUNCTION(BlueprintCallable)
	void SetupEmpty(UAbstractInventoryWidget * GivenParentInventoryWidget, const uint8 GivenPosition);

	//
	UFUNCTION(BlueprintCallable)
	void SelectItem();

	//
	UFUNCTION(BlueprintCallable)
	UWidget * SelectItemController(EUINavigation GivenNavigationType);

	//
	UFUNCTION(BlueprintCallable)
	void UnselectItem();

	//
	UFUNCTION(BlueprintCallable)
	UWidget * UnselectItemController(EUINavigation GivenNavigationType);

	//
	UFUNCTION(BlueprintCallable)
	void UpdateStackCount();

	//
	UFUNCTION(BlueprintCallable)
	void RemoveIfEmpty(const TSet<uint8> & GivenRemovingPositions);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * RemoveItem(const int32 GivenCount);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * InteractItem();

	//
	UFUNCTION(BlueprintCallable)
	UWidget * MoveItem();

	//
	UFUNCTION(BlueprintCallable)
	UWidget * CombineItem();

	//
	UFUNCTION(BlueprintCallable)
	UWidget * DiscardItem();


//
protected:

	//
	virtual void NativeDestruct() override;

	//
	void HandleChangedColorHex(IConsoleVariable * PointerToChangedConsoleVariable, bool IsEmptyColor);

	//
	virtual void NativeOnAddedToFocusPath(const FFocusEvent& InFocusEvent) override;
	virtual void NativeOnRemovedFromFocusPath(const FFocusEvent& InFocusEvent) override;
	
	// 
	virtual FReply NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent) override;

	// 
	virtual void NativeOnFocusLost(const FFocusEvent & InFocusEvent) override;


//
protected:

	//
	friend UAbstractInventoryWidget;
	friend UAbstractItemUseSubMenuWidget;
	friend UBasicInventoryComponent;
};
