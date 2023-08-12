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
#include "Curves/CurveVector.h"
#include "MovementAnimationData.h"
#include "CharacterData.h"
#include "AbstractCharacter.generated.h"


// 
static const float LARGEST_SLOPE_ANGLE = 0.75f;


// 
USTRUCT(BlueprintType)
struct FMovementData {
	GENERATED_BODY()

// 
public:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Units="Centimeters"))
	float MaxWalkSpeed = 165.0f;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Units="Centimeters"))
	float MaxFastWalkSpeed = 350.0f;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(Units="Centimeters"))
	float MaxRunningSpeed = 600.0f;

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="GaitStateEnum"))
	float MaxAcceleration[static_cast<uint32>(GaitStateEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="GaitStateEnum"))
	float MaxDeceleration[static_cast<uint32>(GaitStateEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="GaitStateEnum"))
	float GroundFriction[static_cast<uint32>(GaitStateEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="GaitStateEnum"))
	float RotationInterpSpeed[static_cast<uint32>(GaitStateEnum::Num)];
};


// 
USTRUCT(BlueprintType)
struct FCollisionModel {
	GENERATED_BODY()

// 
public:

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="StanceEnum", Units="Centimeters"))
	float HalfHeight[static_cast<uint32>(StanceEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="StanceEnum", Units="Centimeters"))
	float Radius[static_cast<uint32>(StanceEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="StanceEnum", Units="Centimeters"))
	float MaxStepHeight[static_cast<uint32>(StanceEnum::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="StanceEnum", Units="Degrees"))
	FQuat CollisionRotation[static_cast<uint32>(StanceEnum::Num)];
};


// 
UCLASS()
class ADVANCEDMOVEMENTSYSTEM_API AAbstractCharacter : public APawn, public IMovementAnimationData, public ICharacterData {
	GENERATED_BODY()


// Protected UPROPERTY variable section
protected:

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent * CharacterSkeleton = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent * RotatorComponent = nullptr;

	//
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UArrowComponent * BaseModelComponent = nullptr;

	// 
	UArrowComponent * BaseComponent = nullptr;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FCollisionModel CharacterCollisionModel;

// 
#ifdef UE_BUILD_DEBUG
	UCapsuleComponent * DebugCapsuleCollision = nullptr;
	UArrowComponent * DebugMovementArrow = nullptr;
	UArrowComponent * DebugVelocityArrow = nullptr;
#endif

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData StandingVelocityDirectionMovementData;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData CrouchingVelocityDirectionMovementData;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData StandingLookingDirectionMovementData;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData CrouchingLookingDirectionMovementData;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData StandingAimingMovementData;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FMovementData CrouchingAimingMovementData;


// 
protected:

	// 
	float CurrentDeltaTime;

	// 
	float InverseDeltaTime;

	// 
	FVector CalculatedMovementOffset;

	// Gravity as a offset distance (e.g. units are meters rather than being an acceleration of meters per second squared)
	FVector CurrentGravityOffset;

	// 
	FVector CurrentCollisionWorldPosition;

	// 
	TWeakObjectPtr<UPrimitiveComponent> PreviousFloorComponent = nullptr;
	FTransform PreviousFloorPreviousTransform = FTransform::Identity;

	// 
	FHitResult CollisionResult;
	FCollisionQueryParams CollisionQueryParameters{NAME_None, false, this};
	//FCollisionResponseParams CollisionResponseParameters;
	FCollisionShape CollisionShape;

	// 
	FMovementData CurrentMovementData = StandingLookingDirectionMovementData;

	// 
	float Speed = 0.0f;

	// 
	FRotator PreviousActorRotation = FRotator::ZeroRotator;

	// In meters per second
	FVector CurrentVelocity = FVector::ZeroVector;
	FVector TargetVelocity = FVector::ZeroVector;
	//FVector VelocityUnitVector = FVector::ZeroVector;
	//float VelocityMagnitude = 0.0f;

	// 
	FVector PreviousVelocity = FVector::ZeroVector;
	FVector Acceleration = FVector::ZeroVector;
	float MaxAcceleration = 0.0f;

	// 
	FRotator LastVelocityRotation = FRotator::ZeroRotator;
	FRotator LastMovementInputRotation = FRotator::ZeroRotator;

	// 
	FVector MovementInput = FVector::ZeroVector;
	float MovementInputAmount = 0.0f;
	bool HasMovementInput = false;

	// 
	float AimYawRate = 0.0f;
	float PreviousAimYaw = 0.0f;

	// 
	MovementStateEnum CurrentMovementState = MovementStateEnum::Grounded;
	MovementStateEnum PreviousMovementState = MovementStateEnum::Grounded;

	// 
	GaitStateEnum AllowedGait = GaitStateEnum::FastWalking;
	GaitStateEnum ActualGait = GaitStateEnum::FastWalking;
	GaitStateEnum CurrentGait = GaitStateEnum::FastWalking;
	GaitStateEnum DesiredGait = GaitStateEnum::FastWalking;
	GaitStateEnum PreviousGait = GaitStateEnum::FastWalking;

	// 
	StanceEnum CurrentStance = StanceEnum::Standing;
	RotationStateEnum RotationState = RotationStateEnum::LookingDirection;

	// 
	float CurrentHalfHeight = CharacterCollisionModel.HalfHeight[static_cast<uint32>(CurrentStance)];
	float CurrentRadius = CharacterCollisionModel.Radius[static_cast<uint32>(CurrentStance)];
	float CurrentStepHeight = CharacterCollisionModel.MaxStepHeight[static_cast<uint32>(CurrentStance)];

	// 
	bool IsMoving = false;

	// 
	FRotator InAirRotation = FRotator::ZeroRotator;

	// 
	FRotator TargetRotation = FRotator::ZeroRotator;

	// 
	bool IsThirdPerson = false;


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

// 
#ifdef UE_BUILD_DEBUG
	virtual void OnConstruction(const FTransform & Transform) override;
#endif


// 
protected:

	// 
	void SmoothCharacterRotation(FRotator TargetRotation, const float ConstantInterpSpeed, const float SmoothInterpSpeed, const float & DeltaTime);

	// 
	virtual void ToggleStance();

	// 
	virtual void ToggleGait();

	// 
	virtual void ToggleSprint();

	// 
	virtual void ToggleCameraPerspective();

	// 
	virtual void SetStance(const StanceEnum GivenStance);

	// 
	virtual void SetGait(const GaitStateEnum GivenGait);

	// 
	virtual void SetSprint(const bool IsWantingToSprint);

	// 
	virtual void SetCameraPerspective(const bool NewIsThirdPerson);


// 
public:

	// 
	virtual void GetMovementAnimationData_Implementation(FMovementAnimationDataStruct & GivenMovementAnimationData) const override;


// 
protected:

	// 
	virtual void TransferCharacterData(FArchive & GivenSaveOrLoadSystem) override;


// 
protected:

	// 
	FORCEINLINE virtual void ToGrounded();

	// 
	FORCEINLINE virtual void ToAirborne();


// 
protected:

	// 
	FORCEINLINE virtual void CalculateGravity();

	// 
	FORCEINLINE virtual void CalculatePreviousFloor();

	// 
	FORCEINLINE virtual void CalculateCollisionOffset();

	// 
	FORCEINLINE virtual void CalculateGroundedMovement();

	// 
	FORCEINLINE virtual void CalculateAirborneMovement();

	// 
	FORCEINLINE virtual void CalculateMantlingMovement();

	// 
	FORCEINLINE virtual void CalculateRagdollMovement();

	// 
	FORCEINLINE virtual void CalculateLadderClimbingMovement();

	// 
	FORCEINLINE virtual void CalculateRollingMovement();

	// 
	FORCEINLINE virtual void CalculateSlidingMovement();

	// 
	FORCEINLINE virtual void CalculateSwimmingMovement();

	// 
	FORCEINLINE virtual void CalculateFloatingMovement();

	// 
	FORCEINLINE virtual void StorePreviousValues();

	// 
#ifdef UE_BUILD_DEBUG
	FORCEINLINE virtual void DrawDebugShapes();
#endif
};
