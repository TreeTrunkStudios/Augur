// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LevelDataBlueprints.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class ULevelDataBlueprints : public UInterface {
	GENERATED_BODY()
};


// 
class SAVEGAMESUBSYSTEM_API ILevelDataBlueprints {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const TSet<FName> GetLevelSavePropertyNames() const; // virtual const TSet<FName> GetLevelSavePropertyNames_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreLevelSaveCallback(); // virtual void PreLevelSaveCallback_Implementation() override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostLevelLoadCallback(); // virtual void PostLevelLoadCallback_Implementation() override;
};
