// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "SubtitleInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "AbstractSubtitleWidget.generated.h"


// TODO: Implement all console variables to handle changing subtitle information from a settings menu
// 


// 
struct FAudioWidgetData {

	// 
	const FText Text;

	// 
	float Duration;
};


// 
UCLASS(Abstract)
class SUBTITLESYSTEM_API UAbstractSubtitleWidget : public UUserWidget {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock * AudioSubtitleTextBox = nullptr;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock * VisualSubtitleTextBox = nullptr;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UBorder * AudioSubtitleBackground = nullptr;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UBorder * VisualSubtitleBackground = nullptr;


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	virtual void AddUniqueAudioSubtitle(const int64 GivenAudioKey, const FText Text, const float Duration);

	// 
	UFUNCTION(BlueprintCallable)
	virtual void RemoveUniqueAudioSubtitle(const int64 GivenAudioKey);

	// 
	UFUNCTION(BlueprintCallable)
	void AddUniqueVisualSubtitle(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio);

	// 
	UFUNCTION(BlueprintCallable)
	void RemoveUniqueVisualSubtitle(const AActor * GivenSubtitleActor);

// 
protected:

	// 
	TMap<const int64, FAudioWidgetData> ActiveAudioSubtitleObjects;

	//
	TArray<int64> LocalAudioToRemove;

	// 
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;


// 
#if SUPPORT_VISUAL_SUBTITLES

// 
protected:

	// 
	TMap<const AActor*, FText> ActiveVisualSubtitles;


// 
protected:

	// 
	FORCEINLINE virtual void AddUniqueVisualSubtitle_Internal(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio);

	// 
	FORCEINLINE virtual void RemoveUniqueVisualSubtitle_Internal(const AActor * GivenSubtitleActor);
#endif
};
