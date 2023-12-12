// TreeTrunkStudios (c) 2023


// Necessary includes for this file to function properly
#include "BasicInventoryComponent.h"
#include "AbstractInventoryWidget.h"
#include "Framework/Application/NavigationConfig.h"
#include "Framework/Application/SlateApplication.h"


// Sets default values for this component's properties
UBasicInventoryComponent::UBasicInventoryComponent() : CachedInventoryWidget(nullptr) {
	
	// Ensure that this component NEVER ticks
	PrimaryComponentTick.bCanEverTick = false;

	// Default the inventory widget class to be the static class of the abstract inventory widget
	InventoryWidgetClass = UAbstractInventoryWidget::StaticClass();

	// Initialize the read-only variables assuming an empty inventory to start with
	RemainingSlots = MaxSize;//(SizeX * SizeY);
	//IsSlotFilled.Init(false, RemainingSlots);
}


// Called when the game starts
void UBasicInventoryComponent::BeginPlay() {
	// Call the parent function first to ensure proper usage
	Super::BeginPlay();

	// Reinitialize the read-only variables just in-case something has changed before now
	RemainingSlots = MaxSize;//(SizeX * SizeY);
	//IsSlotFilled.Init(false, RemainingSlots);

	// TEMP
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(RemainingSlots));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(InventoryItems.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt(ChangeInventoryItems.Num()));
	}

	// Ensure that the starting inventory items are all valid items with valid inventory locations
	EnsureInventoryItemSetSafety();

	// Ensure that the starting inventory change items are all valid items (other than this one check, the set should be pseudo-constant otherwise)
	EnsureInventoryChangeSetSafety();

	// TEMP
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(RemainingSlots));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(InventoryItems.Num()));
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt(ChangeInventoryItems.Num()));
	}

	// Ensure that the owner of this component is a pawn
	ensureAlways(Cast<APawn>(GetOwner()));
	
	// If our owning pawn is a player, meaning that they can and will utilize the inventory UI, then create and cache
	//     the UI now, that way it is relatively free on future openings (we want this, as the game does not pause when
	//     the player is inside the inventory, so we do not want any frame drops from excess calculations)
	/*if (Cast<APawn>(GetOwner())->IsPlayerControlled()) {
		CreateInventoryUserInterface();
		CloseUserInterface();
	}

	// Else, if this is owned by an AI, then set the cached UI to nullptr, as it should never be used
	else
		CachedInventoryWidget = nullptr;*/

	//// Take in an InputMapping and all of the necessary InputActions related to this and implement them, so that we
	////     collect from the input mapping all of the input action's keys and give them to the navigation config
	// Collect a reference to our current slate application's navigation configuration struct, and set it to our values
	FNavigationConfig & Tester = FSlateApplication::Get().GetNavigationConfig().Get();
	Tester.bTabNavigation = false;    // Disable tab functionality, as it is dumb
	Tester.bAnalogNavigation = false; // Disable analog functionality, as it can be replicated through the key navigation system
	Tester.bKeyNavigation = true;     // Enable key navigation, else this navigation system wouldn't be able to do anything
	Tester.KeyEventRules.Empty(NavigationInputActions.Num()); // Empty out the array of events so that we can fill them custom-ly

	// If our input mapping and input actions are valid, then...
	if (MenuInputMapping.IsNull() == false && NavigationInputActions.Num() > 0) {

		// Loop through all action key mappings for the given input mapping, and...
		for(const FEnhancedActionKeyMapping & Element : MenuInputMapping.LoadSynchronous()->GetMappings()) {

			// If the given action is one of the action key mapping's actions, then...
			if (const FInputActionToNavigation * CurrentConversion = NavigationInputActions.Find(FInputActionToNavigation{Element.Action})) {

				// Add the current action key mapping's key as the current input action to navigation's navigation enum
				Tester.KeyEventRules.Emplace(Element.Key, CurrentConversion->WantedNavigation);
			}
		}
	}
}


// 
void UBasicInventoryComponent::EnsureInventoryItemSetSafety() {

	// Create an empty set to keep track of all empty items that need to be deleted
	TSet<FBasicInventoryItem> EmptyItemsToBeDeleted;

	// Create local variables to keep track of the remaining valid slots within the current inventory
	uint8 LastValidPosition = 0;
	TBitArray<> LocalSlotInfo{false, MaxSize};

	// Loop through all starting items within the inventory items set
	for (FBasicInventoryItem & CurrentItem : InventoryItems) {

		// If there are no more remaining valid slots, then remove the current item, as it cannot fit
		if (LastValidPosition >= MaxSize) {
			EmptyItemsToBeDeleted.Add(CurrentItem);
			continue;
		}

		// If the current item is empty or contains an empty item name, then add it to the local set and skip it
		if (CurrentItem.ItemDataArray.IsEmpty() || CurrentItem.ItemName.IsNone()) {
			EmptyItemsToBeDeleted.Add(CurrentItem);
			continue;
		}
		
		// Next, we want to collect the item's information from the given supported inventory item data table and ensure that it is valid
		//CurrentItem.ItemData = *reinterpret_cast<FInventoryItemData*>(InventoryItemDataTable.LoadSynchronous()->FindRowUnchecked(CurrentItem.ItemName));
		if (const FInventoryItemData * CurrentItemData = reinterpret_cast<FInventoryItemData*>(InventoryItemDataTable.LoadSynchronous()->FindRowUnchecked(CurrentItem.ItemName))) {
			CurrentItem.ItemData = *CurrentItemData;
		}

		// Else, if it is invalid somehow, then add it to the local set for removal and skip it
		else {
			EmptyItemsToBeDeleted.Add(CurrentItem);
			continue;
		}

		// Create a local set to keep track of all sub inventory items that need to be deleted
		TSet<FInternalInventoryItemData> EmptySubItemsToBeDeleted;

		// Loop through all sub inventory items (basically the actual instances of the current item type)
		for (FInternalInventoryItemData & CurrentInternalItem : CurrentItem.ItemDataArray) {

			// If there are no more remaining valid slots, then remove the current sub item, as it cannot fit
			if (LastValidPosition >= MaxSize) {
				EmptySubItemsToBeDeleted.Add(CurrentInternalItem);
				continue;
			}

			// If the current stack count is less than or equal to zero, then add it to the local sub set and skip it
			if (CurrentInternalItem.CurrentStackCount <= 0) {
				// Goto label for quickly deleting and skipping the current inventory sub item
				ThisItemCannotFitInTheInventorySoDeleteIt:
				EmptySubItemsToBeDeleted.Add(CurrentInternalItem);
				continue;
			}

			// Locally store what kind of rotation this currently is (whether the size for x and y need to be reversed)
			const bool IsRotationHorizontal = (CurrentInternalItem.ItemRotation == EItemRotation::Rotate0);
			const uint8 HorizontalSize = (IsRotationHorizontal ? CurrentItem.ItemData.SizeX : CurrentItem.ItemData.SizeY);
			const uint8 VerticalSize = (IsRotationHorizontal ? CurrentItem.ItemData.SizeY : CurrentItem.ItemData.SizeX);

			// 
			const uint8 HorizontalTotalOffset = (HorizontalSize - 1);
			const uint8 VerticalTotalOffset = (VerticalSize - 1);
			
			// If the item's upper left position or bottom right position is greater than the inventory's max size, or the items upper left and upper right positions are on different rows, then reset the item, as it is invalid
			if ((CurrentInternalItem.Position >= MaxSize) ||
				(((CurrentInternalItem.Position + HorizontalTotalOffset + (VerticalTotalOffset * ColumnCount))) >= MaxSize) ||
				((CurrentInternalItem.Position / ColumnCount) != ((CurrentInternalItem.Position + HorizontalTotalOffset) / ColumnCount))) {

				// Reset its rotation to the default value
				//CurrentInternalItem.ItemRotation = EItemRotation::Rotate0;

				// Reset its location to the last known safe location
				CurrentInternalItem.Position = LastValidPosition;
			}

			// Create a local array to keep track of all available locations
			TArray<uint8> LocalAvailableSlots;
			LocalAvailableSlots.AddUninitialized(VerticalSize * HorizontalSize);
			
			// Goto label for rechecking the validity of the current position for the current inventory sub item
			RestartValidPositionCheck:

			// Redo the valid position check, but, this time, while invalid...
			while ((CurrentInternalItem.Position >= MaxSize) ||
				   (((CurrentInternalItem.Position + HorizontalTotalOffset + (VerticalTotalOffset * ColumnCount))) >= MaxSize) ||
				   ((CurrentInternalItem.Position / ColumnCount) != ((CurrentInternalItem.Position + HorizontalTotalOffset) / ColumnCount))) {
				
				// Increment the position and check if it is outside of the inventory's bounds and, if so, then...
				if (++CurrentInternalItem.Position >= MaxSize) {

					// Remove this item from the current inventory, as it cannot fit at all
					goto ThisItemCannotFitInTheInventorySoDeleteIt;
				}
			}
			
			//Loop through each row that this item takes up, and...
			for (uint8 VerticalIndex = 0; VerticalIndex < VerticalSize; ++VerticalIndex) {

				// Loop through each column this item takes up, and...
				for (uint8 HorizontalIndex = 0; HorizontalIndex < HorizontalSize; ++HorizontalIndex) {

					// Locally store the current 2D location of the weapon within the inventory
					const uint8 CurrentLocation = (CurrentInternalItem.Position + HorizontalIndex + (VerticalIndex * ColumnCount));

					// If this slot is already taken up, then...
					if (LocalSlotInfo[CurrentLocation] == true) {

						// Increment the current internal item's position
						++CurrentInternalItem.Position;

						// Restart this entire validity check, as the entire item is now invalid
						goto RestartValidPositionCheck;
					}

					// If this slot is not taken up already, then locally store it for future ease of use
					LocalAvailableSlots[HorizontalIndex + (HorizontalSize * VerticalIndex)] = CurrentLocation;
				}
			}

			// Loop through all valid slots and fill them, as this item now permanently resides in this inventory
			for (const uint8 & CurrentIndex : LocalAvailableSlots) {
				LocalSlotInfo[CurrentIndex] = true;
			}

			// Infinitely increment the last valid position until it is either a valid slot or an invalid index
			while (LocalSlotInfo.IsValidIndex(LastValidPosition) && LocalSlotInfo[LastValidPosition] == true) {
				++LastValidPosition;
			}

			// If the item's stack count is greater than the item type's max stack count, then reset it to the max stack count
			if (CurrentInternalItem.CurrentStackCount > CurrentItem.ItemData.MaxStackCount) {
				CurrentInternalItem.CurrentStackCount = CurrentItem.ItemData.MaxStackCount;
			}
			
			// Adjust the remaining slots variable to take into account the now valid inventory item
			RemainingSlots -= (CurrentItem.ItemData.SizeX * CurrentItem.ItemData.SizeY);

			// TEMP
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, CurrentItem.ItemData.ToString());
		}

		// Loop through the local set of sub inventory items that are empty or invalid and remove them
		CurrentItem.ItemDataArray.RemoveAll([EmptySubItemsToBeDeleted](const FInternalInventoryItemData & CurrentItem) {
			return EmptySubItemsToBeDeleted.Contains(CurrentItem);
		});

		// If the internal array is now empty, because all instances were invalid, then add the current item to be removed as well
		if (CurrentItem.ItemDataArray.IsEmpty()) {
			EmptyItemsToBeDeleted.Add(CurrentItem);
		}
	}

	// Finally, loop through all items to be removed and remove them from the inventory items set
	for (const FBasicInventoryItem & CurrentItemToRemoveName : EmptyItemsToBeDeleted) {
		InventoryItems.Remove(CurrentItemToRemoveName);
	}
}


// 
void UBasicInventoryComponent::EnsureInventoryChangeSetSafety() {
	
	// Create a local set to keep track of all invalid inventory change items
	TSet<FInventoryChangeInfo> InventoryChangeItemsToRemove;

	// Loop through all current inventory change items, and...
	for (const FInventoryChangeInfo CurrentChangeItem : ChangeInventoryItems) {

		// If the item's name is not valid or the item name given is not an actual supported inventory item, then add it to the local set to be removed
		if (CurrentChangeItem.ItemName.IsNone() || InventoryItemDataTable.LoadSynchronous()->FindRowUnchecked(CurrentChangeItem.ItemName) == nullptr) {
			InventoryChangeItemsToRemove.Add(CurrentChangeItem);
		}
	}

	// Finally, loop through all items that need to be removed and remove them
	for (const FInventoryChangeInfo CurrentItemToRemove : InventoryChangeItemsToRemove) {
		ChangeInventoryItems.Remove(CurrentItemToRemove);
	}
}


// 
int32 UBasicInventoryComponent::InitializeUserInterface(const FName GivenItemName, const int32 WantedCount) {

	// If we are trying to start the user interface with a temporary item, then...
	if (GivenItemName.IsNone() == false) {

		// Attempt to find the inventory item's data already within our current inventory
		if (const FBasicInventoryItem * TargetInventoryItem = InventoryItems.Find(FBasicInventoryItem{GivenItemName})) {

			// If it already exists, then simply create our temp item out of that and give it to the UI
			CachedInventoryWidget->MakeTempItem(FTempInventoryItem{*TargetInventoryItem, FInternalInventoryItemData{WantedCount}});
		}

		// Else, we need to manually collect the wanted item's data from scratch, so...
		else {

			// Collect a pointer to the wanted data table information
			const FInventoryItemData * WantedInventoryData = reinterpret_cast<FInventoryItemData*>(InventoryItemDataTable.LoadSynchronous()->FindRowUnchecked(GivenItemName));

			// For editor only, ensure that the collected inventory item data is always valid, else there's been a serious problem
			ensureAlways(WantedInventoryData != nullptr);

			// If the data table information is invalid, then return an invalid value (-1)
			//     NOTE: This is only staying in release builds to prevent accidental crashes and memory corruption
			if (WantedInventoryData == nullptr) {
				return -1;
			}

			// Finally, now that the data is known to be safe and accurate, create our temp inventory item and give it to the UI
			CachedInventoryWidget->MakeTempItem(FTempInventoryItem{GivenItemName, *WantedInventoryData, FInternalInventoryItemData{WantedCount}});
		}
	}

	// 
	CachedInventoryWidget->MakeVisible();
	
	// Finally, return zero, as the rest of whatever item was given (if any) is staying in the inventory temporarily
	return 0;
}


// 
TSharedPtr<SWidget> UBasicInventoryComponent::GetWidgetToFocus() {
	return CachedInventoryWidget->GetFocusWidget();
}


// 
void UBasicInventoryComponent::BindInputActions(UInputComponent * PlayerInputComponent) {

	//
	//this->CachedInventoryWidget;
}


// 
void UBasicInventoryComponent::CreateInventoryUserInterface() {

	// If our inventory user interface widget has not been created and cached yet, then...
	if (IsValid(CachedInventoryWidget) == false) {

		// Ensure that the owner of this component is a pawn
		ensureAlways(Cast<APawn>(GetOwner()));

		// Ensure that the owner of this component is always player controlled when trying to create the user interface
		ensureAlways(Cast<APawn>(GetOwner())->IsPlayerControlled());

		// Ensure that the pawn has a player controller
		ensureAlways(Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()));

		// Create and cache the inventory user interface now
		CachedInventoryWidget = CreateWidget<UAbstractInventoryWidget>(Cast<APlayerController>(Cast<APawn>(GetOwner())->GetController()), InventoryWidgetClass.Get());

		// Ensure that creation is always successful, no matter what
		ensureAlways(IsValid(CachedInventoryWidget));

		// Bind a quick lambda to pass the inventory widget's close UI delegate to this object's close UI delegate
		CachedInventoryWidget->CloseUserInterfaceDelegate.BindLambda([this]() {
			this->CloseUserInterfaceDelegate.Execute();
		});

		//
		CachedInventoryWidget->Setup(this);
	}

	// Initialize the user interface now, to ensure that its data is all up-to-date
	InitializeUserInterface();
	
	// Execute the owning object's initialize user interface function to handle input and display
	OpenUserInterfaceDelegate.Execute();

	//
	FSlateApplication::Get().ClearUserFocus(FGenericPlatformMisc::GetUserIndexForPlatformUser(CachedInventoryWidget->GetPlayerContext().GetPlayerController()->GetPlatformUserId()));

	// Set our wanted focus widget now, to ensure that we are opening the inventory with a focused widget
	CachedInventoryWidget->SetFocusWidget();
}


// 
void UBasicInventoryComponent::CloseUserInterface() {
	CachedInventoryWidget->MakeInvisible();
	CloseUserInterfaceDelegate.Execute();
}


// 
int32 UBasicInventoryComponent::AddItem(const FName ItemName, int32 WantedCount) {
	
	// If the requested amount is invalid, then exit early, as there's nothing we can do about that
	if (WantedCount <= 0) return WantedCount;

	// Ensure that we either find the item in existence or we add it now to the inventory items list
	FBasicInventoryItem & WantedInventoryItem = InventoryItems.FindOrAdd(FBasicInventoryItem{ItemName});
	
	// If this is a brand new item (e.g. the array is empty), then store that information locally
	const bool IsFullyNewItem = WantedInventoryItem.ItemDataArray.IsEmpty();

	// Locally store as a constant whether the user wants to handle adding items on a full inventory or not
	const bool ShouldUserHandleAddingOnFullInventory = CVarShouldUserHandleAddingOnFullInventory.GetValueOnGameThread();
	
	// If this is a brand new item, then...
	if (IsFullyNewItem == true) {

		// Collect a pointer to the wanted data table information
		const FInventoryItemData * WantedInventoryData = reinterpret_cast<FInventoryItemData*>(InventoryItemDataTable.LoadSynchronous()->FindRowUnchecked(ItemName));

		// For editor only, ensure that the collected inventory item data is always valid, else there's been a serious problem
		ensureAlways(WantedInventoryData != nullptr);

		// If the data table information is invalid, then remove the new item and return an invalid value (-1)
		//     NOTE: This is only staying in release builds to prevent accidental crashes and memory corruption
		if (WantedInventoryData == nullptr) {
			InventoryItems.Remove(WantedInventoryItem);
			return -1;
		}

		// Finally, now that the data is known to be safe and accurate, locally duplicate it for heavy future computational savings
		WantedInventoryItem.ItemData = *WantedInventoryData;
	}
	
	// If our inventory does not have enough slots for the requested item type, then...
	if (RemainingSlots < (WantedInventoryItem.ItemData.SizeX * WantedInventoryItem.ItemData.SizeY)) {

		// If the user wants to handle new items on a full inventory, then initialize the user interface to allow the user to handle the new item
		if (ShouldUserHandleAddingOnFullInventory == true) return InitializeUserInterface(ItemName, WantedCount);

		// Else, return the remaining wanted count, as we cannot store anymore of that amount
		return WantedCount;
	}

	// Change functionality based on the user's selecting auto place type
	switch(static_cast<EAutoPlaceType>(CVarSetAutoPlaceType.GetValueOnGameThread())) {

		// If anything goes wrong, default the auto place type to all and fallthrough to its functionality
		default:
			CVarSetAutoPlaceType->Set(static_cast<int32>(EAutoPlaceType::All));
			[[fallthrough]];

		// If we want to auto place everything, then do nothing, as the auto place logic follows this switch statement
		case EAutoPlaceType::All:
			break;

		// If we only want to auto place stackable objects (e.g. not weapons, key items, etc.)
		case EAutoPlaceType::StacksOnly:

			// If this item is not stackable, then initialize the user interface to allow the user to handle it instead
			if (WantedInventoryItem.ItemData.MaxStackCount == 1) return InitializeUserInterface(ItemName, WantedCount);

			// If the item is stackable, then do nothing, as the auto place logic follows this switch statement
			break;

		// If we only want to auto place stackable objects that are NOT combinable (e.g. not weapons, weapon attachments, key items, etc.)
		case EAutoPlaceType::StacksUnlessCombinable:

			// If this item is not stackable or is combinable, then initialize the user interface to allow the user to handle it instead
			if (WantedInventoryItem.ItemData.MaxStackCount == 1 || WantedInventoryItem.IsCombinable()) return InitializeUserInterface(ItemName, WantedCount);

			// If the item is stackable and NOT combinable, then do nothing, as the auto place logic follows this switch statement
			break;

		// Lastly, if we want nothing to auto place, then...
		case EAutoPlaceType::Disabled:

			// Initialize the user interface to allow the user to handle the new item
			return InitializeUserInterface(ItemName, WantedCount);
	}
	
	// Here we attempt to stack the item with already existing implementations of that item
	// If the item does not support stacking, then skip this step entirely
	if (WantedInventoryItem.ItemData.MaxStackCount > 1) {
		
		// Loop through all currently existing inventory items and...
		for (FInternalInventoryItemData & CurrentSubItem : WantedInventoryItem.ItemDataArray) {

			// If the current item's stack count is less than the maximum, then...
			if (CurrentSubItem.CurrentStackCount < WantedInventoryItem.ItemData.MaxStackCount) {

				// Decrement the wanted count by the number of max stack minus the current item's current stack
				WantedCount -= (WantedInventoryItem.ItemData.MaxStackCount - CurrentSubItem.CurrentStackCount);
				
				// Set the current stack count to the max stack count plus the wanted count IFF the wanted count is negative (e.g. we pulled too much and need to put some back to get wanted count to zero)
				CurrentSubItem.CurrentStackCount = (WantedInventoryItem.ItemData.MaxStackCount + (WantedCount < 0 ? WantedCount : 0));

				// Finally, if we have stacked all that we could and wanted count is now empty, then return zero in success
				if (WantedCount <= 0) return 0;
			}
		}
	}

	// Here we attempt to add the new item by first finding out if there's anywhere it can fit and then fitting it where it can
	// Create a local bit array to handle calculating slot availability
	TArray<bool> LocalSlotAvailability;
	LocalSlotAvailability.AddZeroed(MaxSize);

	// Initialize slot availability bit array now
	// Loop through the current array of inventory items, and...
	for (const FBasicInventoryItem CurrentItemType : InventoryItems) {

		// For each item, loop through its array of sub items, and...
		for (const FInternalInventoryItemData & CurrentItem : CurrentItemType.ItemDataArray) {

			// Locally store as constants the rotation, horizontal size, and vertical size of the current item
			const bool IsHorizontal = (CurrentItem.ItemRotation == EItemRotation::Rotate0);
			const uint8 HorizontalSize = (IsHorizontal ? CurrentItemType.ItemData.SizeX : CurrentItemType.ItemData.SizeY);
			const uint8 VerticalSize = (IsHorizontal ? CurrentItemType.ItemData.SizeY : CurrentItemType.ItemData.SizeX);
			
			// Loop through the current item's vertical size, and...
			for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

				// Loop through the current item's horizontal size, and...
				for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

					// Ensure that the slot being set to true is not already set to true (e.g. there are no overlapping items)
					ensureAlways(LocalSlotAvailability[CurrentItem.Position + HorizontalOffset + (VerticalOffset * ColumnCount)] != true);

					// Finally, for each index, set the current item's offset position to be a full slot (true)
					LocalSlotAvailability[CurrentItem.Position + HorizontalOffset + (VerticalOffset * ColumnCount)] = true;
				}
			}
		}
	}

	// Add a new item at the end of the array and store a reference to it for future ease of use
	FInternalInventoryItemData & TargetNewItem = WantedInventoryItem.ItemDataArray[WantedInventoryItem.ItemDataArray.AddDefaulted()];

	// Loop through all instances of the slot, and...
	for (const bool & IsCurrentSlotFull : LocalSlotAvailability) {

		// If the current slot is available, then attempt to add the item here
		if (IsCurrentSlotFull == false) {

			// Locally store as constants the base horizontal and vertical sizes for ease of use
			uint8 HorizontalSize = WantedInventoryItem.ItemData.SizeX;
			uint8 VerticalSize = WantedInventoryItem.ItemData.SizeY;

			// 
			const uint8 HorizontalTotalOffset = (HorizontalSize - 1);
			const uint8 VerticalTotalOffset = (VerticalSize - 1);

			// If the item's upper left position or bottom right position is greater than the inventory's max size, or the items upper left and upper right positions are on different rows, then reset the item, as it is invalid
			if ((TargetNewItem.Position >= MaxSize) ||
				(((TargetNewItem.Position + HorizontalTotalOffset + (VerticalTotalOffset * ColumnCount))) >= MaxSize) ||
				((TargetNewItem.Position / ColumnCount) != ((TargetNewItem.Position + HorizontalTotalOffset) / ColumnCount))) {

				// Goto label for attempting to rotate the current item if it made it farther than here before failing before
				AttemptToRotateItem:

				// Attempt to rotate the item and do the same check as above again
				if ((TargetNewItem.Position >= MaxSize) ||
					(((TargetNewItem.Position + VerticalTotalOffset + (HorizontalTotalOffset * ColumnCount))) >= MaxSize) ||
					((TargetNewItem.Position / ColumnCount) != ((TargetNewItem.Position + VerticalTotalOffset) / ColumnCount))) {

					// If this position for this item is just impossible, then skip it and move on to the next slot
					SkipToNextSlot:
					++TargetNewItem.Position; // Increment the target new item's position, as this slot is unavailable
					continue;
				}

				// If this item fits when rotated, then set its new rotation
				TargetNewItem.ItemRotation = EItemRotation::Rotate90;

				// Rotate the horizontal and vertical sizes as well for future use
				HorizontalSize = WantedInventoryItem.ItemData.SizeY;
				VerticalSize = WantedInventoryItem.ItemData.SizeX;
			}

			// Next, now that we know it generally fits, we need to ensure that we have the slots for it all, so...
			// Loop through all of the vertical offsets, and...
			for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

				// Loop through all of the horizontal offsets, and...
				for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

					// If there is even a single slot that is not available in the wanted item's location, then...
					if (LocalSlotAvailability[(TargetNewItem.Position + HorizontalOffset + (VerticalOffset * ColumnCount))] == true) {

						// If the item has not been rotated, then attempt to rotate it now
						if (TargetNewItem.ItemRotation == EItemRotation::Rotate0)
							goto AttemptToRotateItem;

						// Else, skip to the next slot, as this one has failed completely
						goto SkipToNextSlot;
					}
				}
			}

			// Decrement the wanted count by the number of max stack, as this is a brand new item
			WantedCount -= WantedInventoryItem.ItemData.MaxStackCount;
				
			// Set the current stack count to the max stack count plus the wanted count IFF the wanted count is negative (e.g. we pulled too much and need to put some back to get wanted count to zero)
			TargetNewItem.CurrentStackCount = (WantedInventoryItem.ItemData.MaxStackCount + (WantedCount < 0 ? WantedCount : 0));

			// Subtract from our remaining slots the number of slots the new item takes up
			RemainingSlots -= (HorizontalSize * VerticalSize);

			// If we have a valid cached inventory widget, then add the new item to it for future use
			if (IsValid(CachedInventoryWidget))
				CachedInventoryWidget->AddItem(WantedInventoryItem, WantedInventoryItem.ItemDataArray.FindLast(TargetNewItem));

			// Finally, if we have stacked all that we could and wanted count is now empty, then return zero in success
			if (WantedCount <= 0) return 0;

			// Loop through all of the vertical offsets, and...
			for (uint8 VerticalOffset = 0; VerticalOffset < VerticalSize; ++VerticalOffset) {

				// Loop through all of the horizontal offsets, and...
				for (uint8 HorizontalOffset = 0; HorizontalOffset < HorizontalSize; ++HorizontalOffset) {

					// Ensure that the slot being set to true is not already set to true (e.g. there are no overlapping items)
					ensureAlways(LocalSlotAvailability[TargetNewItem.Position + HorizontalOffset + (VerticalOffset * ColumnCount)] != true);

					// Update the local slot availability to take into account the new item
					LocalSlotAvailability[(TargetNewItem.Position + HorizontalOffset + (VerticalOffset * ColumnCount))] = true;
				}
			}

			// Else, we have successfully found a position for the current item, so create a new sub item placed after the current one, and continue the loop
			TargetNewItem = WantedInventoryItem.ItemDataArray[WantedInventoryItem.ItemDataArray.Emplace(FInternalInventoryItemData{static_cast<uint8>(TargetNewItem.Position + 1)})];
			continue;
		}

		// Else, increment the target new item's position, as this slot is unavailable
		++TargetNewItem.Position;
	}

	// If we have failed to add the item entirely, then...
	if (TargetNewItem.Position >= MaxSize || WantedCount > 0) {

		// If this was a brand new item entirely, then remove it completely
		if (WantedInventoryItem.ItemDataArray.Num() <= 1)
			InventoryItems.Remove(WantedInventoryItem);
		
		// Else, just remove the temporarily added sub item, as it cannot fit within the wanted inventory
		else WantedInventoryItem.ItemDataArray.RemoveAt(WantedInventoryItem.ItemDataArray.Num() - 1, 1);
	}

	// Finally, if we have any more count left due to not being able to fit any, then...
	// If the user wants to handle new items on a full inventory, then initialize the user interface to allow the user to handle the new item
	if (ShouldUserHandleAddingOnFullInventory == true)
		return InitializeUserInterface(ItemName, WantedCount);

	// Else, return the remaining wanted count, as we cannot store anymore of that amount
	return WantedCount;
}


// 
UWidget * UBasicInventoryComponent::InteractItem(UAbstractInventoryItemWidget * Item) {

	//
	return Item;
}


// 
UWidget * UBasicInventoryComponent::RemoveItem(UAbstractInventoryItemWidget * Item, const int32 WantedCount) {

	//
	ensureAlways(IsValid(Item));

	//
	const int32 ItemIndex = Item->GetItemIndex();
	
	// Ensure that the given item is always valid
	ensureAlways(Item->ItemData->ItemDataArray.IsValidIndex(ItemIndex));

	// If our wanted count is invalid, then do nothing and return the given item to be refocused
	if (WantedCount < 0)
		return Item;

	// Locally store our item data and internal item data for ease of use
	FBasicInventoryItem * ItemData = Item->ItemData;
	FInternalInventoryItemData & InternalItemData = ItemData->ItemDataArray[ItemIndex];
	
	// If we aren't removing all of the current item, then...
	if (const int32 CurrentStackCount = (ItemData->ItemData.MaxStackCount <= 1 ? 1 : InternalItemData.CurrentStackCount); CurrentStackCount > WantedCount) {

		// Remove the wanted count from the current item's stack count
		InternalItemData.CurrentStackCount -= WantedCount;

		// Update our current item's stack count to reflect the new change
		Item->UpdateStackCount();

		// Return the "removed" item, as it should still have UI focus since it was not fully removed
		return Item;
	}

	// Since we now know that this item is being permanently removed, then we need to remove it completely from the UI as well
	UWidget * NewEmptyWidget = CachedInventoryWidget->RemoveItem(Item);

	// Remove the target item now by finding the last instance of it and removing that quickly
	Item->ItemData->ItemDataArray.RemoveAt(Item->ItemData->ItemDataArray.FindLast(InternalItemData));

	// Update the remaining slots to gain back the number of slots the now removed item took up
	RemainingSlots += (Item->ItemData->ItemData.SizeX * Item->ItemData->ItemData.SizeY);

	// If the current item's total array is now empty (we removed the last of that item), then remove the item type's slot entirely
	if (Item->ItemData->ItemDataArray.IsEmpty())
		InventoryItems.Remove(*(Item->ItemData));

	// Finally, return the amount removed for future use
	return NewEmptyWidget;
}


// 
UWidget * UBasicInventoryComponent::DiscardItem(UAbstractInventoryItemWidget * Item) {
	
	// Ensure that the given item is always valid
	ensureAlways(IsValid(Item));

	//
	const int32 ItemIndex = Item->GetItemIndex();

	// Ensure that the given item is always valid
	ensureAlways(Item->ItemData->ItemDataArray.IsValidIndex(ItemIndex));

	// Since we are discarding the item in its entirety, we need to call the discard delegate to spawn it into the world
	DiscardItemDelegate.Execute(Item->ItemData->ItemName, Item->ItemData->ItemDataArray[ItemIndex].CurrentStackCount);

	// Now that we have discarded the item, next is to remove the item entirely from the inventory and UI
	return this->RemoveItem(Item, INT32_MAX);
}


// 
void UBasicInventoryComponent::MoveItemByName(const FName ItemName, const int32 ItemIndex, const uint8 NewPosition) {

	// If the given item name is a valid inventory item, then...
	if (FBasicInventoryItem * CurrentInventoryItem = InventoryItems.Find(FBasicInventoryItem{ItemName})) {

		// If the given item index is valid within the current inventory item's array, then...
		if (CurrentInventoryItem->ItemDataArray.IsValidIndex(ItemIndex)) {

			// Locally collect the current item
			FInternalInventoryItemData & CurrentItem = CurrentInventoryItem->ItemDataArray[ItemIndex];

			// Set the current item's positions to the newly given values
			CurrentItem.Position = NewPosition;
		}
	}
}


// 
void UBasicInventoryComponent::MoveItem(FInternalInventoryItemData & Item, const uint8 NewPosition) {
	Item.Position = NewPosition;
}


// 
void UBasicInventoryComponent::SwapItems(FInternalInventoryItemData & LeftItem, FInternalInventoryItemData & RightItem) {
	LeftItem.Swap(RightItem);
}


// 
void UBasicInventoryComponent::SwapItemsByName(const FName LeftItemName, const int32 LeftItemIndex, const FName RightItemName, const int32 RightItemIndex) {

	// If there is an item in the current inventory with the left item name's value, then...
	if (FBasicInventoryItem * CurrentLeftInventoryItem = InventoryItems.Find(FBasicInventoryItem{LeftItemName})) {

		// If there is an item in the current inventory with the right item name's value, then...
		if (FBasicInventoryItem * CurrentRightInventoryItem = InventoryItems.Find(FBasicInventoryItem{RightItemName})) {

			// If both given indices are valid in their respected inventory item's arrays, then...
			if (CurrentLeftInventoryItem->ItemDataArray.IsValidIndex(LeftItemIndex) && CurrentRightInventoryItem->ItemDataArray.IsValidIndex(RightItemIndex)) {

				// Swap the two items now internally, only changing their given positions
				CurrentLeftInventoryItem->ItemDataArray[LeftItemIndex].Swap(CurrentRightInventoryItem->ItemDataArray[RightItemIndex]);
			}
		}
	}
}


// 
TMap<FName, int64> UBasicInventoryComponent::GetItemStackCounts(const TArray<FName> GivenNames) const {

	// Create a local map to keep track of everything
	TMap<FName, int64> LocalItemStackMap;
	
	// Loop through the array of given names, and...
	for(const FName & CurrentItemName : GivenNames) {

		// Add the current name to the local map, giving it the collected item stack count for the current name
		LocalItemStackMap.Add(CurrentItemName, GetItemStackCount(CurrentItemName));
	}

	// Finally, return the local map
	return LocalItemStackMap;
}


// 
void UBasicInventoryComponent::GetItemStackCountsByRef(TMap<FName, int64> & CountReferenceMap) const {

	// Loop through all given pairs, and...
	for(TPair<FName, int64> & CurrentTargetStackItem : CountReferenceMap) {

		// Collect the item stack count for the current pair's name, storing it back into the given pair
		GetItemStackCountByRef(CurrentTargetStackItem.Key, CurrentTargetStackItem.Value);
	}
}


// 
int64 UBasicInventoryComponent::GetItemStackCount(const FName ItemName) const {

	// Create a local variable to keep track of the collected count
	int64 CollectedCount;

	// Call the other function to make life easier for a tiny extra cost
	GetItemStackCountByRef(ItemName, CollectedCount);

	// Finally, return the final value of the collected count
	return CollectedCount;
}


// 
void UBasicInventoryComponent::GetItemStackCountByRef(const FName ItemName, int64 & CollectedCount) const {

	// Initialize the collected count to zero, as we have found nothing so far
	CollectedCount = 0;

	// Attempt to find the wanted inventory item, and, if successful, then...
	if (const FBasicInventoryItem * TargetInventoryItem = InventoryItems.Find(FBasicInventoryItem{ItemName})) {

		// Locally store whether the current item is stackable or not, as that changes what the current stack count is
		static bool IsItemNonStackable = (TargetInventoryItem->ItemData.MaxStackCount <= 1);

		// Loop through the current inventory item's array of actual items, and...
		for(const FInternalInventoryItemData & CurrentSubItem : TargetInventoryItem->ItemDataArray) {

			// Add up each actual item's current count onto the CollectedCount reference integer
			CollectedCount += (IsItemNonStackable ? 1 : CurrentSubItem.CurrentStackCount);
		}
	}
}


// 
TSet<FBasicInventoryItem> & UBasicInventoryComponent::GetInventoryItems() {
	return InventoryItems;
}


// 
void UBasicInventoryComponent::GetInventoryItemsByRef(TSet<FBasicInventoryItem> & InventoryItemsReference) const {
	InventoryItemsReference = InventoryItems;
}


// 
const uint8 & UBasicInventoryComponent::GetSize() const {
	return MaxSize;
}


// 
void UBasicInventoryComponent::GetSizeByRef(uint8 & InventorySizeReference) const {
	InventorySizeReference = MaxSize;
}


// 
const uint8 & UBasicInventoryComponent::GetColumnSize() const {
	return ColumnCount;
}


// 
void UBasicInventoryComponent::GetColumnSizeByRef(uint8 & InventorySizeReference) const {
	InventorySizeReference = ColumnCount;
}


#if WITH_EDITORONLY_DATA
// Simple function which collects every row's name from a given data table and converts them into an array of strings
TArray<FString> UBasicInventoryComponent::GetValidInventoryItemNames() const {

	// Create a local array of strings
	TArray<FString> ValidNameArray;

	// If we have a valid data table selected, then...
	if (InventoryItemDataTable.IsNull() == false) {

		// Loop through each and every entry within the data table, and...
		for (const FName & CurrentRowName : InventoryItemDataTable.LoadSynchronous()->GetRowNames()) {

			// Convert the row's FName into a string and add it to the local string array
			ValidNameArray.Push(CurrentRowName.ToString());
		}
	}

	// Finally, return a copy of the local string array regardless of whether it's empty or not
	return ValidNameArray;
}
#endif
