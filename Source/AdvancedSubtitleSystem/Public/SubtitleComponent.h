// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "SubtitleInterface.h"
#include "Components/SceneComponent.h"
#include "SubtitleComponent.generated.h"


// 
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FStartAudioSubtitle, const int64, GivenAudioKey, const FText, Text, const float, Duration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopAudioSubtitle, const int64, GivenAudioKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FStartVisualSubtitle, const AActor*, GivenSubtitleActor, const EVisualSubtitleType, GivenType, const FText, Text, USoundBase*, OptionalAudio);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStopVisualSubtitle, const AActor*, GivenSubtitleActor);


// A structure to handle all wanted subtitle data
struct FSubtitleData {

	// Internal struct utilized to make life easier
	struct FInternalSubtitleData {

		// Start time of the current subtitle
		float StartTime = 0.0f;

		// End time of the current subtitle
		float EndTime = 0.0f;

		// The text to be displayed by the current subtitle
		FText Text;
	};

	// The current wave instance's playback time
	float PlaybackTime = 0.0f;

	// The current wave instance's subtitle index (e.g. which element in
	//     the array below are we currently showing the subtitle's for)
	uint32 CurrentSubtitleIndex = (UINT32_MAX - 1);

	// An array of all available subtitles for the current wave instance
	TArray<FInternalSubtitleData> SubtitleInformation;
};


// 
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ADVANCEDSUBTITLESYSTEM_API USubtitleComponent final : public USceneComponent {
	GENERATED_BODY()


// 
public:	

	// Sets default values for this component's properties
	USubtitleComponent();

	// 
	UPROPERTY(BlueprintAssignable)
	FStartAudioSubtitle StartAudioSubtitleDelegate;

	// 
	UPROPERTY(BlueprintAssignable)
	FStopAudioSubtitle StopAudioSubtitleDelegate;

	// 
	UPROPERTY(BlueprintAssignable)
	FStartVisualSubtitle StartVisualSubtitleDelegate;

	// 
	UPROPERTY(BlueprintAssignable)
	FStopVisualSubtitle StopVisualSubtitleDelegate;


// 
protected:

	// Class mapping to ensure that we do not keep adding the exact same audio subtitles over and over again
	TMap<const FWaveInstance*, FSubtitleData> CurrentlyExistingAudioSubtitles;

	// 
	TArray<const FWaveInstance*> NoLongerExistingWaveInstances;


// Only include this functionality iff we wish to have visual subtitles
#if SUPPORT_VISUAL_SUBTITLES
protected:

	// We have no clue what our maximum number of subtitle actors could be, so we need it to be dynamic
	TArray<const AActor*> VisualSubtitleActors;

	// Local trace delegate which is called (on the game thread) whenever the async geometry sweep finishes
	FTraceDelegate VisualSubtitleDelegate;

	// A constant collision shape which is given to the geometry sweep for visual queries
	const FCollisionShape SubtitleCollision = FCollisionShape::MakeSphere(10.0f);

	// Locally store our visual subtitle's trace type as a static const
	static const EAsyncTraceType VisualSubtitleTraceType = VISUAL_SUBTITLES_TRACE_TYPE;

	// The function that gets attached to the trace delegate which contains the actual functionality (not async)
	void HandleVisualSubtitles(const FTraceHandle&, FTraceDatum&);

	// Called when the game starts
	virtual void BeginPlay() override;
#endif


// 
public:	

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;
};
