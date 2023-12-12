// TreeTrunkStudios (c) 2023


// 
#include "AbstractInventoryItemWidget.h"
#include <Blueprint/WidgetTree.h>
#include "AbstractInventoryWidget.h"
#include "AbstractItemUseSubMenuWidget.h"
#include "Components/SizeBox.h"
#include "BasicInventoryComponent.h"


// 
int32 UAbstractInventoryItemWidget::GetItemIndex() const {

	// Ensure that this function is never called on an empty item
	ensureAlways(ItemData != nullptr);

	// Find and return the current item's index based on the item's position
	return ItemData->ItemDataArray.Find(FInternalInventoryItemData{ItemPosition});
}


// 
FBasicInventoryItem UAbstractInventoryItemWidget::GetInventoryItemData() const {

	// If we are not an empty slot, then return our valid item data
	if (ItemData != nullptr)
		return *ItemData;

	// Else, return an empty invalid item data slot, as we are empty as well
	return FBasicInventoryItem{};
}


// 
FInternalInventoryItemData UAbstractInventoryItemWidget::GetItemInstanceData() const {

	//
	if (ItemData != nullptr)
		return static_cast<FInternalInventoryItemData>(ItemData->ItemDataArray[GetItemIndex()]);

	//
	return FInternalInventoryItemData{};
}


// 
void UAbstractInventoryItemWidget::Setup(UAbstractInventoryWidget * GivenParentInventoryWidget, FBasicInventoryItem & GivenInventoryItemType, const int32 GivenItemIndex) {

	// Ensure that Setup is never called more than once for each item instance
	//checkNoReentry();

	// Immediately attach this object onto the color console variables in order to update their color values whenever possible, IFF taken into account
	NewEmptyColorHandle = CVarEmptyItemColorHex->OnChangedDelegate().AddUObject(this, &UAbstractInventoryItemWidget::HandleChangedColorHex, true);
	NewFullColorHandle = CVarFullItemColorHex->OnChangedDelegate().AddUObject(this, &UAbstractInventoryItemWidget::HandleChangedColorHex, false);

	// Collect and cache our current colors 
	EmptyColor = FLinearColor::FromSRGBColor(FColor::FromHex(CVarEmptyItemColorHex.GetValueOnGameThread()));
	FullColor = FLinearColor::FromSRGBColor(FColor::FromHex(CVarFullItemColorHex.GetValueOnGameThread()));

	//
	SetIsFocusable(true);
	SetDesiredFocusWidget(ItemButton);

	// Ensure always that the given item index is a valid sub item within the given inventory item
	ensureAlways(GivenInventoryItemType.ItemDataArray.IsValidIndex(GivenItemIndex));

	// Locally store our own copies of the inventory item's data and index
	ItemData = &GivenInventoryItemType;
	const int32 ItemIndex = GivenItemIndex;

	// Locally store our own copy of the inventory item's position (duplicate for empty items)
	ItemPosition = ItemData->ItemDataArray[ItemIndex].Position;

	// Load this item's icon (assuming it has one) and set the image to show it
	ItemIcon->SetBrushFromTexture(ItemData->ItemData.Icon.LoadSynchronous());
	ItemIcon->SetRenderTransformAngle(ItemData->ItemDataArray[ItemIndex].ItemRotation == EItemRotation::Rotate0 ? 0.0f : 90.0f);

	// TODO: Potentially get fancy by making the text color change based on a percentage of current stack count to max stack count??
	// If the current item's max stack count is greater than zero, then we can calculate a fade effect and make it visible
	if (const int32 CurrentMaxStackCount = (ItemData->ItemData.MaxStackCount <= 1 ? ItemData->ItemData.InternalMaxStackCount : ItemData->ItemData.MaxStackCount); CurrentMaxStackCount > 0) {

		//
		CurrentStackCache = ItemData->ItemDataArray[ItemIndex].CurrentStackCount;
		ItemStackCountText->SetColorAndOpacity(FSlateColor{EmptyColor + ((static_cast<float>(ItemData->ItemDataArray[GetItemIndex()].CurrentStackCount - 1) / CurrentMaxStackCount) * (FullColor - EmptyColor))});
		ItemStackCountText->SetVisibility(ESlateVisibility::Visible);

		// Set this item's stack count text as a localized text version of its current stack count
		ItemStackCountText->SetText(FText::AsNumber(CurrentStackCache));
	}

	// Else, the item is not stackable, either externally or internally, and therefore has no item stack count whatsoever
	else
		ItemStackCountText->SetVisibility(ESlateVisibility::Collapsed);

	//
	ParentInventoryWidget = GivenParentInventoryWidget;

	//
	BackgroundGrid->ClearChildren();

	//
	const uint8 HorizontalSize = (ItemData->ItemDataArray[ItemIndex].ItemRotation == EItemRotation::Rotate0 ? ItemData->ItemData.SizeX : ItemData->ItemData.SizeY);
	const uint8 VerticalSize = (ItemData->ItemDataArray[ItemIndex].ItemRotation == EItemRotation::Rotate0 ? ItemData->ItemData.SizeY : ItemData->ItemData.SizeX);

	//
	const uint8 ColumnSize = ParentInventoryWidget->CurrentInventoryComponent->GetColumnSize();
	const uint8 TotalSize = ParentInventoryWidget->CurrentInventoryComponent->GetSize();

	//
	HighlightWidgets.Empty(HorizontalSize * VerticalSize);
	
	//
	for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

		//
		for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

			//
			const bool ShouldShowLeftLine = (HorizontalOffset == 0);
			const bool ShouldShowTopLine = (VerticalOffset == 0);
			const bool IsRightMostItem = ((ItemPosition + 1) >= TotalSize || ((ItemPosition + HorizontalOffset + 1) % ColumnSize) == 0);
			const bool IsBottomMostItem = ((ItemPosition + (VerticalOffset * ColumnSize) + ColumnSize) >= TotalSize);

			
			//// Add border material and widgets dynamically now
			//
			USizeBox * NewSizeBox = WidgetTree->ConstructWidget<USizeBox>();
			NewSizeBox->SetHeightOverride(64);
			NewSizeBox->SetWidthOverride(64);

			// 
			UImage * NewImage = WidgetTree->ConstructWidget<UImage>();
			NewImage->SetRenderScale(FVector2D(1.19f, 1.19f));

			//
			UMaterialInstanceDynamic * NewDynamicMaterial = UMaterialInstanceDynamic::Create(TargetBorderMaterial.LoadSynchronous(), this);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveLeftLine")), ShouldShowLeftLine ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveTopLine")), ShouldShowTopLine ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveRightLine")), IsRightMostItem ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveBottomLine")), IsBottomMostItem ? 1.0f : 0.0f);
			NewImage->SetBrushFromMaterial(NewDynamicMaterial);

			//
			NewSizeBox->AddChild(NewImage);
			BackgroundGrid->AddChildToUniformGrid(NewSizeBox, VerticalOffset, HorizontalOffset);


			//// Cache the highlight material and widgets dynamically now, for ease of future use
			//
			NewSizeBox = WidgetTree->ConstructWidget<USizeBox>();
			NewSizeBox->SetHeightOverride(64);
			NewSizeBox->SetWidthOverride(64);

			// 
			NewImage = WidgetTree->ConstructWidget<UImage>();
			NewImage->SetRenderScale(FVector2D(1.19f, 1.19f));

			//
			NewDynamicMaterial = UMaterialInstanceDynamic::Create(TargetHighlightMaterial.LoadSynchronous(), this);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveLeftLine")), ShouldShowLeftLine ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveTopLine")), ShouldShowTopLine ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveRightLine")), (HorizontalOffset == (HorizontalSize - 1)) ? 1.0f : 0.0f);
			NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveBottomLine")), (VerticalOffset == (VerticalSize - 1)) ? 1.0f : 0.0f);
			NewImage->SetBrushFromMaterial(NewDynamicMaterial);

			//
			NewSizeBox->AddChild(NewImage);
			BackgroundGrid->AddChildToUniformGrid(NewSizeBox, VerticalOffset, HorizontalOffset);

			//
			NewSizeBox->SetVisibility(ESlateVisibility::Collapsed);
			HighlightWidgets.Push(NewSizeBox);
		}
	}

	//
	//ItemInternalBorder->SetRenderOpacity(0.0f);
	//ItemHighlightBorder->SetVisibility(ESlateVisibility::Collapsed);
	//ItemIcon->SetVisibility(ESlateVisibility::Visible);

	//
	ItemButton->SetVisibility(ESlateVisibility::Visible);

	// Finally, add all of the custom functionality onto this item's button's events
	ItemButton->OnClicked.AddDynamic(this, &UAbstractInventoryItemWidget::SelectItem);
	ItemButton->OnHovered.AddDynamic(this, &UAbstractInventoryItemWidget::SetFocus);

	//
	FCustomWidgetNavigationDelegate LocalDelegate;
	LocalDelegate.BindDynamic(this, &UAbstractInventoryItemWidget::SelectItemController);
	ItemButton->SetNavigationRuleCustom(EUINavigation::Next, LocalDelegate);

	//
	LocalDelegate.Clear();
	LocalDelegate.BindDynamic(this, &UAbstractInventoryItemWidget::UnselectItemController);
	ItemButton->SetNavigationRuleCustom(EUINavigation::Previous, LocalDelegate);
}


// 
void UAbstractInventoryItemWidget::SetupEmpty(UAbstractInventoryWidget * GivenParentInventoryWidget, const uint8 GivenPosition) {

	// Ensure that SetupEmpty is never called more than once for each item instance
	//checkNoReentry();

	//
	SetIsFocusable(true);
	SetDesiredFocusWidget(ItemButton);

	// Default the item data to be empty, along with setting the valid index to -1, as it is not valid
	ItemData = nullptr;

	// Set the item's position to be the given position value
	ItemPosition = GivenPosition;

	//
	ParentInventoryWidget = GivenParentInventoryWidget;

	//
	const uint8 ColumnSize = ParentInventoryWidget->CurrentInventoryComponent->GetColumnSize();
	const uint8 TotalSize = ParentInventoryWidget->CurrentInventoryComponent->GetSize();

	//
	const bool IsRightMostItem = ((ItemPosition + 1) >= TotalSize || ((ItemPosition + 1) % ColumnSize) == 0);
	const bool IsBottomMostItem = ((ItemPosition + ColumnSize) >= TotalSize);

	//
	BackgroundGrid->ClearChildren();

	//
	HighlightWidgets.Empty(1);

	//// Add border material and widgets dynamically now
	//
	USizeBox * NewSizeBox = WidgetTree->ConstructWidget<USizeBox>();
	NewSizeBox->SetHeightOverride(64);
	NewSizeBox->SetWidthOverride(64);

	// 
	UImage * NewImage = WidgetTree->ConstructWidget<UImage>();
	NewImage->SetRenderScale(FVector2D(1.19f, 1.19f));

	//
	UMaterialInstanceDynamic * NewDynamicMaterial = UMaterialInstanceDynamic::Create(TargetBorderMaterial.LoadSynchronous(), this);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveLeftLine")), 1.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveTopLine")), 1.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveRightLine")), IsRightMostItem ? 1.0f : 0.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveBottomLine")), IsBottomMostItem ? 1.0f : 0.0f);
	NewImage->SetBrushFromMaterial(NewDynamicMaterial);

	//
	NewSizeBox->AddChild(NewImage);
	BackgroundGrid->AddChildToUniformGrid(NewSizeBox, 0, 0);
	

	//// Cache the highlight material and widgets dynamically now, for ease of future use
	//
	NewSizeBox = WidgetTree->ConstructWidget<USizeBox>();
	NewSizeBox->SetHeightOverride(64);
	NewSizeBox->SetWidthOverride(64);

	// 
	NewImage = WidgetTree->ConstructWidget<UImage>();
	NewImage->SetRenderScale(FVector2D(1.19f, 1.19f));

	//
	NewDynamicMaterial = UMaterialInstanceDynamic::Create(TargetHighlightMaterial.LoadSynchronous(), this);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveLeftLine")), 1.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveTopLine")), 1.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveRightLine")), 1.0f);
	NewDynamicMaterial->SetScalarParameterValue(FName(TEXT("ShouldHaveBottomLine")), 1.0f);
	NewImage->SetBrushFromMaterial(NewDynamicMaterial);

	//
	NewSizeBox->AddChild(NewImage);
	BackgroundGrid->AddChildToUniformGrid(NewSizeBox, 0, 0);

	//
	NewSizeBox->SetVisibility(ESlateVisibility::Collapsed);
	HighlightWidgets.Push(NewSizeBox);

	//
	//ItemInternalBorder->SetRenderOpacity(1.0f);
	//ItemHighlightBorder->SetVisibility(ESlateVisibility::Collapsed);
	//ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
	ItemStackCountText->SetVisibility(ESlateVisibility::Collapsed);

	//
	ItemButton->SetVisibility(ESlateVisibility::Visible);

	// Finally, add all of the custom functionality onto this item's button's events
	//ItemButton->OnClicked.AddDynamic(this, &UAbstractInventoryItemWidget::SelectItem);
	ItemButton->OnHovered.AddDynamic(this, &UAbstractInventoryItemWidget::SetFocus);

	//
	//FCustomWidgetNavigationDelegate LocalDelegate;
	//LocalDelegate.BindDynamic(this, &UAbstractInventoryItemWidget::SelectItemController);
	//ItemButton->SetNavigationRuleCustom(EUINavigation::Next, LocalDelegate);
	ItemButton->SetNavigationRuleBase(EUINavigation::Next, EUINavigationRule::Stop);
	//this->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractInventoryItemWidget::SelectItemController);

	//
	FCustomWidgetNavigationDelegate LocalDelegate;
	LocalDelegate.BindDynamic(this, &UAbstractInventoryItemWidget::UnselectItemController);
	ItemButton->SetNavigationRuleCustom(EUINavigation::Previous, LocalDelegate);
}


// 
void UAbstractInventoryItemWidget::SelectItem() {

	//
	if (ParentInventoryWidget->TempDataContainer.IsEmpty() == false) {
		return;
	}

	// If this item is empty, then do nothing, as you cannot do anything with an empty slot
	if (ItemData == nullptr || ParentInventoryWidget.IsValid() == false) {
		ItemButton->SetFocus(); // Ensure that the empty slot immediately gets focus again, as nothing should change
		return;
	}

	// Ensure that our item stays highlighted, as it is technically still the focus
	//ItemHighlightBorder->SetVisibility(ESlateVisibility::Visible);

	//
	IsInSubMenu = true;
	
	// Activate our parent widget's sub menu and move it to our target location
	ParentInventoryWidget->ActivateSubMenu(SubMenuAttachment, this);
}


// 
UWidget * UAbstractInventoryItemWidget::SelectItemController(EUINavigation GivenNavigationType) {
	this->SelectItem();
	return ItemButton;
}


// 
void UAbstractInventoryItemWidget::UnselectItem() {

	//
	if (ParentInventoryWidget->TempDataContainer.IsEmpty() == false) {
		ParentInventoryWidget->ReplaceTempItem();
		return;
	}

	// 
	ParentInventoryWidget->MakeInvisible(true);
}


// 
UWidget * UAbstractInventoryItemWidget::UnselectItemController(EUINavigation GivenNavigationType) {
	this->UnselectItem();
	return ItemButton;
}


// 
void UAbstractInventoryItemWidget::UpdateStackCount() {

	// If the item data is valid, then...
	if (ItemData != nullptr) {

		// If this item contains a valid object, has a visible item stack count, AND has a stack count that has changed, then...
		if (const int32 ItemIndex = GetItemIndex(); ItemData->ItemDataArray.IsValidIndex(ItemIndex) && ItemStackCountText->GetVisibility() == ESlateVisibility::Visible && CurrentStackCache != ItemData->ItemDataArray[ItemIndex].CurrentStackCount) {

			// Update our current stack count cache value to the new stack count
			CurrentStackCache = ItemData->ItemDataArray[ItemIndex].CurrentStackCount;

			// Update our current item stack count to the new one, assuming that it has changed
			ItemStackCountText->SetText(FText::AsNumber(CurrentStackCache));

			// Update the color of the current stack count text to the new fade values IFF it is a stackable item, either internally or externally
			if (const int32 CurrentMaxStackCount = (ItemData->ItemData.MaxStackCount <= 1 ? ItemData->ItemData.InternalMaxStackCount : ItemData->ItemData.MaxStackCount); CurrentMaxStackCount > 0)
				ItemStackCountText->SetColorAndOpacity(FSlateColor{EmptyColor + ((static_cast<float>(ItemData->ItemDataArray[GetItemIndex()].CurrentStackCount - 1) / CurrentMaxStackCount) * (FullColor - EmptyColor))});
			else
				ItemStackCountText->SetColorAndOpacity(FSlateColor{EmptyColor});
		}
	}
}


// 
void UAbstractInventoryItemWidget::RemoveIfEmpty(const TSet<uint8> & GivenRemovingPositions) {

	// If this item is empty and is one of the now taken up slot positions, then delete ourself
	if (ItemData == nullptr && GivenRemovingPositions.Contains(this->ItemPosition))
		this->RemoveFromParent();
}


// 
UWidget * UAbstractInventoryItemWidget::RemoveItem(const int32 GivenCount) {
	return ParentInventoryWidget->CurrentInventoryComponent->RemoveItem(this, GivenCount);
}


// 
UWidget * UAbstractInventoryItemWidget::InteractItem() {
	return ParentInventoryWidget->CurrentInventoryComponent->InteractItem(this);
}


// 
UWidget * UAbstractInventoryItemWidget::MoveItem() {
	return ParentInventoryWidget->MoveItem(this);
}


// 
UWidget * UAbstractInventoryItemWidget::CombineItem() {
	return ParentInventoryWidget->CombineItem(this);
}


// 
UWidget * UAbstractInventoryItemWidget::DiscardItem() {
	return ParentInventoryWidget->CurrentInventoryComponent->DiscardItem(this);
}


// 
void UAbstractInventoryItemWidget::NativeDestruct() {

	// Ensure that we first and foremost remove our delegates as to ensure they are never falsely called
	CVarEmptyItemColorHex->OnChangedDelegate().Remove(NewEmptyColorHandle);
	CVarFullItemColorHex->OnChangedDelegate().Remove(NewFullColorHandle);

	//
	//Navigation->Next.CustomDelegate.Unbind();
	
	// Lastly, call our parent functionality
	Super::NativeDestruct();
}


// 
void UAbstractInventoryItemWidget::HandleChangedColorHex(IConsoleVariable * PointerToChangedConsoleVariable, const bool IsEmptyColor) {

	// If our data is invalid when this is called, then remove ourselves and return, as we are an empty inventory slot
	if (ItemData == nullptr) {
		CVarEmptyItemColorHex->OnChangedDelegate().Remove(NewEmptyColorHandle);
		CVarFullItemColorHex->OnChangedDelegate().Remove(NewFullColorHandle);
		return;
	}

	// If we're updating the empty color, then assume the pointer is for its hex string value
	if (IsEmptyColor == true) {
		EmptyColor = FLinearColor::FromSRGBColor(FColor::FromHex(PointerToChangedConsoleVariable->GetString()));
	}

	// Else, assume the pointer is for the full color's hex string value instead
	else {
		FullColor = FLinearColor::FromSRGBColor(FColor::FromHex(PointerToChangedConsoleVariable->GetString()));
	}
	
	// Update the color of the current stack count text to the new fade values IFF it is a stackable item, either internally or externally
	if (const int32 CurrentMaxStackCount = (ItemData->ItemData.MaxStackCount <= 1 ? ItemData->ItemData.InternalMaxStackCount : ItemData->ItemData.MaxStackCount); CurrentMaxStackCount > 0)
		ItemStackCountText->SetColorAndOpacity(FSlateColor{EmptyColor + ((static_cast<float>(ItemData->ItemDataArray[GetItemIndex()].CurrentStackCount - 1) / CurrentMaxStackCount) * (FullColor - EmptyColor))});
	else
		ItemStackCountText->SetColorAndOpacity(FSlateColor{EmptyColor});
}


// 
void UAbstractInventoryItemWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) {

	// 
	Super::NativeOnAddedToFocusPath(InFocusEvent);

	// 
	for (UWidget * CurrentHighlightWidget : HighlightWidgets) {
		CurrentHighlightWidget->SetVisibility(ESlateVisibility::Visible);
	}

	// If the item data is valid, then update the item information for this item type
	if (ItemData != nullptr)
		ParentInventoryWidget->UpdateItemInformation(this, ItemData->ItemData.DisplayName, ItemData->ItemData.Description);

	// Else, update the item information to this widget's default empty item text instead
	else
		ParentInventoryWidget->UpdateItemInformation(this, FText::GetEmpty(), FText::GetEmpty());
}


// 
void UAbstractInventoryItemWidget::NativeOnRemovedFromFocusPath(const FFocusEvent & InFocusEvent) {

	// 
	Super::NativeOnRemovedFromFocusPath(InFocusEvent);

	//
	if (IsInSubMenu == true)
		return;

	// 
	for (UWidget * CurrentHighlightWidget : HighlightWidgets) {
		CurrentHighlightWidget->SetVisibility(ESlateVisibility::Collapsed);
	}
}


// 
FReply UAbstractInventoryItemWidget::NativeOnFocusReceived(const FGeometry & InGeometry, const FFocusEvent & InFocusEvent) {

	// Call the parent function first and store its return value
	const FReply ParentReply = Super::NativeOnFocusReceived(InGeometry, InFocusEvent);

	//
	ItemButton->SetFocus();

	// Return the parent function's return value now
	return ParentReply;
}


// 
void UAbstractInventoryItemWidget::NativeOnFocusLost(const FFocusEvent & InFocusEvent) {
	Super::NativeOnFocusLost(InFocusEvent);
}
