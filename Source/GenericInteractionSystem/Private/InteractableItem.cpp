// TreeTrunkStudios (c) 2023


// 
#include "InteractableItem.h"


// Constructor - sets default values
AInteractableItem::AInteractableItem() {

 	// Ensure that this actor never ticks, or, when it does, it is asynchronous
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;
	PrimaryActorTick.bRunOnAnyThread = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.bTickEvenWhenPaused = false;

	// Create a default UStaticMeshComponent and set it as the current actor's root component
	InteractableItemMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableItemMeshComponent"));
	ensure(this->SetRootComponent(InteractableItemMeshComponent));

	// 
	ItemHintComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("ItemHintComponent"));
	ItemHintComponent->SetupAttachment(this->RootComponent);
	ItemHintComponent->SetVisibility(false);
	ItemHintComponent->bIsScreenSizeScaled = true;
	ItemHintComponent->SetHiddenInGame(false);
	ItemHintComponent->SetHiddenInSceneCapture(true);
	ItemHintComponent->SetCastHiddenShadow(false);
}


// 
bool AInteractableItem::Interact_Implementation() {

	// Default the basic class to return true so that we can do a simple drop-in inventory item at zero extra cost
	return true;
}


// 
bool AInteractableItem::UseItem_Implementation(const FName GivenObjectName, const int64 GivenObjectCount) {

	// Default the basic class to return true so that we can do a simple drop-in inventory item at zero extra cost
	return (InteractableItemNames.Contains(GivenObjectName) && GivenObjectCount > 0);
}


// 
void AInteractableItem::ShowHint_Implementation() {

	// TODO: Default this functionality to show/fade in a billboard above the static mesh that displays a wanted material
	// 
	ItemHintComponent->SetVisibility(true);
}


// 
void AInteractableItem::HideHint_Implementation() {

	// TODO: Default this functionality to hide the shown billboard by fading it out quickly
	// 
	ItemHintComponent->SetVisibility(false);
}
