// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Pawn.h"
#include "EnhancedInputSubsystems.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "ComponentVisualizer.h"
#include "AbstractCharacter.generated.h"


// 
USTRUCT(BlueprintType)
struct FCollisionModel {
	GENERATED_BODY()

// 
public:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Units="Centimeters"))
	float HalfHeight = 50.0f;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Units="Centimeters"))
	float CrouchedHalfHeight = 25.0f;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Units="Centimeters"))
	float Radius = 25.0f;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(Units="Centimeters"))
	float MaxStepHeight = 15.0f;
};


// 
UCLASS()
class ADVANCEDMOVEMENTSYSTEM_API AAbstractCharacter : public APawn {
	GENERATED_BODY()


// Protected UPROPERTY variable section
protected:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent * CharacterSkeleton = nullptr;

	// 
	UArrowComponent * BaseComponent = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCollisionModel CharacterCollisionModel;

// 
#ifdef UE_BUILD_DEBUG
	UCapsuleComponent * DebugCapsuleCollision = nullptr;
#endif


// 
protected:

	// 
	float CurrentHalfHeight = CharacterCollisionModel.HalfHeight;

	// 
	bool IsCrouching : 1;
	uint8 GaitStatus : 2;


// 
public:
	
	// Sets default values for this pawn's properties
	AAbstractCharacter();


// 
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


// 
public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) override;

// 
#ifdef UE_BUILD_DEBUG
	virtual void OnConstruction(const FTransform & Transform) override;
#endif
};
