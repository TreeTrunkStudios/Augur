// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "SubtitleInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "AbstractSubtitleSupportingWidget.generated.h"


// 
struct FAudioWidgetData {

	// 
	const FText Text;

	// 
	float Duration;
};


// 
UCLASS()
class GLOBALSYSTEM_API UAbstractSubtitleSupportingWidget : public UUserWidget {
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
public:

	// 
	virtual void AddUniqueAudioSubtitle(const int64 GivenAudioKey, const FText Text, const float Duration);

	// 
	virtual void RemoveUniqueAudioSubtitle(const int64 GivenAudioKey);

	// 
	void AddUniqueVisualSubtitle(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio);

	// 
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
