// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/Button.h>
#include "InventoryDataTypes.h"
#include "AbstractExamineMenuWidget.h"
#include "AbstractItemUseSubMenuWidget.generated.h"


// 
class UAbstractInventoryItemWidget;


// 
UCLASS(Abstract)
class GENERICINVENTORYSYSTEM_API UAbstractItemUseSubMenuWidget : public UUserWidget {
	GENERATED_BODY()

// Section containing all protected UPROPERTY variables
protected:

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * RemoveButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UPanelWidget * ContainerBase;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * InteractButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * CombineButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * MoveButton;
	
	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * ExamineButton;

	//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton * DiscardButton;
	
	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	TSubclassOf<UAbstractExamineMenuWidget> ExamineMenuClass = UAbstractExamineMenuWidget::StaticClass();
	

	//
	TWeakObjectPtr<UAbstractInventoryItemWidget> FocusedInventoryItem;
	TWeakObjectPtr<const UPanelWidget> CachedGivenAttachment;
	TWeakObjectPtr<UAbstractExamineMenuWidget> ExamineMenuToForceClose;


// Section containing all public UFUNCTION functions
public:

	//
	virtual bool Initialize() override;

	//
	virtual void BeginDestroy() override;

	//
	void UpdateViewportPosition(FViewport * ChangedViewport, uint32 Unknown);

	// 
	void Setup(const UPanelWidget * GivenAttachment, UAbstractInventoryItemWidget * GivenInventoryItem);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * InteractItem(EUINavigation GivenNavigation);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * MoveItem(EUINavigation GivenNavigation);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * CombineItem(EUINavigation GivenNavigation);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * ExamineItem(EUINavigation GivenNavigation);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * DiscardItem(EUINavigation GivenNavigation);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * ExitMenu(EUINavigation GivenNavigation);


	//
	UFUNCTION(BlueprintCallable)
	void ForceCloseSubMenu();
	

	//
	UFUNCTION(BlueprintCallable)
	void InteractItemButton();

	//
	UFUNCTION(BlueprintCallable)
	void MoveItemButton();

	//
	UFUNCTION(BlueprintCallable)
	void CombineItemButton();

	//
	UFUNCTION(BlueprintCallable)
	void ExamineItemButton();

	//
	UFUNCTION(BlueprintCallable)
	void DiscardItemButton();

	//
	UFUNCTION(BlueprintCallable)
	void ExitMenuButton();

	//
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateInteractButtonText(const EInventoryUseType & GivenItemUseType);
};
