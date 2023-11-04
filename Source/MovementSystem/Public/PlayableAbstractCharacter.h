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
class MOVEMENTSYSTEM_API APlayableAbstractCharacter : public AAbstractCharacter {
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
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EViewMode", Units="Centimeters"))
	FVector CameraOffset[static_cast<uint8>(EViewMode::Num)];

	// 
	FVector TargetRelativeOffset = CameraOffset[static_cast<uint32>(CurrentViewMode)];
	FVector PreviousRelativeOffset = TargetRelativeOffset;

	// 
	FVector PreviousMovementInput = FVector::ZeroVector;

	// 
	UPROPERTY(EditAnywhere)
	double MinCameraAngle = -90.0;
	
	// 
	UPROPERTY(EditAnywhere)
	double MaxCameraAngle = 90.0;

	// 
	//UPROPERTY(Replicated, ReplicatedUsing="ReplicateTargetRotationHandler")
	//FRotator ReplicatedTargetRotation;

	// 
	/*UFUNCTION()
	void ReplicateTargetRotationHandler() {

		// Apply the given yaw rotation to the actor as a whole, rotating us in world space
		this->RotatorComponent->AddRelativeRotation(FRotator(0.0, ReplicatedTargetRotation.Yaw, 0.0));

		// Locally store the previous relative offset that the camera was placed into
		const FVector LocalRelativeOffset = ((CurrentViewMode == EViewMode::ThirdPerson) ? PlayerCamera->GetRelativeRotation().RotateVector(CameraOffset[(uint32)EViewMode::ThirdPerson]) : FVector::ZeroVector);

		// Lastly, apply the given pitch to just the camera, as it is the only thing that needs to rotate on that axis, while also clamping the value (no rotating your head broken)
		this->PlayerCamera->SetRelativeRotation(FRotator(ReplicatedTargetRotation.Pitch, 0.0, 0.0));

		// If third person, then add the change in relative position that should occur due to the change in rotation
		if (CurrentViewMode == EViewMode::ThirdPerson) {
			PlayerCamera->AddRelativeLocation(PlayerCamera->GetRelativeRotation().RotateVector(CameraOffset[(uint32)EViewMode::ThirdPerson]) - LocalRelativeOffset);
		}
	}*/


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

	// 
	//virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const override;


// Input handling protected functions
protected:

	// 
	//UFUNCTION(Server, Reliable)
	//virtual void HandleMovementInput_Replicated(const FVector GivenMovementVector);

	// 
	virtual void ReplicateActorWorldRotation() override {
		Super::ReplicateActorWorldRotation();
		this->PlayerCamera->SetRelativeRotation(FRotator(ReplicatedRelativeRotation.Pitch, 0.0, 0.0));
	}

	// 
	virtual void HandleMovementInput(const FInputActionInstance & Instance);

	// 
	virtual void HandleLookingInput(const FInputActionInstance & Instance);

	// 
	virtual void ToggleCameraPerspective() override;


// 
protected:

	// 
	virtual void TransferCharacterData(FArchive & GivenSaveOrLoadSystem) override;
};
