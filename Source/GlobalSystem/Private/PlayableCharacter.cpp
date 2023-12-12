// TreeTrunkStudios (c) 2023


// 
#include "PlayableCharacter.h"
#include "EnhancedInputComponent.h"
#include "InputImageSubsystem.h"


// 
void APlayableCharacter::HandleItemInteraction(const FName & GivenItemName, AInteractableItem * GivenActor) {

	// DEBUG: Post the interactable item's given item name and the given actor's name
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Emerald, GivenItemName.ToString() + FString(TEXT(": ")) + (GivenActor ? GivenActor->GetName() : FString(TEXT("nullptr"))));
	}
}


// 
void APlayableCharacter::HandleInteractItem(const FBasicInventoryItem * InventoryItemToUse, const int32 TargetItemArrayIndex) {

	// Ensure that this function is NEVER called with a nullptr item struct
	ensureAlways(InventoryItemToUse != nullptr);

	// Switch functionality based on the given item type's use type
	switch(InventoryItemToUse->ItemData.ItemUseType) {

		// We need to use this item, so...
		case EInventoryUseType::Use:
			break;

		// We need to equip this item, so...
		case EInventoryUseType::Equip:
			break;

		// Default to doing nothing, as none of the other enum states mean anything to us for this game
		default:
			break;
	}
}


// 
void APlayableCharacter::HandleTransferItem(const FName ItemToTransferName, const int32 ItemStackCount, const int32 SlotIndex) {

	// TODO: Implement functionality to move the given item to the wanted quick slot
	// 
}


// 
void APlayableCharacter::HandleDiscardItem(const FName ItemToDiscardName, const int32 ItemStackCount) {

	// TODO: Create a sub-class of AInteractableItem (or edit the normal class to support this functionality) which can
	//     initialize to a fixed count (e.g. rather than randomly generating a pickup amount on interaction, instead it
	//     spawns with that fixed count which always returns that value)
	//GetWorld()->SpawnActor<AInteractableItem>();
}


// 
void APlayableCharacter::HandleCustomInventoryUpgrades(const int32 ExtrasBitmask) {

	// TODO: Implement a custom subsystem in order to handle inventory-specific upgrades (e.g. increased drop rates and such)
	// 
}


// 
void APlayableCharacter::HandleInventoryUserInterface() {

	//
	UEnhancedInputLocalPlayerSubsystem * LocalEnhancedInputSubsystem = Cast<APlayerController>(GetController())->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	//
	LocalEnhancedInputSubsystem->InjectInputVectorForAction(InputActionMovement, FVector::ZeroVector, InputActionMovement->Modifiers, InputActionMovement->Triggers);
	
	//
	LocalEnhancedInputSubsystem->RemoveMappingContext(InputMapping);

	//
	BasicInventoryComponent->CreateInventoryUserInterface();
}


// 
void APlayableCharacter::OpenInventoryUserInterface() const {

	// 
	APlayerController * CurrentPlayerController = Cast<APlayerController>(GetController());
	//CurrentPlayerController->SetShowMouseCursor(true);
	CurrentPlayerController->SetInputMode(FInputModeGameAndUI{});
}


// 
void APlayableCharacter::CloseInventoryUserInterface() const {

	//
	UEnhancedInputLocalPlayerSubsystem * LocalEnhancedInputSubsystem = Cast<APlayerController>(GetController())->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

	//
	LocalEnhancedInputSubsystem->AddMappingContext(InputMapping, 10);

	// 
	APlayerController * CurrentPlayerController = Cast<APlayerController>(GetController());
	//CurrentPlayerController->SetShowMouseCursor(false);
	CurrentPlayerController->SetInputMode(FInputModeGameOnly{});
}


// 
APlayableCharacter::APlayableCharacter() : Super() {
	
	// 
	SubtitleComponent = CreateDefaultSubobject<USubtitleComponent>(TEXT("SubtitleComponent"));

	// 
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// 
	BasicInventoryComponent = CreateDefaultSubobject<UBasicInventoryComponent>(TEXT("BasicInventoryComponent"));

	// 
	TargetSubtitleWidgetClass = ConstructorHelpers::FClassFinder<UAbstractSubtitleWidget>(TEXT("/Game/UserInterfaces/GameplayWidgets/Subtitles/SubtitleWidget")).Class;
}


// 
void APlayableCharacter::BeginPlay() {

	// Handle the parent class' begin play functionality first
	Super::BeginPlay();

	//// Handle online functionality here
	// Only proceed with the following functionality (subtitles and interactable checking) iff we are the local machine,
	//     as these are cosmetic functionalities that do not need to occur on the server
	//if (IsLocallyControlled() == false) return;


	//// Handle all custom subtitle functionality here
	// Locally spawn an abstract subtitle widget of the target subtitle widget class
	AbstractSubtitleWidget = Cast<UAbstractSubtitleWidget>(CreateWidget(Cast<APlayerController>(GetController()), TargetSubtitleWidgetClass));

	// Ensure that the widget is always created successfully
	check(AbstractSubtitleWidget);

	// Bind all of the subtitle component's delegates onto functions contained within the abstract subtitle widget for simplistic handling
	SubtitleComponent->StartAudioSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::AddUniqueAudioSubtitle);
	SubtitleComponent->StopAudioSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::RemoveUniqueAudioSubtitle);
	SubtitleComponent->StartVisualSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::AddUniqueVisualSubtitle);
	SubtitleComponent->StopVisualSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::RemoveUniqueVisualSubtitle);

	// Add the abstract subtitle widget to the current actor's player's screen
	AbstractSubtitleWidget->AddToPlayerScreen();


	//// Handle all custom interaction functionality here
	// Tie the wanted interaction function of this class to the owned interaction component for handling inventory items
	InteractionComponent->CollectItemDelegate.BindUObject(this, &APlayableCharacter::HandleItemInteraction);


	//// Handle all custom inventory functionality here
	BasicInventoryComponent->InteractItemDelegate.BindUObject(this, &APlayableCharacter::HandleInteractItem);
	BasicInventoryComponent->TransferItemDelegate.BindUObject(this, &APlayableCharacter::HandleTransferItem);
	BasicInventoryComponent->DiscardItemDelegate.BindUObject(this, &APlayableCharacter::HandleDiscardItem);
	BasicInventoryComponent->HandleCustomInventoryUpgradesDelegate.BindUObject(this, &APlayableCharacter::HandleCustomInventoryUpgrades);
	BasicInventoryComponent->OpenUserInterfaceDelegate.BindUObject(this, &APlayableCharacter::OpenInventoryUserInterface);
	BasicInventoryComponent->CloseUserInterfaceDelegate.BindUObject(this, &APlayableCharacter::CloseInventoryUserInterface);
	
	// Only create and cache the inventory user interface widget IFF the component is owned by a player, as AI cannot use widgets
	if (IsPlayerControlled()) {
		
		// Create the inventory user interface now in order to cache it for easier functionality in the future
		BasicInventoryComponent->CreateInventoryUserInterface();

		// Immediately remove the cached inventory user interface from the player's screen, as we do not want to display it yet
		BasicInventoryComponent->CloseUserInterface();
	}


	//// Handle creating the asynchronous infinite vision sweep for better performance at a slight cost to responsiveness on weaker hardware
	// Locally create a delegate 
	CameraCenterDelegate = FTraceDelegate::CreateUObject(this, &APlayableCharacter::HandleCameraCenterCollision);

	// Start our asynchronous infinite sweeping into the world in order to test for anything that must respond to our sight
	GetWorld()->AsyncSweepByChannel(CameraCenterTraceType, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, CameraCenterCollision, FCollisionQueryParams{NAME_None, false, this}, FCollisionResponseParams::DefaultResponseParam, &CameraCenterDelegate);
}


// 
void APlayableCharacter::Tick(const float DeltaTime) {

	// 
	Super::Tick(DeltaTime);

	// 
	// 
}


// 
void APlayableCharacter::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent) {

	// Call the parent setup process now, to allow all of that to still function as wanted
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get the local player subsystem
	UEnhancedInputLocalPlayerSubsystem * InputSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(Cast<APlayerController>(GetController())->GetLocalPlayer());

	// Add our wanted menu mapping context at a lower priority than the normal input mapping
	InputSubSystem->AddMappingContext(InputMappingMenu, 0);

	// Locally store the enhanced input component by simply casting the current input component to the wanted type
	UEnhancedInputComponent * EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	// Setup the input action for the interact functionality to be immediately passed to this character's interaction component for handling
	EnhancedInput->BindAction(InputActionInteraction, ETriggerEvent::Triggered, InteractionComponent, &UInteractionComponent::Interact);
	EnhancedInput->BindAction(InputActionOpenInventory, ETriggerEvent::Triggered, this, &APlayableCharacter::HandleInventoryUserInterface);

	//
	BasicInventoryComponent->BindInputActions(PlayerInputComponent);
}


// 
void APlayableCharacter::HandleCameraCenterCollision(const FTraceHandle & GivenTraceHandle, FTraceDatum & GivenTraceDatum) const {

	// 
	SubtitleComponent->HandleVisualSubtitles(GivenTraceHandle, GivenTraceDatum);

	// 
	InteractionComponent->HandleInteractions(GivenTraceHandle, GivenTraceDatum);

	// Continue our asynchronous infinite sweeping into the world in order to test for anything that must respond to our sight
	GetWorld()->AsyncSweepByChannel(CameraCenterTraceType, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, CameraCenterCollision, FCollisionQueryParams{NAME_None, false, this}, FCollisionResponseParams::DefaultResponseParam, &CameraCenterDelegate);
}
