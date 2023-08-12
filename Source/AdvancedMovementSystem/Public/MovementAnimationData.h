// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MovementAnimationData.generated.h"


// 
UENUM(BlueprintType)
enum class MovementStateEnum : uint8 {
	Grounded,
	Airborne,
	Mantling,
	Ragdoll,
	LadderClimbing,
	Rolling,
	Sliding,
	Swimming,
	Floating,
	Num
};


// 
UENUM(BlueprintType)
enum class GaitStateEnum : uint8 {
	Walking,
	FastWalking,
	Running,
	Num
};


// 
UENUM(BlueprintType)
enum class RotationStateEnum : uint8 {
	VelocityDirection,
	LookingDirection,
	Aiming,
	Num
};


// 
UENUM(BlueprintType)
enum class ViewModeEnum : uint8 {
	FirstPerson,
	ThirdPerson,
	Num
};


// 
UENUM(BlueprintType)
enum class MovementDirectionEnum : uint8 {
	Forward,
	Right,
	Left,
	Backward,
	Num
};


// 
UENUM(BlueprintType)
enum class StanceEnum : uint8 {
	Standing,
	Crouching,
	Prone,
	Num
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
	MovementStateEnum MovementState;

	// 
	UPROPERTY(BlueprintReadWrite)
	MovementStateEnum PreviousMovementState;

	// 
	UPROPERTY(BlueprintReadWrite)
	RotationStateEnum RotationMode;

	// 
	UPROPERTY(BlueprintReadWrite)
	GaitStateEnum Gait;

	// 
	UPROPERTY(BlueprintReadWrite)
	uint8 MovementAction;

	// 
	UPROPERTY(BlueprintReadWrite)
	StanceEnum Stance;

	// 
	UPROPERTY(BlueprintReadWrite)
	ViewModeEnum ViewMode;

	// 
	UPROPERTY(BlueprintReadWrite)
	uint8 OverlayState;

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
		LocalString += (FString(TEXT("AimingRotation: ")) + AimingRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorPreviousRotation: ")) + ActorPreviousRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorControlRotation: ")) + ActorControlRotation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("ActorWorldLocation: ")) + ActorWorldLocation.ToString() + FString("\n"));
		LocalString += (FString(TEXT("MovementState: ")) + FString::FromInt((int32)MovementState) + FString("\n"));
		LocalString += (FString(TEXT("PreviousMovementState: ")) + FString::FromInt((int32)PreviousMovementState) + FString("\n"));
		LocalString += (FString(TEXT("RotationMode: ")) + FString::FromInt((int32)RotationMode) + FString("\n"));
		LocalString += (FString(TEXT("Gait: ")) + FString::FromInt((int32)Gait) + FString("\n"));
		LocalString += (FString(TEXT("MovementAction: ")) + FString::FromInt(MovementAction) + FString("\n"));
		LocalString += (FString(TEXT("Stance: ")) + FString::FromInt((int32)Stance) + FString("\n"));
		LocalString += (FString(TEXT("ViewMode: ")) + FString::FromInt((int32)ViewMode) + FString("\n"));
		LocalString += (FString(TEXT("OverlayState: ")) + FString::FromInt(OverlayState) + FString("\n"));

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
