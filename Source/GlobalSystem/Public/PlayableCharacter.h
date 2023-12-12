// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "SubtitleComponent.h"
#include "InteractionComponent.h"
#include "BasicInventoryComponent.h"
#include "AbstractSubtitleWidget.h"
#include "PlayableAbstractCharacter.h"
#include "InteractableItem.h"
#include "SubtitleInterface.h"
#include "PlayableCharacter.generated.h"


// 
UCLASS()
class GLOBALSYSTEM_API APlayableCharacter : public APlayableAbstractCharacter {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	USubtitleComponent * SubtitleComponent = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	UInteractionComponent * InteractionComponent = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	UBasicInventoryComponent * BasicInventoryComponent = nullptr;

	// 
	UPROPERTY(BlueprintReadOnly)
	UAbstractSubtitleWidget * AbstractSubtitleWidget = nullptr;

	// 
	TSubclassOf<UAbstractSubtitleWidget> TargetSubtitleWidgetClass = nullptr;


// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	UInputMappingContext * InputMappingMenu = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	UInputAction * InputActionInteraction = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, NoClear, meta=(NoResetToDefault))
	UInputAction * InputActionOpenInventory = nullptr;


// 
protected:

	// Local trace delegate which is called (on the game thread) whenever the async geometry sweep finishes
	FTraceDelegate CameraCenterDelegate;

	// A constant collision shape which is given to the geometry sweep for visual queries
	const FCollisionShape CameraCenterCollision = FCollisionShape::MakeSphere(10.0f);

	// Locally store our visual subtitle's trace type as a static const, utilizing the larger trace type of either visual subtitles or interactions
	static constexpr EAsyncTraceType CameraCenterTraceType = (VISUAL_SUBTITLES_TRACE_TYPE >= INTERACTION_TRACE_TYPE ? VISUAL_SUBTITLES_TRACE_TYPE : INTERACTION_TRACE_TYPE);


// Section containing protected delegate handling functions
protected:

	// 
	void HandleItemInteraction(const FName & GivenItemName, AInteractableItem * GivenActor);

	//
	void HandleInteractItem(const FBasicInventoryItem * InventoryItemToUse, const int32 TargetItemArrayIndex);

	//
	void HandleTransferItem(const FName ItemToTransferName, const int32 ItemStackCount, const int32 SlotIndex);

	//
	void HandleDiscardItem(const FName ItemToDiscardName, const int32 ItemStackCount);

	//
	void HandleCustomInventoryUpgrades(const int32 ExtrasBitmask);

	//
	void HandleInventoryUserInterface();

	//
	void OpenInventoryUserInterface() const;

	//
	void CloseInventoryUserInterface() const;


// 
public:

	// Sets default values for this pawn's properties
	APlayableCharacter();


// 
protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

	// 
	void HandleCameraCenterCollision(const FTraceHandle & GivenTraceHandle, FTraceDatum & GivenTraceDatum) const;
};
