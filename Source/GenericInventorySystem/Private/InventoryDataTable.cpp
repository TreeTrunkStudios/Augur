// TreeTrunkStudios (c) 2023


// 
#include "InventoryDataTable.h"


#if WITH_EDITORONLY_DATA
// Simple function which collects every row's name from a given data table and converts them into an array of strings
TArray<FString> UInventoryDataTable::GetValidInventoryItemNames() const {

	// Create a local array of strings
	TArray<FString> ValidNameArray;

	// Loop through each and every entry within the data table, and...
	for (const FName & CurrentRowName : this->GetRowNames()) {

		// Convert the row's FName into a string and add it to the local string array
		ValidNameArray.Push(CurrentRowName.ToString());
	}

	// Finally, return a copy of the local string array regardless of whether it's empty or not
	return ValidNameArray;
}
#endif
