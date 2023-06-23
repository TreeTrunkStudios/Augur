// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "AbstractCharacter.h"
#include "Camera/CameraComponent.h"
#include "CharacterData.h"
#include "PlayableAbstractCharacter.generated.h"


//
UCLASS()
class ADVANCEDMOVEMENTSYSTEM_API APlayableAbstractCharacter : public AAbstractCharacter, public ICharacterData {
	GENERATED_BODY()


// Protected UPROPERTY variable section
protected:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent * PlayerCamera = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent * RotatorComponent = nullptr;


// Protected variable section
protected:

	// 
	UInputMappingContext * InputMapping = nullptr;

	// 
	UInputAction * InputActionMovement = nullptr;
	UInputAction * InputActionLooking = nullptr;
	UInputAction * InputActionMouseLooking = nullptr;


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
