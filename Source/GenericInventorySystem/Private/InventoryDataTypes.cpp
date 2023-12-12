// TreeTrunkStudios (c) 2023


// 
#include "InventoryDataTypes.h"


// Simple helper function to quickly swap the positions and potentially rotations of two internal inventory items
void FInternalInventoryItemData::Swap(FInternalInventoryItemData & Other, const bool ShouldSwapRotation) {

	// If we should swap the rotation as well, then do that aspect now
	if (ShouldSwapRotation == true) {
		const EItemRotation SwapRotation = this->ItemRotation;
		this->ItemRotation = Other.ItemRotation;
		Other.ItemRotation = SwapRotation;
	}

	// Swap the positions of the two items
	const uint8 SwapPosition = this->Position;
	this->Position = Other.Position;
	Other.Position = SwapPosition;
}


// TODO: Should probably only check position, as, if you have two items at the same position, then you have a real problem
bool FInternalInventoryItemData::operator==(const FInternalInventoryItemData & OtherStruct) const {
	return (/*this->ItemRotation == OtherStruct.ItemRotation && */this->Position == OtherStruct.Position/* && this->CurrentStackCount == OtherStruct.CurrentStackCount && this->UpgradeBitmask == OtherStruct.UpgradeBitmask*/);
}


//
bool FBasicInventoryItem::IsCombinable() const {
	return (ItemData.CombinableMapping.IsEmpty() == false);
}


// 
bool FBasicInventoryItem::IsCombinableWith(const FName OtherItem) const {
	return ItemData.CombinableMapping.Contains(OtherItem);
}


// 
FName FBasicInventoryItem::Combine(const FName OtherItem) const {
	if (const FName * ValidCombinedItemName = ItemData.CombinableMapping.Find(OtherItem))
		return *ValidCombinedItemName;
	return NAME_None;
}


// 
bool FBasicInventoryItem::operator==(const FBasicInventoryItem & Other) const {
	return ItemName.IsEqual(Other.ItemName);
}


// 
bool FInventoryChangeInfo::operator==(const FInventoryChangeInfo & Other) const {
	return ItemName.IsEqual(Other.ItemName);
}


// 
bool FTempInventoryItem::IsEmpty() const {
	return (ItemName.IsNone() && ItemData.CurrentStackCount < 0 && ItemData.Position == 255);
}


//
bool FTempInventoryItem::IsCombinable() const {
	return (ItemTypeData.CombinableMapping.IsEmpty() == false);
}


// 
bool FTempInventoryItem::IsCombinableWith(const FName OtherItem) const {
	return ItemTypeData.CombinableMapping.Contains(OtherItem);
}


// 
FName FTempInventoryItem::Combine(const FName OtherItem) const {
	if (const FName * ValidCombinedItemName = ItemTypeData.CombinableMapping.Find(OtherItem))
		return *ValidCombinedItemName;
	return NAME_None;
}


// 
FTempInventoryItem::FTempInventoryItem(const FBasicInventoryItem & InventoryItemType, const FInternalInventoryItemData & InternalInventoryData) {
	ItemName = InventoryItemType.ItemName;
	ItemTypeData = InventoryItemType.ItemData;
	ItemData = InternalInventoryData;
}


// 
FTempInventoryItem::FTempInventoryItem(const FBasicInventoryItem & InventoryItemType, const int32 & InternalInventoryDataIndex) {
	ensureAlways(InventoryItemType.ItemDataArray.IsValidIndex(InternalInventoryDataIndex));
	ItemName = InventoryItemType.ItemName;
	ItemTypeData = InventoryItemType.ItemData;
	ItemData = InventoryItemType.ItemDataArray[InternalInventoryDataIndex];
}


// 
bool FTempInventoryItem::operator==(const FTempInventoryItem & Other) const {
	return ItemName.IsEqual(Other.ItemName);
}
