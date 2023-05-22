// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StoryDataBlueprints.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UStoryDataBlueprints : public UInterface {
	GENERATED_BODY()
};


// 
class ADVANCEDSAVEGAMESUBSYSTEM_API IStoryDataBlueprints {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const TSet<FName> GetStorySavePropertyNames() const; // virtual const TSet<FName> GetStorySavePropertyNames_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreStorySaveCallback(); // virtual void PreStorySaveCallback_Implementation() override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostStoryLoadCallback(); // virtual void PostStoryLoadCallback_Implementation() override;
};
