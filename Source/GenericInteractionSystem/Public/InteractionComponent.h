// TreeTrunkStudios (c) 2023


// Only preprocess this header once
#pragma once


// All needed includes for the current file to properly function
#include "CoreMinimal.h"
#include "InteractableItem.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


// Generic define that allows controlling the async trace type of interaction calls
#ifndef INTERACTION_TRACE_TYPE
#define INTERACTION_TRACE_TYPE EAsyncTraceType::Single
#endif


// Generic define that allows adjusting the default array size of interactable actors
#ifndef INTERACTION_STATIC_INDEX
#define INTERACTION_STATIC_INDEX 4
#endif


// Declare a C++ only delegate which can only trigger a single function,
//     given an FName and a pointer to an AInteractableItem object
DECLARE_DELEGATE_TwoParams(FCollectItem, const FName&, AInteractableItem*);


// Declare a C++ only delegate which can only trigger a single function,
//     given an FName and a pointer to an AInteractableItem object
DECLARE_DELEGATE_TwoParams(FSuccessfullyUsedItem, const FName&, uint64);


// Actor component which handles all potential basic interaction functionalities for the parent object
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GENERICINTERACTIONSYSTEM_API UInteractionComponent final : public UActorComponent {
	GENERATED_BODY()


// Section containing all public variables
public:	

	// Simple public delegate that allows the parent object to handle collections of items however they please
	FCollectItem CollectItemDelegate;

	// Simple public delegate that allows the parent object to handle collections of items however they please
	FSuccessfullyUsedItem SuccessfullyUsedItemDelegate;


// Section containing all protected variables
protected:

	// We have no clue what our maximum number of Interaction actors could be, so we need it to be dynamic
	UPROPERTY()
	TArray<AInteractableItem*> InteractableActors;


// Section containing all public functions
public:

	// Constructor - sets default values for this component's properties
	UInteractionComponent();

	// Function which handles asynchronous trace calls for potential interaction supporting actors
	void HandleInteractions(const FTraceHandle&, FTraceDatum&);

	// Interact with any and all items that are currently contained within our interaction actors buffer
	void Interact();
	void Interact(FName GivenObjectName);
	void Interact(FName GivenObjectName, int64 GivenObjectCount);

	// Called when the game starts
	virtual void BeginPlay() override;
};
