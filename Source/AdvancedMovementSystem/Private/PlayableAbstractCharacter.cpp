// Sidney Towers
// TreeTrunkStudios (c) 2023


// All required include files
#include "PlayableAbstractCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/PawnMovementComponent.h"


// Sets default values
APlayableAbstractCharacter::APlayableAbstractCharacter() {

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

	// 
	// 
}


// Called every frame
void APlayableAbstractCharacter::Tick(float DeltaTime) {

	// TODO: Implement functionality that is player specific, such as...
	//     Placeholder

	// Call parent class' tick functionality first
	Super::Tick(DeltaTime);

	// 
	RotatorComponent->SetWorldLocation(CharacterSkeleton->GetSocketLocation(FName(TEXT("Head"))));

	// 
	PlayerCamera->SetRelativeLocation(FMath::VInterpTo(PlayerCamera->GetRelativeLocation(), (IsThirdPerson ? ThirdPersonCameraOffset : FirstPersonCameraOffset), DeltaTime, 5.0f));
}


// Called to bind functionality to input
void APlayableAbstractCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {

	// Call parent class' setup player input component first
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem * InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// 
	ensureAlways(InputSubSystem != nullptr);

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
void APlayableAbstractCharacter::HandleMovementInput(const FInputActionInstance & Instance) {

	// ERROR: This class is made to function entirely WITHOUT a movement component, which causes errors. Apologies for the inconvienience!!
	ensureAlways(!GetMovementComponent());

	// Locally store the movement input vector from the FInputActionInstance, rather than getting it four times per function call
	FVector2D MovementInputVector2D = Instance.GetValue().Get<FVector2D>();

	// Locally store the forward and right vector, rather than calling that attrocious TransformVectorNoScale function four times
	FVector ActorForwardVector = this->RotatorComponent->GetForwardVector();
	FVector ActorRightVector = this->RotatorComponent->GetRightVector();

	// Finally, add to the movement input vector the newly calculated movement 3D vector based off the given 2D inputs
	Internal_AddMovementInput(FVector(((ActorForwardVector.X * MovementInputVector2D.Y) + (ActorRightVector.X * MovementInputVector2D.X)),
									  ((ActorForwardVector.Y * MovementInputVector2D.Y) + (ActorRightVector.Y * MovementInputVector2D.X)),
									  0.0), false);
}


// 
void APlayableAbstractCharacter::HandleLookingInput(const FInputActionInstance & Instance) {

	// Locally store the looking input vector from the FInputActionInstance, rather than getting it two times per function call
	FVector2D LookingInputVector2D = Instance.GetValue().Get<FVector2D>();
	
	// Apply the given yaw rotation to the actor as a whole, rotating us in world space
	this->RotatorComponent->AddRelativeRotation(FRotator(0.0, LookingInputVector2D.X, 0.0));

	// Lastly, apply the given pitch to just the camera, as it is the only thing that needs to rotate on that axis, while also clamping the value (no rotating your head broken)
	double LocalCameraPitch = (this->PlayerCamera->GetComponentRotation().Pitch + LookingInputVector2D.Y);
	this->PlayerCamera->SetRelativeRotation(FRotator(FMath::Clamp(LocalCameraPitch, -90.0, 90.0), 0.0, 0.0));
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
