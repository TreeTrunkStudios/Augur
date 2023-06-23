// Sidney Towers
// TreeTrunkStudios (c) 2023


// All required include files
#include "AbstractCharacter.h"
#include "DrawDebugHelpers.h"


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
	//BaseComponent->SetupAttachment(this->RootComponent);
	BaseComponent->SetRelativeTransform(FTransform::Identity);
	this->SetRootComponent(BaseComponent);

	// 
	CharacterSkeleton = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterSkeleton"));
	CharacterSkeleton->SetupAttachment(this->RootComponent);
	CharacterSkeleton->SetRelativeTransform(FTransform::Identity);
	CharacterSkeleton->SetRelativeRotation(FRotator(0.0, -90.0, 0.0));

	// 
#ifdef UE_BUILD_DEBUG
	DebugCapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("DebugCapsuleCollision"));
	DebugCapsuleCollision->SetRelativeTransform(FTransform::Identity);
	DebugCapsuleCollision->SetupAttachment(this->RootComponent);
#endif

	// 
	this->SetActorRelativeTransform(FTransform::Identity);
}


// Called when the game starts or when spawned
void AAbstractCharacter::BeginPlay() {

	// Call parent class' begin play functionality first
	Super::BeginPlay();
	
	// 
	//
}


// Called every frame
void AAbstractCharacter::Tick(float DeltaTime) {

	// Call parent class' tick functionality first
	Super::Tick(DeltaTime);

	// 
	DrawDebugCapsule(GetWorld(), GetActorLocation() + FVector(0.0, 0.0, CharacterCollisionModel.HalfHeight + CharacterCollisionModel.MaxStepHeight), CharacterCollisionModel.HalfHeight, CharacterCollisionModel.Radius, FQuat::Identity, FColor::Red, false, DeltaTime);

	// TESTING ONLY
	this->AddActorWorldOffset(this->ConsumeMovementInputVector() * 10.0);
	
}


// Simply call the parent's functionality, as this class is for all character movement
void AAbstractCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


// 
#ifdef UE_BUILD_DEBUG
void AAbstractCharacter::OnConstruction(const FTransform & Transform) {
	Super::OnConstruction(Transform);
	DebugCapsuleCollision->SetRelativeLocation(Transform.GetLocation() + FVector(0.0, 0.0, CharacterCollisionModel.HalfHeight + CharacterCollisionModel.MaxStepHeight));
	DebugCapsuleCollision->SetCapsuleHalfHeight(CharacterCollisionModel.HalfHeight);
	DebugCapsuleCollision->SetCapsuleRadius(CharacterCollisionModel.Radius);
}
#endif
