// TreeTrunkStudios (c) 2023


// 
#include "PlayableCharacter.h"


// 
APlayableCharacter::APlayableCharacter() {
	
	// 
	SubtitleComponent = CreateDefaultSubobject<USubtitleComponent>(TEXT("SubtitleComponent"));

	// 
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));

	// 
	BasicInventoryComponent = CreateDefaultSubobject<UBasicInventoryComponent>(TEXT("BasicInventoryComponent"));

	// 
	TargetSubtitleWidgetClass = ConstructorHelpers::FClassFinder<UAbstractSubtitleWidget>(TEXT("/Game/UserInterfaces/GameplayWidgets/SubtitleWidget")).Class;
}


// 
void APlayableCharacter::BeginPlay() {

	// Handle the parent class' begin play functionality first
	Super::BeginPlay();

	//// 
	// Only proceed with the following functionality (subtitles and interactable checking) iff we are the local machine,
	//     as these are cosmetic functionalities that do not need to occur on the server
	if (IsLocallyControlled() == false) return;

	//// 
	// 
	AbstractSubtitleWidget = Cast<UAbstractSubtitleWidget>(CreateWidget(Cast<APlayerController>(GetController()), TargetSubtitleWidgetClass));

	// 
	check(AbstractSubtitleWidget);

	// 
	SubtitleComponent->StartAudioSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::AddUniqueAudioSubtitle);
	SubtitleComponent->StopAudioSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::RemoveUniqueAudioSubtitle);
	SubtitleComponent->StartVisualSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::AddUniqueVisualSubtitle);
	SubtitleComponent->StopVisualSubtitleDelegate.BindUObject(AbstractSubtitleWidget, &UAbstractSubtitleWidget::RemoveUniqueVisualSubtitle);

	// 
	AbstractSubtitleWidget->AddToPlayerScreen();


	//// Handle creating the asynchronous infinite vision sweep for better performance at a slight cost to responsiveness on weaker hardware
	// Locally create a delegate 
	CameraCenterDelegate = FTraceDelegate::CreateUObject(this, &APlayableCharacter::HandleCameraCenterCollision);

	// Start our asynchronous infinite sweeping into the world in order to test for anything that must respond to our sight
	GetWorld()->AsyncSweepByChannel(CameraCenterTraceType, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, CameraCenterCollision, FCollisionQueryParams{NAME_None, false, this}, FCollisionResponseParams::DefaultResponseParam, &CameraCenterDelegate);
}


// 
void APlayableCharacter::Tick(float DeltaTime) {

	// 
	Super::Tick(DeltaTime);

	// 
	// 
}


// 
void APlayableCharacter::HandleCameraCenterCollision(const FTraceHandle & GivenTraceHandle, FTraceDatum & GivenTraceDatum) {

	// 
	SubtitleComponent->HandleVisualSubtitles(GivenTraceHandle, GivenTraceDatum);

	// 
	InteractionComponent->HandleInteractions(GivenTraceHandle, GivenTraceDatum);

	// Continue our asynchronous infinite sweeping into the world in order to test for anything that must respond to our sight
	GetWorld()->AsyncSweepByChannel(CameraCenterTraceType, PlayerCamera->GetComponentLocation(), PlayerCamera->GetComponentLocation() + (PlayerCamera->GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, CameraCenterCollision, FCollisionQueryParams{NAME_None, false, this}, FCollisionResponseParams::DefaultResponseParam, &CameraCenterDelegate);
}
