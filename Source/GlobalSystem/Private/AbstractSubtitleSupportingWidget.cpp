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
	ActiveAudioSubtitleObjects.Add(GivenAudioKey, FAudioWidgetData{Text, Duration});
}


// 
void UAbstractSubtitleSupportingWidget::RemoveUniqueAudioSubtitle(const int64 GivenAudioKey) {
	ActiveAudioSubtitleObjects.Remove(GivenAudioKey);
}


// 
void UAbstractSubtitleSupportingWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime) {

	// Create a local text to handle the formatting of multiple subtitles into a single text box
	FText LocalText;

	// Empty all of the assets, but keep the size, as it is dynamically expanding
	LocalAudioToRemove.Empty(LocalAudioToRemove.Num());

	// Loop through all audio subtitle elements, and...
	for (auto & CurrentAudioSubtitle : ActiveAudioSubtitleObjects) {

		// Decrement their duration
		CurrentAudioSubtitle.Value.Duration -= InDeltaTime;

		// If their new duration is at or below zero (0.0f), then...
		if (CurrentAudioSubtitle.Value.Duration <= 0.0f) {

			// Add the current key index to the audio to remove array (cannot remove it right now else it'll mess up the for loop)
			LocalAudioToRemove.Emplace(CurrentAudioSubtitle.Key);
		}

		// If our local text is empty, then we just want to set our text information directly
		else if (LocalText.IsEmptyOrWhitespace()) {
			LocalText = CurrentAudioSubtitle.Value.Text;
		}

		// Else, we want to add the previous and new text together, intersected by a newline symbol
		else {
			LocalText = FText::FormatOrdered(FTextFormat::FromString("{0}\r\n{1}"), CurrentAudioSubtitle.Value.Text, LocalText);
		}
	}

	// Loop through all audio keys that need to be removed due to being empty and remove them
	for (const uint64 & CurrentAudioKeyToRemove : LocalAudioToRemove) {
		ActiveAudioSubtitleObjects.FindAndRemoveChecked(CurrentAudioKeyToRemove);
	}

	// Finally, set the audio subtitle text box's text to the formatted local text
	AudioSubtitleTextBox->SetText(LocalText);

#if SUPPORT_VISUAL_SUBTITLES

	// Create a local text to handle the formatting of multiple subtitles into a single text box
	FText LocalVisualText;

	// Loop through all visual subtitle elements, and...
	for (auto & CurrentVisualSubtitle : ActiveVisualSubtitles) {

		// If our local text is empty, then we just want to set our text information directly
		if (LocalVisualText.IsEmptyOrWhitespace()) {
			LocalVisualText = CurrentVisualSubtitle.Value;
		}

		// Else, we want to add the previous and new text together, intersected by a newline symbol
		else {
			LocalVisualText = FText::FormatOrdered(FTextFormat::FromString("{0}\r\n{1}"), CurrentVisualSubtitle.Value, LocalVisualText);
		}
	}

	// Finally, set the audio subtitle text box's text to the formatted local text
	VisualSubtitleTextBox->SetText(LocalVisualText);
#endif
}


// 
void UAbstractSubtitleSupportingWidget::AddUniqueVisualSubtitle(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio) {
#if SUPPORT_VISUAL_SUBTITLES
	AddUniqueVisualSubtitle_Internal(GivenSubtitleActor, GivenType, Text, OptionalAudio);
#else
	return;
#endif
}


// 
void UAbstractSubtitleSupportingWidget::RemoveUniqueVisualSubtitle(const AActor * GivenSubtitleActor) {
#if SUPPORT_VISUAL_SUBTITLES
	RemoveUniqueVisualSubtitle_Internal(GivenSubtitleActor);
#else
	return;
#endif
}


// 
#if SUPPORT_VISUAL_SUBTITLES


// 
void UAbstractSubtitleSupportingWidget::AddUniqueVisualSubtitle_Internal(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio) {

	// Ensure that we never find a visual subtitle that is empty, as that would be a bug
	check(Text.IsEmptyOrWhitespace() == false);

	// 
	switch (GivenType) {

		// If an internal hint, then attempt to call the given optional audio file now, as internal hints normally have unique audio
		//     attached to them (note: if it is nullptr, then this function does nothing due to internal checks in CreateAudio2D)
		case EVisualSubtitleType::InternalHint:
			UGameplayStatics::SpawnSound2D(GetWorld(), OptionalAudio, CVarVisualSubtitlesAudioVolume.GetValueOnGameThread());
			[[fallthrough]];

		// Regardless, all visual subtitle types' text default to being placed within the same subtitle location concurrently
		case EVisualSubtitleType::Environmental:
		default:
			ActiveVisualSubtitles.Add(GivenSubtitleActor, Text);
			break;
	}
}


// 
void UAbstractSubtitleSupportingWidget::RemoveUniqueVisualSubtitle_Internal(const AActor * GivenSubtitleActor) {
	ActiveVisualSubtitles.FindAndRemoveChecked(GivenSubtitleActor);
}


#endif
