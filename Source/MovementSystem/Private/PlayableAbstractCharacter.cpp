// Sidney Towers
// TreeTrunkStudios (c) 2023


// All required include files
#include "PlayableAbstractCharacter.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
APlayableAbstractCharacter::APlayableAbstractCharacter() : Super() {

	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bTickEvenWhenPaused = false;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PrePhysics;
	PrimaryActorTick.EndTickGroup = ETickingGroup::TG_StartPhysics;

	// 
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->SetupAttachment(this->RotatorComponent);
	PlayerCamera->SetRelativeTransform(FTransform::Identity);

	// Initialize the actor's relative transform to an identity to remove all potential misplacements
	this->SetActorRelativeTransform(FTransform::Identity);

	// Collect the wanted input mapping context from blueprints at constructor time and store it as a raw pointer (as it should always be valid objects 24/7)
	InputMapping = ConstructorHelpers::FObjectFinder<UInputMappingContext>(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/InputSystem/InputMappings/PlayableCharacterMappings.PlayableCharacterMappings'")).Object;

	// Collect the wanted input actions from blueprints at constructor time and store them as raw pointers (as they should always be valid objects 24/7)
	InputActionMovement = ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/InputActionsCharacter/Movement.Movement'")).Object;
	InputActionLooking = ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/InputActionsCharacter/Looking.Looking'")).Object;
	InputActionMouseLooking = ConstructorHelpers::FObjectFinder<UInputAction>(TEXT("/Script/EnhancedInput.InputAction'/Game/InputSystem/InputActionsCharacter/MouseLooking.MouseLooking'")).Object;
}


// Called when the game starts or when spawned
void APlayableAbstractCharacter::BeginPlay() {

	// Call parent class' begin play functionality first
	Super::BeginPlay();

	// Set our player camera's relative location to initialize at the wanted relative offset based on our current view mode
	TargetRelativeOffset = CameraOffset[static_cast<uint32>(CurrentViewMode)];
	PreviousRelativeOffset = TargetRelativeOffset;
	PlayerCamera->SetRelativeLocation(TargetRelativeOffset);
}


// Called every frame
void APlayableAbstractCharacter::Tick(float DeltaTime) {

	// Call parent class' tick functionality first
	Super::Tick(DeltaTime);

	// Locally store our target socket location, as it would get expensive otherwise
	const FVector HeadSocketLocation = CharacterSkeleton->GetSocketLocation(FName(TEXT("Head")));

	// Interpolate our rotator's world location to follow the skeletal mesh's head smoothly
	RotatorComponent->SetWorldLocation(FMath::VInterpTo(RotatorComponent->GetComponentLocation(), HeadSocketLocation, DeltaTime, 5.0f));

	// If our target relative offset is different from our previous relative offset, then...
	if (TargetRelativeOffset.Equals(PreviousRelativeOffset) == false) {

		// Calculate our local delta this frame
		const FVector LocalDelta = (FMath::VInterpTo(PreviousRelativeOffset, TargetRelativeOffset, DeltaTime, 5.0f) - PreviousRelativeOffset);

		// Add the local delta onto the player camera
		PlayerCamera->AddRelativeLocation(LocalDelta);

		// Then also add the local delta onto the previous relative offset
		PreviousRelativeOffset += LocalDelta;
	}

	// Check for if there is something between the head socket and the target camera location and, if so, then adjust our relative position accordingly
	FHitResult LocalCameraCheck;
	if (FPhysicsInterface::GeomSweepSingle(GetWorld(), FCollisionShape::MakeSphere(10.0f), FQuat::Identity, LocalCameraCheck, HeadSocketLocation, HeadSocketLocation + RotatorComponent->GetComponentQuat().RotateVector(PlayerCamera->GetRelativeLocation()), ECollisionChannel::ECC_Visibility, FCollisionQueryParams{ NAME_None, false, this }, FCollisionResponseParams::DefaultResponseParam)) {
		const FVector LocalVector = RotatorComponent->GetComponentQuat().UnrotateVector(LocalCameraCheck.Location - LocalCameraCheck.TraceEnd);
		PlayerCamera->AddRelativeLocation(LocalVector);
		PreviousRelativeOffset += LocalVector;
	}
}


// Called to bind functionality to input
void APlayableAbstractCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {

	// Call parent class' setup player input component first
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem * InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Clear out existing mapping, and add our mapping
	InputSubSystem->ClearAllMappings();
	InputSubSystem->AddMappingContext(InputMapping, 0);

	// Locally store the enhanced input component by simply casting the current input component to the wanted type
	UEnhancedInputComponent * EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Attach all necessary input functions to their target actions
	EnhancedInput->BindAction(InputActionMovement, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::HandleMovementInput);
	EnhancedInput->BindAction(InputActionLooking, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::HandleLookingInput);
	EnhancedInput->BindAction(InputActionMouseLooking, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::HandleLookingInput);

	// 
	EnhancedInput->BindAction(InputActionChangeGait, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::ToggleGait);

	// 
	EnhancedInput->BindAction(InputActionChangeStance, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::ToggleStance);

	// 
	EnhancedInput->BindAction(InputActionChangeSprint, ETriggerEvent::Triggered, this, &APlayableAbstractCharacter::ToggleSprint);
}


// 
void APlayableAbstractCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {

	// 
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// 
	DOREPLIFETIME(APlayableAbstractCharacter, ReplicatedTargetRotation);
}


// 
void APlayableAbstractCharacter::HandleMovementInput(const FInputActionInstance & Instance) {

	// ERROR: This class is made to function entirely WITHOUT a movement component, which causes errors. Apologies for the inconvienience!!
	ensureAlways(!GetMovementComponent());

	// 
	//ensureAlways(Instance.GetValue().Get<FVector>().IsNearlyZero() == false);

	// Locally store the current instance's movement input vector for ease of use
	const FVector WantedMovementInput = RotatorComponent->GetComponentQuat().RotateVector(Instance.GetValue().Get<FVector>());

	// 
	MovementInput = WantedMovementInput;

	// If the movement input has not changed, then do not waste the networking bandwidth "updating" it
	//if (WantedMovementInput.Equals(PreviousMovementInput) == false) {

		// Update the previous value to be the new value
		//PreviousMovementInput = WantedMovementInput;

		// Update the movement input vector globally, to the server, and then to the clients as well
		//this->HandleMovementInput_Replicated(WantedMovementInput);

		// 
		//HandleMovementInput_Replicated_Implementation(WantedMovementInput);
	//}

	// Simply rotate our input vector by the rotation of the RotatorComponent in order to make the inputs follow the camera's Yaw rotation
	//     NOTE: This is done this way in order to allow future implementations with a changing gravity axis, as well as to be more performant with vector intrensics
	//Internal_AddMovementInput(RotatorComponent->GetComponentQuat().RotateVector(Instance.GetValue().Get<FVector>()), false);
}


// 
void APlayableAbstractCharacter::HandleMovementInput_Replicated_Implementation(const FVector GivenMovementVector) {
	
	// 
	//ensureAlways(GivenMovementVector.IsNearlyZero() == false);

	// Simply rotate our input vector by the rotation of the RotatorComponent in order to make the inputs follow the camera's Yaw rotation
	//     NOTE: This is done this way in order to allow future implementations with a changing gravity axis, as well as to be more performant with vector intrensics
	//Internal_AddMovementInput(GivenMovementVector, false);
	MovementInput = GivenMovementVector;
}


// 
void APlayableAbstractCharacter::HandleLookingInput(const FInputActionInstance & Instance) {

	// Locally store the looking input vector from the FInputActionInstance, rather than getting it two times per function call
	const FVector2D LookingInputVector2D = Instance.GetValue().Get<FVector2D>();

	// 
	const double LocalCameraPitch = (PlayerCamera->GetRelativeRotation().Pitch + LookingInputVector2D.Y);
	ReplicatedTargetRotation = FRotator(FMath::Clamp(LocalCameraPitch, MinCameraAngle, MaxCameraAngle), LookingInputVector2D.X, 0.0);

	// 
	this->ReplicateTargetRotationHandler();
}


// 
void APlayableAbstractCharacter::ToggleCameraPerspective() {

	// Call the parent functionality first
	Super::ToggleCameraPerspective();

	// 
	PreviousRelativeOffset = FVector::ZeroVector;

	// 
	if (CurrentViewMode == EViewMode::ThirdPerson) {
		TargetRelativeOffset = PlayerCamera->GetRelativeRotation().RotateVector(CameraOffset[(uint32)EViewMode::ThirdPerson]);
	}

	// 
	else {
		TargetRelativeOffset = CameraOffset[(uint32)EViewMode::FirstPerson];
	}

	// 
	TargetRelativeOffset -= PlayerCamera->GetRelativeLocation();
}


// 
void APlayableAbstractCharacter::TransferCharacterData(FArchive & GivenSaveOrLoadSystem) {

	// 
	Super::TransferCharacterData(GivenSaveOrLoadSystem);
	
	// Locally collect all player transform data including camera rotations
	FTransform LocalTransform = this->GetTransform();
	double CameraRotationYaw = this->RotatorComponent->GetComponentRotation().Yaw;
	double CameraRotationPitch = this->PlayerCamera->GetComponentRotation().Pitch;

	// Load or save information from the given archive
	GivenSaveOrLoadSystem << LocalTransform;
	GivenSaveOrLoadSystem << CameraRotationYaw;
	GivenSaveOrLoadSystem << CameraRotationPitch;

	// Finally, reset all transform data in case of a load
	this->SetActorTransform(LocalTransform);
	this->RotatorComponent->SetRelativeRotation(FRotator(0.0, CameraRotationYaw, 0.0));
	this->PlayerCamera->SetRelativeRotation(FRotator(FMath::Clamp(CameraRotationPitch, -90.0, 90.0), 0.0, 0.0));
}
