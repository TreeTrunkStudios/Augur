// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BillboardComponent.h"
#include "InteractableItem.generated.h"


// 
UCLASS()
class GENERICINTERACTIONSYSTEM_API AInteractableItem : public AActor {
	GENERATED_BODY()
	

// Section for all public variables
public:	
	
	// Create a default static mesh that is also the root component which can be set to a
	//     target static mesh anywhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent * InteractableItemMeshComponent;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBillboardComponent * ItemHintComponent;


// Section for all protected variables
protected:

	// The name utilized if this item is collectable by the player, which allows other
	//     components to collect the item's relavent information from databases elsewhere
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName InventoryItemName = NAME_None;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FName> InteractableItemNames;


// Section for all public functions
public:	
	
	// Constructor - sets default values
	AInteractableItem();

	// 
	const FName & GetInventoryItemName() const { return InventoryItemName; }

	//
	const TArray<FName> & GetInteractableItemNames() const { return InteractableItemNames; }

	// 
	virtual void OnConstruction(const FTransform & Transform) override {
		if (InteractableItemMeshComponent->GetStaticMesh()) {
			ItemHintComponent->SetRelativeLocation(FVector(0.0, 0.0, (InteractableItemMeshComponent->GetStaticMesh()->GetBounds().BoxExtent.Z * 2) + 10.0));
		}
	}

	// Function which handles the actual interaction functionality
	//     NOTE: If this returns true, then it is a collectable or mountable
	//     interactable, meaning that it is to be sent to the parent of this
	//     component to be further handled through a simple delegate
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Interact();
	// virtual bool Interact_Implementation() override;

	// Function which handles the ability to use an item on an interactable item
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool UseItem(const FName GivenObjectName = NAME_None, const int64 GivenObjectCount = 0);
	// virtual bool UseItem_Implementation(FName GivenObjectName, uint64 GivenObjectCount) override;

	// Function which handles allowing to trigger a hint of some sort to inform
	//     the player that this item, which is in the player's view, is interactable
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowHint();
	// virtual void ShowHint_Implementation() override;

	// Function which handles hiding the above function's functionality, as this
	//     item is no longer within the player's perspective
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideHint();
	// virtual void HideHint_Implementation() override;
};
