// TreeTrunkStudios (c) 2023


// 
#include "AbstractInventoryWidget.h"
#include <Blueprint/WidgetTree.h>
#include "Components/GridSlot.h"
#include "Animation/UMGSequencePlayer.h"
#include <EnhancedInputComponent.h>
#include <Widgets/SViewport.h>


// 
void UAbstractInventoryWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime) {

	// Call our parent functionality first and foremost
	Super::NativeTick(MyGeometry, InDeltaTime);

	// If our cached widget is invalid, then skip all functionality, as it means we have not been initialized yet
	if (CachedFocusedWidget.IsValid() == false || GetVisibility() != ESlateVisibility::Visible)
		return;

	// If our focused keyboard widget is valid, then...
	if (const TSharedPtr<SWidget> & FoundWidget = FSlateApplication::Get().GetKeyboardFocusedWidget(); FoundWidget.IsValid() && InvalidToFocusWidgetTypes.Contains(FoundWidget->GetType()) == false) {

		// If our cached widget is different than the found widget, then update the focus now
		if (FoundWidget != CachedFocusedWidget) {
			CachedFocusedWidget = FoundWidget;
			SetWidgetFocus(CachedFocusedWidget);
		}
	}

	// Else, if our focused widget has become invalid, then forcibly set its focus again now
	else
		SetWidgetFocus(CachedFocusedWidget);
}


// 
void UAbstractInventoryWidget::ForceCloseUserInterface() {

	//
	ItemUseSubMenuWidget->ForceCloseSubMenu();

	//
	this->MakeInvisible(true);
}


// 
void UAbstractInventoryWidget::SetFocusWidget() {

	// If our cached focus widget is valid, then simply return that
	if (CachedFocusedWidget.IsValid() == false) {

		// Loop through all of the grid slots, and...
		for (const UPanelSlot * SlotFill : InventoryGridSystem->GetSlots()) {

			// Locally store the current grid slot
			const UGridSlot * CurrentGridSlot = Cast<UGridSlot>(SlotFill);
		
			// If the current grid slot is somehow invalid, then skip it
			if (IsValid(CurrentGridSlot) == false) continue;

			// If this is the first inventory item, then...
			if (CurrentGridSlot->GetColumn() == 0 && CurrentGridSlot->GetRow() == 0) {

				// Return the very first item within the grid for focus purposes
				CachedFocusedWidget = CurrentGridSlot->Content->GetCachedWidget();
				break;
			}
		}
	}

	// Finally, ensure that the widget focus is set to our newly cached focus widget
	SetWidgetFocus(CachedFocusedWidget);
}


// 
TSharedPtr<SWidget> UAbstractInventoryWidget::GetFocusWidget() {
	return CachedFocusedWidget;
}


// 
void UAbstractInventoryWidget::Setup(UBasicInventoryComponent * GivenInventoryComponent) {

	// Ensure always that the given inventory component pointer is always valid (blueprints apparently doesn't support references yet)
	ensureAlways(IsValid(GivenInventoryComponent));

	// Locally store the given inventory component to cache it for easier future use
	CurrentInventoryComponent = GivenInventoryComponent;

	// Ensure that the grid is completely empty on the off-chance that we are re-setting up an inventory widget
	InventoryGridSystem->ClearChildren();

	// Create a local set of positions for each slot filled within the inventory for ease of future use
	TSet<uint8> LocalSlotFill;

	// Locally store a constant reference to the current inventory component's column size
	const uint8 ColumnSize = CurrentInventoryComponent->GetColumnSize();

	// Loop through all inventory items given by the inventory component, and...
	for (FBasicInventoryItem & CurrentItemType : CurrentInventoryComponent->GetInventoryItems()) {
		
		// Loop through each sub item of the current inventory item type, and...
		for (int32 CurrentIndex = 0; CurrentIndex < CurrentItemType.ItemDataArray.Num(); ++CurrentIndex) {

			// Add the item internally to the inventory grid as its own sub-widget
			this->AddItem(CurrentItemType, CurrentIndex, false);

			// Locally store a constant reference to the current sub-item
			const FInternalInventoryItemData & CurrentItem = CurrentItemType.ItemDataArray[CurrentIndex];

			// Locally store constants to the current item's horizontal size and vertical size based on its rotation
			const uint8 HorizontalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? CurrentItemType.ItemData.SizeX : CurrentItemType.ItemData.SizeY);
			const uint8 VerticalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? CurrentItemType.ItemData.SizeY : CurrentItemType.ItemData.SizeX);

			// Loop through the vertical size of the item, and...
			for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

				// Loop through the horizontal size of the item, and...
				for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

					// Mark all of this item's full slots as to allow empty slots to fill the rest
					LocalSlotFill.Add(CurrentItem.Position + HorizontalOffset + (VerticalOffset * ColumnSize));
				}
			}
		}
	}

	// Loop through each and every one of the slot counts, and...
	const uint8 MaxSize = CurrentInventoryComponent->GetSize();
	for (uint8 CurrentPosition = 0; CurrentPosition < MaxSize; ++CurrentPosition) {

		// If the current slot is already filled, then skip it
		if (LocalSlotFill.Contains(CurrentPosition)) continue;

		// Create the newly wanted inventory item widget to represent the current item
		UAbstractInventoryItemWidget * NewInventoryItem = WidgetTree->ConstructWidget<UAbstractInventoryItemWidget>(PerInventoryItemClass.Get());

		// Add the newly created inventory item as a grid slot into the inventory grid widget with the wanted row and column locations
		UGridSlot * NewlyAddedGridItem = InventoryGridSystem->AddChildToGrid(NewInventoryItem,
									CurrentPosition / ColumnSize,
									CurrentPosition % ColumnSize);
		
		// Setup the new widget as an empty widget that contains nothing
		NewInventoryItem->SetupEmpty(this, CurrentPosition);

		// Finally, ensure that this new empty slot only takes up a single column and row position
		NewlyAddedGridItem->SetColumnSpan(1);
		NewlyAddedGridItem->SetRowSpan(1);
	}

	// Make this widget visible to the user, as it is now created and fully initialized
	this->MakeVisible();

	// Finally, if this is the initial setup of the current widget, then setup button functionality to handle mouse selections
	if (SelectButton->OnClicked.Contains(this, TEXT("&UAbstractInventoryWidget::SelectItem")) == false)
		SelectButton->OnClicked.AddDynamic(this, &UAbstractInventoryWidget::SelectItem);
	if (UnselectButton->OnClicked.Contains(this, TEXT("&UAbstractInventoryWidget::ExitInventory")) == false)
		UnselectButton->OnClicked.AddDynamic(this, &UAbstractInventoryWidget::ExitInventory);

	//
	//UEnhancedInputComponent * CurrentInputComponent = Cast<UEnhancedInputComponent>(GetPlayerContext().GetPawn()->InputComponent.Get());
	//CurrentInputComponent->BindAction(ForceCloseInterfaceInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &UAbstractInventoryWidget::ForceCloseUserInterface);
}


// 
void UAbstractInventoryWidget::SetupWithNewItem(UBasicInventoryComponent * GivenInventoryComponent, const FTempInventoryItem GivenNewItem) {
	this->MakeTempItem(GivenNewItem);
	this->Setup(GivenInventoryComponent);
}


// 
void UAbstractInventoryWidget::MakeVisible() {

	// Loop through all grid slots within the inventory grid system, and...
	for (const UPanelSlot * CurrentGridSlot : InventoryGridSystem->GetSlots()) {

		// Ensure that the content stored within the grid slot is always a child of UAbstractInventoryItemWidget
		ensureAlways(IsValid(Cast<UAbstractInventoryItemWidget>(CurrentGridSlot->Content.Get())));

		// Update the current grid slot's inventory item's stack count
		Cast<UAbstractInventoryItemWidget>(CurrentGridSlot->Content.Get())->UpdateStackCount();
	}

	// Make the current widget visible again
	this->SetVisibility(ESlateVisibility::Visible);
	
	// Add the widget to be permanently on the player's screen
	this->AddToPlayerScreen(10);
	
	// Finally, play the reveal window animation forward to make this widget truly visible
	this->PlayAnimationForward(RevealWindow);
}


// 
void UAbstractInventoryWidget::MakeInvisible(bool IsInternal) {

	// Play the reveal window animation in reverse and, upon the animation finishing...
	this->PlayAnimationReverse(RevealWindow)->OnSequenceFinishedPlaying().AddLambda([this](UUMGSequencePlayer&) {

		// Set the current widget's visibility to collapsed
		this->SetVisibility(ESlateVisibility::Collapsed);

		// Remove ourself from the current player's screen, as we are no longer visible and should not be wasting performance
		this->RemoveFromParent();
	});

	//
	//FSlateApplication::Get().SetAllUserFocusToGameViewport(EFocusCause::SetDirectly);

	// Call the delegate to allow other's to start closing this widget, functionality-wise
	if (IsInternal == true)
		CloseUserInterfaceDelegate.Execute();
}


// 
void UAbstractInventoryWidget::UpdateItemInformation(UAbstractInventoryItemWidget * GivenFocusedWidget, const FText & ItemName, const FText & ItemDescription) {
	CurrentItemWidget = GivenFocusedWidget;
	ItemNameText->SetText(ItemName);
	ItemDescriptionText->SetText(ItemDescription);
}


// 
bool UAbstractInventoryWidget::DoesContainItem(const FName & GivenItemName) {
	return CurrentInventoryComponent->GetInventoryItems().Contains(FBasicInventoryItem{GivenItemName});
}


// 
UAbstractInventoryItemWidget * UAbstractInventoryWidget::FindLastItem(const FName & GivenItemName) {

	//
	if (FBasicInventoryItem * FoundInventoryItem = CurrentInventoryComponent->GetInventoryItems().Find(FBasicInventoryItem{GivenItemName})) {

		//
		FInternalInventoryItemData & FoundInternalItem = FoundInventoryItem->ItemDataArray[FoundInventoryItem->ItemDataArray.Num() - 1];

		//
		for (UPanelSlot * CurrentSlot : InventoryGridSystem->GetSlots()) {

			//
			UAbstractInventoryItemWidget * CurrentInventoryItem = Cast<UAbstractInventoryItemWidget>(CurrentSlot->Content);

			//
			if (CurrentInventoryItem->ItemData == FoundInventoryItem && CurrentInventoryItem->ItemPosition == FoundInternalItem.Position) {

				//
				return CurrentInventoryItem;
			}
		}
	}

	//
	return nullptr;
}


// 
void UAbstractInventoryWidget::AddItem(FBasicInventoryItem & GivenItemType, const int32 ItemIndex, const bool ShouldCheckForEmptyWidgets) {

	// Ensure always that the given item index is a valid index for the given item type's array
	ensureAlways(GivenItemType.ItemDataArray.IsValidIndex(ItemIndex));

	// Locally store a constant reference to the current inventory component's column size
	const uint8 ColumnSize = CurrentInventoryComponent->GetColumnSize();
	
	//
	if (ShouldCheckForEmptyWidgets == true) {

		// Create a local set to contain 
		TSet<uint8> NewlyFilledSlots;
		
		// Locally store a constant reference to the current sub-item
		const FInternalInventoryItemData & CurrentItem = GivenItemType.ItemDataArray[ItemIndex];

		// Locally store constants to the current item's horizontal size and vertical size based on its rotation
		const uint8 HorizontalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? GivenItemType.ItemData.SizeX : GivenItemType.ItemData.SizeY);
		const uint8 VerticalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? GivenItemType.ItemData.SizeY : GivenItemType.ItemData.SizeX);

		// Loop through the vertical size of the current item, and...
		for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

			// Loop through the horizontal size of the current item, and...
			for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

				// Mark all of this item's requested slot positions
				NewlyFilledSlots.Add(CurrentItem.Position + HorizontalOffset + (VerticalOffset * ColumnSize));
			}
		}

		// Loop through every single grid slot making up the current inventory UI, and...
		for(const UPanelSlot * CurrentGridSlot : InventoryGridSystem->GetSlots()) {

			// Ensure that the content stored within the grid slot is always a child of UAbstractInventoryItemWidget
			ensureAlways(IsValid(Cast<UAbstractInventoryItemWidget>(CurrentGridSlot->Content.Get())));

			// Remove this item if it is empty and its position is contained within the given set
			Cast<UAbstractInventoryItemWidget>(CurrentGridSlot->Content.Get())->RemoveIfEmpty(NewlyFilledSlots);
		}
	}
	
	// Locally store a reference to the current sub item we are working with for ease of future use
	const FInternalInventoryItemData & CurrentItem = GivenItemType.ItemDataArray[ItemIndex];

	// Create the newly wanted inventory item widget to represent the current item
	UAbstractInventoryItemWidget * NewInventoryItem = WidgetTree->ConstructWidget<UAbstractInventoryItemWidget>(PerInventoryItemClass.Get());

	// Add the newly created inventory item as a grid slot into the inventory grid widget with the wanted row and column locations
	UGridSlot * NewlyAddedGridItem = InventoryGridSystem->AddChildToGrid(NewInventoryItem,
								CurrentItem.Position / ColumnSize,
								CurrentItem.Position % ColumnSize);
	
	// Setup the new widget for it to contain all of the important information it needs
	NewInventoryItem->Setup(this, GivenItemType, ItemIndex);

	// Finally, update the newly added grid slot's column and row spans to take into account multi-square items and their rotations
	NewlyAddedGridItem->SetColumnSpan(CurrentItem.ItemRotation == EItemRotation::Rotate0 ? GivenItemType.ItemData.SizeX : GivenItemType.ItemData.SizeY);
	NewlyAddedGridItem->SetRowSpan(CurrentItem.ItemRotation == EItemRotation::Rotate0 ? GivenItemType.ItemData.SizeY : GivenItemType.ItemData.SizeX);
}


// 
UWidget * UAbstractInventoryWidget::MoveItem(UAbstractInventoryItemWidget * Item) {

	//
	return Item;
}


// 
UWidget * UAbstractInventoryWidget::RemoveItem(UAbstractInventoryItemWidget * Item) {

	//
	ensureAlways(IsValid(Item));

	// Locally store a constant reference to the current inventory component's column size
	const uint8 ColumnSize = CurrentInventoryComponent->GetColumnSize();

	//
	const FInternalInventoryItemData & CurrentItem = Item->ItemData->ItemDataArray[Item->GetItemIndex()];

	// Locally store constants to the current item's horizontal size and vertical size based on its rotation
	const uint8 HorizontalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? Item->ItemData->ItemData.SizeX : Item->ItemData->ItemData.SizeY);
	const uint8 VerticalSize = (CurrentItem.ItemRotation == EItemRotation::Rotate0 ? Item->ItemData->ItemData.SizeY : Item->ItemData->ItemData.SizeX);

	// Store the first empty widget made to be focused on when finished
	UWidget * LocalWidget = nullptr;

	// Loop through the vertical size of the item, and...
	for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

		// Loop through the horizontal size of the item, and...
		for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

			// Locally store our current offset position for this item
			const uint8 CurrentPosition = (CurrentItem.Position + HorizontalOffset + (VerticalOffset * ColumnSize));

			// Create the newly wanted empty inventory item widget to represent the current item
			UAbstractInventoryItemWidget * NewInventoryItem = WidgetTree->ConstructWidget<UAbstractInventoryItemWidget>(PerInventoryItemClass.Get());

			// Add the newly created inventory item as a grid slot into the inventory grid widget with the wanted row and column locations
			UGridSlot * NewlyAddedGridItem = InventoryGridSystem->AddChildToGrid(NewInventoryItem,
										CurrentPosition / ColumnSize,
										CurrentPosition % ColumnSize);
		
			// Setup the new widget as an empty widget that contains nothing
			NewInventoryItem->SetupEmpty(this, CurrentPosition);

			// Finally, ensure that this new empty slot only takes up a single column and row position
			NewlyAddedGridItem->SetColumnSpan(1);
			NewlyAddedGridItem->SetRowSpan(1);

			//
			if (VerticalOffset == 0 && HorizontalOffset == 0)
				LocalWidget = NewInventoryItem;
		}
	}

	// Remove this widget entirely from the player's inventory UI
	Item->RemoveFromParent();

	// Finally, return a pointer to the first empty widget so that we know what to focus on next
	return LocalWidget;
}


// 
UWidget * UAbstractInventoryWidget::CombineItem(UAbstractInventoryItemWidget * Item) {

	//
	return Item;
}


// 
void UAbstractInventoryWidget::SwapItem() {}


// 
void UAbstractInventoryWidget::MakeTempItem(const FTempInventoryItem GivenNewTempItem) {
	
}


// 
void UAbstractInventoryWidget::ReplaceTempItem() {
	
}


// 
void UAbstractInventoryWidget::SelectItem() {
	if (IsValid(CurrentItemWidget))
		CurrentItemWidget->SelectItem();
}


// 
void UAbstractInventoryWidget::ExitInventory() {
	MakeInvisible(true);
}


// 
void UAbstractInventoryWidget::RotateTempItem() {

	// If the temp item is valid, then...
	if (IsValid(CurrentTempItemReference) && TempDataContainer.ItemName.IsNone() == false) {

		// Rotate the temp item and be done
		TempDataContainer.ItemData.ItemRotation = (TempDataContainer.ItemData.ItemRotation == EItemRotation::Rotate0 ? EItemRotation::Rotate90 : EItemRotation::Rotate0);
	}
}


// 
void UAbstractInventoryWidget::ActivateSubMenu(UPanelWidget * GivenAttachment, UAbstractInventoryItemWidget * GivenInventoryItem) {
	ItemUseSubMenuWidget->Setup(GivenAttachment, GivenInventoryItem);
}


// 
void UAbstractInventoryWidget::SetWidgetFocus(const TSharedPtr<SWidget> & GivenWidget) {

	//
	//FSlateApplication::Get().ClearKeyboardFocus();

	// If the given widget is both valid and has changed, then update our cached version of it
	if (GivenWidget.IsValid()) {

		// Collect our owning local player and set our user focus to be the given widget instead as a fallback
		FSlateApplication::Get().SetUserFocus(FGenericPlatformMisc::GetUserIndexForPlatformUser(GetPlayerContext().GetPlayerController()->GetPlatformUserId()), GivenWidget, EFocusCause::SetDirectly);

		//GetOwningLocalPlayer()->GetSlateOperations().SetUserFocus(GivenWidget.ToSharedRef(), EFocusCause::SetDirectly);

		// Set our keyboard focus to be the current widget, and, if NOT successful, then...
		/*if (!FSlateApplication::Get().SetKeyboardFocus(GivenWidget)) {

			// Collect our owning local player and set our user focus to be the given widget instead as a fallback
			if (ULocalPlayer * LocalPlayer = GetOwningLocalPlayer()) {
				LocalPlayer->GetSlateOperations().SetUserFocus(GivenWidget.ToSharedRef(), EFocusCause::SetDirectly);
			}
		}*/
	}
}
