// Sidney Towers
// TreeTrunkStudios (c) 2023


// All required include files
#include "AbstractCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


// 
#define SHOULD_HAVE_ACCURATE_PREVIOUS_FLOOR_TRANSFORMS


// Sets default values
AAbstractCharacter::AAbstractCharacter() {

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.bRunOnAnyThread = false;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryActorTick.EndTickGroup = ETickingGroup::TG_EndPhysics; //TG_StartPhysics

	// 
	BaseComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("BaseComponent"));
	BaseComponent->SetRelativeTransform(FTransform::Identity);
	this->SetRootComponent(BaseComponent);

	// 
	BaseModelComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("BaseModelComponent"));
	BaseModelComponent->SetupAttachment(BaseComponent);
	BaseModelComponent->SetRelativeTransform(FTransform::Identity);
	BaseModelComponent->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// 
	CharacterSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterSkeleton"));
	CharacterSkeleton->SetupAttachment(BaseModelComponent);
	CharacterSkeleton->SetRelativeTransform(FTransform::Identity);

	// Create and initialize the player camera component
	RotatorComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("RotatorComponent"));
	RotatorComponent->SetupAttachment(BaseComponent);
	RotatorComponent->SetRelativeTransform(FTransform::Identity);

	// 
#ifdef UE_BUILD_DEBUG
	DebugCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DebugCapsuleCollision"));
	DebugCapsuleCollision->SetRelativeTransform(FTransform::Identity);
	DebugCapsuleCollision->SetupAttachment(BaseComponent);
	DebugCapsuleCollision->SetHiddenInGame(false);

	DebugMovementArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DebugMovementArrow"));
	DebugMovementArrow->SetRelativeTransform(FTransform::Identity);
	DebugMovementArrow->SetupAttachment(BaseComponent);
	DebugMovementArrow->SetHiddenInGame(false);
	DebugMovementArrow->SetArrowColor(FLinearColor::Blue);
	DebugMovementArrow->ArrowSize = 0.95f;

	DebugVelocityArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("DebugVelocityArrow"));
	DebugVelocityArrow->SetRelativeTransform(FTransform::Identity);
	DebugVelocityArrow->SetupAttachment(BaseComponent);
	DebugVelocityArrow->SetHiddenInGame(false);
	DebugVelocityArrow->SetArrowColor(FLinearColor::Yellow);
#endif

	// 
	this->SetActorRelativeTransform(FTransform::Identity);
}


// Called when the game starts or when spawned
void AAbstractCharacter::BeginPlay() {

	// Call parent class' begin play functionality first
	Super::BeginPlay();

	// 
	this->RootComponent->SetWorldRotation(FRotator(0.0));
	
	// 
	DrawDebugShapes();
}


// Called every frame
void AAbstractCharacter::Tick(float DeltaTime) {

	// Call parent class' tick functionality first
	Super::Tick(DeltaTime);

	//// Set Essential Values
	// Locally store the current delta time given by the Tick function for easier access within movement functions rather than passing it all
	CurrentDeltaTime = DeltaTime;

	// Locally store the inverse of the delta time, as it is used multiple times and it is better, performance-wise, to locally
	//     store the division to invert it once and multiply it after that, then it is to do all of those individual divisions
	InverseDeltaTime = (1.0f / DeltaTime);

	// Initialize our calculated offset to be zero, as we should start not moving
	CalculatedMovementOffset = FVector::ZeroVector;

	// Consume the given movement input vector and locally store it for more simplistic access
	MovementInput = this->ConsumeMovementInputVector().GetClampedToMaxSize(1.0f);


	// Calculate movement input variables
	MovementInputAmount = MovementInput.Size();
	HasMovementInput = (MovementInputAmount > 0.0f);

	// If we have movement input, then calculate the rotation of said movement input
	if (HasMovementInput) LastMovementInputRotation = MovementInput.ToOrientationRotator();

	// Calculate the aim yaw rate by getting the difference in aim yaws
	AimYawRate = FMath::Abs((this->RotatorComponent->GetRelativeRotation().Yaw - PreviousAimYaw) * InverseDeltaTime);


	// Ensure that our collision shape is fully updated as the capsule that surrounds the entire player body
	WallCollisionShape.SetCapsule(FMath::FInterpTo(WallCollisionShape.GetCapsuleRadius(), CharacterCollisionModel.Radius[static_cast<uint8>(CurrentStance)], DeltaTime, 5.0f), FMath::FInterpTo(WallCollisionShape.GetCapsuleHalfHeight(), CharacterCollisionModel.HalfHeight[static_cast<uint8>(CurrentStance)], DeltaTime, 5.0f));
	FloorCollisionShape.SetCapsule(WallCollisionShape.GetCapsuleRadius(), FMath::FInterpTo(FloorCollisionShape.GetCapsuleHalfHeight(), CharacterCollisionModel.MaxStepHeight[static_cast<uint8>(CurrentStance)], DeltaTime, 5.0f));


	// 
	CurrentCollisionRotation = (GetActorQuat() + FMath::QInterpTo(CurrentCollisionRotation, CharacterCollisionModel.CollisionRotation[static_cast<uint8>(CurrentStance)], DeltaTime, 5.0f));


	// Calculate and locally store the current world position of the center of the collision box for the character
#ifdef ALLOW_ROTATING_GRAVITY
	CurrentCollisionWorldPosition = (GetActorLocation() + GetActorQuat().RotateVector(FVector(0.0f, 0.0f, WallCollisionShape.GetCapsuleHalfHeight() + FloorCollisionShape.GetCapsuleHalfHeight())));
#else
	CurrentCollisionWorldPosition = (GetActorLocation() + FVector(0.0f, 0.0f, WallCollisionShape.GetCapsuleHalfHeight() + FloorCollisionShape.GetCapsuleHalfHeight()));
#endif

	// Reset all of our collision query parameters now for a clean sweep later
	CollisionQueryParameters.bTraceComplex = false;
	CollisionQueryParameters.bReturnPhysicalMaterial = false;
	CollisionQueryParameters.bDebugQuery = true;

	// Calculate gravity to handle any and all custom gravity logic
	CalculateGravity();

	// Switch on movement state, as what occurs each from of movement calculating depends entirely on the state of the character itself
	switch (CurrentMovementState) {

		// If we are grounded, then we call the override-able grounded calculator function to handle that state
		case EMovementState::Grounded:
			CalculateGroundedMovement();
			break;

		// If we are grounded, then we call the override-able grounded calculator function to handle that state
		case EMovementState::Airborne:
			CalculateAirborneMovement();
			break;

		// 
		case EMovementState::Mantling:

			// 
			CalculateMantlingMovement();
			break;

		// 
		case EMovementState::Ragdoll:

			//
			CalculateRagdollMovement();
			break;

		// 
		case EMovementState::LadderClimbing:

			// 
			CalculateLadderClimbingMovement();
			break;

		// 
		case EMovementState::Rolling:

			// 
			CalculateRollingMovement();
			break;

		// 
		case EMovementState::Sliding:

			// 
			CalculateSlidingMovement();
			break;

		// 
		case EMovementState::Swimming:

			// 
			CalculateSwimmingMovement();
			break;

		// 
		case EMovementState::Floating:

			// 
			CalculateFloatingMovement();
			break;

		// Do nothing, as this is an error case and should be treated as such
		case EMovementState::Num:
		default:
			break;
	}


	// Next, Cache Values
	StorePreviousValues();
	
#ifdef UE_BUILD_DEBUG
	// And, next, Draw Debug Shapes
	DrawDebugShapes();
#endif

	// Lastly, apply the wanted world transform as an offset and move along
	this->RootComponent->SetWorldLocation(this->RootComponent->GetComponentTransform().GetTranslation() + CalculatedMovementOffset, false, nullptr, ETeleportType::None);
}


// 
#ifdef UE_BUILD_DEBUG
void AAbstractCharacter::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);
	DebugCapsuleCollision->SetRelativeLocation(Transform.GetLocation() + FVector(0.0, 0.0, CharacterCollisionModel.HalfHeight[static_cast<uint8>(CurrentStance)] + CharacterCollisionModel.MaxStepHeight[static_cast<uint8>(CurrentStance)]));
	DebugCapsuleCollision->SetCapsuleHalfHeight(CharacterCollisionModel.HalfHeight[static_cast<uint8>(CurrentStance)]);
	DebugCapsuleCollision->SetCapsuleRadius(CharacterCollisionModel.Radius[static_cast<uint8>(CurrentStance)]);
}
#endif


// 
void AAbstractCharacter::SmoothCharacterRotation(FRotator NewTargetRotation, const float ConstantInterpSpeed, const float SmoothInterpSpeed, const float & DeltaTime) {

	// 
	TargetRotation = FMath::RInterpConstantTo(TargetRotation, NewTargetRotation, DeltaTime, ConstantInterpSpeed);

	// 
	CharacterSkeleton->SetRelativeRotation(FMath::RInterpTo(CharacterSkeleton->GetRelativeRotation(), TargetRotation, DeltaTime, SmoothInterpSpeed));
}


// 
void AAbstractCharacter::ToggleStance() {
	CurrentStance = ((CurrentStance == EStance::Standing) ? EStance::Crouching : EStance::Standing);
}


// 
void AAbstractCharacter::ToggleGait() {
	DesiredGait = ((DesiredGait == EGaitState::FastWalking) ? EGaitState::Walking : EGaitState::FastWalking);
}


// 
void AAbstractCharacter::ToggleSprint() {
	if (DesiredGait != EGaitState::Running) PreviousGait = DesiredGait;
	DesiredGait = ((DesiredGait != EGaitState::Running) ? EGaitState::Running : PreviousGait);
}


// 
void AAbstractCharacter::ToggleCameraPerspective() {
	CurrentViewMode = (CurrentViewMode == EViewMode::FirstPerson ? EViewMode::ThirdPerson : EViewMode::FirstPerson);
}


// 
void AAbstractCharacter::SetStance(const EStance GivenStance) {
	CurrentStance = GivenStance;
}


// 
void AAbstractCharacter::SetGait(const EGaitState GivenGait) {
	PreviousGait = DesiredGait;
	DesiredGait = GivenGait;
}


// 
void AAbstractCharacter::SetSprint(const bool IsWantingToSprint) {
	if (DesiredGait != EGaitState::Running) PreviousGait = DesiredGait;
	DesiredGait = (IsWantingToSprint ? EGaitState::Running : PreviousGait);
}


// 
void AAbstractCharacter::SetCameraPerspective(const EViewMode NewViewMode) {
	CurrentViewMode = NewViewMode;
}


// 
double AAbstractCharacter::GetSpeedMultipleByAngle() {
	return (1.0 + ((1.0 - CollisionResult.ImpactNormal.Z) * (MAX_SPEED_ANGLE_MULTIPLIER / (1.0 - LARGEST_SLOPE_ANGLE)) * (FMath::IsNegativeOrNegativeZero(TargetVelocity.Z) ? 1.0 : -1.0)));
}


// 
double AAbstractCharacter::GetSpeedMultipleByFloorType() {

	// If the physical material is valid, then convert its SurfaceType into an integer to index the FloorTypeSpeedMultiplier array, else use the default value of 0
	return FloorTypeSpeedMultiplier[static_cast<uint8>(CollisionResult.PhysMaterial.IsValid() ? CollisionResult.PhysMaterial->SurfaceType : 0)];
}


// 
void AAbstractCharacter::GetMovementAnimationData_Implementation(FMovementAnimationDataStruct & GivenMovementAnimationData) const {
	GivenMovementAnimationData.Velocity = CurrentVelocity;
	GivenMovementAnimationData.Acceleration = Acceleration;
	GivenMovementAnimationData.MovementInput = MovementInput;
	GivenMovementAnimationData.IsMoving = IsMoving;
	GivenMovementAnimationData.HasMovementInput = HasMovementInput;
	GivenMovementAnimationData.Speed = Speed;
	GivenMovementAnimationData.MovementInputAmount = MovementInputAmount;
	GivenMovementAnimationData.AimYawRate = AimYawRate;
	GivenMovementAnimationData.CapsuleRadius = WallCollisionShape.GetCapsuleRadius();
	GivenMovementAnimationData.CapsuleHalfHeight = WallCollisionShape.GetCapsuleHalfHeight();
	GivenMovementAnimationData.MaxAcceleration = MaxAcceleration;
	GivenMovementAnimationData.AimingRotation = RotatorComponent->GetRelativeRotation();
	GivenMovementAnimationData.ActorPreviousRotation = PreviousActorRotation;
	GivenMovementAnimationData.ActorControlRotation = CharacterSkeleton->GetRelativeRotation();
	GivenMovementAnimationData.ActorWorldLocation = GetActorLocation();
	GivenMovementAnimationData.MovementState = CurrentMovementState;
	GivenMovementAnimationData.PreviousMovementState = PreviousMovementState;
	GivenMovementAnimationData.RotationMode = RotationState;
	GivenMovementAnimationData.Gait = CurrentGait;
	GivenMovementAnimationData.Stance = CurrentStance;
	GivenMovementAnimationData.ViewMode = CurrentViewMode;
	GivenMovementAnimationData.InAirPredictionTime = InAirPredictionTime;
}


// 
void AAbstractCharacter::TransferCharacterData(FArchive & GivenSaveOrLoadSystem) {

	// TODO: Implement save game system to save all of the relevant information, such as locations (relative and world), rotations (relative and world), etc.
	// 
}


// 
void AAbstractCharacter::ToGrounded() {

	// Update the current movement state to the new grounded state
	CurrentMovementState = EMovementState::Grounded;

	// Update our previous floor to now be the floor which we just landed on now that we're grounded
	PreviousFloorComponent = CollisionResult.Component;
	PreviousFloorPreviousTransform = PreviousFloorComponent->GetComponentTransform();
}


// 
void AAbstractCharacter::ToAirborne() {

	// Update the current movement state to the new airborne state
	CurrentMovementState = EMovementState::Airborne;

	// Set our previous floor component to be nullptr, as there will no longer be floors changing our anything, as we are falling now
	PreviousFloorComponent = nullptr;

	// Ensure that our in-air rotation is the same as our current rotation
	InAirRotation = CharacterSkeleton->GetRelativeRotation();
}


// 
void AAbstractCharacter::CalculateGravity() {
	CurrentGravityOffset = FVector(0.0f, 0.0f, -980.665f * CurrentDeltaTime * CurrentDeltaTime);
}

// 
void AAbstractCharacter::CalculatePreviousFloor() {

	// If the pointer to the previous floor component is invalid (e.g. there is NOT a floor), then simply return
	if (!PreviousFloorComponent.IsValid()) {
		return;
	}

	// Locally store the previous floor component's transform
	const FTransform & CurrentPreviousFloorTransform = PreviousFloorComponent->GetComponentTransform();

	// If the previous floor's previous location does not equal its current location, then...
	if (FTransform::AreTranslationsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

#ifdef SHOULD_HAVE_ACCURATE_PREVIOUS_FLOOR_TRANSFORMS

		// Simply subtract the previous floor's previous location from its current location to get the difference in movement 
		const FVector DeltaLocation = (FTransform::SubtractTranslations(CurrentPreviousFloorTransform, PreviousFloorPreviousTransform));

		// Collision check to ensure that there is nothing between your old position and the new position
		//     (e.g. standing still on a moving block that moves you into a non-moving wall)
		FPhysicsInterface::GeomSweepSingle(GetWorld(), WallCollisionShape, CurrentCollisionRotation, CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + DeltaLocation, ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam);

		// Finally, implement what's left of the delta location now
		CalculatedMovementOffset += (DeltaLocation * (CollisionResult.bBlockingHit ? CollisionResult.Time : 1.0));
#else
		CalculatedMovementOffset += (FTransform::SubtractTranslations(CurrentPreviousFloorTransform, PreviousFloorPreviousTransform));
#endif
	}

	// If the previous floor's previous rotation does not equal its current rotation, then...
	if (FTransform::AreRotationsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

		// Locally store the rotational difference between the current and previous rotations
		const FQuat DeltaRotation = (CurrentPreviousFloorTransform.GetRotation() - PreviousFloorPreviousTransform.GetRotation());

		// TODO: Convert rotational differences into positional differences
		// 
	}

	// If the previous floor's previous scale does not equal its current scale, then...
	if (FTransform::AreScale3DsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

		// Locally store the scale difference between the current and previous scales
		const FVector DeltaScale = (CurrentPreviousFloorTransform.GetScale3D() - PreviousFloorPreviousTransform.GetScale3D());

		// TODO: Convert scale differences into positional differences
		// 
	}

#ifndef SHOULD_HAVE_ACCURATE_PREVIOUS_FLOOR_TRANSFORMS
	FPhysicsInterface::GeomSweepSingle(GetWorld(), WallCollisionShape, CurrentCollisionRotation, CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + CalculatedMovementOffset, ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam));
	CalculatedMovementOffset *= CollisionResult.Time;
#endif

	// Update our current collision world position by all other aspects taken into account above in order to have accurate results
	CurrentCollisionWorldPosition += CalculatedMovementOffset;
}


// 
void AAbstractCharacter::CalculateGroundedMovement() {
	
	// Check if our previous floor (if valid) has transformed in any way and store the following movement within the CalculatedMovementOffset as an additive vector
	CalculatePreviousFloor();

	// TODO: Potentially take into account previous velocity and/or gravity
	// Locally store our current actor's world position plus any previous movement offsets that should be taken into account such as previous floor movement
	const FVector CurrentWorldPosition = (GetActorLocation() + CalculatedMovementOffset);


	//// Velocity and acceleration calculations
	// Else, we are grounded, so we need to calculate our next movement now, so...
	// Calculate our target velocity by multiplying our movement input by our current gait's max movement speed
	TargetVelocity = (MovementInput * CurrentMovementData.MaxSpeed[static_cast<uint8>(AllowedGait)]);

	// Calculate our current acceleration by taking the difference in velocities divided by delta time
	Acceleration = FVector(FVector2D(TargetVelocity - PreviousVelocity) * InverseDeltaTime, 0.0);

	// Calculate our max acceleration by utilizing our current gait's either acceleration or deceleration depending on whether we are slowing down or not
	MaxAcceleration = FMath::FInterpTo(MaxAcceleration, ((TargetVelocity.SizeSquared() >= PreviousVelocity.SizeSquared()) ? CurrentMovementData.MaxAcceleration[(int32)CurrentGait] : CurrentMovementData.MaxDeceleration[(int32)CurrentGait]), CurrentDeltaTime, 10.0f);

	// If our current acceleration is greater than our max acceleration, then...
	const float CurrentAccelerationSizeSquared = Acceleration.SizeSquared();
	if (CurrentAccelerationSizeSquared > FMath::Square(MaxAcceleration)) {

		// Clamp our current acceleration by our max acceleration to ensure that we are not speeding up or rotating too quickly
		Acceleration *= (MaxAcceleration * FMath::InvSqrt(CurrentAccelerationSizeSquared));

		// Next, we store our target velocity as our new current acceleration multiplied by delta time
		TargetVelocity = FVector(FVector2D((Acceleration * CurrentDeltaTime) + PreviousVelocity), TargetVelocity.Z);
	}


	// Now we do the WallCheck to ensure that we are not walking through objects
	CalculateCollisionOffset(TargetVelocity);
	

	//// Now we do the FloorCheck to ensure that we are not walking over large openings (rather than changing the collision shape twice, we just make a new temporary shape instead, since it is a one-off)
	// First we do a floor check shape sweep with a capsule of our current radius and a half height of our current step height to ensure that there is floor that we can walk on
	// If our floor collision trace does NOT hit something, then we are to assume we are now airborne, so...
	if (!FPhysicsInterface::GeomSweepSingle(GetWorld(), FloorCollisionShape, CurrentCollisionRotation, CollisionResult, CurrentWorldPosition + (TargetVelocity * CurrentDeltaTime), CurrentWorldPosition, ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam)) {

		//// AIRBORNE
		// Update our movement offset to be where the collision sweep ended plus gravity and move on
		CalculatedMovementOffset += (CurrentGravityOffset + (CollisionResult.TraceStart - CollisionResult.TraceEnd));

		// Call the function which handles the main transitioning to the new airborne state
		ToAirborne();

		// Finally, do an early return, as we do not need to calculate the rest of the grounded functionality
		return;
	}


	// 
	PreviousFloorComponent = CollisionResult.Component;


	// Now, for the final trace, we need to do a line trace on the impact point to get accurate results, as well as the physical material (if one exists)
	CollisionQueryParameters.bReturnPhysicalMaterial = true;
	// TODO: Fix raycasting so that it relies on the actor's rotation to be able to handle changing gravitational rotations
	//FVector StepHeightOffset = GetActorQuat().RotateVector(FVector(0.0, 0.0, FloorCollisionShape.GetCapsuleHalfHeight()));
	if (CollisionResult.bStartPenetrating == false) FPhysicsInterface::RaycastSingle(GetWorld(), CollisionResult, FVector(CollisionResult.ImpactPoint.X, CollisionResult.ImpactPoint.Y, CollisionResult.Location.Z + FloorCollisionShape.GetCapsuleHalfHeight()), FVector(CollisionResult.ImpactPoint.X, CollisionResult.ImpactPoint.Y, CollisionResult.Location.Z - FloorCollisionShape.GetCapsuleHalfHeight()), ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam);
	else FPhysicsInterface::RaycastSingle(GetWorld(), CollisionResult, FVector(CollisionResult.Location.X, CollisionResult.Location.Y, CollisionResult.Location.Z + FloorCollisionShape.GetCapsuleHalfHeight()), FVector(CollisionResult.Location.X, CollisionResult.Location.Y, CollisionResult.Location.Z - FloorCollisionShape.GetCapsuleHalfHeight()), ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam);
	CollisionQueryParameters.bReturnPhysicalMaterial = false;


	// 
	if (CollisionResult.bBlockingHit) {

		// Next, we need to ensure that the floor we hit above is actually walkable, else we also want to go to the airborne state
		if (CollisionResult.ImpactNormal.Z < LARGEST_SLOPE_ANGLE) {

			//// AIRBORNE
			// Update our movement offset to be where the collision sweep ended plus gravity and move on
			CalculatedMovementOffset += (CurrentGravityOffset + (TargetVelocity * CurrentDeltaTime));

			// Call the function which handles the main transitioning to the new airborne state
			ToAirborne();

			// Finally, do an early return, as we do not need to calculate the rest of the grounded functionality
			return;
		}

		// Next, we take all of our new collision information and adjust our target velocity for the z axis only depending on whether the floor is a walkable slope or not
		TargetVelocity.Z = FMath::FInterpTo(TargetVelocity.Z, ((CollisionResult.ImpactPoint.Z - CurrentWorldPosition.Z) * InverseDeltaTime), CurrentDeltaTime, 5.0f);


		// 
		CurrentVelocity = (TargetVelocity * (GetSpeedMultipleByAngle() * GetSpeedMultipleByFloorType()));


		// 
		PreviousFloorComponent = CollisionResult.Component;
	}


	// 
	else {
		CollisionResult.ImpactNormal.Z = 1.0;
		CollisionResult.PhysMaterial = nullptr;
		CurrentVelocity = (TargetVelocity * (GetSpeedMultipleByAngle() * GetSpeedMultipleByFloorType()));
	}


	// If this is a new floor component, then update all of the previous floor information to handle such
	PreviousFloorPreviousTransform = PreviousFloorComponent->GetComponentTransform();


	// Next, locally store the magnitude of the current velocity's vector to save performance costs in the future
	Speed = CurrentVelocity.Size();


	// Lastly, apply the current velocity multiplied by delta time as our final movement offset for the user's input
	CalculatedMovementOffset += (CurrentVelocity * CurrentDeltaTime);


	// Calculate whether we are moving
	IsMoving = (Speed > 5.0f);

	// Calculate the last velocity rotation iff we are moving
	if (IsMoving) LastVelocityRotation = CurrentVelocity.ToOrientationRotator();


	// Update Character Movement
		// Set the allowed gait
	if (CurrentStance == EStance::Crouching) {
		AllowedGait = (DesiredGait == EGaitState::Walking ? EGaitState::Walking : EGaitState::FastWalking);
	}
	else {
		if (DesiredGait == EGaitState::Running) {
			const bool CanSprint = ((MovementInputAmount > 0.9f) && (RotationState == ERotationState::VelocityDirection || (RotationState == ERotationState::LookingDirection && (FMath::Abs((CharacterSkeleton->GetRelativeRotation() - LastMovementInputRotation).GetNormalized().Yaw) < 50.0f))));
			AllowedGait = (CanSprint ? EGaitState::Running : EGaitState::FastWalking);
		}
		else {
			AllowedGait = DesiredGait;
		}
	}

	// Determine the actual gait
	if (Speed >= (((CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::Running)] - CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::FastWalking)]) * 0.25f) + CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::FastWalking)])) ActualGait = EGaitState::Running;
	else if (Speed >= (((CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::FastWalking)] - CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::Walking)]) * 0.25f) + CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::Walking)])) ActualGait = (AllowedGait == EGaitState::Running ? EGaitState::Running : EGaitState::FastWalking);
	else ActualGait = ActualGait = EGaitState::Walking;
	//if (Speed >= (CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::FastWalking)] + 10.0f)) ActualGait = EGaitState::Running;
	//else if (Speed >= (CurrentMovementData.MaxSpeed[static_cast<uint8>(EGaitState::Walking)] + 10.0f)) ActualGait = (AllowedGait == EGaitState::Running ? EGaitState::Running : EGaitState::FastWalking);
	//else ActualGait = EGaitState::Walking;

	/*
	if (Speed >= (CurrentMovementData.MaxFastWalkSpeed + 10.0f)) {
		ActualGait = (AllowedGait == EGaitState::Running ? EGaitState::Running : EGaitState::FastWalking);
	} else {
		ActualGait = ((Speed >= (CurrentMovementData.MaxWalkSpeed + 10.0f)) ? EGaitState::FastWalking : EGaitState::Walking);
	}
	*/

	// 
	if (ActualGait != CurrentGait) {
		CurrentGait = ActualGait;
		// Call optional function for if the gait changes
	}

	// Update Dynamic Movement Settings
	switch (RotationState) {

		// 
	case ERotationState::VelocityDirection:
		CurrentMovementData = ((CurrentStance == EStance::Crouching) ? CrouchingVelocityDirectionMovementData : StandingVelocityDirectionMovementData);
		break;

		// 
	default:
	case ERotationState::LookingDirection:
		CurrentMovementData = ((CurrentStance == EStance::Crouching) ? CrouchingLookingDirectionMovementData : StandingLookingDirectionMovementData);
		break;

		// 
	case ERotationState::Aiming:
		CurrentMovementData = ((CurrentStance == EStance::Crouching) ? CrouchingAimingMovementData : StandingAimingMovementData);
		break;
	}

	// Update grounded rotation
	// If we can update moving rotation, then...
	if ((IsMoving && HasMovementInput) || (Speed > 150.0f)) {

		// Change grounded rotation functionality based off the current rotation state
		switch (RotationState) {

			// 
		case ERotationState::VelocityDirection:
			SmoothCharacterRotation(FRotator(0.0, LastVelocityRotation.Yaw, 0.0), 800.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			break;

			// 
		default:
		case ERotationState::LookingDirection:

			// 
			if (CurrentGait == EGaitState::Running) {
				SmoothCharacterRotation(FRotator(0.0, LastVelocityRotation.Yaw, 0.0), 500.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			}

			// 
			else {
				SmoothCharacterRotation(FRotator(0.0, (RotatorComponent->GetRelativeRotation().Yaw + (IsValid(CharacterSkeleton->GetAnimInstance()) ? CharacterSkeleton->GetAnimInstance()->GetCurveValue(FName(TEXT("YawOffset"))) : 0.0)), 0.0), 500.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			}

			// 
			break;

			// 
		case ERotationState::Aiming:
			SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw, 0.0), 1000.0, 20.0, CurrentDeltaTime);
			break;
		}
	}

	// else, we cannot updating moving rotation, so...
	else {

		// If we are in first person or third person while aiming, then...
		if ((CurrentViewMode == EViewMode::FirstPerson) || (RotationState == ERotationState::Aiming)) {

			// Limit Rotation
			const double YawDelta = (RotatorComponent->GetRelativeRotation() - CharacterSkeleton->GetRelativeRotation()).GetNormalized().Yaw;
			if ((YawDelta < -100.0f) || (YawDelta > 100.0f)) {
				SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw - ((YawDelta > 0.0f) ? 100.0f : -100.0f), 0.0f), 0.0f, 20.0f, CurrentDeltaTime);
			}
		}

		// Apply the RotationAmount curve from Turn In Place animations (fixed at an animated framerate of 30 fps)
		const float AnimCurveValue = (IsValid(CharacterSkeleton->GetAnimInstance()) ? CharacterSkeleton->GetAnimInstance()->GetCurveValue(FName(TEXT("RotationAmount"))) : 0.0f);
		if (FMath::Abs(AnimCurveValue) > 0.001f) {
			CharacterSkeleton->AddRelativeRotation(FRotator(0.0f, (AnimCurveValue * CurrentDeltaTime * 30.0f), 0.0f));
			TargetRotation = CharacterSkeleton->GetRelativeRotation();
		}
	}
}


// 
void AAbstractCharacter::CalculateAirborneMovement() {

	// Add more gravity to the current velocity
	CurrentVelocity += (CurrentGravityOffset * InverseDeltaTime);

	// Locally store our current position plus our current velocity (since we are falling in a set direction)
	const FVector LocalActorPosition = (this->GetActorLocation());// +CalculatedMovementOffset);

	// If our collision trace hits something, then we are to assume we are now grounded, so...
	if (FPhysicsInterface::GeomSweepSingle(GetWorld(), FloorCollisionShape, CurrentCollisionRotation, CollisionResult, LocalActorPosition, LocalActorPosition + (CurrentVelocity * CurrentDeltaTime), ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam)) {

		//// GROUNDED
		// Ensure that we land on the collision impact point, while also maintaining our current velocity
		CalculatedMovementOffset += (FVector(CollisionResult.Location.X, CollisionResult.Location.Y, CollisionResult.Location.Z) - CollisionResult.TraceStart);

		// Call the function which handles the main transitioning to the new grounded state
		ToGrounded();

		// Finally, do an early return, as we do not need to calculate the rest of the airborne functionality
		return;
	}

	// Else, we are airborne, so we need to calcualte our next movement now, so...
	// TODO: Calculate airborne movement now (apply gravity to velocity, apply user input to velocity IFF allowed, and account for any and all collisions)
	// 


	// If we are still falling, then updating our target movement offset (cheaply, as three subtractions are faster than three multiplications)
	CalculatedMovementOffset += (CollisionResult.TraceEnd - CollisionResult.TraceStart);
	

	// Now, check for wall collisions and update our current velocity accordingly
	CalculateCollisionOffset(CurrentVelocity);


	// Set our speed to be the size of our current velocity
	Speed = CurrentVelocity.Size(); // CalculatedMovementOffset.Size() * InverseDeltaTime;



	// Calculate whether we are moving
	IsMoving = (Speed > 5.0f);

	// Calculate the last velocity rotation iff we are moving
	if (IsMoving) LastVelocityRotation = CurrentVelocity.ToOrientationRotator();


	// Check to see if there is a surface in front of us that we might land on with our current velocity
	FPhysicsInterface::GeomSweepSingle(GetWorld(), FloorCollisionShape, CurrentCollisionRotation, CollisionResult, CollisionResult.TraceEnd,
		CollisionResult.TraceEnd + (FVector(CurrentVelocity.X, CurrentVelocity.Y, FMath::Clamp(CurrentVelocity.Z, -4000.0f, -200.0f)).GetUnsafeNormal() * FMath::GetMappedRangeValueClamped(FVector2D(0.0f, -4000.0f), FVector2D(50.0f, 2000.0f), CurrentVelocity.Z)),
		ECollisionChannel::ECC_Visibility, FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams::DefaultResponseParam);

	// If there is something to land on and we are falling, then set our prediction time to be the time it took to hit said thing, else set to -1.0f, as we have no prediction
	InAirPredictionTime = ((CollisionResult.bBlockingHit && CurrentVelocity.Z < -200.0f) ? CollisionResult.Time : -1.0f);


	// If we are aiming, then...
	if (RotationState == ERotationState::Aiming) {

		// Rotate our skeleton to face our rotator component (our current camera rotation) and set our in-air rotation to be the character skeleton's relative rotation
		SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw, 0.0), 0.0f, 15.0f, CurrentDeltaTime);
		InAirRotation = CharacterSkeleton->GetRelativeRotation();
	}

	// Else, if we are not aiming, then...
	else {

		// Rotate our skeleton to face our current in-air rotation
		SmoothCharacterRotation(FRotator(0.0, InAirRotation.Yaw, 0.0), 0.0f, 5.0f, CurrentDeltaTime);
	}

	// If the user is pressing their movement inputs, then check for potential ledges to mantle mid-air
	if (HasMovementInput) {

		// 
		// MantleCheck(150.0, 50.0, 70.0, 30.0, 30.0, EDebugType::ForOneFrame);
	}
}


// 
void AAbstractCharacter::CalculateMantlingMovement() {
}


// 
void AAbstractCharacter::CalculateRagdollMovement() {
}


// 
void AAbstractCharacter::CalculateLadderClimbingMovement() {
}


// 
void AAbstractCharacter::CalculateRollingMovement() {
}


// 
void AAbstractCharacter::CalculateSlidingMovement() {
}


// 
void AAbstractCharacter::CalculateSwimmingMovement() {
}


// 
void AAbstractCharacter::CalculateFloatingMovement() {
}


// 
void AAbstractCharacter::CalculateCollisionOffset(FVector & EditableVelocity) {

	// First we do a geometry sweep to see if there is a wall between where we are and where we want to be, and, if so, then...
	if (FPhysicsInterface::GeomSweepSingle(GetWorld(), WallCollisionShape, CurrentCollisionRotation, CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + (EditableVelocity * CurrentDeltaTime), ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam)) {

		// If there is something that completely blocks our path, then...
		if (CollisionResult.bStartPenetrating || FMath::IsNearlyEqualByULP(CollisionResult.Time, 0.0f) == false) {

			// If our path is completely blocked, then project our wanted velocity onto the plane made by the wall's impact normal (minus z)
			EditableVelocity = FVector::VectorPlaneProject(EditableVelocity, CollisionResult.ImpactNormal);

			// Next, we do a secondary check to ensure that, if there's a new wall for our new velocity, that we do not clip into it
			if (FPhysicsInterface::GeomSweepSingle(GetWorld(), WallCollisionShape, CurrentCollisionRotation, CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + (EditableVelocity * CurrentDeltaTime), ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam)) {
				EditableVelocity *= (CollisionResult.Time - 0.01f /*To avoid accidentally clipping into walls*/);
			}
		}

		// Else, simply scale back our velocity and be done
		else EditableVelocity *= (CollisionResult.Time - 0.01f /*To avoid accidentally clipping into walls*/);
	}

	// Small performance optimization to just add the new editable velocity into the current collision world position rather than recalculating that up to three or more times
	CurrentCollisionWorldPosition += (EditableVelocity * CurrentDeltaTime);

	// Ensure that the current overlapping collisions array is empty with a slack size of two
	WallOverlapCollisions.Empty(2); /*Assume that, most likely, there will never be more than two wall-like objects moving at you at a time*/

	// Test for if there are any wall-like objects moving at you that should cause your position to displace
	if (FPhysicsInterface::GeomOverlapMulti(GetWorld(), WallCollisionShape, CurrentCollisionWorldPosition, CurrentCollisionRotation, WallOverlapCollisions, ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam, FCollisionObjectQueryParams{FCollisionObjectQueryParams::InitType::AllDynamicObjects})) {

		// Loop through all currently overlapping objects, and...
		for (const FOverlapResult & CurrentResult : WallOverlapCollisions) {

			// Check if they are still overlapping and, if so, collect their penetration details and...
			FMTDResult LocalResult;
			if (CurrentResult.Component->ComputePenetration(LocalResult, WallCollisionShape, CurrentCollisionWorldPosition, CurrentCollisionRotation)) {

				// Locally store the distance vector required to move us out of the currently overlapping object
				const FVector LocalVector = (LocalResult.Direction * LocalResult.Distance);

				// Instead of making our velocity change from the movement, we are just going to apply the movement change directly so that our character doesn't do dumb looking things (video game logic)
				CalculatedMovementOffset += LocalVector;

				// Finally, also apply the move to our current collision world position, as we have officially moved due to this object
				CurrentCollisionWorldPosition += LocalVector;
			}
		}
	}
}


// 
void AAbstractCharacter::StorePreviousValues() {

	// Store the current velocity as the previous now
	PreviousVelocity = CurrentVelocity;

	// Store the current aim yaw as previous now
	PreviousAimYaw = RotatorComponent->GetRelativeRotation().Yaw;

	// 
	PreviousMovementState = CurrentMovementState;

	// 
	PreviousActorRotation = CharacterSkeleton->GetRelativeRotation();
}


// 
void AAbstractCharacter::DrawDebugShapes() {

	// 
	DebugCapsuleCollision->SetRelativeLocation(FVector(0.0, 0.0, WallCollisionShape.GetCapsuleHalfHeight() + FloorCollisionShape.GetCapsuleHalfHeight()));
	DebugCapsuleCollision->SetCapsuleHalfHeight(WallCollisionShape.GetCapsuleHalfHeight());
	DebugCapsuleCollision->SetCapsuleRadius(WallCollisionShape.GetCapsuleRadius());


	// Draw a debug arrow to visualize our actual movement
	DebugVelocityArrow->SetWorldScale3D(FVector(Speed / CurrentMovementData.MaxSpeed[static_cast<uint8>(AllowedGait)]));
	DebugVelocityArrow->ArrowLength = Speed;
	if (!CalculatedMovementOffset.IsNearlyZero()) DebugVelocityArrow->SetWorldRotation(CalculatedMovementOffset.ToOrientationRotator());

	// Draw a debug arrow to visualize what our wanted direction is
	DebugMovementArrow->SetWorldRotation((MovementInput * (CurrentMovementData.MaxSpeed[static_cast<uint8>(AllowedGait)] * CurrentDeltaTime)).ToOrientationRotator());
}
