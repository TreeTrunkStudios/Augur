// TreeTrunkStudios (c) 2023


// Only build this file once
#pragma once


// Necessary includes for this file to function properly
#include <InputMappingContext.h>

#include "CoreMinimal.h"
#include "AbstractInventoryItemWidget.h"
#include "Components/ActorComponent.h"
#include "InventoryDataTypes.h"
#include "BasicInventoryComponent.generated.h"


// Forward declare the abstract inventory widget class to prevent a circular dependency
class UAbstractInventoryWidget;


// Declare all necessary delegate types here
DECLARE_DELEGATE_TwoParams(FGiveItemAndIndexDelegate, const FBasicInventoryItem*, const int32);
DECLARE_DELEGATE_TwoParams(FGivenItemNameAndStackCountDelegate, const FName, const int32);
DECLARE_DELEGATE_ThreeParams(FGivenItemNameStackCountAndTargetSlotDelegate, const FName, const int32, const int32);
DECLARE_DELEGATE_OneParam(FHandleCustomInventoryUpgradesDelegate, const int32);
DECLARE_DELEGATE(FToggleUserInterfaceDelegate);


//
USTRUCT(BlueprintType)
struct FInputActionToNavigation {
	GENERATED_BODY()

// Section containing public UPROPERTY variables
public:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputAction> WantedInputAction;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUINavigation WantedNavigation = EUINavigation::Invalid;


// Section containing public C++ functions
public:

	// Default constructor
	FInputActionToNavigation() = default;

	// Constructor when given an item type's name only
	explicit FInputActionToNavigation(const TSoftObjectPtr<UInputAction> & GivenInputAction) : WantedInputAction(GivenInputAction), WantedNavigation(EUINavigation::Invalid) {}

	// Constructor when given an item type's name and its data type
	explicit FInputActionToNavigation(const TSoftObjectPtr<UInputAction> & GivenInputAction, const EUINavigation & GivenNavigationType) : WantedInputAction(GivenInputAction), WantedNavigation(GivenNavigationType) {}

	// Copy constructor
	FInputActionToNavigation(const FInputActionToNavigation & Other) : WantedInputAction(Other.WantedInputAction), WantedNavigation(Other.WantedNavigation) {}


// Section containing public C++ operator override functions
public:
	
	// Checks to see if this and another object are equal in name (which implies equal in data as well)
	bool operator==(const FInputActionToNavigation & Other) const {
		return (WantedInputAction == Other.WantedInputAction);
	}


// Section containing public friend C++ functions
public:

	// Function for allowing this structure to be handled as a member of a set, map, or any other hash-based data structure
	friend FORCEINLINE uint32 GetTypeHash(const FInputActionToNavigation & Given) {
		return GetTypeHash(Given.WantedInputAction);
		//return FCrc::MemCrc32(&Given, sizeof(FInputActionToNavigation));
	}
};


// Basic inventory component which handles collecting dynamic inventory information from a data table and utilizing it
//     to support all different kinds of inventory systems, other than a weight-based system
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GENERICINVENTORYSYSTEM_API UBasicInventoryComponent : public UActorComponent {
	GENERATED_BODY()

// Section containing all protected UPROPERTY variables
protected:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	TSubclassOf<UAbstractInventoryWidget> InventoryWidgetClass;

	// Soft object pointer (meaning not loaded immediately) containing the database to all supported inventory items by FName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault, RequiredAssetDataTags="RowStructure=/Script/GenericInventorySystem.InventoryItemData"))
	TSoftObjectPtr<UDataTable> InventoryItemDataTable;

	// Map containing the inventory change information for the given FNames, allowing support for certain items that can expand the player's inventory by a set amount
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetValidInventoryItemNames"))
	TSet<FInventoryChangeInfo> ChangeInventoryItems;

	// Map containing the basic inventory item information for the given FNames, supporting all types of inventory items
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FBasicInventoryItem> InventoryItems;
	
	// The x-axis size of the current inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=255, UIMin=0, UIMax=255))
	uint8 MaxSize = 24;

	// The y-axis size of the current inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=255, UIMin=0, UIMax=255))
	uint8 ColumnCount = 4;

	// A simple integer to keep track of how many item slots still exist in the current inventory
	UPROPERTY(BlueprintReadOnly, meta=(ClampMin=0, ClampMax=65025, UIMin=0, UIMax=65025))
	int32 RemainingSlots = MaxSize;

	//
	UPROPERTY(BlueprintReadOnly)
	UAbstractInventoryWidget * CachedInventoryWidget;


	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UInputMappingContext> MenuInputMapping;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSet<FInputActionToNavigation> NavigationInputActions;


// Section containing all public C++ delegate variables
public:

	//
	FGiveItemAndIndexDelegate InteractItemDelegate;

	//
	FGivenItemNameAndStackCountDelegate DiscardItemDelegate;

	// 
	FGivenItemNameStackCountAndTargetSlotDelegate TransferItemDelegate;

	// 
	FHandleCustomInventoryUpgradesDelegate HandleCustomInventoryUpgradesDelegate;

	//
	FToggleUserInterfaceDelegate OpenUserInterfaceDelegate;

	//
	FToggleUserInterfaceDelegate CloseUserInterfaceDelegate;

	
// Section containing all protected C++ variables	
protected:
	
	// A simple bit array to keep track of which slots are still available in the current inventory
	//TBitArray<> IsSlotFilled{false, RemainingSlots};
	

// Section containing all public C++ functions
public:	
	
	// Sets default values for this component's properties
	UBasicInventoryComponent();


// Section containing all protected C++ functions
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;

	//
	FORCEINLINE virtual void EnsureInventoryItemSetSafety();

	//
	FORCEINLINE virtual void EnsureInventoryChangeSetSafety();

	//
	int32 InitializeUserInterface(FName GivenItemName = NAME_None, const int32 WantedCount = -1);


// Section containing all public UFUNCTION functions
public:

	//
	TSharedPtr<SWidget> GetWidgetToFocus();

	//
	UFUNCTION(BlueprintCallable)
	void BindInputActions(UInputComponent * PlayerInputComponent);

	//
	UFUNCTION(BlueprintCallable)
	void CreateInventoryUserInterface();

	//
	UFUNCTION(BlueprintCallable)
	void CloseUserInterface();

	//
	UFUNCTION(BlueprintCallable)
	int32 AddItem(const FName ItemName, int32 WantedCount);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * InteractItem(UAbstractInventoryItemWidget * Item);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * RemoveItem(UAbstractInventoryItemWidget * Item, const int32 WantedCount);

	//
	UFUNCTION(BlueprintCallable)
	UWidget * DiscardItem(UAbstractInventoryItemWidget * Item);

	//
	UFUNCTION(BlueprintCallable)
	void MoveItemByName(const FName ItemName, const int32 ItemIndex, const uint8 NewPosition);

	//
	UFUNCTION(BlueprintCallable)
	void MoveItem(FInternalInventoryItemData & Item, const uint8 NewPosition);

	//
	UFUNCTION(BlueprintCallable)
	void SwapItems(FInternalInventoryItemData & LeftItem, FInternalInventoryItemData & RightItem);

	//
	UFUNCTION(BlueprintCallable)
	void SwapItemsByName(const FName LeftItemName, const int32 LeftItemIndex, const FName RightItemName, const int32 RightItemIndex);

	//
	UFUNCTION(BlueprintCallable)
	TMap<FName, int64> GetItemStackCounts(const TArray<FName> GivenNames) const;

	//
	UFUNCTION(BlueprintCallable)
	void GetItemStackCountsByRef(TMap<FName, int64> & CountReferenceMap) const;

	//
	UFUNCTION(BlueprintCallable)
	int64 GetItemStackCount(const FName ItemName) const;

	//
	UFUNCTION(BlueprintCallable)
	void GetItemStackCountByRef(const FName ItemName, int64 & CollectedCount) const;

	//
	UFUNCTION(BlueprintCallable)
	TSet<FBasicInventoryItem> & GetInventoryItems();

	//
	UFUNCTION(BlueprintCallable)
	void GetInventoryItemsByRef(TSet<FBasicInventoryItem> & InventoryItemsReference) const;

	//
	UFUNCTION(BlueprintCallable)
	const uint8 & GetSize() const;

	//
	UFUNCTION(BlueprintCallable)
	void GetSizeByRef(uint8 & InventorySizeReference) const;

	//
	UFUNCTION(BlueprintCallable)
	const uint8 & GetColumnSize() const;

	//
	UFUNCTION(BlueprintCallable)
	void GetColumnSizeByRef(uint8 & InventorySizeReference) const;

	// Simple function which collects every row's name from a given data table and converts them into an array of strings
#if WITH_EDITORONLY_DATA
	UFUNCTION()
	TArray<FString> GetValidInventoryItemNames() const;
#endif
};
