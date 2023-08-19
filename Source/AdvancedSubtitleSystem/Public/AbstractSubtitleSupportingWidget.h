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
UCLASS()
class ADVANCEDSUBTITLESYSTEM_API UAbstractSubtitleSupportingWidget : public UUserWidget {
	GENERATED_BODY()
	

// 
protected:

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock * AudioSubtitleTextBox = nullptr;

	// 
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	class UTextBlock * EnvironmentalSubtitleTextBox = nullptr;


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
	virtual void AddUniqueVisualSubtitle(const AActor * GivenSubtitleActor, const EVisualSubtitleType GivenType, const FText Text, USoundBase * OptionalAudio);

	// 
	UFUNCTION(BlueprintCallable)
	virtual void RemoveUniqueVisualSubtitle(const AActor * GivenSubtitleActor);
};
