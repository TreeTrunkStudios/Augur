// TreeTrunkStudios (c) 2023


// 
#include "AbstractExamineMenuWidget.h"
#include <EnhancedInputComponent.h>
#include <Components/Image.h>


// 
void UAbstractExamineMenuWidget::Setup(UWidget * CreatingWidget, UStaticMesh * GivenStaticMesh, const int32 OptionalOffsetY) {

	//
	GetPlayerContext().GetPlayerController()->SetShowMouseCursor(false);

	//
	WidgetThatCreatedThis = CreatingWidget;

	//
	ExamineMeshActor = GetWorld()->SpawnActor<ARenderInventoryItemExamineMesh>(FVector(-100000.0), FRotator::ZeroRotator);

	//
	UMaterialInstanceDynamic * NewDynamicMaterial = UMaterialInstanceDynamic::Create(TargetDynamicMaterialClass.LoadSynchronous(), this);
	NewDynamicMaterial->SetTextureParameterValue(FName(TEXT("RenderTargetTexture")), ExamineMeshActor->Setup(GivenStaticMesh));

	//
	ExamineMeshImage->SetBrushFromMaterial(NewDynamicMaterial);
	ExamineMeshImage->SetRenderTranslation(FVector2D(0.0, OptionalOffsetY));

	//
	UEnhancedInputComponent * CurrentInputComponent = Cast<UEnhancedInputComponent>(GetPlayerContext().GetPawn()->InputComponent.Get());

	//
	CurrentInputComponent->BindAction(RotationInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &UAbstractExamineMenuWidget::HandleRotationInput);
	CurrentInputComponent->BindAction(ZoomInputAction.LoadSynchronous(), ETriggerEvent::Triggered, this, &UAbstractExamineMenuWidget::HandleZoomInput);

	//
	if (IsValid(SelectButton)) {

		//
		SelectButton->OnClicked.AddDynamic(this, &UAbstractExamineMenuWidget::HandleSelectButton);

		//
		SelectButton->Navigation = NewObject<UWidgetNavigation>(SelectButton);

		//
		SelectButton->Navigation->Up.Rule = EUINavigationRule::Stop;
		SelectButton->Navigation->Down.Rule = EUINavigationRule::Stop;
		SelectButton->Navigation->Left.Rule = EUINavigationRule::Stop;
		SelectButton->Navigation->Right.Rule = EUINavigationRule::Stop;
		SelectButton->Navigation->Next.Rule = EUINavigationRule::Custom;
		SelectButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractExamineMenuWidget::HandleSelect);
		SelectButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
		SelectButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractExamineMenuWidget::HandleUnselect);

		//
		SelectButton->BuildNavigation();
	}

	//
	if (IsValid(UnselectButton)) {
		UnselectButton->OnClicked.AddDynamic(this, &UAbstractExamineMenuWidget::HandleUnselectButton);
	}
}


// 
UWidget * UAbstractExamineMenuWidget::GetWidgetToFocus() const {
	return SelectButton;
}


void UAbstractExamineMenuWidget::ForceClose() {

	// Reshow the mouse cursor, as it is now valid again
	GetPlayerContext().GetPlayerController()->SetShowMouseCursor(true);

	// Ensure that our input bindings are erased, as we are no longer important (or existing)
	UEnhancedInputComponent * CurrentInputComponent = Cast<UEnhancedInputComponent>(GetPlayerContext().GetPawn()->InputComponent.Get());
	CurrentInputComponent->ClearBindingsForObject(this);

	// Ensure that the examine mesh actor is completely destroyed since it is no longer needed
	GetWorld()->DestroyActor(ExamineMeshActor);
	
	// Since we are no longer needed, give ourselves up to the garbage collector now
	this->RemoveFromParent();
}


// 
void UAbstractExamineMenuWidget::HandleRotationInput(const FInputActionValue & GivenInputActionValue) {
	const FVector2D RotationInputScaled = GivenInputActionValue.Get<FVector2D>() * CVarInventoryExamineRotationSensitivity.GetValueOnGameThread();
	ExamineMeshActor->AddRotation(FRotator(RotationInputScaled.X, RotationInputScaled.Y, 0.0));
}


// 
void UAbstractExamineMenuWidget::HandleZoomInput(const FInputActionValue & GivenInputActionValue) {
	ExamineMeshActor->AddZoom(GivenInputActionValue.Get<float>() * CVarInventoryExamineZoomSensitivity.GetValueOnGameThread());
}


// 
UWidget * UAbstractExamineMenuWidget::HandleSelect(EUINavigation GivenNavigation) {

	//// TODO: Implement functionality for rotating and discovering new objects off this item's examine
	return this;
}


// 
UWidget * UAbstractExamineMenuWidget::HandleUnselect(EUINavigation GivenNavigation) {

	//
	ForceClose();
	
	// Finally, return a pointer to the widget that created this so that it may have its focus back
	return WidgetThatCreatedThis.Get();
}


// 
void UAbstractExamineMenuWidget::HandleSelectButton() {
	this->HandleSelect(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractExamineMenuWidget::HandleUnselectButton() {
	this->HandleUnselect(EUINavigation::Previous)->SetFocus();
}
