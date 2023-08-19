// TreeTrunkStudios (c) 2023


// 
#pragma once


//
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SubtitleInterface.generated.h"


// A bunch of code relies on a pointer size equating to int64, due to UE5 blueprints (stupidly) only understanding fixed data types
static_assert(sizeof(uintptr_t) == sizeof(int64));


// 
#ifndef SUPPORT_VISUAL_SUBTITLES
#define SUPPORT_VISUAL_SUBTITLES true
#endif


// 
#ifndef VISUAL_SUBTITLES_TRACE_TYPE
#define VISUAL_SUBTITLES_TRACE_TYPE EAsyncTraceType::Single
#endif


// 
#ifndef VISUAL_SUBTITLE_STATIC_INDEX
#define VISUAL_SUBTITLE_STATIC_INDEX 4
#endif


// Only include these static console variables if we want visual subtitles
#if SUPPORT_VISUAL_SUBTITLES

// Console variable for storing the global controller state (none, left stick moves, or right stick moves)
static TAutoConsoleVariable<float> CVarVisualSubtitlesAudioVolume(
	TEXT("subtitles.visual.AudioVolume"),
	1.0f,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe);
#endif


// 
UENUM()
enum class EVisualSubtitleType : uint8 {
	Environmental,
	InternalHint,
	Num UMETA(Hidden)
};


// 
UENUM()
enum class ESubtitleVisualType : uint8 {
	ToggleVisibility,
	FadeColorAlpha,
	Typewriter,
	Num UMETA(Hidden)
};


// This class does not need to be modified
UINTERFACE(MinimalAPI)
class USubtitleInterface : public UInterface { GENERATED_BODY() };


// 
class ADVANCEDSUBTITLESYSTEM_API ISubtitleInterface {
	GENERATED_BODY()


// 
public:

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const FText GetText() const;
	// virtual const FText GetText_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	const EVisualSubtitleType GetType() const;
	// virtual const EVisualSubtitleType GetType_Implementation() const override;

	// 
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	USoundBase * GetOptionalAudio() const;
	// virtual USoundBase * GetOptionalAudio_Implementation() const override;
};
