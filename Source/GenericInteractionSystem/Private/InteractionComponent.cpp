// TreeTrunkStudios (c) 2023


// 
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

	// 
	Super::BeginPlay();

	//
	InteractionActors.Empty(INTERACTION_STATIC_INDEX);
}


// 
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
		AActor * LocalActor = CurrentCollision.GetActor();

		// Locally store a pointer to the actor as the wanted Interaction interface, to see if it supports it on the C++ side of things
		//const IInteractionInterface * CollidedInteraction = Cast<IInteractionInterface>(CurrentCollision.GetActor());

		// If the current collision actor does not support the Interaction interfaces, then skip it, as it is useless
		if (LocalActor->Implements<UInteractionInterface>() == false) continue;

		// Locally store the index of our current actor within the Interactions array, if any
		int32 LocalInteractionIndex = InteractionActors.Find(LocalActor);

		// If the hit actor has not already started their Interactions, then...
		if (LocalInteractionIndex < 0) {

			// Add our new actor to the end of our current interaction actors array and update our local Interaction index to be the new index
			LocalInteractionIndex = InteractionActors.Emplace(LocalActor);

			// Start our new interactions now, giving our owning widget the type and text of the Interactions
			IInteractionInterface::Execute_ShowHint(LocalActor);
		}

		// Add our current actor's array index into the reoccurring actor bitmask, as we do not wish to remove it as of now
		ReoccurringActorBitmask |= (static_cast<BitMaskSize>(1) << LocalInteractionIndex);
	}

	// Loop through all currently existing Interaction actor's, and...
	for (int32 CurrentIndex = (InteractionActors.Num() - 1); CurrentIndex >= 0; --CurrentIndex) {

		// If our current actor's index is zero, then it did not reoccur and needs to be removed now
		if ((ReoccurringActorBitmask & (static_cast<BitMaskSize>(1) << CurrentIndex)) == 0) {

			// Stop this Interaction from further rendering
			IInteractionInterface::Execute_HideHint(InteractionActors[CurrentIndex]);

			// Remove this Interaction from the current array of Interactions
			InteractionActors.RemoveAt(CurrentIndex, 1, false);
		}
	}
}


// Interact with any and all items that are currently contained within our interaction actors buffer
void UInteractionComponent::Interact() {

	// Loop through all currently focused interactable actors, and...
	for (AActor * CurrentActor : InteractionActors) {

		// Interact with the item, where returning true means that the item is collectable, in which case...
		if (IInteractionInterface::Execute_Interact(CurrentActor) == true) {

			// Collect the current item and leave it up to the owning actor of this component on how it is dealt with
			CollectItemDelegate.Broadcast(CurrentActor);
		}
	}

	// Finally, empty out our current interaction actors array, as all items have been interacted with
	InteractionActors.Empty(INTERACTION_STATIC_INDEX);
}
