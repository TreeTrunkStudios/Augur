// TreeTrunkStudios (c) 2023


// 
#include "AbstractSubtitleSupportingWidget.h"
#include "Kismet/GameplayStatics.h"


// 
// 


// 
void UAbstractSubtitleSupportingWidget::AddUniqueAudioSubtitle(const int64 GivenAudioKey, const FText Text, const float Duration) {

	// Ensure that we never draw an audio subtitle that is empty (utilized to help allow audio files to have "subtitle dead air")
	if (Text.IsEmpty()) return;

	// 
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, Duration, FColor::Blue, Text.ToString());
}


// 
void UAbstractSubtitleSupportingWidget::RemoveUniqueAudioSubtitle(const int64 GivenAudioKey) {
}


// 
void UAbstractSubtitleSupportingWidget::AddUniqueVisualSubtitle(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio) {

	// Ensure that we never find a visual subtitle that is empty, as that would be a bug
	check(Text.IsEmptyOrWhitespace() == false);

	// Attempt to call the given optional audio file now (note: if it is nullptr, then this function does nothing due to internal checks in CreateAudio2D)
	UGameplayStatics::SpawnSound2D(GetWorld(), OptionalAudio, CVarVisualSubtitlesAudioVolume.GetValueOnGameThread());

	// 
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Green, FString::FromInt((uint64)GivenSubtitleActor) + FString(": ") + FString::FromInt((uint32)GivenType) + FString(" - ") + Text.ToString());
}


// 
void UAbstractSubtitleSupportingWidget::RemoveUniqueVisualSubtitle(const AActor * GivenSubtitleActor) {

	// 
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::FromInt((uint64)GivenSubtitleActor));
}
