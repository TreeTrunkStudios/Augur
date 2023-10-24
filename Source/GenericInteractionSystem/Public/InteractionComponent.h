// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InteractionInterface.h"
#include "Components/ActorComponent.h"
#include "InteractionComponent.generated.h"


// 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCollectItem, AActor*, GivenInteractionActor);


// 
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GENERICINTERACTIONSYSTEM_API UInteractionComponent final : public UActorComponent {
	GENERATED_BODY()


// 
public:	

	// Sets default values for this component's properties
	UInteractionComponent();

	// 
	UPROPERTY(BlueprintAssignable)
	FCollectItem CollectItemDelegate;


// 
protected:

	// We have no clue what our maximum number of Interaction actors could be, so we need it to be dynamic
	TArray<AActor*> InteractionActors;


// 
public:

	// 
	void HandleInteractions(const FTraceHandle&, FTraceDatum&);

	// Interact with any and all items that are currently contained within our interaction actors buffer
	void Interact();

	// Called when the game starts
	virtual void BeginPlay() override;
};
