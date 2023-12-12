// TreeTrunkStudios (c) 2023


// 
#include "AbstractUpgradeSpecWidget.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Image.h"


//
void UAbstractUpgradeSpecWidget::Setup(const EUpgradeTypes & CurrentUpgradeType, const int64 & CurrentBitmaskValue, const FPerUpgradeData & CurrentUpgradeData, const TSoftObjectPtr<UTexture2D> & EmptySlotTexture, const TSoftObjectPtr<UTexture2D> & FullSlotTexture) {

	// Start by setting the spec name text to the necessary current upgrade type's value (done through a global function)
	SpecNameText->SetText(ConvertUpgradeTypesToText(CurrentUpgradeType));

	// Initialize our current upgraded value to be our base value
	int32 CurrentUpgradedValue = CurrentUpgradeData.BaseValue;

	// Loop through the max number of upgrades this category supports, and...
	for (uint8 Index = 0; Index < CurrentUpgradeData.MaxUpgradeCount; ++Index) {

		// If our current index is less than the number of upgrades this current spec contains, then...
		if (Index < CurrentBitmaskValue) {

			// Increment our current upgrade value by a single step of upgrade
			CurrentUpgradedValue += CurrentUpgradeData.PerStepIncrement;

			// If we have a valid full slot texture, then...
			if (FullSlotTexture.IsNull() == false) {

				// Create a new image widget to display the wanted full upgrade slot texture
				UImage * NewImage = WidgetTree->ConstructWidget<UImage>();

				// Initialize the image with the given full slot texture, ensuring that the image's size is fixed to the texture's size
				NewImage->SetBrushFromSoftTexture(FullSlotTexture, true);

				// Finally, add the new image to the uniform grid as for the current column (index)
				SpecVisualContainer->AddChildToUniformGrid(NewImage, 0, Index);
			}
		}

		// Else, if this is an empty upgrade and we have a valid empty slot texture, then...
		else if (FullSlotTexture.IsNull() == false && EmptySlotTexture.IsNull() == false) {

			// Create a new image widget to display the wanted empty upgrade slot texture
			UImage * NewImage = WidgetTree->ConstructWidget<UImage>();

			// Initialize the image with the given empty slot texture, ensuring that the image's size is fixed to the texture's size
			NewImage->SetBrushFromSoftTexture(EmptySlotTexture, true);

			// Finally, add the new image to the uniform grid as for the current column (index)
			SpecVisualContainer->AddChildToUniformGrid(NewImage, 0, Index);
		}

		// Else, just break from the for loop, as there's nothing else for us to do now
		else
			break;
	}

	// Finally, set our specification's wanted value text based on the upgraded value and the current type
	SpecValueText->SetText(ConvertUpgradeTypesToText(CurrentUpgradeType, CurrentUpgradedValue));
}
