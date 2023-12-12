// TreeTrunkStudios (c) 2023


// All needed includes for the current file to properly function
#include "InteractionComponent.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent() {

	// Set this component to be initialized when the game starts, and to never be ticked ever
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bRunOnAnyThread = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
}


// Called when the game starts
void UInteractionComponent::BeginPlay() {

	// Call the parent class' BeginPlay function
	Super::BeginPlay();

	// Empty out and reserve the default interactable actor count for interaction handling
	InteractableActors.Empty(INTERACTION_STATIC_INDEX);
}


// Function which handles asynchronous trace calls for potential interaction supporting actors
void UInteractionComponent::HandleInteractions(const FTraceHandle & GivenTraceHandle, FTraceDatum & GivenTraceDatum) {

// If our target size is less than or equal to 32, then keep the bitmask at a 32 bit size to better performance, else make it 64 bits
#if (INTERACTION_STATIC_INDEX <= 32)
	using BitMaskSize = uint32;
#else
	using BitMaskSize = uint64;
#endif

	// Create a local bitmask to handle accounting for still existing Interaction actors
	BitMaskSize ReoccurringActorBitmask = 0;

	// For each collision that occurred, loop through them in order of hit
	for (const FHitResult & CurrentCollision : GivenTraceDatum.OutHits) {

		// Locally store a const pointer to the current collision's actor rather than calling the expensive GetActor function multiple times
		AInteractableItem * LocalInteractableActor{Cast<AInteractableItem>(CurrentCollision.GetActor())};

		// If the current collided actor is not an interactable item actor, then skip it, as it is useless to us
		if (IsValid(LocalInteractableActor) == false) continue;

		// Locally store the index of our current actor within the Interactions array, if any
		int32 LocalInteractionIndex = InteractableActors.Find(LocalInteractableActor);

		// If the hit actor has not already started their Interactions, then...
		if (LocalInteractionIndex < 0) {

			// Add our new actor to the end of our current interaction actors array and update our local Interaction index to be the new index
			LocalInteractionIndex = InteractableActors.Emplace(LocalInteractableActor);

			// Start our new interactions now, giving our owning widget the type and text of the Interactions
			LocalInteractableActor->ShowHint();
		}

		// Add our current actor's array index into the reoccurring actor bitmask, as we do not wish to remove it as of now
		ReoccurringActorBitmask |= (static_cast<BitMaskSize>(1) << LocalInteractionIndex);
	}

	// Loop through all currently existing Interaction actor's, and...
	for (int32 CurrentIndex = (InteractableActors.Num() - 1); CurrentIndex >= 0; --CurrentIndex) {

		// If our current actor's index is zero, then it did not reoccur and needs to be removed now
		if ((ReoccurringActorBitmask & (static_cast<BitMaskSize>(1) << CurrentIndex)) == 0) {

			// Stop this Interaction from further rendering
			InteractableActors[CurrentIndex]->HideHint();

			// Remove this Interaction from the current array of Interactions
			InteractableActors.RemoveAt(CurrentIndex, 1, false);
		}
	}
}


// 
void UInteractionComponent::Interact() {
	this->Interact(NAME_None, 0);
}


// 
void UInteractionComponent::Interact(FName GivenObjectName) {
	this->Interact(GivenObjectName, 1);
}


// Interact with any and all items that are currently contained within our interaction actors buffer
void UInteractionComponent::Interact(FName GivenObjectName/* = NAME_None*/, int64 GivenObjectCount/* = 0*/) {

	// Loop through all currently focused interactable actors, and...
	for (AInteractableItem * CurrentActor : InteractableActors) {

		// If this is an invalid actor, then skip it and move on
		if (IsValid(CurrentActor) == false) continue;

		// If we were given an actual object name, then attempt to use the item on the current actor
		if (GivenObjectName != NAME_None && CurrentActor->UseItem(GivenObjectName, GivenObjectCount)) {

			// Inform the actor that the given item was utilized and return how much was utilized
			SuccessfullyUsedItemDelegate.Execute(GivenObjectName, GivenObjectCount);
		}

		// Else, if either we were not given an item to use, or the use item failed, then attempt to
		//     just interact with the current actor instead
		else if (CurrentActor->Interact()) {

			// Collect the current item and leave it up to the owning actor of this component on how it is dealt with
			CollectItemDelegate.Execute(CurrentActor->GetInventoryItemName(), CurrentActor);
		}
	}

	// Finally, empty out our current interaction actors array, as all items have been interacted with
	InteractableActors.Empty(INTERACTION_STATIC_INDEX);
}
