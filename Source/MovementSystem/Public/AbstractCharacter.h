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
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "PhysicalFloorTypes.h"
#include "Components/SkeletalMeshComponent.h"
#include "AbstractCharacter.generated.h"


// 
static const double MAX_SPEED_ANGLE_MULTIPLIER = 0.2;
static const double LARGEST_SLOPE_ANGLE = FMath::Cos(FMath::DegreesToRadians(48.5));


// 
USTRUCT(BlueprintType)
struct FMovementData {
	GENERATED_BODY()

// 
public:

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EGaitState", Units="Centimeters"))
	float MaxSpeed[static_cast<uint8>(EGaitState::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EGaitState"))
	float MaxAcceleration[static_cast<uint8>(EGaitState::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EGaitState"))
	float MaxDeceleration[static_cast<uint8>(EGaitState::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EGaitState"))
	float GroundFriction[static_cast<uint8>(EGaitState::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EGaitState"))
	float RotationInterpSpeed[static_cast<uint8>(EGaitState::Num)];
};


// 
USTRUCT(BlueprintType)
struct FCollisionModel {
	GENERATED_BODY()

// 
public:

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EStance", Units="Centimeters"))
	float HalfHeight[static_cast<uint8>(EStance::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EStance", Units="Centimeters"))
	float Radius[static_cast<uint8>(EStance::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EStance", Units="Centimeters"))
	float MaxStepHeight[static_cast<uint8>(EStance::Num)];

	// 
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EStance"))
	FQuat CollisionRotation[static_cast<uint8>(EStance::Num)];
};


// 
UCLASS(Abstract, Blueprintable)
class MOVEMENTSYSTEM_API AAbstractCharacter : public APawn, public IMovementAnimationData, public ICharacterData {
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
	UPROPERTY(EditAnywhere, meta=(ArraySizeEnum="EPhysicalSurfaceType"))
	double FloorTypeSpeedMultiplier[static_cast<uint16>(EPhysicalSurfaceType::Num)];


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
	FCollisionShape WallCollisionShape;
	FCollisionShape FloorCollisionShape;

	// 
	TArray<FOverlapResult> WallOverlapCollisions;

	// 
	FQuat CurrentCollisionRotation;

	// 
	FMovementData CurrentMovementData = StandingLookingDirectionMovementData;

	// 
	float Speed = 0.0f;

	// 
	FRotator PreviousActorRotation = FRotator::ZeroRotator;

	// In meters per second
	FVector CurrentVelocity = FVector::ZeroVector;
	FVector TargetVelocity = FVector::ZeroVector;

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
	float InAirPredictionTime = -1.0f;

	// 
	EMovementState CurrentMovementState = EMovementState::Grounded;
	EMovementState PreviousMovementState = EMovementState::Grounded;

	// 
	EGaitState AllowedGait = EGaitState::FastWalking;
	EGaitState ActualGait = EGaitState::FastWalking;
	EGaitState CurrentGait = EGaitState::FastWalking;
	EGaitState DesiredGait = EGaitState::FastWalking;
	EGaitState PreviousGait = EGaitState::FastWalking;

	// 
	EStance CurrentStance = EStance::Standing;
	ERotationState RotationState = ERotationState::LookingDirection;

	// 
	bool IsMoving = false;

	// 
	FRotator InAirRotation = FRotator::ZeroRotator;

	// 
	FRotator TargetRotation = FRotator::ZeroRotator;

	// 
	EViewMode CurrentViewMode = EViewMode::FirstPerson;


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
	UFUNCTION(BlueprintCallable)
	virtual void ToggleCameraPerspective();

	// 
	virtual void SetStance(const EStance GivenStance);

	// 
	virtual void SetGait(const EGaitState GivenGait);

	// 
	virtual void SetSprint(const bool IsWantingToSprint);

	// 
	virtual void SetCameraPerspective(const EViewMode NewViewMode);


// 
protected:

	// 
	virtual double GetSpeedMultipleByAngle();

	// 
	virtual double GetSpeedMultipleByFloorType();


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
	FORCEINLINE virtual void CalculateCollisionOffset(FVector & EditableVelocity);

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
