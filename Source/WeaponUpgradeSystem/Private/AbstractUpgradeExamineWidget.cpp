// TreeTrunkStudios (c) 2023


// 
#include "AbstractUpgradeExamineWidget.h"
#include "WeaponUpgradeTypes.h"
#include "Blueprint/WidgetTree.h"


//
bool UAbstractUpgradeExamineWidget::Setup(const FName & ItemName, const int64 & UpgradeBitmask, const int32 & CurrentStackCount) {

	//
	if (UpgradeItemDataTable.IsNull() == false) {
		
		//
		if (const FWeaponUpgradeData * TargetUpgradeData = reinterpret_cast<FWeaponUpgradeData*>(UpgradeItemDataTable.LoadSynchronous()->FindRowUnchecked(ItemName))) {

			//
			uint8 BitCountOffset = 0;

			//
			for (uint8 Index = 0; Index < static_cast<uint8>(EUpgradeTypes::Num); ++Index) {

				//
				const FPerUpgradeData & CurrentUpgradeData = TargetUpgradeData->UpgradeData[Index];

				//
				if ((CurrentUpgradeData.AllocatedUpgradeBitCount > 0) && (CurrentUpgradeData.MaxUpgradeCount > 0)) {

					//
					BitCountOffset += CurrentUpgradeData.AllocatedUpgradeBitCount;
				}
				
				//
				const int64 CurrentUpgradeBitmask = ((~(INT64_MAX << BitCountOffset)) & UpgradeBitmask);

				//
				UAbstractUpgradeSpecWidget * NewUpgradeSpecWidget = WidgetTree->ConstructWidget<UAbstractUpgradeSpecWidget>(UpgradeSpecWidgetClass.Get());

				//
				NewUpgradeSpecWidget->Setup(static_cast<EUpgradeTypes>(Index), CurrentUpgradeBitmask, CurrentUpgradeData, EmptySlotTexture, FullSlotTexture);

				//
				SpecificationInfoContainer->AddChild(NewUpgradeSpecWidget);
			}

			//
			this->SetVisibility(ESlateVisibility::Visible);
			return true;
		}
	}

	//
	this->SetVisibility(ESlateVisibility::Collapsed);
	return false;
}


// Simple function which collects every row's name from a given data table and converts them into an array of strings
#if WITH_EDITORONLY_DATA
TArray<FString> UAbstractUpgradeExamineWidget::GetValidInventoryItemNames() const {

	// Create a local array of strings
	TArray<FString> ValidNameArray;

	// If we have a valid data table selected, then...
	if (UpgradeItemDataTable.IsNull() == false) {

		// Loop through each and every entry within the data table, and...
		for (const FName & CurrentRowName : UpgradeItemDataTable.LoadSynchronous()->GetRowNames()) {

			// Convert the row's FName into a string and add it to the local string array
			ValidNameArray.Push(CurrentRowName.ToString());
		}
	}

	// Finally, return a copy of the local string array regardless of whether it's empty or not
	return ValidNameArray;
}
#endif
