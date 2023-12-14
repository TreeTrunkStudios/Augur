// TreeTrunkStudios (c) 2023


// 
#include "AbstractFloatOptionWidget.h"


//
bool UAbstractFloatOptionWidget::Initialize() {

	//
	const bool ReturnValue = Super::Initialize();

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
