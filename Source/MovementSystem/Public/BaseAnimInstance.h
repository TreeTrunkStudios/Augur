// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MovementAnimationData.h"
#include "Curves/CurveFloat.h"
#include "Curves/CurveVector.h"
#include "Animation/AnimSequenceBase.h"
#include "BaseAnimInstance.generated.h"


// 
UCLASS()
class MOVEMENTSYSTEM_API UBaseAnimInstance : public UAnimInstance {
	GENERATED_BODY()


// 
public:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * DiagonalScaleAmountCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * StrideBlendNormalWalkCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * StrideBlendNormalRunCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * StrideBlendCrouchedWalkCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * LeanInAirCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveFloat * LandPredictionCurve;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector * YawOffsetFrontBack;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCurveVector * YawOffsetLeftRight;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * NormalTransitionRightDynamicAnim;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * NormalTransitionLeftDynamicAnim;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * StandingNormalTurnLeft90;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * CrouchingNormalTurnLeft90;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * StandingNormalTurnRight90;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * CrouchingNormalTurnRight90;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * StandingNormalTurnLeft180;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * CrouchingNormalTurnLeft180;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * StandingNormalTurnRight180;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimSequenceBase * CrouchingNormalTurnRight180;


// 
protected:

	// 
	FMovementAnimationDataStruct MovementAnimationData;

	// 
	FHitResult CollisionTestResults;

	// X = Left, Y = Right, Z = Forward
	//FVector YawTime;

	// 
	FRotator SmoothedAimingRotation;

	// 
	//FRotator SpineRotation;

	// 
	FVector2D AimingAngle;
	FVector2D SmoothedAimingAngle;

	// 
	float ElapsedDelayTime = 0.0f;

	// 
	float InputYawOffsetTime = 0.0f;
	float AimSweepTime = 0.0f;

	// 
	//float EnableAimOffset = 0.0f;

	// 
	//float BasePoseN = 0.0f;
	//float BasePoseCLF = 0.0f;

	// 
	//float SpineAdd = 0.0f;
	//float HeadAdd = 0.0f;
	//float ArmLeftAdd = 0.0f;
	//float ArmRightAdd = 0.0f;

	// 
	//float HandLeft = 0.0f;
	//float HandRight = 0.0f;
	
	// 
	//float EnableHandLeftIK = 0.0f;
	//float EnableHandRightIK = 0.0f;

	// 
	//float ArmLeftLocalSpace = 0.0f;
	//float ArmLeftMeshSpace = 0.0f;
	//float ArmRightLocalSpace = 0.0f;
	//float ArmRightMeshSpace = 0.0f;

	// 
	bool ShouldMove = false;

	// 
	bool RotateLeft = false;
	bool RotateRight = false;

	// 
	//float RotateRate = 0.0f;

	// 
	float LeftFootLockAlpha;
	FVector LeftFootLockLocation;
	FRotator LeftFootLockRotation;

	// 
	FVector LeftFootOffsetLocation;
	FRotator LeftFootOffsetRotation;

	// 
	float RightFootLockAlpha;
	FVector RightFootLockLocation;
	FRotator RightFootLockRotation;

	// 
	FVector RightFootOffsetLocation;
	FRotator RightFootOffsetRotation;

	// 
	float PelvisAlpha = 0.0f;
	FVector PelvisOffset;

	// x = front, y = back, z = left, w = right
	FVector4 VelocityBlend;
	// x = front, y = back, z = left, w = right
	FVector4 Yaw;

	// 
	float DiagonalScaleAmount;

	// 
	FVector RelativeAccelerationAmount;

	// X = LR, Y = FB
	FVector2D LeanAmount;

	// 
	float WalkRunBlend;
	float StrideBlend;
	float StandingPlayRate;
	float CrouchingPlayRate;

	// 
	EMovementDirection MovementDirection;

	// 
	bool IsDynamicMontageGateOpen = true;
	float DynamicMontageGateTimer = 0.0f;

	// 
	float RotationScale = 1.0f;

	// 
	//float FallSpeed = 0.0f;
	float LandPrediction = 0.0f;

	// 
	//float FlailRate = 0.0f;

	// 
	bool Pivot;
	float PivotTimer = 0.0f;

	// 
	bool Jumped;
	float JumpedTimer = 0.0f;

	// 
	float JumpPlayRate;

	// 
	float MainGroundedStateMachineWeight = 0.0f;

	// 
	float LookTowardsCameraStatesLookingForwardsStateWeight = 0.0f;
	float LookTowardsCameraStatesLookingLeftAndBackStateWeight = 0.0f;
	float LookTowardsCameraStatesLookingRightAndBackStateWeight = 0.0f;

	// 
	float LookTowardsCameraStatesCurrentStateTime = 0.0f;

	// 
	float StandingLocomotionStatesMovingStateWeight = 0.0f;
	float StandingLocomotionStatesStopStateWeight = 0.0f;

	// 
	float StandingLocomotionDetailMachineWeight = 0.0f;
	float StandingLocomotionDetailRunStartStateWeight = 0.0f;
	float StandingLocomotionDetailWalkToRunStateWeight = 0.0f;

	// 
	float StandingLocomotionDetailCurrentStateTime = 0.0f;

	// 
	float StandingLocomotionDetailFirstPivotRelevantAnimTimeRemaining = 0.0f;
	float StandingLocomotionDetailSecondPivotRelevantAnimTimeRemaining = 0.0f;
	float StandingLocomotionDetailWalkToRunRelevantAnimTimeRemaining = 0.0f;

	// 
	float StandingDirectionalStatesMoveLeftBackwardStateWeight = 0.0f;
	float StandingDirectionalStatesMoveRightBackwardStateWeight = 0.0f;

	// 
	float CrouchingLocomotionStatesMovingStateWeight = 0.0f;
	float CrouchingLocomotionStatesStopStateWeight = 0.0f;

	// 
	float CrouchingDirectionalStatesMoveLeftForwardStateWeight = 0.0f;
	float CrouchingDirectionalStatesMoveRightBackwardStateWeight = 0.0f;

	// 
	float JumpStatesJumpLeftFootRelevantAnimTimeRemaining = 0.0f;
	float JumpStatesJumpRightFootRelevantAnimTimeRemaining = 0.0f;

	// 
	float MainMovementStatesLandRelevantAnimTimeRemaining = 0.0f;
	

// 
public:

	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeThreadSafeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Native thread safe update override point. Executed on a worker thread just prior to graph update 
	// for linked anim instances, only called when the hosting node(s) are relevant
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	void PivotTriggered();

	// 
	UFUNCTION(BlueprintCallable)
	void JumpTriggered();

	// 
	UFUNCTION(BlueprintCallable)
	void PlayTransition(UAnimSequenceBase * Asset, const float BlendInTime, const float BlendOutTime, const float PlayRate, const float StartTime);


// 
protected:

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool GetShouldMove() const { return ShouldMove; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool GetShouldNotMove() const { return (GetShouldMove() == false); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool WasPreviouslyRolling() const { return (MovementAnimationData.PreviousMovementState == EMovementState::Rolling); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsRagdoll() const { return (MovementAnimationData.MovementState == EMovementState::Ragdoll); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotRagdoll() const { return (MovementAnimationData.MovementState != EMovementState::Ragdoll); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetSpeed() const { return MovementAnimationData.Speed; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsRunning() const { return (MovementAnimationData.Gait == EGaitState::Running); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFastWalking() const { return (MovementAnimationData.Gait == EGaitState::FastWalking); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsWalking() const { return (MovementAnimationData.Gait == EGaitState::Walking); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotWalkingAndNotMainGroundedWeightFull() const { return ((MovementAnimationData.Gait != EGaitState::Walking) && (FMath::IsNearlyEqualByULP(MainGroundedStateMachineWeight, 1.0f) == false)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotWalkingAndMainGroundedWeightFull() const { return ((MovementAnimationData.Gait != EGaitState::Walking) && FMath::IsNearlyEqualByULP(MainGroundedStateMachineWeight, 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsWalkingAndWeightGaitLow() const { return (IsWalking() && (GetCurveValue(FName(TEXT("Weight_Gait"))) < 1.2f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsPerformingAnAction() const { return ((MovementAnimationData.MovementState == EMovementState::Mantling) || (MovementAnimationData.MovementState == EMovementState::LadderClimbing) || (MovementAnimationData.MovementState == EMovementState::Rolling)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotPerformingAnAction() const { return (IsPerformingAnAction() == false); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStanding() const { return (MovementAnimationData.Stance == EStance::Standing); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsCrouching() const { return (MovementAnimationData.Stance == EStance::Crouching); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsProne() const { return (MovementAnimationData.Stance == EStance::Prone); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMovingOrRotating() const { return (GetShouldMove() || IsRotatingLeft() || IsRotatingRight()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsGrounded() const { return (MovementAnimationData.MovementState == EMovementState::Grounded); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotGrounded() const { return (MovementAnimationData.MovementState != EMovementState::Grounded); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsAirborne() const { return (MovementAnimationData.MovementState == EMovementState::Airborne); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotAirborne() const { return (MovementAnimationData.MovementState != EMovementState::Airborne); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotGroundedOrNotStanding() const { return (!IsGrounded() || !IsStanding()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool HasNoMovementInput() const { return (MovementAnimationData.HasMovementInput == false); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool HasMovementInput() const { return MovementAnimationData.HasMovementInput; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsRotatingLeft() const { return RotateLeft; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsRotatingRight() const { return RotateRight; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsRotating() const { return (IsRotatingLeft() || IsRotatingRight()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool HasMovementInputOrIsRotating() const { return (HasMovementInput() || IsRotating()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool HasMovementInputOrIsRotatingOrSpeedIsSprinting() const { return (HasMovementInputOrIsRotating() || MovementAnimationData.Speed > 650.0f); }

	// FMath::Clamp(GetSkelMeshComponent()->GetPhysicsLinearVelocity(FName(TEXT("root"))).Size() * (1.0f / 1000.0f), 0.0f, 1.0f);
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetFlailRate() const { return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 1000.0f), FVector2D(0.0f, 1.0f), GetSkelMeshComponent()->GetPhysicsLinearVelocity(FName(TEXT("root"))).Size()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetLeftFootLockLocation() const { return LeftFootLockLocation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetLeftFootLockRotation() const { return LeftFootLockRotation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetLeftFootLockAlpha() const { return LeftFootLockAlpha; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetLeftFootOffsetLocation() const { return LeftFootOffsetLocation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetLeftFootOffsetRotation() const { return LeftFootOffsetRotation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetRightFootLockLocation() const { return RightFootLockLocation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetRightFootLockRotation() const { return RightFootLockRotation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetRightFootLockAlpha() const { return RightFootLockAlpha; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetRightFootOffsetLocation() const { return RightFootOffsetLocation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetRightFootOffsetRotation() const { return RightFootOffsetRotation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetPelvisAlpha() const { return PelvisAlpha; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetPelvisOffset() const { return PelvisOffset; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetEnableLeftHandIK() const { return FMath::Lerp(0.0f, GetCurveValue(FName(TEXT("Enable_HandIK_L"))), GetCurveValue(FName(TEXT("Layering_Arm_L")))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetEnableRightHandIK() const { return FMath::Lerp(0.0f, GetCurveValue(FName(TEXT("Enable_HandIK_R"))), GetCurveValue(FName(TEXT("Layering_Arm_R")))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetSpineRotation() const { return FRotator(0.0f, AimingAngle.X * 0.25f, 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetEnableAimOffset() const { return FMath::Lerp(1.0f, 0.0f, GetCurveValue(FName(TEXT("Mask_AimOffset")))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsVelocityDirection() const { return (MovementAnimationData.RotationMode == ERotationState::VelocityDirection); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsNotVelocityDirection() const { return (MovementAnimationData.RotationMode != ERotationState::VelocityDirection); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsSmoothedLookingForward() const { return ((SmoothedAimingAngle.X >= -125.0f) && (SmoothedAimingAngle.X <= 125.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsSmoothedLookingBackRight() const { return ((SmoothedAimingAngle.X >= 130.0f) && (SmoothedAimingAngle.X <= 180.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsSmoothedLookingBackLeft() const { return ((SmoothedAimingAngle.X >= -180.0f) && (SmoothedAimingAngle.X <= -130.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsLookingLeftAndBackToLookingForwards() const { return (IsSmoothedLookingForward() && (LookTowardsCameraStatesLookingLeftAndBackStateWeight != 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsLookingForwardsToLookingLeftAndBack() const { return (IsSmoothedLookingBackLeft() && (LookTowardsCameraStatesLookingForwardsStateWeight != 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsLookingRightAndBackToLookingForwards() const { return (IsSmoothedLookingForward() && (LookTowardsCameraStatesLookingRightAndBackStateWeight != 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsLookingForwardsToLookingRightAndBack() const { return (IsSmoothedLookingBackRight() && (LookTowardsCameraStatesLookingForwardsStateWeight != 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsToNoOffset() const { return (LookTowardsCameraStatesCurrentStateTime > 2.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetYawTime() const { return FVector(GetLeftYawTime(), GetRightYawTime(), GetForwardYawTime()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetLeftYawTime() const { return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 180.0f), FVector2D(0.5f, 0.0f), FMath::Abs(SmoothedAimingAngle.X)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetRightYawTime() const { return FMath::GetMappedRangeValueClamped(FVector2D(0.0f, 180.0f), FVector2D(0.5f, 1.0f), FMath::Abs(SmoothedAimingAngle.X)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetForwardYawTime() const { return FMath::GetMappedRangeValueClamped(FVector2D(-180.0f, 180.0f), FVector2D(0.0f, 1.0f), SmoothedAimingAngle.X); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector2D GetSmoothedAimingAngle() const { return SmoothedAimingAngle; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector2D GetAimingAngle() const { return AimingAngle; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector4 GetVelocityBlend() const { return VelocityBlend; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector4 GetYaw() const { return Yaw; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetInputYawOffsetTime() const { return InputYawOffsetTime; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetElapsedDelayTime() const { return ElapsedDelayTime; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetAimSweepTime() const { return AimSweepTime; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetBasePoseStandingCurveValue() const { return GetCurveValue(FName(TEXT("BasePose_N"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetBasePoseCrouchedCurveValue() const { return GetCurveValue(FName(TEXT("BasePose_CLF"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetSpineLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Spine_Add"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetHeadLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Head_Add"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmLeftLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Arm_L_Add"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmRightLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Arm_R_Add"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetHandLeftLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Hand_L"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetHandRightLayeringCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Hand_R"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmLeftLayeringLocalSpaceCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Arm_L_LS"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmRightLayeringLocalSpaceCurveValue() const { return GetCurveValue(FName(TEXT("Layering_Arm_R_LS"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmLeftLayeringMeshSpaceCurveValue() const { return (1.0f - FMath::Floor(GetArmLeftLayeringLocalSpaceCurveValue())); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetArmRightLayeringMeshSpaceCurveValue() const { return (1.0f - FMath::Floor(GetArmRightLayeringLocalSpaceCurveValue())); }

	// FMath::Clamp(((MovementAnimationData.AimYawRate - 90.0f) * (1.0f / 180.0f) * (3.0f - 1.15f)) + 1.15f, 1.15f, 3.0f);
	// FMath::Clamp(((MovementAnimationData.AimYawRate - 90.0f) * 0.01027777777777778f) + 1.15f, 1.15f, 3.0f);
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetRotateRate() const { return FMath::GetMappedRangeValueClamped(FVector2D(90.0f, 270.0f), FVector2D(1.15f, 3.0f), MovementAnimationData.AimYawRate); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetDiagonalScaleAmount() const { return DiagonalScaleAmount; }// DiagonalScaleAmountCurve->GetFloatValue(FMath::Abs(VelocityBlend.X + VelocityBlend.Y)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetFallSpeed() const { return MovementAnimationData.Velocity.Z; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetFallSpeedAbs() const { return FMath::Abs(MovementAnimationData.Velocity.Z); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FRotator GetSmoothedAimingRotation() const { return SmoothedAimingRotation; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector GetRelativeAccelerationAmount() const { return RelativeAccelerationAmount; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline FVector2D GetLeanAmount() const { return LeanAmount; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetWalkRunBlend() const { return WalkRunBlend; }
	
	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetStrideBlend() const { return StrideBlend; }
	
	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetStandingPlayRate() const { return StandingPlayRate; }
	
	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetCrouchingPlayRate() const { return CrouchingPlayRate; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline EMovementDirection GetMovementDirection() const { return MovementDirection; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMovementDirectionForward() const { return (MovementDirection == EMovementDirection::Forward); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMovementDirectionBackward() const { return (MovementDirection == EMovementDirection::Backward); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMovementDirectionLeft() const { return (MovementDirection == EMovementDirection::Left); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMovementDirectionRight() const { return (MovementDirection == EMovementDirection::Right); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool GetIsDynamicMontageGateOpen() const { return IsDynamicMontageGateOpen; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetRotationScale() const { return RotationScale; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetLandPrediction() const { return LandPrediction; }
	
	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool GetPivot() const { return Pivot; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool GetJumped() const { return Jumped; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetJumpPlayRate() const { return JumpPlayRate; }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsCrouchedMovingToCrouchedStop() const { return (GetShouldNotMove() && (CrouchingLocomotionStatesMovingStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsCrouchedStopToCrouchedStill() const { return (CrouchingLocomotionStatesStopStateWeight == 1.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailMachineWeightFull() const { return (StandingLocomotionDetailMachineWeight == 1.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailMachineWeightNotFull() const { return (StandingLocomotionDetailMachineWeight != 1.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFeetCrossingZero() const { return (GetCurveValue(FName(TEXT("Feet_Crossing"))) == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetHipOrientationBiasCurveValue() const { return GetCurveValue(FName(TEXT("HipOrientation_Bias"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline float GetFeetPositionCurveValue() const { return GetCurveValue(FName(TEXT("Feet_Position"))); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsHipOrientationNegativeAndFeetCrossingZero() const { return ((GetHipOrientationBiasCurveValue() < -0.5f) && IsFeetCrossingZero()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsHipOrientationPositiveAndFeetCrossingZero() const { return ((GetHipOrientationBiasCurveValue() > 0.5f) && IsFeetCrossingZero()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsHipOrientationAbsSmall() const { return ((FMath::Abs(GetHipOrientationBiasCurveValue()) < 0.5f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsHipOrientationAbsLarge() const { return ((FMath::Abs(GetHipOrientationBiasCurveValue()) > 0.5f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsCrouchingHipOrientationAbsSmallAndFeetCrossingZeroAndMoveLeftForwardOne() const { return (IsHipOrientationAbsSmall() && IsFeetCrossingZero() && (CrouchingDirectionalStatesMoveLeftForwardStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsCrouchingHipOrientationAbsSmallAndFeetCrossingZeroAndMoveRightBackwardOne() const { return (IsHipOrientationAbsSmall() && IsFeetCrossingZero() && (CrouchingDirectionalStatesMoveRightBackwardStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingHipOrientationAbsSmallAndFeetCrossingZeroAndMoveLeftBackwardOne() const { return (IsHipOrientationAbsSmall() && IsFeetCrossingZero() && (StandingDirectionalStatesMoveLeftBackwardStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingHipOrientationAbsSmallAndFeetCrossingZeroAndMoveRightBackwardOne() const { return (IsHipOrientationAbsSmall() && IsFeetCrossingZero() && (StandingDirectionalStatesMoveRightBackwardStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsBasePoseCrouchedCurveValueLarge() const { return (GetBasePoseCrouchedCurveValue() >= 0.5f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsBasePoseCrouchedCurveValueSmall() const { return (GetBasePoseCrouchedCurveValue() < 0.5f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFeetPositionLeft() const { return FMath::IsNegativeOrNegativeZero(GetFeetPositionCurveValue()); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFeetPositionRight() const { return (IsFeetPositionLeft() == false); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFeetPositionAbsLocking() const { return (FMath::Abs(GetFeetPositionCurveValue()) >= 0.5f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsFeetPositionAbsPlanting() const { return (FMath::Abs(GetFeetPositionCurveValue()) < 0.5f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsJumpStatesJumpLeftFootRelevantAnimTimeRemainingZero() const { return (JumpStatesJumpLeftFootRelevantAnimTimeRemaining == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsJumpStatesJumpRightFootRelevantAnimTimeRemainingZero() const { return (JumpStatesJumpRightFootRelevantAnimTimeRemaining == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsShouldNotMoveAndStandingLocomotionStatesMovingStateWeightFull() const { return (GetShouldNotMove() && (StandingLocomotionStatesMovingStateWeight == 1.0f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionStatesStopStateWeightFull() const { return (StandingLocomotionStatesStopStateWeight == 1.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsPivotAndStandingLocomotionDetailCurrentStateTimeActive() const { return (GetPivot() && (StandingLocomotionDetailCurrentStateTime > 0.1f)); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailWalkToRunStateWeightEmpty() const { return (StandingLocomotionDetailWalkToRunStateWeight == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailRunStartStateWeightEmpty() const { return (StandingLocomotionDetailRunStartStateWeight == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailFirstPivotRelevantAnimTimeRemainingEmpty() const { return (StandingLocomotionDetailFirstPivotRelevantAnimTimeRemaining == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailSecondPivotRelevantAnimTimeRemainingEmpty() const { return (StandingLocomotionDetailSecondPivotRelevantAnimTimeRemaining == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsMainMovementStatesLandRelevantAnimTimeRemainingZero() const { return (MainMovementStatesLandRelevantAnimTimeRemaining == 0.0f); }

	// 
	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	inline bool IsStandingLocomotionDetailWalkToRunRelevantAnimTimeRemainingEmpty() const { return (StandingLocomotionDetailWalkToRunRelevantAnimTimeRemaining == 0.0f); }


// 
protected:

	// 
	void TurnInPlace(const FRotator TargetRotation, const float PlayRateScale, const float StartTime, const bool OverrideCurrent);

	// 
	void PlayDynamicTransition(const float RetriggerDelay, UAnimSequenceBase * Asset, const float BlendInTime, const float BlendOutTime, const float PlayRate, const float StartTime);
};
