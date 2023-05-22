// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StoryData.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UStoryData : public UInterface {
	GENERATED_BODY()
};


// 
class ADVANCEDSAVEGAMESUBSYSTEM_API IStoryData {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 
	virtual void TransferStoryData(FArchive & GivenSaveOrLoadSystem) = 0;
};
