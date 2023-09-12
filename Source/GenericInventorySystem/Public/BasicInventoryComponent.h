// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryTypes.h"
#include "BasicInventoryComponent.generated.h"


// 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENERICINVENTORYSYSTEM_API UBasicInventoryComponent : public UActorComponent {
	GENERATED_BODY()

// 
public:	
	
	// Sets default values for this component's properties
	UBasicInventoryComponent();


// 
protected:
	
	// Called when the game starts
	virtual void BeginPlay() override;


// 
public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;		
};
