// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementAnimationData.generated.h"


// 
UENUM()
enum class EMovementState : uint8 {
	Grounded,
	Airborne,
	Mantling,
	Ragdoll,
	LadderClimbing,
	Rolling,
	Sliding,
	Swimming,
	Floating,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class EGaitState : uint8 {
	Walking,
	FastWalking,
	Running,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class ERotationState : uint8 {
	VelocityDirection,
	LookingDirection,
	Aiming,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class EViewMode : uint8 {
	FirstPerson,
	ThirdPerson,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class EMovementDirection : uint8 {
	Forward,
	Right,
	Left,
	Backward,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class EStance : uint8 {
	Standing,
	Crouching,
	Prone,
	Num UMETA(Hidden)
};


// 
USTRUCT(BlueprintType)
struct FMovementAnimationDataStruct {
	GENERATED_BODY()

	// 
	UPROPERTY(BlueprintReadWrite)
	FVector Velocity;

	// 
	UPROPERTY(BlueprintReadWrite)
	FVector Acceleration;

	// 
	UPROPERTY(BlueprintReadWrite)
	FVector MovementInput;

	// 
	UPROPERTY(BlueprintReadWrite)
	bool IsMoving;

	// 
	UPROPERTY(BlueprintReadWrite)
	bool HasMovementInput;

	// 
	UPROPERTY(BlueprintReadWrite)
	float Speed;

	// 
	UPROPERTY(BlueprintReadWrite)
	float MovementInputAmount;

	// 
	UPROPERTY(BlueprintReadWrite)
	float AimYawRate;

	// 
	UPROPERTY(BlueprintReadWrite)
	float CapsuleRadius;

	// 
	UPROPERTY(BlueprintReadWrite)
	float CapsuleHalfHeight;

	// 
	UPROPERTY(BlueprintReadWrite)
	float MaxAcceleration;

	// 
	UPROPERTY(BlueprintReadWrite)
	float InAirPredictionTime;

	// 
	UPROPERTY(BlueprintReadWrite)
	FRotator AimingRotation;

	// 
	UPROPERTY(BlueprintReadWrite)
	FRotator ActorPreviousRotation;

	// 
	UPROPERTY(BlueprintReadWrite)
	FRotator ActorControlRotation;

	// 
	UPROPERTY(BlueprintReadWrite)
	FVector ActorWorldLocation;

	// 
	UPROPERTY(BlueprintReadWrite)
	EMovementState MovementState;

	// 
	UPROPERTY(BlueprintReadWrite)
	EMovementState PreviousMovementState;

	// 
	UPROPERTY(BlueprintReadWrite)
	ERotationState RotationMode;

	// 
	UPROPERTY(BlueprintReadWrite)
	EGaitState Gait;

	// 
	UPROPERTY(BlueprintReadWrite)
	EStance Stance;

	// 
	UPROPERTY(BlueprintReadWrite)
	EViewMode ViewMode;

	// 
	//UFUNCTION(BlueprintCallable, BlueprintPure, meta=(BlueprintThreadSafe))
	FString ToString() {

		// 
		FString LocalString;

		// 
		LocalString += (FString(TEXT("Velocity: ")) + Velocity.ToString() + FString("\n"));
		LocalString += (FString(TEXT("Acceleration: ")) + Acceleration.ToString() + FString("\n"));
		LocalString += (FString(TEXT("MovementInput: ")) + MovementInput.ToString() + FString("\n"));
		LocalString += (FString(TEXT("IsMoving: ")) + FString(IsMoving ? TEXT("true") : TEXT("false")) + FString("\n"));
		LocalString += (FString(TEXT("HasMovementInput: ")) + FString(HasMovementInput ? TEXT("true") : TEXT("false")) + FString("\n"));
		LocalString += (FString(TEXT("Speed: ")) + FString::SanitizeFloat(Speed) + FString("\n"));
		LocalString += (FString(TEXT("MovementInputAmount: ")) + FString::SanitizeFloat(MovementInputAmount) + FString("\n"));
		LocalString += (FString(TEXT("AimYawRate: ")) + FString::SanitizeFloat(AimYawRate) + FString("\n"));
		LocalString += (FString(TEXT("CapsuleRadius: ")) + FString::SanitizeFloat(CapsuleRadius) + FString("\n"));
		LocalString += (FString(TEXT("CapsuleHalfHeight: ")) + FString::SanitizeFloat(CapsuleHalfHeight) + FString("\n"));
		LocalString += (FString(TEXT("MaxAcceleration: ")) + FString::SanitizeFloat(MaxAcceleration) + FString("\n"));
		LocalString += (FString(TEXT("InAirPredictionTime: ")) + FString::SanitizeFloat(InAirPredictionTime) + FString("\n"));
		LocalString += (FString(TEXT("AimingRotation: ")) + AimingRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorPreviousRotation: ")) + ActorPreviousRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorControlRotation: ")) + ActorControlRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorWorldLocation: ")) + ActorWorldLocation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("MovementState: ")) + FString::FromInt((int32)MovementState) + FString("\n"));
		LocalString += (FString(TEXT("PreviousMovementState: ")) + FString::FromInt((int32)PreviousMovementState) + FString("\n"));
		LocalString += (FString(TEXT("RotationMode: ")) + FString::FromInt((int32)RotationMode) + FString("\n"));
		LocalString += (FString(TEXT("Gait: ")) + FString::FromInt((int32)Gait) + FString("\n"));
		LocalString += (FString(TEXT("Stance: ")) + FString::FromInt((int32)Stance) + FString("\n"));
		LocalString += (FString(TEXT("ViewMode: ")) + FString::FromInt((int32)ViewMode) + FString("\n"));

		// 
		return LocalString;
	}
};


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UMovementAnimationData : public UInterface {
	GENERATED_BODY()
};


// 
class ADVANCEDMOVEMENTSYSTEM_API IMovementAnimationData {
	GENERATED_BODY()


// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// 
	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	//const FMovementAnimationDataStruct GetMovementAnimationData() const;
	// virtual const FMovementAnimationDataStruct GetMovementAnimationData_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetMovementAnimationData(FMovementAnimationDataStruct & GivenMovementAnimationData) const;
	// virtual void GetMovementAnimationData_Implementation(FMovementAnimationDataStruct & GivenMovementAnimationData) const override;
};
