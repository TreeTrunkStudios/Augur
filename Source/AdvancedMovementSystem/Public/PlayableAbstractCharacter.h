// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractCharacter.h"
#include "Camera/CameraComponent.h"
#include "PlayableAbstractCharacter.generated.h"


//
UCLASS()
class ADVANCEDMOVEMENTSYSTEM_API APlayableAbstractCharacter : public AAbstractCharacter {
	GENERATED_BODY()


// Protected UPROPERTY variable section
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent * PlayerCamera = nullptr;


// Protected variable section
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputMappingContext * InputMapping = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionMovement = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionLooking = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionMouseLooking = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionChangeGait = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionChangeStance = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UInputAction * InputActionChangeSprint = nullptr;


// 
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector FirstPersonCameraOffset;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector ThirdPersonCameraOffset;


// 
public:

	// Sets default values for this pawn's properties
	APlayableAbstractCharacter();


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


// Input handling protected functions
protected:

	// 
	virtual void HandleMovementInput(const FInputActionInstance & Instance);

	// 
	virtual void HandleLookingInput(const FInputActionInstance & Instance);


// 
protected:

	// 
	virtual void TransferCharacterData(FArchive & GivenSaveOrLoadSystem) override;
};
