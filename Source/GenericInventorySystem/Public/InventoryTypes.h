// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InventoryTypes.generated.h"


// 
UENUM()
enum class EInventoryUseType : uint8 {
	None,
	Use,
	Equip,
	Unknown
};


// 
UENUM()
enum class EItemRotation : uint8 {
	Rotate0,
	Rotate90,
	Rotate180,
	Rotate270
};


// 
UENUM()
enum class EAutoPlaceType : uint8 {

	// Ensure that we never auto place any item, causing the inventory screen to appear everytime
	None = 0b00000000,

	// If it is stackable, then we should auto place the item
	Stacks = 0b00000001,

	// If it is combinable, then we should auto place the item
	Combinables = 0b00000010,

	// If it is a key item, then we should auto place the item
	KeyItems = 0b00000100,

	// Ensure that we auto place all items, only causing the inventory screen to appear if we are out of space
	All = 0b11111111
};


//
template<bool IsReversable = true>
class TCombinationMapping;


// 
class ACombinationMapSingleton final {

// 
public:

	// 
	static ACombinationMapSingleton & Get() {

		// 
		static ACombinationMapSingleton LocalSingleton;
		return LocalSingleton;
	}


// 
private:

	// 
	ACombinationMapSingleton() = default;

	// 
	void AddCombination(TSubclassOf<AActor> ClassA, TSubclassOf<AActor> ClassB, TSubclassOf<AActor> FinalClass) {

		// 
		// 
	}

	// 
	friend class TCombinationMapping<true>;
	friend class TCombinationMapping<false>;
};


// 
template<bool IsReversable>
class TCombinationMapping final {

// All public constructors
public:

	// 
	TCombinationMapping() = delete;

	// A single public constructor 
	explicit TCombinationMapping(TSubclassOf<AActor> ClassA, TSubclassOf<AActor> ClassB, TSubclassOf<AActor> FinalClass) {

		// 
		ACombinationMapSingleton::Get().AddCombination(ClassA, ClassB, FinalClass);

		// 
		if constexpr (IsReversable == true) {
			ACombinationMapSingleton::Get().AddCombination(ClassB, ClassA, FinalClass);
		}
	}
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInventoryItemInterface : public UInterface {	GENERATED_BODY() };


// 
class GENERICINVENTORYSYSTEM_API IInventoryItemInterface {
	GENERATED_BODY()

// 
public:

	// 
	// 
};



// 
UCLASS(BlueprintType)
class UBasicInventoryTypes final : public UObject, public IInventoryItemInterface {
	GENERATED_BODY()

// 
public:

	// 
	EItemRotation ItemRotation : 2;

	// 
	uint16 PositionX : 9;

	// 
	uint16 PositionY : 9;

	// 
	uint16 CurrentStackCount : 12;

	// 
	UPROPERTY(meta=(MustImplement="InventoryItemInterface"))
	TSubclassOf<AActor> ItemClass;

	// 
	UPROPERTY(meta=(MustImplement="InventoryItemInterface"))
	TArray<TSubclassOf<AActor>> OptionalExtraItemClasses;


// 
public:

	// 
	UBasicInventoryTypes() = default;
	UBasicInventoryTypes(const TSubclassOf<AActor> & GivenItemClass) {

		// 
		check(ItemClass->ImplementsInterface(UInventoryItemInterface::StaticClass()));

		// 
		ItemClass = GivenItemClass;
	}

	// 
	bool IsCombinable() {
		return false;
	}

	// 
	UObject * GetDefaultInformation() {
		check(IsValid(ItemClass));
		return ItemClass.GetDefaultObject();
	}
};


// 
static TAutoConsoleVariable<int32> CVarAutoPlaceType(
	TEXT("inventory.pickup.autoplace"),
	static_cast<int32>(EAutoPlaceType::All),
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);
