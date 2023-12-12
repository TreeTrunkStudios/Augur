// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponUpgradeTypes.generated.h"


//
UENUM()
enum class EUpgradeTypes : uint8 {
	BulletVelocity,
	Range,
	Recoil,
	Spread,
	FireRate,
	ReloadSpeed,
	MagazineCapacity,
	Num
};


//
#define LOCTEXT_NAMESPACE "UpgradeMenuText"


// Easily convert the above enum into wanted localized text literals
inline FText ConvertUpgradeTypesToText(const EUpgradeTypes & GivenUpgradeType) {

	// Switch functionality based on the given upgrade type value
	switch(GivenUpgradeType) {

		// For each and every supported upgrade type, return it's name within a localized text format all within the same namespace
		case EUpgradeTypes::BulletVelocity:
			return LOCTEXT("BulletVelocity", "Bullet Velocity");
		case EUpgradeTypes::Range:
			return LOCTEXT("Range", "Range");
		case EUpgradeTypes::Recoil:
			return LOCTEXT("Recoil", "Recoil");
		case EUpgradeTypes::Spread:
			return LOCTEXT("Spread", "Spread");
		case EUpgradeTypes::FireRate:
			return LOCTEXT("FireRate", "Rate of Fire");
		case EUpgradeTypes::ReloadSpeed:
			return LOCTEXT("ReloadSpeed", "Reload Speed");
		case EUpgradeTypes::MagazineCapacity:
			return LOCTEXT("MagazineCapacity", "Round Capacity");

		// If there is an invalid upgrade type value given, then return a NOT localized error message
		default:
			return FText::FromString("ERROR: Unsupported upgrade type given!!");
	}
}


// Easily convert the above enum into wanted localized text literals for the post-end if wanted
inline FText ConvertUpgradeTypesToText(const EUpgradeTypes & GivenUpgradeType, const int32 & GivenValue) {

	// Create a local variable to store whichever text value we want from the given upgrade type
	FText PostOpText;

	// Switch functionality based on the given upgrade type value
	switch(GivenUpgradeType) {

		// For each and every supported upgrade type, set the post op value to the type's identifier text format all within the same namespace
		case EUpgradeTypes::BulletVelocity:
			PostOpText = LOCTEXT("BulletVelocityPost", "meters/sec");
			break;
		case EUpgradeTypes::Range:
			PostOpText = LOCTEXT("RangePost", "meters");
			break;
		case EUpgradeTypes::Recoil:
			PostOpText = LOCTEXT("RecoilPost", "joules");
			break;
		case EUpgradeTypes::Spread:
			PostOpText = LOCTEXT("SpreadPost", "meters/degree");
			break;
		case EUpgradeTypes::FireRate:
			PostOpText = LOCTEXT("FireRatePost", "rounds/sec");
			break;
		case EUpgradeTypes::ReloadSpeed:
			PostOpText = LOCTEXT("ReloadSpeedPost", "milliseconds");
			break;
		case EUpgradeTypes::MagazineCapacity:
			PostOpText = LOCTEXT("MagazineCapacityPost", "rounds");
			break;

		// If there is an invalid upgrade type value given, then set the post op value to a NOT localized NULL value
		default:
			PostOpText = FText::FromString("NULL");
			break;
	}

	// Finally, return the wanted text formatted to display the localized numeric with the localized post op text
	return FText::FormatOrdered(LOCTEXT("PostOpFormatting", "{0} {1}"), FText::AsNumber(GivenValue), PostOpText);
}


//
inline TSoftObjectPtr<UTexture2D> ConvertUpgradeTypesToIcons(const EUpgradeTypes & GivenUpgradeType) {

	// Switch functionality based on the given upgrade type value
	switch(GivenUpgradeType) {

		// For each and every supported upgrade type, return it's name within a localized text format all within the same namespace
		case EUpgradeTypes::BulletVelocity:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/BulletVelocity.BulletVelocity'")));
		case EUpgradeTypes::Range:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/Range.Range'")));
		case EUpgradeTypes::Recoil:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/Recoil.Recoil'")));
		case EUpgradeTypes::Spread:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/Spread.Spread'")));
		case EUpgradeTypes::FireRate:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/FireRate.FireRate'")));
		case EUpgradeTypes::ReloadSpeed:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/ReloadSpeed.ReloadSpeed'")));
		case EUpgradeTypes::MagazineCapacity:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/InventoryIcons/MagazineCapacity.MagazineCapacity'")));

		// If there is an invalid upgrade type value given, then return a NOT localized error message
		default:
			return TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Invalid/RedX.RedX'")));
	}
}


//
#undef LOCTEXT_NAMESPACE


//
USTRUCT(BlueprintType)
struct FPerUpgradeData {
	GENERATED_USTRUCT_BODY()

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 BaseValue;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 PerStepIncrement;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 AllocatedUpgradeBitCount;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	uint8 MaxUpgradeCount;
};


//
USTRUCT(BlueprintType)
struct FWeaponUpgradeData : public FTableRowBase {
	GENERATED_USTRUCT_BODY()
	
	//
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//FName ItemName;

	//
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EUpgradeTypes"))
	FPerUpgradeData UpgradeData[static_cast<uint8>(EUpgradeTypes::Num)];
};
