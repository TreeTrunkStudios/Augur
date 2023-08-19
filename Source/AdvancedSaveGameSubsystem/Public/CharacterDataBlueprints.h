// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterDataBlueprints.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCharacterDataBlueprints : public UInterface {
	GENERATED_BODY()
};


// 
class ADVANCEDSAVEGAMESUBSYSTEM_API ICharacterDataBlueprints {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const TSet<FName> GetCharacterSavePropertyNames() const;
	// virtual const TSet<FName> GetCharacterSavePropertyNames_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PreCharacterSaveCallback();
	// virtual void PreCharacterSaveCallback_Implementation() override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PostCharacterLoadCallback();
	// virtual void PostCharacterLoadCallback_Implementation() override;
};
