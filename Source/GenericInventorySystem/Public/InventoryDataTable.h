// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryDataTable.generated.h"


// 
UCLASS()
class GENERICINVENTORYSYSTEM_API UInventoryDataTable : public UDataTable {
	GENERATED_BODY()

// Section containing all public UFUNCTION functions
public:

	// Simple function which collects every row's name from a given data table and converts them into an array of strings
#if WITH_EDITORONLY_DATA
	UFUNCTION()
	TArray<FString> GetValidInventoryItemNames() const;
#endif
	
};
