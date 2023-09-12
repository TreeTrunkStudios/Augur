// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputImageSubsystem.generated.h"


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class INPUTIMAGESUBSYSTEM_API UInputImageSubsystem final : public ULocalPlayerSubsystem {
	GENERATED_BODY()


// 
public:

	// 
	//UImage * GetCurrentInputImage(FKey);

	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	virtual void Deinitialize() override;

	// 
	//
};
