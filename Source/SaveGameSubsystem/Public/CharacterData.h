// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterData.generated.h"


// This class does not need to be modified
UINTERFACE(MinimalAPI, BlueprintType)
class UCharacterData : public UInterface {
	GENERATED_BODY()
};


// 
class SAVEGAMESUBSYSTEM_API ICharacterData {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 
	virtual void TransferCharacterData(FArchive & GivenSaveOrLoadSystem) = 0;
};
