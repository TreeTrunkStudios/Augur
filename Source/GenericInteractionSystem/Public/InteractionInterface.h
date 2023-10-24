// TreeTrunkStudios (c) 2023


// 
#pragma once


//
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"


// A bunch of code relies on a pointer size equating to int64, due to UE5 blueprints (stupidly) only understanding fixed data types
//static_assert(sizeof(uintptr_t) == sizeof(int64));


// 
#ifndef INTERACTION_TRACE_TYPE
#define INTERACTION_TRACE_TYPE EAsyncTraceType::Single
#endif


// 
#ifndef INTERACTION_STATIC_INDEX
#define INTERACTION_STATIC_INDEX 4
#endif


// This class does not need to be modified
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface { GENERATED_BODY() };


// 
class GENERICINTERACTIONSYSTEM_API IInteractionInterface {
	GENERATED_BODY()


// 
public:

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool Interact();
	// virtual bool Interact_Implementation() override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ShowHint();
	// virtual void ShowHint_Implementation() override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void HideHint();
	// virtual void HideHint_Implementation() override;
};
