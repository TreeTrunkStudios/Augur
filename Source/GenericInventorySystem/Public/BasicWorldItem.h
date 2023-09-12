// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InventoryTypes.h"
#include "BasicWorldItem.generated.h"


// 
UCLASS()
class GENERICINVENTORYSYSTEM_API ABasicWorldItem : public AActor, public IInventoryItemInterface {
	GENERATED_BODY()
	
// 
public:	
	
	// Sets default values for this actor's properties
	ABasicWorldItem();


// 
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


// 
public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
