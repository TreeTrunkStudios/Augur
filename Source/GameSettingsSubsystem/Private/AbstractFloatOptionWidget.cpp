// TreeTrunkStudios (c) 2023


// 
#include "AbstractFloatOptionWidget.h"


//
bool UAbstractFloatOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

	// Editor Only - Ensures that the item does not crash if the owning local player is invalid
#if WITH_EDITOR
	if (IsValid(GetOwningLocalPlayer()) == false)
		return ReturnValue;
#endif

	//
	SpinBox->SetValue(SettingsOptionConsoleVariable->GetFloat());
	HandleStateUpdate(SpinBox->GetValue());
	
	//
	SpinBox->OnValueChanged.AddDynamic(this, &UAbstractFloatOptionWidget::HandleStateUpdate);
	SpinBox->OnValueCommitted.AddDynamic(this, &UAbstractFloatOptionWidget::HandleStateChange);

	// 
	return ReturnValue;
}


// 
void UAbstractFloatOptionWidget::HandleStateUpdate(float NewState) {
	const float LocalMinValue = FMath::Min(SpinBox->GetMinSliderValue(), SpinBox->GetMinValue());
	const float LocalMaxValue = FMath::Max(SpinBox->GetMaxSliderValue(), SpinBox->GetMaxValue());
	ProgressBar->SetPercent(FMath::GetMappedRangeValueClamped(FVector2D{LocalMinValue, LocalMaxValue}, FVector2D{0, 1}, NewState));
}


// 
void UAbstractFloatOptionWidget::HandleStateChange(float NewState, ETextCommit::Type CommitType) {
	SettingsOptionConsoleVariable->SetWithCurrentPriority(NewState);
}


// 
void UAbstractFloatOptionWidget::UpdateToTargetLevel(const ESettingsLevel & GivenSettingsLevel) {
	SpinBox->SetValue(SettingsLevelValues[static_cast<uint8>(GivenSettingsLevel)]);
}
