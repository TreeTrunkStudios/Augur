// TreeTrunkStudios (c) 2023


// 
#include "SubtitleComponent.h"
#include "AudioDeviceManager.h"
#include "AudioDevice.h"


// Sets default values for this component's properties
USubtitleComponent::USubtitleComponent() {

	// Set this component to be initialized when the game starts, and to be ticked every frame, but on a separate thread
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bRunOnAnyThread = false; /*Sadly, the tick function must run on the game thread due to audio manager dumminess*/
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bTickEvenWhenPaused = false;
}


// Called every frame due to needing access to a wave instance as soon as it is spawned/ticked
// NOTE: Must be called on the game thread, as, for some reason, audio information is only collectable from the audio thread
//     (which we do not have access to) and the game thread, which is super dumb and incredibly pointlessly limiting, but whatevz
void USubtitleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {

	// Tick the parent function of the current component
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//// TODO: Implement distance checking for subtitles to ensure that sounds the player cannot hear are not subtitled (might not be necessary)
	// Locally store the global audio device manager singleton (static, as it only needs to happen once, since it's a singleton)
	static FAudioDeviceManager * LocalAudioDeviceManager = FAudioDeviceManager::Get();

	// Locally store a pointer to the currently active audio device
	FAudioDevice * CurrentAudioDevice = LocalAudioDeviceManager->GetActiveAudioDevice().GetAudioDevice();

	// Locally store an array of the currently active wave instance pointers
	CurrentlyExistingAudioSubtitles.GenerateKeyArray(NoLongerExistingWaveInstances);

	// Loop through all currently active wave instances, and...
	for (const FWaveInstance * CurrentWave : CurrentAudioDevice->GetActiveWaveInstances()) {
		
		// If our current sound wave has no subtitles, then don't even bother with the future calculations, just skip it
		if (CurrentWave->WaveData->Subtitles.Num() <= 0) continue;

		// Remove the current wave from our no longer existing wave instances, if it exists, as it is still in use
		NoLongerExistingWaveInstances.RemoveSingleSwap(CurrentWave, false);

		// Locally collect or add an FSubtitleData to the component's TMap and locally store the reference to it
		FSubtitleData & LocalSubtitleData = CurrentlyExistingAudioSubtitles.FindOrAdd(CurrentWave);

		//// INITIALIZE NEW SUBTITLE SUPPORTING ASSETS NOW
		// If the current subtitle index equals UINT32_MAX, then this is a new subtitle (Add) and needs to be initialized
		if (LocalSubtitleData.CurrentSubtitleIndex == (UINT32_MAX - 1)) {

			// Increment our current subtitle index to remove it from the "new subtitle" state and into the "waiting for
			//     first subtitle to start" state (necessary for future looping wave behaviour, apologies)
			++LocalSubtitleData.CurrentSubtitleIndex;

			// Initialize the size of our subtitle information array to equal the size of the wave data's subtitles array
			LocalSubtitleData.SubtitleInformation.Empty(CurrentWave->WaveData->Subtitles.Num());

			// Loop through all of this current wave instance's subtitles, to calculate what gets shown and when
			const TArray<FSubtitleCue> & LocalSubtitleArray = CurrentWave->WaveData->Subtitles;
			for (uint32 CurrentIndex = 0; CurrentIndex < static_cast<uint32>(LocalSubtitleArray.Num()); ++CurrentIndex) {

				// Locally store the subtitle array's current index subtitle cue
				const FSubtitleCue & CurrentSubtitleCue = LocalSubtitleArray[CurrentIndex];

				// Locally store the next index
				const uint32 NextIndex = (CurrentIndex + 1);

				// Locally store the current subtitle's ending time by taking the next subtitle's starting time (or,
				//     if there is no next subtitle, then taking the entire wave's duration) and using it as our current
				//     subtitle's ending time, as we do not want them to overlap, nor can we add a simple solution that
				//     removes this problem since we're using UE5's extremely dumb and limited built-in solution since it
				//     already exists (sorry!)
				const float CurrentSubtitleEndingTime = ((LocalSubtitleArray.Num() == NextIndex) ?
					CurrentWave->WaveData->Duration : LocalSubtitleArray[NextIndex].Time);

				// Ensure that there are never two or more overlapping subtitles, as, if there are, they should be combined into one
				check((CurrentSubtitleEndingTime - CurrentSubtitleCue.Time) > 0.0f);

				// Add this new subtitle information onto the end of the array, giving it the starting time, ending time, and text
				LocalSubtitleData.SubtitleInformation.Emplace(FSubtitleData::FInternalSubtitleData{CurrentSubtitleCue.Time, CurrentSubtitleEndingTime, CurrentSubtitleCue.Text});
			}
		}

		//// UPDATE PRE-EXISTING SUBTITLE SUPPORTING ASSETS NOW
		// Else, this is a subtitle that currently exists (Find) and needs to be updated accordingly
		else {

			// Update our current wave instance's playback time to itself plus delta time
			LocalSubtitleData.PlaybackTime += DeltaTime;
		}

		// Locally store the index of the next subtitle element in the array
		const uint32 NextIndex = (LocalSubtitleData.CurrentSubtitleIndex + 1);

		//// CHECK IF THE NEXT SUBTITLE FOR THE CURRENT SUBTITLE SUPPORTING ASSET SHOULD START NOW AND, IF SO, START IT
		// If our next index is valid, then we have a potential next subtitle that we need to check for
		if (LocalSubtitleData.SubtitleInformation.IsValidIndex(NextIndex)) {

			// Locally store our next subtitle data for simplistic access
			const FSubtitleData::FInternalSubtitleData & LocalSubtitle = LocalSubtitleData.SubtitleInformation[NextIndex];

			// If we should start the next subtitle, then...
			if (LocalSubtitleData.PlaybackTime >= LocalSubtitle.StartTime) {

				// Increment our current subtitle index, as it has changed
				++LocalSubtitleData.CurrentSubtitleIndex;

				// Start our next audio subtitle now for this wave instance (have to subtract our current playback time
				//     from the end time to get the full wanted duration, using playback time instead of start time to
				//     account for framerate misses, as framerates are never perfect and floats have rounding point errors)
				StartAudioSubtitleDelegate.Broadcast(reinterpret_cast<int64>(CurrentWave), LocalSubtitle.Text, LocalSubtitle.EndTime - LocalSubtitleData.PlaybackTime);
			}
		}

		//// CHECK IF A LOOPING AUDIO ASSET AT THE END OF ITS DURATION AND, IF SO, THEN LOOP ITS INFORMATION
		// Else, then reset our current subtitle index to be the max, as to handle looping wave instances at no cost to non-looping ones
		else if (CurrentWave->WaveData->bLooping && LocalSubtitleData.PlaybackTime >= CurrentWave->WaveData->Duration) {

			// Modulo our playback time by the current wave's duration to loop back to the start of the audio when necessary
			LocalSubtitleData.PlaybackTime = fmodf(LocalSubtitleData.PlaybackTime, CurrentWave->WaveData->Duration);

			// Reset our current subtitle index to UINT32_MAX to act like this is a new post-initialized wave instance
			LocalSubtitleData.CurrentSubtitleIndex = UINT32_MAX;
		}
	}

	// Loop through all of the wave instances that no longer are active, and...
	for (const FWaveInstance * CurrentWaveToBeRemoved : NoLongerExistingWaveInstances) {

		// Stop the current audio subtitle (optional function only really used if the player leaves the audio's influence ring)
		StopAudioSubtitleDelegate.Broadcast(reinterpret_cast<int64>(CurrentWaveToBeRemoved));
		
		// Remove the wave instance from the map entirely, as it no longer should exist
		CurrentlyExistingAudioSubtitles.Remove(CurrentWaveToBeRemoved);
	}
}


// Only include this functionality iff we wish to have visual subtitles
#if SUPPORT_VISUAL_SUBTITLES

// Called when the game starts
void USubtitleComponent::BeginPlay() {

	// 
	Super::BeginPlay();

	//// 
	//
	VisualSubtitleActors.Empty(VISUAL_SUBTITLE_STATIC_INDEX);

	// 
	VisualSubtitleDelegate = FTraceDelegate::CreateUObject(this, &USubtitleComponent::HandleVisualSubtitles);

	// Start our asynchronous infinite sweeping into the world in order to test for any visual subtitles
	GetWorld()->AsyncSweepByChannel(VisualSubtitleTraceType, GetComponentLocation(), GetComponentLocation() + (GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, SubtitleCollision, FCollisionQueryParams{NAME_None, false, GetOwner()}, FCollisionResponseParams::DefaultResponseParam, &VisualSubtitleDelegate);
}


// 
// NOTE: Even though this function is called from an async geometry sweep (shape trace), it is called on the game thread
void USubtitleComponent::HandleVisualSubtitles(const FTraceHandle & GivenTraceHandle, FTraceDatum & GivenTraceDatum) {

// If our target size is less than or equal to 32, then keep the bitmask at a 32 bit size to better performance, else make it 64 bits
#if (VISUAL_SUBTITLE_STATIC_INDEX <= 32)
	using BitMaskSize = uint32;
#else
	using BitMaskSize = uint64;
#endif

	// Create a local bitmask to handle accounting for still existing subtitle actors
	BitMaskSize ReoccurringActorBitmask = 0;

	// For each collision that occurred, loop through them in order of hit
	for (const FHitResult & CurrentCollision : GivenTraceDatum.OutHits) {

		// Locally store a const pointer to the current collision's actor rather than calling the expensive GetActor function multiple times
		const AActor * LocalActor = CurrentCollision.GetActor();

		// Locally store a pointer to the actor as the wanted subtitle interface, to see if it supports it on the C++ side of things
		//const ISubtitleInterface * CollidedVisualSubtitle = Cast<ISubtitleInterface>(CurrentCollision.GetActor());

		// If the current collision actor does not support the subtitle interfaces, then skip it, as it is useless
		if (LocalActor->Implements<USubtitleInterface>() == false) continue;

		// Locally store the index of our current actor within the subtitles array, if any
		int32 LocalSubtitleIndex = VisualSubtitleActors.Find(LocalActor);

		// If the hit actor has not already started their subtitles, then...
		if (LocalSubtitleIndex < 0) {

			// Add our new actor to the end of our current visual subtitle actors array and update our local subtitle index to be the new index
			LocalSubtitleIndex = VisualSubtitleActors.Emplace(LocalActor);

			// Start our new visual subtitles now, giving our owning widget the type and text of the subtitles
			StartVisualSubtitleDelegate.Broadcast(LocalActor, ISubtitleInterface::Execute_GetType(LocalActor), ISubtitleInterface::Execute_GetText(LocalActor), ISubtitleInterface::Execute_GetOptionalAudio(LocalActor));
		}

		// Add our current actor's array index into the reoccurring actor bitmask, as we do not wish to remove it as of now
		ReoccurringActorBitmask |= (static_cast<BitMaskSize>(1) << LocalSubtitleIndex);
	}

	// Loop through all currently existing subtitle actor's, and...
	for (int32 CurrentIndex = (VisualSubtitleActors.Num() - 1); CurrentIndex > 0; --CurrentIndex) {

		// If our current actor's index is zero, then it did not reoccur and needs to be removed now
		if ((ReoccurringActorBitmask & (static_cast<BitMaskSize>(1) << CurrentIndex)) == 0) {

			// Stop this subtitle from further rendering
			StopVisualSubtitleDelegate.Broadcast(VisualSubtitleActors[CurrentIndex]);

			// Remove this subtitle from the current array of subtitles
			VisualSubtitleActors.RemoveAt(CurrentIndex, 1, false);
		}
	}

	// Finally, start another async trace to do it all over again
	GetWorld()->AsyncSweepByChannel(VisualSubtitleTraceType, GetComponentLocation(), GetComponentLocation() + (GetComponentQuat().GetForwardVector() * 1000.0), FQuat::Identity, ECollisionChannel::ECC_Visibility, SubtitleCollision, FCollisionQueryParams{NAME_None, false, GetOwner()}, FCollisionResponseParams::DefaultResponseParam, &VisualSubtitleDelegate);
}
#endif
