// TreeTrunkStudios (c) 2023


// Only preprocess this file once
#pragma once


// Necessary includes to function properly
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "InventoryDataTypes.generated.h"


//
#ifndef FORCE_CLOSE_SUB_MENU_FUNCTION_NAME
#define FORCE_CLOSE_SUB_MENU_FUNCTION_NAME FName("ForceCloseSubMenu")
#endif


// Enumerator for handling automatic item inventory placement functionality on pickup
enum class EAutoPlaceType : uint8 {
	All,
	StacksOnly,
	StacksUnlessCombinable,
	Disabled
};


// Enumerator for handling what an inventory item can do from the inventory menu directly
UENUM()
enum class EInventoryUseType : uint8 {
	None,
	Use,
	Equip,
	Unknown
};


// Enumerator (though could be swapped to be a boolean right now) for handling the rotation status of the inventory item
UENUM()
enum class EItemRotation : uint8 {
	Rotate0,
	Rotate90
};


// Data table structure which contains all static information about a specific inventory item
USTRUCT(BlueprintType)
struct FInventoryItemData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()

	// This is the name utilized to display within the inventory what this object is
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText DisplayName;

	// Same as the above, but for the description of the item instead of just its name
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	// This is a soft pointer to the texture that represents this object within the inventory menu
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;

	// This is an enumerator containing what type of interaction functionality this object supports from the menu, if any
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EInventoryUseType ItemUseType;

	// Used to specify how much a single item can stack by into a single slot
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=9999, UIMin=0, UIMax=9999))
	int32 MaxStackCount;

	// Used for items that CANNOT stack, allowing them to maintain an internal buffer (e.g. for ammunition remaining in a stored firearm)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="MaxStackCount == 1"))
	int32 InternalMaxStackCount;

	// This is the size this object takes up on the x-axis
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=255, UIMin=0, UIMax=255))
	uint8 SizeX;

	// This is the size this object takes up on the y-axis
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=255, UIMin=0, UIMax=255))
	uint8 SizeY;

	// This is a mapping of names of other inventory objects that this object can combine with, forming the resulting object of name given
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetValidInventoryItemNames"))
	TMap<FName, FName> CombinableMapping;

	// 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//FRotator ExamineDefaultRotation;

	// This is a mapping of rotations that this object can be examined into that unlock objects of their correlating names
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FName, FRotator> ExamineTargetRotations;

	// This is a soft pointer (not loaded into memory) of the static mesh that is utilized whenever this object is examined
	UPROPERTY(EditAnywhere, BlueprintReadOnly)//, meta=(/*MustImplement="Interface_CollisionDataProvider", */MetaClass="StaticMesh, SkeletalMesh", AllowedClasses="StaticMesh, SkeletalMesh"))
	TSoftObjectPtr<UStaticMesh> ExamineMesh;

	// 
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//TMap<FName, TSoftObjectPtr<UStaticMesh>> ExtraMeshMap;

	// Within editor only, this converts this struct into a string for testing purposes
#if WITH_EDITORONLY_DATA
	FString ToString() const {
		return (DisplayName.ToString() + ": " + Description.ToString());
	}
#endif
};


// Structure handling the internal inventory item information of a specific item instance
USTRUCT(BlueprintType)
struct FInternalInventoryItemData {
	GENERATED_USTRUCT_BODY()

	// An extra 64 bit bitmask utilized for generic upgrading systems, in whatever way the developers want, as it is
	//     just simply passed to the owning character for handling
	UPROPERTY(BlueprintReadOnly, meta=(Bitmask))
	int64 UpgradeBitmask;

	// The current item stack count (or internal item stack count) of the current inventory item instance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=9999, UIMin=0, UIMax=9999))
	int32 CurrentStackCount;
	
	// The current rotation status of this inventory item instance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemRotation ItemRotation;

	// The current one-dimensional position of this inventory item instance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0, ClampMax=255, UIMin=0, UIMax=255))
	uint8 Position;


// Section containing public C++ functions
public:

	// Default constructor
	FInternalInventoryItemData() : UpgradeBitmask(0), CurrentStackCount(-1), ItemRotation(EItemRotation::Rotate0), Position(255) {}

	// Constructor when given a position only
	explicit FInternalInventoryItemData(const uint8 GivenPosition) : UpgradeBitmask(0), CurrentStackCount(-1), ItemRotation(EItemRotation::Rotate0), Position(GivenPosition) {}

	// Constructor when given a stack count only
	explicit FInternalInventoryItemData(const int32 GivenStackCount) : UpgradeBitmask(0), CurrentStackCount(GivenStackCount), ItemRotation(EItemRotation::Rotate0), Position(255) {}

	// Copy constructor
	explicit FInternalInventoryItemData(const FInternalInventoryItemData & Other) : UpgradeBitmask(Other.UpgradeBitmask), CurrentStackCount(Other.CurrentStackCount), ItemRotation(Other.ItemRotation), Position(Other.Position) {}

	// Internal swap functionality that swaps two given internal inventory item instances' data in-place
	void Swap(FInternalInventoryItemData & Other, const bool ShouldSwapRotation = false);


// Section containing public C++ operator override functions
public:
	
	// Could most likely dumb this down to just position checking, as no two items should ever share the same position, but, just in case, it doesn't hurt to overly check, I think
	bool operator==(const FInternalInventoryItemData & OtherStruct) const;


// Section containing public friend C++ functions
public:

	// Function for allowing this structure to be handled as a member of a set, map, or any other hash-based data structure
	friend FORCEINLINE uint32 GetTypeHash(const FInternalInventoryItemData & Given) {
		return FCrc::MemCrc32(&Given, sizeof(FInternalInventoryItemData));
	}
};


// Structure utilized to contain both an inventory item's general data, along with the array of per item instances
USTRUCT(BlueprintType)
struct FBasicInventoryItem {
	GENERATED_USTRUCT_BODY()

// Section containing public UPROPERTY variables
public:

	// The current inventory item type's name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetValidInventoryItemNames"))
	FName ItemName;

	// The current inventory item type's data table information (duplicated and cached for easier use)
	UPROPERTY(BlueprintReadOnly)
	FInventoryItemData ItemData;
	
	// The current inventory item type's array of actual item instances (must always contain at least one instance for
	//     this object to continue existing)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FInternalInventoryItemData> ItemDataArray;


// Section containing public UFUNCTION functions
public:

	// Checks if this item type can be combined with anything at all
	bool IsCombinable() const;

	// Checks if this item type can be combined with the given item type specifically
	bool IsCombinableWith(const FName OtherItem) const;

	// Attempts to combine this item type with the given item type, returning NAME_None if failed, or, if successful,
	//     returns the name of the object that is created from combining the two items
	FName Combine(const FName OtherItem) const;


// Section containing public C++ functions
public:

	// Default constructor
	FBasicInventoryItem() = default;

	// Constructor when given an item type's name only
	explicit FBasicInventoryItem(const FName GivenName) : ItemName(GivenName), ItemData() {}

	// Constructor when given an item type's name and its data type
	explicit FBasicInventoryItem(const FName GivenName, const FInventoryItemData & GivenData) : ItemName(GivenName), ItemData(GivenData) {}

	// Copy constructor
	FBasicInventoryItem(const FBasicInventoryItem & Other) : ItemName(Other.ItemName), ItemData(Other.ItemData), ItemDataArray(Other.ItemDataArray) {}


// Section containing public C++ operator override functions
public:
	
	// Checks to see if this and another object are equal in name (which implies equal in data as well)
	bool operator==(const FBasicInventoryItem & Other) const;


// Section containing public friend C++ functions
public:

	// Function for allowing this structure to be handled as a member of a set, map, or any other hash-based data structure
	friend FORCEINLINE uint32 GetTypeHash(const FBasicInventoryItem & Given) {
		return GetTypeHash(Given.ItemName);
	}
};


// Struct for handling objects which change the size and functionality of the player's inventory (e.g. RE4 remake's
//     case size upgrades or case type upgrades)
USTRUCT(BlueprintType)
struct FInventoryChangeInfo {
	GENERATED_USTRUCT_BODY()

// Section containing public UPROPERTY variables
public:

	// The name of the item which, when collected, will change the player's inventory
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetValidInventoryItemNames"))
	FName ItemName;

	// A bitmask which handles storing any and all extra functionality given by the inventory upgrade
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Bitmask))
	int32 ExtraFunctionalityBitmask;
	
	// The new size of the inventory, as a one-dimensional array for optimization purposes
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 NewSize;


// Section containing public C++ functions
public:

	// Default constructor
	FInventoryChangeInfo() = default;

	// Constructor given only an item's name
	explicit FInventoryChangeInfo(const FName GivenName) : ItemName(GivenName), ExtraFunctionalityBitmask(0), NewSize(0) {}

	// Copy constructor
	FInventoryChangeInfo(const FInventoryChangeInfo & Other) : ItemName(Other.ItemName), ExtraFunctionalityBitmask(Other.ExtraFunctionalityBitmask), NewSize(Other.NewSize) {}


// Section containing public C++ operator override functions
public:
	
	// Boolean which checks that the names are the same, which implies that all other data is equal as well
	bool operator==(const FInventoryChangeInfo & Other) const;


// Section containing public friend C++ functions
public:

	// Function for allowing this structure to be handled as a member of a set, map, or any other hash-based data structure
	friend FORCEINLINE uint32 GetTypeHash(const FInventoryChangeInfo & Given) {
		return GetTypeHash(Given.ItemName);
	}
};


// Structure that stores a temporary object's data for use when moving or combining objects within the inventory UI
//     NOTE: Same as FInternalInventoryItemData, but replaces the array of instances with a singular instance
USTRUCT(BlueprintType)
struct FTempInventoryItem {
	GENERATED_USTRUCT_BODY()

// Section containing public UPROPERTY variables
public:

	// The selected item's name
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(GetOptions="GetValidInventoryItemNames"))
	FName ItemName;

	// The selected item's type data
	UPROPERTY(BlueprintReadOnly)
	FInventoryItemData ItemTypeData;
	
	// The selected item's actual instance data
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInternalInventoryItemData ItemData;


// Section containing public UFUNCTION functions
public:

	//
	bool IsEmpty() const;

	// Checks if this item type can be combined with anything at all
	bool IsCombinable() const;

	// Checks if this item type can be combined with the given item type specifically
	bool IsCombinableWith(const FName OtherItem) const;

	// Attempts to combine this item type with the given item type, returning NAME_None if failed, or, if successful,
	//     returns the name of the object that is created from combining the two items
	FName Combine(const FName OtherItem) const;


// Section containing public C++ functions
public:

	// Default constructor
	FTempInventoryItem() = default;

	// Constructor when given an item name only
	explicit FTempInventoryItem(const FName GivenName) : ItemName(GivenName), ItemTypeData(), ItemData() {}

	// Constructor when given an item name and its data
	explicit FTempInventoryItem(const FName GivenName, const FInventoryItemData & GivenData) : ItemName(GivenName), ItemTypeData(GivenData) {}

	// Constructor when given an item name, its data, and the internal instance data by reference
	explicit FTempInventoryItem(const FName GivenName, const FInventoryItemData & GivenData, const FInternalInventoryItemData & GivenInventoryData) : ItemName(GivenName), ItemTypeData(GivenData), ItemData(GivenInventoryData) {}

	// Copy constructor
	FTempInventoryItem(const FTempInventoryItem & Other) : ItemName(Other.ItemName), ItemTypeData(Other.ItemTypeData), ItemData(Other.ItemData) {}

	// Constructor when given a basic inventory item and its internal instance by references
	FTempInventoryItem(const FBasicInventoryItem & InventoryItemType, const FInternalInventoryItemData & InternalInventoryData);

	// Constructor when given a basic inventory item by reference and the internal item's index
	FTempInventoryItem(const FBasicInventoryItem & InventoryItemType, const int32 & InternalInventoryDataIndex);


// Section containing public C++ operator override functions
public:
	
	//
	bool operator==(const FTempInventoryItem & Other) const;


// Section containing public friend C++ functions
public:

	// Function for allowing this structure to be handled as a member of a set, map, or any other hash-based data structure
	friend FORCEINLINE uint32 GetTypeHash(const FTempInventoryItem & Given) {
		return GetTypeHash(Given.ItemName);
	}
};


// Static console variable for handling how the current player wishes for their inventory to auto place, if at all
static TAutoConsoleVariable<int32> CVarSetAutoPlaceType{
	TEXT("Set Auto Place Type"),
	static_cast<int32>(EAutoPlaceType::All),
	TEXT("Utilized to change the behaviour of how new items that are picked up go into the inventory automatically."),
	ECVF_Set_NoSinkCall_Unsafe
};


// Static console variable for handling if the current player wishes to handle new items that cannot fit into the inventory as it is
static TAutoConsoleVariable<bool> CVarShouldUserHandleAddingOnFullInventory{
	TEXT("Should User Manually Handle Adding On Full Inventory."),
	true,
	TEXT("Boolean for if items that cannot fit within the inventory should be ignored or should be given to the user for deciding what to do with."),
	ECVF_Set_NoSinkCall_Unsafe
};
