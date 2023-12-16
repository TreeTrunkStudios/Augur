// TreeTrunkStudios (c) 2023


// 
#include "AbstractKeyBindingOptionWidget.h"
#include "InputImageSubsystem.h"


// 
void FSingleKeySelectorData::UpdateKeyData(ULocalPlayer * OwningLocalPlayer) const {

	// If everything worked out, then set this action mapping's key as our current key selector's selected key
	InputKeySelector->SetSelectedKey(CurrentKey);

	// If we also have an image at the same index, then update that as well to reflect the newly selected key
	if (OptionalKeyImage.IsValid()) {

		//
		if (IsValid(OwningLocalPlayer) == false)
			return;

		// Store a class pointer to the current player's common input subsystem
		const UInputImageSubsystem * CurrentInputImageSubsystem = OwningLocalPlayer->GetSubsystem<UInputImageSubsystem>();

		// Ensure that the input image subsystem pointer is always properly collected (valid)
		ensureAlways(IsValid(CurrentInputImageSubsystem));

		// Set our current image to the texture of our current key
		OptionalKeyImage->SetBrushFromTexture(CurrentInputImageSubsystem->GetExactInputImage(CurrentKey));

		// Ensure that the image is visible
		OptionalKeyImage->SetVisibility(ESlateVisibility::Visible);
	}
}


//
bool UAbstractKeyBindingOptionWidget::Initialize() {

	// Locally cache the return value of our parent functionality
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
#if WITH_EDITOR
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
#endif

	//
	for (FSingleKeySelectorData & CurrentKeySelectorData : KeySelectorDataArray) {
		CurrentKeySelectorData.InputKeySelector = Cast<UInputKeySelector>(GetWidgetFromName(CurrentKeySelectorData.InputKeySelectorName));
		CurrentKeySelectorData.OptionalClearMappingButton = Cast<UButton>(GetWidgetFromName(CurrentKeySelectorData.OptionalClearMappingButtonName));
		CurrentKeySelectorData.OptionalKeyImage = Cast<UImage>(GetWidgetFromName(CurrentKeySelectorData.OptionalKeyImageName));
	}

	//
	NameText->SetText(SettingNameText);
	
	// Create an unsigned integer to store our current key selector index
	uint8 CurrentIndex = 0;
	
	// Loop through all mappings within our given input mapping context, and...
	for (const FEnhancedActionKeyMapping & CurrentActionKeyMapping : TargetInputMappingContext->GetMappings()) {

		// If we have no input key selectors left, or if we have an invalid key selector, then exit the for loop and just give up
		if (KeySelectorDataArray.IsValidIndex(CurrentIndex) == false || KeySelectorDataArray[CurrentIndex].InputKeySelector.IsValid() == false)
			break;

		// Locally store a reference to our current key selector's data for ease of use
		FSingleKeySelectorData & CurrentKeySelectorData = KeySelectorDataArray[CurrentIndex];

		// If the current input action is an invalid key, then skip it and continue
		if (TargetEscapeKeys.Contains(CurrentActionKeyMapping.Key) == true)
			SkipCurrentLoop:
			continue;

		// If the current action key mapping's action is equal to our target input action, then...
		if (CurrentActionKeyMapping.Action == TargetInputAction) {

			// Loop through all of the modifiers attached to the current input action, and...
			for (const TObjectPtr<UInputModifier> & CurrentModifier : CurrentActionKeyMapping.Modifiers) {

				// If any of the current object's modifiers do not match our target input modifiers, then skip this mapping
				if (TargetInputModifiers.Contains(CurrentModifier) == false) {
					goto SkipCurrentLoop;
				}
			}

			//
			CurrentKeySelectorData.CurrentKey = CurrentActionKeyMapping.Key;

			//
			CurrentKeySelectorData.UpdateKeyData(GetOwningLocalPlayer());

			//
			CurrentKeySelectorData.InputKeySelector->OnKeySelected.AddDynamic(this, &UAbstractKeyBindingOptionWidget::HandleStateChange);
			CurrentKeySelectorData.InputKeySelector->EscapeKeys = TargetEscapeKeys;

			//
			if (CurrentKeySelectorData.OptionalClearMappingButton.IsValid()) {
				CurrentKeySelectorData.OptionalClearMappingButton->OnClicked.AddDynamic(this, &UAbstractKeyBindingOptionWidget::HandleRemoveMappingButton);
				CurrentKeySelectorData.OptionalClearMappingButton->OnHovered.AddDynamic(this, &UWidget::SetFocus);
				FCustomWidgetNavigationDelegate Local;
				Local.BindDynamic(this, &UAbstractKeyBindingOptionWidget::HandleRemoveMapping);
				CurrentKeySelectorData.OptionalClearMappingButton->SetNavigationRuleCustom(EUINavigation::Next, Local);
			}

			// Increment our current index, as we just utilized this one
			++CurrentIndex;
		}
	}

	//
	while(KeySelectorDataArray.IsValidIndex(CurrentIndex) && KeySelectorDataArray[CurrentIndex].InputKeySelector.IsValid()) {

		//
		FSingleKeySelectorData & CurrentKeySelectorData = KeySelectorDataArray[CurrentIndex];

		//
		CurrentKeySelectorData.CurrentKey = FKey{NAME_None};

		//
		CurrentKeySelectorData.UpdateKeyData(GetOwningLocalPlayer());

		//
		CurrentKeySelectorData.InputKeySelector->OnKeySelected.AddDynamic(this, &UAbstractKeyBindingOptionWidget::HandleStateChange);
		CurrentKeySelectorData.InputKeySelector->EscapeKeys = TargetEscapeKeys;

		//
		++CurrentIndex;
	}

	//
	if (IsValid(ClearAllMappingsButton)) {
		ClearAllMappingsButton->OnClicked.AddDynamic(this, &UAbstractKeyBindingOptionWidget::HandleRemoveAllMappingsButton);
		ClearAllMappingsButton->OnHovered.AddDynamic(ClearAllMappingsButton, &UWidget::SetFocus);
		FCustomWidgetNavigationDelegate Local;
		Local.BindDynamic(this, &UAbstractKeyBindingOptionWidget::HandleRemoveAllMappings);
		ClearAllMappingsButton->SetNavigationRuleCustom(EUINavigation::Next, Local);
	}

	// Lastly, return our cached parent function's return value
	return ReturnValue;
}


// 
void UAbstractKeyBindingOptionWidget::NativeOnAddedToFocusPath(const FFocusEvent & InFocusEvent) {

	//
	DescriptionTextWidget->SetText(TargetInputAction ? TargetInputAction->ActionDescription : FText::GetEmpty());

	// 
	Super::NativeOnAddedToFocusPath(InFocusEvent);
}


// 
void UAbstractKeyBindingOptionWidget::HandleStateChange(FInputChord SelectedKey) {

	// Loop through all of our key selectors and...
	for (FSingleKeySelectorData & CurrentKeySelectorData : KeySelectorDataArray) {

		// If this is a valid key selector and is the key selector that just changed, then...
		if (CurrentKeySelectorData.InputKeySelector.IsValid() && CurrentKeySelectorData.InputKeySelector->GetSelectedKey() == SelectedKey) {

			// If the user somehow selected an escape key, then reset the key selector and leave
			if (TargetEscapeKeys.Contains(SelectedKey.Key) == true) {
				CurrentKeySelectorData.InputKeySelector->SetSelectedKey(CurrentKeySelectorData.CurrentKey);
				return;
			}

			// If the current key is valid, then unmap it from the current input mapping context
			if (CurrentKeySelectorData.CurrentKey.IsValid())
				TargetInputMappingContext->UnmapKey(TargetInputAction, CurrentKeySelectorData.CurrentKey);

			// Map the newly wanted key into our current input mapping context and update its modifiers to our target ones
			TargetInputMappingContext->MapKey(TargetInputAction, SelectedKey.Key).Modifiers = TargetInputModifiers;

			// Finally, update our current key selector's key to be the newly given key
			CurrentKeySelectorData.CurrentKey = SelectedKey.Key;
		}
	}
}


// 
void UAbstractKeyBindingOptionWidget::HandleRemoveMappingButton() {
	HandleRemoveMapping(EUINavigation::Next)->SetFocus();
}


// 
UWidget * UAbstractKeyBindingOptionWidget::HandleRemoveMapping(EUINavigation GivenNavigation) {

	// Loop through all of our key selectors and...
	for (FSingleKeySelectorData & CurrentKeySelectorData : KeySelectorDataArray) {

		// If this is the clear mapping button that just got pressed, then...
		if (CurrentKeySelectorData.OptionalClearMappingButton.IsValid() && CurrentKeySelectorData.OptionalClearMappingButton->HasUserFocus(GetPlayerContext().GetPlayerController())) {

			// Unmap this current binding's key
			TargetInputMappingContext->UnmapKey(TargetInputAction, CurrentKeySelectorData.CurrentKey);

			// Reset it to be an empty key
			CurrentKeySelectorData.CurrentKey = FKey{NAME_None};

			// Update the key selector and optional image now
			CurrentKeySelectorData.UpdateKeyData(GetOwningLocalPlayer());

			// Return our clear all mappings button, as we do not need to move mappings
			return CurrentKeySelectorData.OptionalClearMappingButton.Get();
		}
	}

	// Else, if somehow this function was called with no focused clear mapping button, then return nullptr as to not crash
	return nullptr;
}


// 
void UAbstractKeyBindingOptionWidget::HandleRemoveAllMappingsButton() {
	HandleRemoveAllMappings(EUINavigation::Next)->SetFocus();
}


// 
UWidget * UAbstractKeyBindingOptionWidget::HandleRemoveAllMappings(EUINavigation GivenNavigation) {

	// Unmap all of our current keys from our target input action within our target input mapping context
	TargetInputMappingContext->UnmapAllKeysFromAction(TargetInputAction);

	// Loop through all of our key selectors and...
	for (FSingleKeySelectorData & CurrentKeySelectorData : KeySelectorDataArray) {

		// Reset them to be empty keys
		CurrentKeySelectorData.CurrentKey = FKey{NAME_None};

		// Update the key selectors and optional images now
		CurrentKeySelectorData.UpdateKeyData(GetOwningLocalPlayer());
	}

	// Return our clear all mappings button, as we do not need to move mappings
	return ClearAllMappingsButton;
}
