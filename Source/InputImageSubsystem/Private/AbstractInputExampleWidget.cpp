// TreeTrunkStudios (c) 2023


// 
#include "AbstractInputExampleWidget.h"
#include <Blueprint/WidgetTree.h>
#include <Components/Image.h>


// 
bool UAbstractInputExampleWidget::Initialize() {

	// Locally store the return value of the parent class's initialize function
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
	
	// Store a class pointer to the current player's common input subsystem
	UInputImageSubsystem * CurrentInputImageSubsystem = GetOwningLocalPlayer()->GetSubsystem<UInputImageSubsystem>();

	// Ensure that the input image subsystem pointer is always properly collected (valid)
	ensureAlways(IsValid(CurrentInputImageSubsystem));

	// Attach this object to the target delegate system now and store its handle value
	CurrentInputImageSubsystem->OnControllerTypeChanged.AddUObject(this, &UAbstractInputExampleWidget::UpdateWithNewInputDeviceType);

	// Update the input example's icon to the relevant widgets based on the current input device type
	RenderWantedInputs(CurrentInputImageSubsystem);

	// Finally, set the input example's text to the relevant widgets
	InputExampleText->SetText(IsValid(OwningInputAction) ? OwningInputAction->ActionDescription : FText::GetEmpty());

	// Finally, return the parent created value
	return ReturnValue;
}


void UAbstractInputExampleWidget::BeginDestroy() {

	// Call the parent functionality now to actually destroy this object
	Super::BeginDestroy();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
	if (IsValid(GetOwningLocalPlayer()) == false)
		return;

	// Store a class pointer to the current player's common input subsystem
	UInputImageSubsystem * CurrentInputImageSubsystem = GetOwningLocalPlayer()->GetSubsystem<UInputImageSubsystem>();

	// Ensure that the input image subsystem pointer is always properly collected (valid)
	ensureAlways(IsValid(CurrentInputImageSubsystem));

	// Remove this object from the target delegate system now
	CurrentInputImageSubsystem->OnControllerTypeChanged.RemoveAll(this);
}


// 
void UAbstractInputExampleWidget::UpdateWithNewInputDeviceType(UInputImageSubsystem * GivenInputImageSubsystem) const {

	// Ensure that the input image subsystem pointer is always properly collected (valid)
	ensureAlways(IsValid(GivenInputImageSubsystem));

	// Update the input example's icon to the relevant widgets based on the new input device type
	RenderWantedInputs(GivenInputImageSubsystem);
}


//
void UAbstractInputExampleWidget::RenderWantedInputs(const UInputImageSubsystem * CurrentInputImageSubsystem) const {

	//
	const TArray<TObjectPtr<UTexture2D>> AssignedImages = CurrentInputImageSubsystem->GetAllInputImages(OwningInputMapping, OwningInputAction);

	//
	const uint8 MaxShownIconCount = FMath::Min(MaxNumberOfShownIcons, static_cast<uint8>(AssignedImages.Num()));

	//
	ensureAlways(MaxShownIconCount > 0);

	// 
	InputExampleGrid->ClearChildren();
	
	//
	if (MaxShownIconCount == 1) {

		// 
		UImage * NewImage = WidgetTree->ConstructWidget<UImage>();
		NewImage->SetBrushFromTexture(AssignedImages[0]);
		InputExampleGrid->AddChildToUniformGrid(NewImage, 0, 0);
	}

	//
	else if (MaxShownIconCount == 4) {

		// 
		UImage * NewImage = WidgetTree->ConstructWidget<UImage>();
		NewImage->SetBrushFromTexture(AssignedImages[0]);
		InputExampleGrid->AddChildToUniformGrid(NewImage, 0, 1);

		// 
		for (uint8 Index = 1; Index < MaxShownIconCount; ++Index) {
			NewImage = WidgetTree->ConstructWidget<UImage>();
			NewImage->SetBrushFromTexture(AssignedImages[Index]);
			InputExampleGrid->AddChildToUniformGrid(NewImage, 1, Index - 1);
		}
	}

	//
	else {

		// 
		for (uint8 Index = 0; Index < MaxShownIconCount; ++Index) {
			UImage * NewImage = WidgetTree->ConstructWidget<UImage>();
			NewImage->SetBrushFromTexture(AssignedImages[Index]);
			InputExampleGrid->AddChildToUniformGrid(NewImage, 0, Index);
		}
	}
}
