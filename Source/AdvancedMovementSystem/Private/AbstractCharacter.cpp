// Sidney Towers
// TreeTrunkStudios (c) 2023


// All required include files
#include "AbstractCharacter.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AAbstractCharacter::AAbstractCharacter() {

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryActorTick.EndTickGroup = ETickingGroup::TG_StartPhysics;

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
	//CharacterSkeleton->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

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
	GetWorld()->bDebugDrawAllTraceTags = true;
	
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


	// Update our target half height, as it should interpolate based on the target stance
	CurrentHalfHeight = FMath::FInterpTo(CurrentHalfHeight, CharacterCollisionModel.HalfHeight[static_cast<uint32>(CurrentStance)], DeltaTime, 5.0f);
	CurrentRadius = FMath::FInterpTo(CurrentRadius, CharacterCollisionModel.Radius[static_cast<uint32>(CurrentStance)], DeltaTime, 5.0f);
	CurrentStepHeight = FMath::FInterpTo(CurrentStepHeight, CharacterCollisionModel.MaxStepHeight[static_cast<uint32>(CurrentStance)], DeltaTime, 5.0f);

	// Calculate and locally store the world position of the center of the collision box for the character
	CurrentCollisionWorldPosition = (GetActorLocation() + FVector(0.0f, 0.0f, CurrentHalfHeight + CurrentStepHeight));

	// Reset all of our collision query parameters now for a clean sweep later
	CollisionQueryParameters.bTraceComplex = false;
	CollisionQueryParameters.bReturnPhysicalMaterial = false;
	CollisionQueryParameters.bDebugQuery = true;

	// Ensure that our collision shape is fully updated as the capsule that surrounds the entire player body
	CollisionShape.SetCapsule(CurrentRadius, CurrentHalfHeight);

	// Calculate gravity to handle any and all custom gravity logic
	CalculateGravity();

	// Switch on movement state, as what occurs each from of movement calculating depends entirely on the state of the character itself
	switch (CurrentMovementState) {

		// If we are grounded, then we call the override-able grounded calculator function to handle that state
		case MovementStateEnum::Grounded:
			CalculateGroundedMovement();
			break;

		// If we are grounded, then we call the override-able grounded calculator function to handle that state
		case MovementStateEnum::Airborne:
			CalculateAirborneMovement();
			break;

		// 
		case MovementStateEnum::Mantling:

			// 
			CalculateMantlingMovement();
			break;

		// 
		case MovementStateEnum::Ragdoll:

			//
			CalculateRagdollMovement();
			break;

		// 
		case MovementStateEnum::LadderClimbing:

			// 
			CalculateLadderClimbingMovement();
			break;

		// 
		case MovementStateEnum::Rolling:

			// 
			CalculateRollingMovement();
			break;

		// 
		case MovementStateEnum::Sliding:

			// 
			CalculateSlidingMovement();
			break;

		// 
		case MovementStateEnum::Swimming:

			// 
			CalculateSwimmingMovement();
			break;

		// 
		case MovementStateEnum::Floating:

			// 
			CalculateFloatingMovement();
			break;

		// Do nothing, as this is an error case and should be treated as such
		case MovementStateEnum::Num:
		default:
			break;
	}


	// Next, Cache Values
	StorePreviousValues();
	
#ifdef UE_BUILD_DEBUG
	// And, next, Draw Debug Shapes
	DrawDebugShapes();

	// 
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("PreviousFloorComponent: ")) + (PreviousFloorComponent.IsValid() ? PreviousFloorComponent->GetName() : FString(TEXT("nullptr"))));
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("PreviousFloorPreviousTransform: ")) + PreviousFloorPreviousTransform.ToString());
	}
#endif

	// Lastly, apply the wanted world transform as an offset and move along
	this->RootComponent->SetWorldLocation(this->RootComponent->GetComponentTransform().GetTranslation() + CalculatedMovementOffset, false, nullptr, ETeleportType::None);
}


// 
#ifdef UE_BUILD_DEBUG
void AAbstractCharacter::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);
	DebugCapsuleCollision->SetRelativeLocation(Transform.GetLocation() + FVector(0.0, 0.0, CharacterCollisionModel.HalfHeight[static_cast<uint32>(CurrentStance)] + CharacterCollisionModel.MaxStepHeight[static_cast<uint32>(CurrentStance)]));
	DebugCapsuleCollision->SetCapsuleHalfHeight(CharacterCollisionModel.HalfHeight[static_cast<uint32>(CurrentStance)]);
	DebugCapsuleCollision->SetCapsuleRadius(CharacterCollisionModel.Radius[static_cast<uint32>(CurrentStance)]);
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
	CurrentStance = ((CurrentStance == StanceEnum::Standing) ? StanceEnum::Crouching : StanceEnum::Standing);
}


// 
void AAbstractCharacter::ToggleGait() {
	DesiredGait = ((DesiredGait == GaitStateEnum::FastWalking) ? GaitStateEnum::Walking : GaitStateEnum::FastWalking);
}


// 
void AAbstractCharacter::ToggleSprint() {
	if (DesiredGait != GaitStateEnum::Running) PreviousGait = DesiredGait;
	DesiredGait = ((DesiredGait != GaitStateEnum::Running) ? GaitStateEnum::Running : PreviousGait);
}


// 
void AAbstractCharacter::ToggleCameraPerspective() {
	IsThirdPerson = !IsThirdPerson;
}


// 
void AAbstractCharacter::SetStance(const StanceEnum GivenStance) {
	CurrentStance = GivenStance;
}


// 
void AAbstractCharacter::SetGait(const GaitStateEnum GivenGait) {
	PreviousGait = DesiredGait;
	DesiredGait = GivenGait;
}


// 
void AAbstractCharacter::SetSprint(const bool IsWantingToSprint) {
	if (DesiredGait != GaitStateEnum::Running) PreviousGait = DesiredGait;
	DesiredGait = (IsWantingToSprint ? GaitStateEnum::Running : PreviousGait);
}


// 
void AAbstractCharacter::SetCameraPerspective(const bool NewIsThirdPerson) {
	IsThirdPerson = NewIsThirdPerson;
}


// 
void AAbstractCharacter::GetMovementAnimationData_Implementation(FMovementAnimationDataStruct & GivenMovementAnimationData) const {
	GivenMovementAnimationData.Velocity = CurrentVelocity;
	GivenMovementAnimationData.Acceleration = Acceleration;
	GivenMovementAnimationData.MovementInput = LastControlInputVector;
	GivenMovementAnimationData.IsMoving = IsMoving;
	GivenMovementAnimationData.HasMovementInput = HasMovementInput;
	GivenMovementAnimationData.Speed = Speed;
	GivenMovementAnimationData.MovementInputAmount = MovementInputAmount;
	GivenMovementAnimationData.AimYawRate = AimYawRate;
	GivenMovementAnimationData.CapsuleRadius = CurrentRadius;
	GivenMovementAnimationData.CapsuleHalfHeight = CurrentHalfHeight;
	GivenMovementAnimationData.MaxAcceleration = MaxAcceleration;
	GivenMovementAnimationData.AimingRotation = RotatorComponent->GetRelativeRotation();
	GivenMovementAnimationData.ActorPreviousRotation = PreviousActorRotation;
	GivenMovementAnimationData.ActorControlRotation = this->CharacterSkeleton->GetRelativeRotation();
	GivenMovementAnimationData.ActorWorldLocation = this->GetActorLocation();
	GivenMovementAnimationData.MovementState = CurrentMovementState;
	GivenMovementAnimationData.PreviousMovementState = PreviousMovementState;
	GivenMovementAnimationData.RotationMode = RotationState;
	GivenMovementAnimationData.Gait = CurrentGait;
	GivenMovementAnimationData.MovementAction = 0;
	GivenMovementAnimationData.Stance = CurrentStance;
	GivenMovementAnimationData.ViewMode = ViewModeEnum::FirstPerson;
	GivenMovementAnimationData.OverlayState = 0;
}


// 
void AAbstractCharacter::TransferCharacterData(FArchive & GivenSaveOrLoadSystem) {}


// 
void AAbstractCharacter::ToGrounded() {
}


// 
void AAbstractCharacter::ToAirborne() {
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

	// Keep a counter to see if an actual change in transform has occured or not
	//     TODO: Could potentially change this to be !CalculatedMovementOffset.IsZero(); or !CalculatedMovementOffset.IsNearlyZero();
	uint8 ChangeCounter = 0;

	// If the previous floor's previous location does not equal its current location, then...
	if (FTransform::AreTranslationsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

		// Simply subtract the previous floor's previous location from its current location to get the difference in movement 
		FVector DeltaLocation = (FTransform::SubtractTranslations(CurrentPreviousFloorTransform, PreviousFloorPreviousTransform));

		// Collision check to ensure that there is nothing between your old position and the new position
		//     (e.g. standing still on a moving block that moves you into a non-moving wall)
		FPhysicsInterface::GeomSweepSingle(GetWorld(), CollisionShape, FQuat::Identity, CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + DeltaLocation, ECollisionChannel::ECC_Visibility, CollisionQueryParameters, FCollisionResponseParams::DefaultResponseParam);
		//GetWorld()->SweepSingleByChannel(CollisionResult, CurrentCollisionWorldPosition, CurrentCollisionWorldPosition + DeltaLocation, FQuat::Identity, ECollisionChannel::ECC_Visibility, CollisionShape, CollisionQueryParameters);

		// Finally, implement what's left of the delta location now
		CalculatedMovementOffset += (DeltaLocation * (CollisionResult.bBlockingHit ? CollisionResult.Time : 1.0f));

		// Increment the change counter since there was an actual positional change that now needs collision checking
		++ChangeCounter;
	}

	// If the previous floor's previous rotation does not equal its current rotation, then...
	if (FTransform::AreRotationsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

		// Locally store the rotational difference between the current and previous rotations
		FQuat DeltaRotation = (CurrentPreviousFloorTransform.GetRotation() - PreviousFloorPreviousTransform.GetRotation());

		// TODO: Convert rotational differences into positional differences
		// 

		// Increment the change counter since there was an actual positional change that now needs collision checking
		++ChangeCounter;
	}

	// If the previous floor's previous scale does not equal its current scale, then...
	if (FTransform::AreScale3DsEqual(PreviousFloorPreviousTransform, CurrentPreviousFloorTransform) == false) {

		// Locally store the scale difference between the current and previous scales
		FVector DeltaScale = (CurrentPreviousFloorTransform.GetScale3D() - PreviousFloorPreviousTransform.GetScale3D());

		// TODO: Convert scale differences into positional differences
		// 

		// Increment the change counter since there was an actual positional change that now needs collision checking
		++ChangeCounter;
	}

	// Finally, save the new transform to be the next frame's previous transform
	PreviousFloorPreviousTransform = CurrentPreviousFloorTransform;

	// If there has been a change in transform, then we need to check for walls now, else movement will be inaccurate
	if (ChangeCounter > 0) CalculateCollisionOffset();
}


// 
void AAbstractCharacter::CalculateGroundedMovement() {
	
	// Check if our previous floor (if valid) has transformed in any way and store the following movement within the CalculatedMovementOffset as an additive vector
	CalculatePreviousFloor();

	// TODO: Should we take into account our previous velocity??
	// Locally store our current collision world position plus any previous movement offsets that should be taken into account
	FVector LocalActorPosition = (CurrentCollisionWorldPosition + CalculatedMovementOffset);

	// If our collision trace does NOT hit something OR the floor is too steep to walk on, then we are to assume we are now airborne, so...
	// TODO: Update to FPhysicsInterface::GeomSweepSingle
	if (!GetWorld()->SweepSingleByChannel(CollisionResult, LocalActorPosition, LocalActorPosition + CurrentGravityOffset, CharacterCollisionModel.CollisionRotation[static_cast<uint32>(CurrentStance)], ECollisionChannel::ECC_Visibility, CollisionShape, CollisionQueryParameters) || (CollisionResult.ImpactNormal.Z > LARGEST_SLOPE_ANGLE)) {

		//// AIRBORNE
		// Update the current movement state to the new airborne state
		CurrentMovementState = MovementStateEnum::Airborne;

		// Update our movement offset to be where the collision sweep ended plus our previous velocity and move on
		CalculatedMovementOffset += (CurrentGravityOffset + (PreviousVelocity * CurrentDeltaTime));

		//// No need to calculate airborne as we already took into account movement in the previous calculations
		// Call the function to calculate the new airborne movement functionality to ensure that we are not left one frame behind due to the swap
		//CalculateAirborneMovement();

		// Finally, do an early return, as we do not need to calculate the rest of the grounded functionality
		return;
	}

	// Else, we are grounded, so we need to calculate our next movement now, so...
	// TODO: Calculate our grounded movement functionality now (apply user input to velocity, limit velocity by acceleration/deceleration, account for angular ground, account for missing ground, and account for any and all collisions
	//


	//// Take into account that we need to move closer to the final floor location
	//CalculatedMovementOffset += ((CollisionResult.TraceEnd - CollisionResult.TraceStart) * CollisionResult.Time);


	//// Velocity and acceleration calculations
	// 
	TargetVelocity = (MovementInput * ((AllowedGait == GaitStateEnum::Running ? CurrentMovementData.MaxRunningSpeed : (AllowedGait == GaitStateEnum::FastWalking ? CurrentMovementData.MaxFastWalkSpeed : CurrentMovementData.MaxWalkSpeed))));

	// 
	Acceleration = ((TargetVelocity - CurrentVelocity) * InverseDeltaTime);

	// 
	MaxAcceleration = FMath::FInterpTo(MaxAcceleration, ((TargetVelocity.SizeSquared() >= CurrentVelocity.SizeSquared()) ? CurrentMovementData.MaxAcceleration[(int32)CurrentGait] : CurrentMovementData.MaxDeceleration[(int32)CurrentGait]), CurrentDeltaTime, 5.0f);

	// Same as Acceleration.GetClampedToMaxSize(MaxAcceleration); but way more optimized
	float CurrentAccelerationSizeSquared = Acceleration.SizeSquared();
	if (CurrentAccelerationSizeSquared > FMath::Square(MaxAcceleration)) {
		const float Scale = MaxAcceleration * FMath::InvSqrt(CurrentAccelerationSizeSquared);
		Acceleration.X *= Scale;
		Acceleration.Y *= Scale;
		Acceleration.Z *= Scale;
	}
	
	// 
	CurrentVelocity += (Acceleration * CurrentDeltaTime);


	// 
	CalculatedMovementOffset += (CurrentVelocity * CurrentDeltaTime);


	// 
	CalculateCollisionOffset();

	// 
	LocalActorPosition = (CurrentCollisionWorldPosition + CalculatedMovementOffset);
	FVector MaxStepVector = FVector(0.0f, 0.0f, CharacterCollisionModel.MaxStepHeight[static_cast<uint32>(CurrentStance)]);

	// Collect the current floor and store it as the previous floor
	// TODO: Update to FPhysicsInterface::GeomSweepSingle
	GetWorld()->SweepSingleByChannel(CollisionResult, LocalActorPosition + MaxStepVector, LocalActorPosition - MaxStepVector, CharacterCollisionModel.CollisionRotation[static_cast<uint32>(CurrentStance)], ECollisionChannel::ECC_Visibility, CollisionShape, CollisionQueryParameters);
	
	// If this is a new floor component, then update all of the information
	if (PreviousFloorComponent.Get() != CollisionResult.GetComponent()) {
		PreviousFloorComponent = CollisionResult.Component;
		if (PreviousFloorComponent.IsValid())
			PreviousFloorPreviousTransform = PreviousFloorComponent->GetComponentTransform();
	}

	// Ensure that our position offset takes into account how far we are from the floor
	CalculatedMovementOffset += FVector(0.0f, 0.0f, (CollisionResult.Location.Z - LocalActorPosition.Z));


	// 
	Speed = CurrentVelocity.Size();



	// Calculate whether we are moving
	IsMoving = (CalculatedMovementOffset.SizeSquared() > 0.0f);

	// Calculate the last velocity rotation iff we are moving
	if (IsMoving) LastVelocityRotation = CalculatedMovementOffset.ToOrientationRotator();




	// Update Character Movement
		// Set the allowed gait
	if (CurrentStance == StanceEnum::Crouching) {
		AllowedGait = (DesiredGait == GaitStateEnum::Walking ? GaitStateEnum::Walking : GaitStateEnum::FastWalking);
	}
	else {
		if (DesiredGait == GaitStateEnum::Running) {

			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString((FMath::Abs((Acceleration.ToOrientationRotator() - RotatorComponent->GetRelativeRotation()).GetNormalized().Yaw) < 50.0f) ? TEXT("Can Sprint") : TEXT("Cannot Sprint")));

			// Can Sprint
			AllowedGait = ((HasMovementInput && (RotationState == RotationStateEnum::VelocityDirection ||
				((RotationState == RotationStateEnum::LookingDirection && (FMath::Abs((Acceleration.ToOrientationRotator() -
					RotatorComponent->GetRelativeRotation()).GetNormalized().Yaw) < 50.0f)))) && (MovementInputAmount > 0.9f)) ?
				GaitStateEnum::Running : GaitStateEnum::FastWalking);
		}
		else {
			AllowedGait = DesiredGait;
		}
	}

	// Determine the actual gait
	//if (Speed >= (((CurrentMovementData.MaxRunningSpeed - CurrentMovementData.MaxFastWalkSpeed) * 0.5f) + CurrentMovementData.MaxFastWalkSpeed)) ActualGait = GaitStateEnum::Running;
	//else if (Speed >= (((CurrentMovementData.MaxFastWalkSpeed - CurrentMovementData.MaxWalkSpeed) * 0.5f) + CurrentMovementData.MaxWalkSpeed)) ActualGait = GaitStateEnum::FastWalking;
	//else ActualGait = AllowedGait;
	if (Speed >= (CurrentMovementData.MaxFastWalkSpeed + 10.0f)) ActualGait = GaitStateEnum::Running;
	else if (Speed >= (CurrentMovementData.MaxWalkSpeed + 10.0f)) ActualGait = GaitStateEnum::FastWalking;
	else ActualGait = AllowedGait;

	// 
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("AllowedGait: ") + FString::FromInt(static_cast<int32>(AllowedGait))));
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("ActualGait: ") + FString::FromInt(static_cast<int32>(ActualGait))));
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("CurrentGait: ") + FString::FromInt(static_cast<int32>(CurrentGait))));
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("DesiredGait: ") + FString::FromInt(static_cast<int32>(DesiredGait))));
		GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Orange, FString(TEXT("PreviousGait: ") + FString::FromInt(static_cast<int32>(PreviousGait))));
	}

	/*
	if (Speed >= (CurrentMovementData.MaxFastWalkSpeed + 10.0f)) {
		ActualGait = (AllowedGait == GaitStateEnum::Running ? GaitStateEnum::Running : GaitStateEnum::FastWalking);
	} else {
		ActualGait = ((Speed >= (CurrentMovementData.MaxWalkSpeed + 10.0f)) ? GaitStateEnum::FastWalking : GaitStateEnum::Walking);
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
	case RotationStateEnum::VelocityDirection:
		CurrentMovementData = ((CurrentStance == StanceEnum::Crouching) ? CrouchingVelocityDirectionMovementData : StandingVelocityDirectionMovementData);
		break;

		// 
	default:
	case RotationStateEnum::LookingDirection:
		CurrentMovementData = ((CurrentStance == StanceEnum::Crouching) ? CrouchingLookingDirectionMovementData : StandingLookingDirectionMovementData);
		break;

		// 
	case RotationStateEnum::Aiming:
		CurrentMovementData = ((CurrentStance == StanceEnum::Crouching) ? CrouchingAimingMovementData : StandingAimingMovementData);
		break;
	}

	// Update grounded rotation
	// If we can update moving rotation, then...
	if ((IsMoving && HasMovementInput) || (Speed > 150.0f)) {

		// Change grounded rotation functionality based off the current rotation state
		switch (RotationState) {

			// 
		case RotationStateEnum::VelocityDirection:
			SmoothCharacterRotation(FRotator(0.0, LastVelocityRotation.Yaw, 0.0), 800.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			break;

			// 
		default:
		case RotationStateEnum::LookingDirection:

			// 
			if (CurrentGait == GaitStateEnum::Running) {
				SmoothCharacterRotation(FRotator(0.0, LastVelocityRotation.Yaw, 0.0), 500.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			}

			// 
			else {
				SmoothCharacterRotation(FRotator(0.0, (RotatorComponent->GetRelativeRotation().Yaw + (IsValid(CharacterSkeleton->GetAnimInstance()) ? CharacterSkeleton->GetAnimInstance()->GetCurveValue(FName(TEXT("YawOffset"))) : 0.0)), 0.0), 500.0, (CurrentMovementData.RotationInterpSpeed[(unsigned long long)CurrentGait] * FMath::GetMappedRangeValueClamped(FVector2D(0.0, 300.0), FVector2D(1.0, 3.0), AimYawRate)), CurrentDeltaTime);
			}

			// 
			break;

			// 
		case RotationStateEnum::Aiming:
			SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw, 0.0), 1000.0, 20.0, CurrentDeltaTime);
			break;
		}
	}

	// else, we cannot updating moving rotation, so...
	else {

		// If we are in first person or third person while aiming, then...
		if (!IsThirdPerson || (IsThirdPerson && RotationState == RotationStateEnum::Aiming)) {

			// Limit Rotation
			double YawDelta = (RotatorComponent->GetRelativeRotation() - CharacterSkeleton->GetRelativeRotation()).GetNormalized().Yaw;
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Red, FString(TEXT("YawDelta == ")) + FString::SanitizeFloat(YawDelta));
			if ((YawDelta < -100.0f) || (YawDelta > 100.0f)) {
				SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw - ((YawDelta > 0.0f) ? 100.0f : -100.0f), 0.0f), 0.0f, 20.0f, CurrentDeltaTime);
			}
		}

		// Apply the RotationAmount curve from Turn In Place animations (fixed at an animated framerate of 30 fps)
		float AnimCurveValue = (IsValid(CharacterSkeleton->GetAnimInstance()) ? CharacterSkeleton->GetAnimInstance()->GetCurveValue(FName(TEXT("RotationAmount"))) : 0.0f);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, CurrentDeltaTime, FColor::Red, FString(TEXT("AnimCurveValue == ")) + FString::SanitizeFloat(AnimCurveValue));
		if (FMath::Abs(AnimCurveValue) > 0.001f) {
			CharacterSkeleton->AddRelativeRotation(FRotator(0.0f, (AnimCurveValue * CurrentDeltaTime * 30.0f), 0.0f));
			TargetRotation = CharacterSkeleton->GetRelativeRotation();
		}
	}
}


// 
void AAbstractCharacter::CalculateAirborneMovement() {

	// Locally store our current position plus our current velocity (since we are falling in a set direction)
	FVector LocalActorPosition = (this->GetActorLocation() + (CurrentVelocity * CurrentDeltaTime) + FVector(0.0f, 0.0f, CurrentHalfHeight));

	// If our collision trace hits something, then we are to assume we are now grounded, so...
	// TODO: Update to FPhysicsInterface::GeomSweepSingle
	if (GetWorld()->SweepSingleByChannel(CollisionResult, LocalActorPosition, LocalActorPosition + CurrentGravityOffset, CharacterCollisionModel.CollisionRotation[static_cast<uint32>(CurrentStance)], ECollisionChannel::ECC_Visibility, CollisionShape, CollisionQueryParameters)) {

		//// GROUNDED
		// Update the current movement state to the new grounded state
		CurrentMovementState = MovementStateEnum::Grounded;

		// 
		CalculatedMovementOffset += ((CollisionResult.TraceEnd - CollisionResult.TraceStart) * CollisionResult.Time);

		// Call the function to calculate the new grounded movement functionality to ensure that we are not left one frame behind due to the swap
		//CalculateGroundedMovement();

		// Finally, do an early return, as we do not need to calculate the rest of the airborne functionality
		return;
	}

	// Else, we are airborne, so we need to calcualte our next movement now, so...
	// TODO: Calculate airborne movement now (apply gravity to velocity, apply user input to velocity IFF allowed, and account for any and all collisions)
	// 

	// 
	CurrentVelocity += (CurrentGravityOffset * InverseDeltaTime);

	// 
	CalculatedMovementOffset += (CollisionResult.TraceEnd - this->GetActorLocation());

	// 
	CalculateCollisionOffset();

	// Ensure that there is no previous floor, as we are now in the air and do not need such a thing
	PreviousFloorComponent = nullptr;
	PreviousFloorPreviousTransform = FTransform::Identity;


	// 
	Speed = CalculatedMovementOffset.Size() * InverseDeltaTime;



	// Calculate whether we are moving
	IsMoving = (CalculatedMovementOffset.SizeSquared() > 0.0f);

	// Calculate the last velocity rotation iff we are moving
	if (IsMoving) LastVelocityRotation = CalculatedMovementOffset.ToOrientationRotator();




	// 
	if (RotationState == RotationStateEnum::Aiming) {

		// 
		SmoothCharacterRotation(FRotator(0.0, RotatorComponent->GetRelativeRotation().Yaw, 0.0), 0.0f, 15.0f, CurrentDeltaTime);
		InAirRotation = CharacterSkeleton->GetRelativeRotation();
	}

	// 
	else {

		// 
		SmoothCharacterRotation(FRotator(0.0, InAirRotation.Yaw, 0.0), 0.0f, 5.0f, CurrentDeltaTime);
	}

	// 
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
void AAbstractCharacter::CalculateCollisionOffset() {

	// 
	// 
}


// 
void AAbstractCharacter::StorePreviousValues() {

	// Store the current velocity as the previous now
	PreviousVelocity = CurrentVelocity;

	// Store the current aim yaw as previous now
	PreviousAimYaw = this->RotatorComponent->GetRelativeRotation().Yaw;

	// 
	PreviousMovementState = CurrentMovementState;

	// 
	PreviousActorRotation = this->CharacterSkeleton->GetRelativeRotation();
}


// 
void AAbstractCharacter::DrawDebugShapes() {

	// 
	DebugCapsuleCollision->SetRelativeLocation(FVector(0.0, 0.0, CurrentHalfHeight + CharacterCollisionModel.MaxStepHeight[static_cast<uint32>(CurrentStance)]));
	DebugCapsuleCollision->SetCapsuleHalfHeight(CurrentHalfHeight);
	DebugCapsuleCollision->SetCapsuleRadius(CurrentRadius);


	// Draw a debug arrow to visualize our actual movement
	DebugVelocityArrow->SetWorldScale3D(FVector(Speed / ((AllowedGait == GaitStateEnum::Running ? CurrentMovementData.MaxRunningSpeed : (AllowedGait == GaitStateEnum::FastWalking ? CurrentMovementData.MaxFastWalkSpeed : CurrentMovementData.MaxWalkSpeed)))));
	DebugVelocityArrow->ArrowLength = Speed;
	if (!CalculatedMovementOffset.IsNearlyZero()) DebugVelocityArrow->SetWorldRotation(CalculatedMovementOffset.ToOrientationRotator());

	// Draw a debug arrow to visualize what our wanted direction is
	FVector LocalVector = (MovementInput * ((AllowedGait == GaitStateEnum::Running ? CurrentMovementData.MaxRunningSpeed : (AllowedGait == GaitStateEnum::FastWalking ? CurrentMovementData.MaxFastWalkSpeed : CurrentMovementData.MaxWalkSpeed))) * CurrentDeltaTime);
	DebugMovementArrow->SetWorldRotation(LocalVector.ToOrientationRotator());
}
