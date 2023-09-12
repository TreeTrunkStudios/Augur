// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#include "SaveGameSubsystem.h"
#include "CharacterData.h"
#include "LevelData.h"
#include "StoryData.h"
#include "CharacterDataBlueprints.h"
#include "LevelDataBlueprints.h"
#include "StoryDataBlueprints.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelActorContainer.h"
#include "Compression/OodleDataCompression.h"
#include "Compression/OodleDataCompressionUtil.h"
#include "Engine/LevelBounds.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerController.h"
#include "Serialization/BufferArchive.h"
#include "Serialization/MemoryReader.h"


// DEBUG ONLY
uint8 SubSystemIndex = 0;



////


// 
bool IsCharacterData(const AActor * CurrentActor) {
	return (IsValid(CurrentActor) && (CurrentActor->Implements<UCharacterData>() || CurrentActor->Implements<UCharacterDataBlueprints>()));
}


// 
bool IsLevelData(const AActor * CurrentActor) {
	return (IsValid(CurrentActor) && (CurrentActor->Implements<ULevelData>() || CurrentActor->Implements<ULevelDataBlueprints>()));
}


// 
bool IsStoryData(const AActor * CurrentActor) {
	return (IsValid(CurrentActor) && (CurrentActor->Implements<UStoryData>() || CurrentActor->Implements<UStoryDataBlueprints>()));
}


// 
void CalculateActorBoxHelperFunction(const AActor * CurrentActor, FBox & CurrentLevelBounds) {

	// First ensure that the current actor is both valid and level bounds relevant, then...
	if (IsValid(CurrentActor) && CurrentActor->IsLevelBoundsRelevant()) {

		// Summate the current actor's component's bounding boxes with the current level's bounds, if they exist and are valid
		FBox ActorBox = CurrentActor->GetComponentsBoundingBox(true);
		if (ActorBox.IsValid) CurrentLevelBounds += ActorBox;
	}
}


// 
UClass * GetClassFromString(const FString & ClassName) {

	// If the given string is empty or is None, then simply return nullptr
	if (ClassName.IsEmpty() || ClassName == TEXT("None")) return nullptr;

	// If the given class name is not a short package name, then simply find the object, else, we need to find the first object
	UClass * LocalClass = ((!FPackageName::IsShortPackageName(ClassName)) ?
		FindObject<UClass>(nullptr, *ClassName) :
		FindFirstObject<UClass>(*ClassName, EFindFirstObjectOptions::None, ELogVerbosity::Warning, TEXT("SaveGameSubsystem::GetClassFromString")));

	// If the LocalClass is still not valid, then load the class directly
	if (!LocalClass) LocalClass = LoadObject<UClass>(nullptr, *ClassName);

	// Ensure that the given string always produces a valid class pointer
	ensureAlways(LocalClass);

	// Finally, return the now valid and found class pointer
	return LocalClass;
}


// 
void SaveActorPerLevelHelperFunction(AActor * CurrentActor, TArray<uint8> & CurrentActorData) {

	// 
	if (!IsValid(CurrentActor)) return;

	// 
	bool DoesImplementLevelDataBlueprints = CurrentActor->Implements<ULevelDataBlueprints>();

	// Check on whether the actor implements the target interfaces at all
	if (CurrentActor->Implements<ULevelData>() || DoesImplementLevelDataBlueprints) {

		// If it does, then create a local variable to handle all of the saving data bytes
		TMemoryWriter<32> LocalLevelArchive{CurrentActorData, false, false, FName(TEXT("LevelArchive"))};

		// Ensure that, prior to any saving, we call the callback function to ensure that all data is handled properly
		if (DoesImplementLevelDataBlueprints) {
			ILevelDataBlueprints::Execute_PreLevelSaveCallback(CurrentActor);
		}

		// Now, check whether it implements it within C++
		if (ILevelData * SavableLevelActor = Cast<ILevelData>(CurrentActor)) {

			// DEBUG ONLY
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("\t") + CurrentActor->GetName());

			// If so, then save the actor's data now through the C++ only TransferLevelData function
			SavableLevelActor->TransferLevelData(LocalLevelArchive);
		}

		// 
		if (DoesImplementLevelDataBlueprints) {

			// Finally, check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
			const TSet<FName> CurrentSavablePropertyNames = ILevelDataBlueprints::Execute_GetLevelSavePropertyNames(CurrentActor);
			if (CurrentSavablePropertyNames.Num() > 0) {

				// If so, then we need to loop through all properties within the current actor, and...
				for (FProperty * Property = CurrentActor->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

					// If the current property has the same name as a savable one, then...
					if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

						// Serialize the current property now
						LocalLevelArchive.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentActor), Property->ElementSize);

						// DEBUG ONLY
						//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, Property->GetName() + FString(TEXT(" - ")) + FString::FromInt(Property->ElementSize));
					}
				}
			}
		}

		// 
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, CurrentActor->GetName() + FString(TEXT(" - ")) + FString::FromInt(LocalLevelArchive.TotalSize()));
	}
}


// 
void LoadLevelPerActorHelperFunction(AActor * CurrentActor, TArray<uint8> & CurrentActorData) {

	// 
	if (!IsValid(CurrentActor)) return;

	// 
	bool DoesImplementLevelDataBlueprints = CurrentActor->Implements<ULevelDataBlueprints>();

	// Check on whether the actor implements the target interfaces at all
	if (CurrentActor->Implements<ULevelData>() || DoesImplementLevelDataBlueprints) {

		// If it does, then create a local variable to handle all of the saving data bytes
		FMemoryReader LocalLevelArchive{CurrentActorData, true};

		// If there is no data (e.g. this actor has already been loaded previously), then we should just return
		if (LocalLevelArchive.TotalSize() <= 0) {
			return;
		}

		// Now, check whether it implements it within C++
		if (ILevelData * SavableLevelActor = Cast<ILevelData>(CurrentActor)) {

			// DEBUG ONLY
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString("\t") + CurrentActor->GetName());

			// If so, then save the actor's data now through the C++ only TransferLevelData function
			SavableLevelActor->TransferLevelData(LocalLevelArchive);
		}

		// 
		if (DoesImplementLevelDataBlueprints) {

			// Finally, check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
			const TSet<FName> CurrentSavablePropertyNames = ILevelDataBlueprints::Execute_GetLevelSavePropertyNames(CurrentActor);
			if (CurrentSavablePropertyNames.Num() > 0) {

				// If so, then we need to loop through all properties within the current actor, and...
				for (FProperty * Property = CurrentActor->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

					// If the current property has the same name as a savable one, then...
					if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

						// Serialize the current property now
						LocalLevelArchive.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentActor), Property->ElementSize);

						// DEBUG ONLY
						//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, Property->GetName() + FString(TEXT(" - ")) + FString::FromInt(Property->ElementSize));
					}
				}
			}

			// Ensure that, after all loading is done, we call the callback function to ensure that all data is handled properly
			ILevelDataBlueprints::Execute_PostLevelLoadCallback(CurrentActor);
		}

		// 
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Orange, CurrentActor->GetName() + FString(TEXT(" - ")) + FString::FromInt(LocalLevelArchive.TotalSize()));
	}
}


//// 


// 
void USaveGameSubsystem::Initialize(FSubsystemCollectionBase & Collection) {
	Super::Initialize(Collection);
	++SubSystemIndex;

	// DEBUG ONLY - Print out the total number of bytes that require saving
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("USaveGameSubsystem.Initialize: ")) + FString::FromInt((int32)SubSystemIndex));
	}
}


// 
void USaveGameSubsystem::Deinitialize() {
	Super::Deinitialize();

	// DEBUG ONLY - Print out the total number of bytes that require saving
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("USaveGameSubsystem.Deinitialize: ")) + FString::FromInt((int32)SubSystemIndex));
	}
}



////


// 
void USaveGameSubsystem::SetCurrentGameID(const FString & NewGameID) {
	CurrentSaveGameID = NewGameID;
}


// 
void USaveGameSubsystem::SetLevelSavingTypes(const LevelSavingTypesEnum NewLevelSavingTypes) {
	LevelSavingTypes = NewLevelSavingTypes;
}


// 
void USaveGameSubsystem::SetCharacterDataCompression(const CompressionOptionsEnum NewCharacterDataCompression) {
	CharacterDataCompression = NewCharacterDataCompression;
}


// 
void USaveGameSubsystem::SetLevelDataCompression(const CompressionOptionsEnum NewLevelDataCompression) {
	LevelDataCompression = NewLevelDataCompression;
}


// 
void USaveGameSubsystem::SetStoryDataCompression(const CompressionOptionsEnum NewStoryDataCompression) {
	StoryDataCompression = NewStoryDataCompression;
}


// 
void USaveGameSubsystem::SetStoredDataCompression(const CompressionOptionsEnum NewStoredDataCompression) {
	StoredDataCompression = NewStoredDataCompression;
}


////


// 
void USaveGameSubsystem::StoreCharacters() {

	// Create a global counter in order to keep track of what character's get saved
	uint8 LocalCounter = 0;

	// Loop through all currently existing player controllers
	for (FConstPlayerControllerIterator CurrentPlayerController = GetWorld()->GetPlayerControllerIterator(); CurrentPlayerController; ++CurrentPlayerController) {

		// Locally store the current player controller's controlled pawn
		APawn * CurrentPawn = CurrentPlayerController->Get()->GetPawn();

		// Check if this actor implements the blueprint side of character saving
		bool DoesImplementCharacterDataBlueprints = CurrentPawn->Implements<UCharacterDataBlueprints>();

		// Only proceed with the following functionality IFF the current character is savable
		if (CurrentPawn->Implements<UCharacterData>() || DoesImplementCharacterDataBlueprints) {

			// If this character has already been saved, then reset the data array to be zero (but keep the memory allocated)
			if (StoredSavedCharacterData.Contains(LocalCounter)) StoredSavedCharacterData.FindChecked(LocalCounter).Reset();

			// Create a local array to store all serialized character data
			TArray<uint8> UncompressedData;

			// Locally create the memory writer, as there is currently a memory issue with utilizing it as a global due to the crappy usage of offsets
			TMemoryWriter<32> CharacterDataWriter{UncompressedData, false, false, FName(TEXT("CharacterDataArchive"))};

			// Ensure that, prior to any saving, we call the callback function to ensure that all data is handled properly if supported
			if (DoesImplementCharacterDataBlueprints) {
				ICharacterDataBlueprints::Execute_PreCharacterSaveCallback(CurrentPawn);
			}

			// If the current player controller supports the ICharacterData interface, then save their data
			if (ICharacterData * SavableCharacter = Cast<ICharacterData>(CurrentPawn)) {
				SavableCharacter->TransferCharacterData(CharacterDataWriter);
			}

			// Lastly, if we are dealing with the blueprints side of character saving, then...
			if (DoesImplementCharacterDataBlueprints) {

				// Check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
				const TSet<FName> CurrentSavablePropertyNames = ICharacterDataBlueprints::Execute_GetCharacterSavePropertyNames(CurrentPawn);
				if (CurrentSavablePropertyNames.Num() > 0) {

					// If so, then we need to loop through all properties within the current actor, and...
					for (FProperty * Property = CurrentPawn->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

						// Ensure that the current property is valid and, if it is not, then skip it and continue
						if (!Property) continue;

						// If the current property has the same name as a savable one, then...
						if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

							// Serialize the current property now
							CharacterDataWriter.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentPawn), Property->ElementSize);
						}
					}
				}
			}

			// If the store system requires that data is compressed prior to being saved, then compress it now
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, UncompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Globally store the important compressed character data now
				StoredSavedCharacterData.Add(LocalCounter, CompressionBuffer);
			}

			// Else, globally store the uncompressed character data now
			else {
				StoredSavedCharacterData.Add(LocalCounter, UncompressedData);
			}

			// Ensure that we are not utilizing extra pointless memory
			StoredSavedCharacterData.FindChecked(LocalCounter).Shrink();

			// Finally, increment the counter (only incrementing the counter for each character that actually supports saving, that way we completely ignore all unsavable characters)
			++LocalCounter;
		}
	}
}


// 
void USaveGameSubsystem::StoreCharacter(const uint8 & CharacterIndex) {

	// Create a global counter in order to keep track of what character's get saved
	uint8 LocalCounter = 0;

	// Loop through all currently existing player controllers
	for (FConstPlayerControllerIterator CurrentPlayerController = GetWorld()->GetPlayerControllerIterator(); CurrentPlayerController; ++CurrentPlayerController) {

		// Locally store the current player controller's controlled pawn
		APawn * CurrentPawn = CurrentPlayerController->Get()->GetPawn();

		// Check if this actor implements the blueprint side of character saving
		bool DoesImplementCharacterDataBlueprints = CurrentPawn->Implements<UCharacterDataBlueprints>();

		// Only proceed with the following functionality IFF the current character is savable
		if (CurrentPawn->Implements<UCharacterData>() || DoesImplementCharacterDataBlueprints) {

			// If this is not the currently wanted character index, then skip and keep trying
			if (LocalCounter != CharacterIndex) {
				++LocalCounter;
				continue;
			}

			// If this character has already been saved, then reset the data array to be zero (but keep the memory allocated)
			if (StoredSavedCharacterData.Contains(LocalCounter)) StoredSavedCharacterData.FindChecked(LocalCounter).Reset();

			// Create a local array to store all serialized character data
			TArray<uint8> UncompressedData;

			// Locally create the memory writer, as there is currently a memory issue with utilizing it as a global due to the crappy usage of offsets
			TMemoryWriter<32> CharacterDataWriter{UncompressedData, false, false, FName(TEXT("CharacterDataArchive"))};

			// Ensure that, prior to any saving, we call the callback function to ensure that all data is handled properly if supported
			if (DoesImplementCharacterDataBlueprints) {
				ICharacterDataBlueprints::Execute_PreCharacterSaveCallback(CurrentPawn);
			}

			// If the current player controller supports the ICharacterData interface, then save their data
			if (ICharacterData * SavableCharacter = Cast<ICharacterData>(CurrentPawn)) {
				SavableCharacter->TransferCharacterData(CharacterDataWriter);
			}

			// Lastly, if we are dealing with the blueprints side of character saving, then...
			if (DoesImplementCharacterDataBlueprints) {

				// Check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
				const TSet<FName> CurrentSavablePropertyNames = ICharacterDataBlueprints::Execute_GetCharacterSavePropertyNames(CurrentPawn);
				if (CurrentSavablePropertyNames.Num() > 0) {

					// If so, then we need to loop through all properties within the current actor, and...
					for (FProperty * Property = CurrentPawn->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

						// Ensure that the current property is valid and, if it is not, then skip it and continue
						if (!Property) continue;

						// If the current property has the same name as a savable one, then...
						if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

							// Serialize the current property now
							CharacterDataWriter.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentPawn), Property->ElementSize);
						}
					}
				}
			}

			// If the store system requires that data is compressed prior to being saved, then compress it now
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, UncompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Globally store the important compressed character data now
				StoredSavedCharacterData.Add(LocalCounter, CompressionBuffer);
			}

			// Else, globally store the uncompressed character data now
			else {
				StoredSavedCharacterData.Add(LocalCounter, UncompressedData);
			}

			// Ensure that we are not utilizing extra pointless memory
			StoredSavedCharacterData.FindChecked(LocalCounter).Shrink();

			// Finally, return, as this is the wanted index and it, and only it, has now been saved
			return;
		}
	}

	// DEBUG ONLY
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: Unable to find a storable character at index: ")) + FString::FromInt(CharacterIndex));
}


// 
void USaveGameSubsystem::SaveCharacters() {

	// Start by collecting and storing all current character data
	StoreCharacters();

	// Loop through all stored character information, and...
	for (auto & CurrentCharacterData : StoredSavedCharacterData) {

		// If the stored character compression is different than the character data compression, then...
		if (CharacterDataCompression != StoredDataCompression) {

			// Create a local byte buffer array to store the uncompressed data
			TArray<uint8> LocalUnCompressedData;

			// DEBUG ONLY - Print all compression related information now
			if (GEngine) {

				// Create two local integers
				int32 Compressed, Decompressed;

				// Collect the compressed array's size information, ensuring that it is always successful
				ensureAlways(FOodleCompressedArray::PeekSizes(CurrentCharacterData.Value, Compressed, Decompressed) != 0);

				// Print out all compression related information now, informing the user of the current map's name along with it
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
			}

			// Uncompress the currently compressed stored data and store it within the above byte array
			FOodleCompressedArray::DecompressToTArray(LocalUnCompressedData, CurrentCharacterData.Value);

			// Next, if the save system requires that character data is compressed prior to being saved, then compress it now
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(CharacterDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LocalUnCompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Finally, write the re-compressed data directly to file
				ensureAlways(FFileHelper::SaveArrayToFile(CompressionBuffer, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}

			// Else, just write the decompressed data directly to file
			else {
				ensureAlways(FFileHelper::SaveArrayToFile(LocalUnCompressedData, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}
		}

		// Else, just write the compressed data directly to file
		else {
			ensureAlways(FFileHelper::SaveArrayToFile(CurrentCharacterData.Value, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CurrentCharacterData.Key) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
		}
	}

	// Lastly, completely empty the memory buffer in order to give the user back their memory, as we no longer need to locally store it
	//StoredSavedCharacterData.Empty();
}


// 
void USaveGameSubsystem::SaveCharacter(const uint8 & CharacterIndex) {

	// 
	StoreCharacter(CharacterIndex);

	// 
	if (StoredSavedCharacterData.Contains(CharacterIndex)) {

		// 
		TArray<uint8> & CurrentCharacterData = StoredSavedCharacterData.FindChecked(CharacterIndex);

		// If the stored character compression is different than the character data compression, then...
		if (CharacterDataCompression != StoredDataCompression) {

			// Create a local byte buffer array to store the uncompressed data
			TArray<uint8> LocalUnCompressedData;

			// DEBUG ONLY - Print all compression related information now
			if (GEngine) {

				// Create two local integers
				int32 Compressed, Decompressed;

				// Collect the compressed array's size information, ensuring that it is always successful
				ensureAlways(FOodleCompressedArray::PeekSizes(CurrentCharacterData, Compressed, Decompressed) != 0);

				// Print out all compression related information now, informing the user of the current map's name along with it
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
			}

			// Uncompress the currently compressed stored data and store it within the above byte array
			FOodleCompressedArray::DecompressToTArray(LocalUnCompressedData, CurrentCharacterData);

			// Next, if the save system requires that character data is compressed prior to being saved, then compress it now
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(CharacterDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LocalUnCompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Finally, write the re-compressed data directly to file
				ensureAlways(FFileHelper::SaveArrayToFile(CompressionBuffer, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}

			// Else, just write the decompressed data directly to file
			else {
				ensureAlways(FFileHelper::SaveArrayToFile(LocalUnCompressedData, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}
		}

		// Else, just write the compressed data directly to file
		else {
			ensureAlways(FFileHelper::SaveArrayToFile(CurrentCharacterData, (FString(TEXT("CharacterData\\CharacterData")) + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(CharacterIndex) + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
		}
	}

	// 
	else {
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: Unable to find a savable character at index: ")) + FString::FromInt(CharacterIndex));
	}
}


// 
void USaveGameSubsystem::LoadCharacters() {

	// Create a global counter in order to keep track of what character's get saved
	uint8 LocalCounter = 0;

	// Loop through all currently existing player controllers
	for (FConstPlayerControllerIterator CurrentPlayerController = GetWorld()->GetPlayerControllerIterator(); CurrentPlayerController; ++CurrentPlayerController) {

		// Locally store the current player controller's controlled pawn
		APawn * CurrentPawn = CurrentPlayerController->Get()->GetPawn();

		// 
		if (!IsValid(CurrentPawn)) continue;

		// Check if this actor implements the blueprint side of character saving
		bool DoesImplementCharacterDataBlueprints = CurrentPawn->Implements<UCharacterDataBlueprints>();

		// Only proceed with the following functionality IFF the current character is savable
		if (CurrentPawn->Implements<UCharacterData>() || DoesImplementCharacterDataBlueprints) {

			// If this character has already been saved, then reset the data array to be zero (but keep the memory allocated)
			if (StoredSavedCharacterData.Contains(LocalCounter)) StoredSavedCharacterData.FindChecked(LocalCounter).Reset();


			// Create a local array of uncompressed character data for later loading purposes
			TArray<uint8> CharacterData;

			// If we compress the data prior to storing it, then we know we need to potentially decompress it upon collection
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local buffer array to store the data directly from file
				TArray<uint8> CompressedData;

				// Load data and, if it fails, then there is none to load and we should just utilize the defaults
				if (FFileHelper::LoadFileToArray(CompressedData, FString(TEXT("CharacterData\\CharacterData") + CurrentSaveGameID + TEXT("\\Character") + FString::FromInt(LocalCounter) + TEXT(".save")).GetCharArray().GetData()) == false) {

					// DEBUG ONLY - Print out the total number of bytes that require saving
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved character data, as none exists. Apologies.")));
					return;
				}

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressedData, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Uncompress the currently compressed stored data and store it within the above byte array
				FOodleCompressedArray::DecompressToTArray(CharacterData, CompressedData);
			}

			// Else, we can just work directly with the retrieved file data, if there is any
			else {

				// Load data and, if it fails, then there is none to load and we should just utilize the defaults
				if (FFileHelper::LoadFileToArray(CharacterData, FString(TEXT("CharacterData\\CharacterData") + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(LocalCounter) + TEXT(".save")).GetCharArray().GetData()) == false) {

					// DEBUG ONLY - Print out the total number of bytes that require saving
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved character data, as none exists. Apologies.")));
					return;
				}
			}

			// If we wish to compress the data prior to storing it locally, then...
			if (StoredDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, CharacterData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Globally store the important compressed character data now
				StoredSavedCharacterData.Add(LocalCounter, CompressionBuffer).Shrink();
			}

			// Else, we can just directly store the file data into the stored location and be done
			else {
				StoredSavedCharacterData.Add(LocalCounter, CharacterData).Shrink();
			}

			// Create a local reader archive in order to load the data back into their wanted objects
			FMemoryReader LocalCharacterArchive{CharacterData, true};
			LocalCharacterArchive.Seek(0);

			// If the current player controller supports the ICharacterData interface, then load their C++ data
			if (ICharacterData * SavableCharacter = Cast<ICharacterData>(CurrentPawn)) {
				SavableCharacter->TransferCharacterData(LocalCharacterArchive);
			}

			// Lastly, if we are dealing with the blueprints side of character saving, then...
			if (DoesImplementCharacterDataBlueprints) {

				// Check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
				const TSet<FName> CurrentSavablePropertyNames = ICharacterDataBlueprints::Execute_GetCharacterSavePropertyNames(CurrentPawn);
				if (CurrentSavablePropertyNames.Num() > 0) {

					// If so, then we need to loop through all properties within the current actor, and...
					for (FProperty * Property = CurrentPawn->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

						// Ensure that the current property is valid and, if it is not, then skip it and continue
						if (!Property) continue;

						// If the current property has the same name as a savable one, then...
						if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

							// Serialize the current property now
							LocalCharacterArchive.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentPawn), Property->ElementSize);
						}
					}
				}
			}

			// Ensure that, prior to any saving, we call the callback function to ensure that all data is handled properly if supported
			if (DoesImplementCharacterDataBlueprints) {
				ICharacterDataBlueprints::Execute_PostCharacterLoadCallback(CurrentPawn);
			}

			// Ensure that, no matter what, the load works perfectly and all data is completely loaded (regardless of whether it loaded correctly or not)
			ensureAlways(LocalCharacterArchive.AtEnd());

			// Finally, increment the counter (only incrementing the counter for each character that actually supports saving, that way we completely ignore all unsavable characters)
			++LocalCounter;
		}
	}
}


// 
void USaveGameSubsystem::LoadCharacter(const uint8 & CharacterIndex) {

	// Create a global counter in order to keep track of what character's get saved
	uint8 LocalCounter = 0;

	// Loop through all currently existing player controllers
	for (FConstPlayerControllerIterator CurrentPlayerController = GetWorld()->GetPlayerControllerIterator(); CurrentPlayerController; ++CurrentPlayerController) {

		// Locally store the current player controller's controlled pawn
		APawn* CurrentPawn = CurrentPlayerController->Get()->GetPawn();

		// Check if this actor implements the blueprint side of character saving
		bool DoesImplementCharacterDataBlueprints = CurrentPawn->Implements<UCharacterDataBlueprints>();

		// Only proceed with the following functionality IFF the current character is savable
		if (CurrentPawn->Implements<UCharacterData>() || DoesImplementCharacterDataBlueprints) {

			// If this character has already been saved, then reset the data array to be zero (but keep the memory allocated)
			if (StoredSavedCharacterData.Contains(LocalCounter)) StoredSavedCharacterData.FindChecked(LocalCounter).Reset();

			// If this is not the currently wanted character index, then skip and keep trying
			if (LocalCounter != CharacterIndex) {
				++LocalCounter;
				continue;
			}

			// Create a local array of uncompressed character data for later loading purposes
			TArray<uint8> CharacterData;

			// If we compress the data prior to storing it, then we know we need to potentially decompress it upon collection
			if (CharacterDataCompression != CompressionOptionsEnum::None) {

				// Create a local buffer array to store the data directly from file
				TArray<uint8> CompressedData;

				// Load data and, if it fails, then there is none to load and we should just utilize the defaults
				if (FFileHelper::LoadFileToArray(CompressedData, FString(TEXT("CharacterData\\CharacterData") + CurrentSaveGameID + TEXT("\\Character") + FString::FromInt(LocalCounter) + TEXT(".save")).GetCharArray().GetData()) == false) {

					// DEBUG ONLY - Print out the total number of bytes that require saving
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved character data, as none exists. Apologies.")));
					return;
				}

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressedData, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Uncompress the currently compressed stored data and store it within the above byte array
				FOodleCompressedArray::DecompressToTArray(CharacterData, CompressedData);
			}

			// Else, we can just work directly with the retrieved file data, if there is any
			else {

				// Load data and, if it fails, then there is none to load and we should just utilize the defaults
				if (FFileHelper::LoadFileToArray(CharacterData, FString(TEXT("CharacterData\\CharacterData") + CurrentSaveGameID + FString(TEXT("\\Character")) + FString::FromInt(LocalCounter) + TEXT(".save")).GetCharArray().GetData()) == false) {

					// DEBUG ONLY - Print out the total number of bytes that require saving
					if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved character data, as none exists. Apologies.")));
					return;
				}
			}

			// If we wish to compress the data prior to storing it locally, then...
			if (StoredDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, CharacterData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, FString(TEXT("Character ")) + FString::FromInt(LocalCounter) + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Globally store the important compressed character data now
				StoredSavedCharacterData.Add(LocalCounter, CompressionBuffer).Shrink();
			}

			// Else, we can just directly store the file data into the stored location and be done
			else {
				StoredSavedCharacterData.Add(LocalCounter, CharacterData).Shrink();
			}

			// Create a local reader archive in order to load the data back into their wanted objects
			FMemoryReader LocalCharacterArchive{ CharacterData, true };
			LocalCharacterArchive.Seek(0);

			// If the current player controller supports the ICharacterData interface, then load their C++ data
			if (ICharacterData* SavableCharacter = Cast<ICharacterData>(CurrentPawn)) {
				SavableCharacter->TransferCharacterData(LocalCharacterArchive);
			}

			// Lastly, if we are dealing with the blueprints side of character saving, then...
			if (DoesImplementCharacterDataBlueprints) {

				// Check if the actor has overridden the GetLevelDataStruct function in either Blueprints or C++ (e.g. it returns an array of size greater than zero)
				const TSet<FName> CurrentSavablePropertyNames = ICharacterDataBlueprints::Execute_GetCharacterSavePropertyNames(CurrentPawn);
				if (CurrentSavablePropertyNames.Num() > 0) {

					// If so, then we need to loop through all properties within the current actor, and...
					for (FProperty* Property = CurrentPawn->GetClass()->PropertyLink; Property != NULL; Property = Property->PropertyLinkNext) {

						// Ensure that the current property is valid and, if it is not, then skip it and continue
						if (!Property) continue;

						// If the current property has the same name as a savable one, then...
						if (CurrentSavablePropertyNames.Contains(Property->GetFName())) {

							// Serialize the current property now
							LocalCharacterArchive.Serialize(Property->ContainerPtrToValuePtr<void>(CurrentPawn), Property->ElementSize);
						}
					}
				}
			}

			// Ensure that, prior to any saving, we call the callback function to ensure that all data is handled properly if supported
			if (DoesImplementCharacterDataBlueprints) {
				ICharacterDataBlueprints::Execute_PostCharacterLoadCallback(CurrentPawn);
			}

			// Ensure that, no matter what, the load works perfectly and all data is completely loaded (regardless of whether it loaded correctly or not)
			ensureAlways(LocalCharacterArchive.AtEnd());

			// Finally, return, as this is the wanted index and it, and only it, has now been saved
			return;
		}
	}

	// DEBUG ONLY
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: Unable to find a loadable character at index: ")) + FString::FromInt(CharacterIndex));
}


////


// 
template<bool IsPersistentLevel>
void USaveGameSubsystem::SaveLevelHelperFunction(const ULevel * CurrentLevelPointer, TMap<FName, TArray<uint8>> & CurrentLevelMap) {

	// Ensure that the current map is empty, as we should not need to keep anything previously stored
	CurrentLevelMap.Reset();

	// Only continue with all of this functionality if the current level pointer is valid
	if (IsValid(CurrentLevelPointer)) {

		// Initialize a bounding box
		FBox CurrentLevelBoundingBox{ForceInit};

		// Check for if this level even contains actor clusters, else we just want to skip it altogether
		if (CurrentLevelPointer->bActorClusterCreated && CurrentLevelPointer->ActorCluster.Get()) {

			// DEBUG ONLY - Let the tester know that this level does utilize actor clusters
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" supports actor clusters!")));

			// Loop through all actors contained within this level's actor cluster
			for (AActor * CurrentActor : CurrentLevelPointer->ActorCluster.Get()->Actors) {

				// If the current actor is either invalid, or is also a dynamic actor, then skip it and move on
				if (!CurrentActor || CurrentLevelPointer->ActorsForGC.Contains(CurrentActor) || CurrentLevelPointer->Actors.Contains(CurrentActor)) continue;

				// If this is NOT a persistent level, then we also want to calculate the current level's bounding box via its static actors
				if constexpr (IsPersistentLevel == false) CalculateActorBoxHelperFunction(CurrentActor, CurrentLevelBoundingBox);

				// Only save the current actor IFF it has not been saved previously
				if (IsLevelData(CurrentActor) && !CurrentLevelMap.Contains(CurrentActor->GetFName())) SaveActorPerLevelHelperFunction(CurrentActor, CurrentLevelMap.Add(CurrentActor->GetFName()));
			}
		}

		// DEBUG ONLY - Else, no, we do not support actor clusters, so we are skipping them
		else if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" does not support actor clusters...")));


		// Loop through all currently existing actors of the current level, and...
		for (AActor * CurrentActor : CurrentLevelPointer->Actors) {

			// If the current actor is either invalid, or is also a dynamic actor, then skip it and move on
			if (!CurrentActor || CurrentLevelPointer->ActorsForGC.Contains(CurrentActor)) continue;

			// If this is NOT a persistent level, then we also want to calculate the current level's bounding box via its static actors
			if constexpr (IsPersistentLevel == false) CalculateActorBoxHelperFunction(CurrentActor, CurrentLevelBoundingBox);

			// Only save the current actor IFF it has not been saved previously
			if (IsLevelData(CurrentActor) && !CurrentLevelMap.Contains(CurrentActor->GetFName())) SaveActorPerLevelHelperFunction(CurrentActor, CurrentLevelMap.Add(CurrentActor->GetFName()));
		}


		// TODO: Change how these actors are saved, as to save their class' name rather than the actor's name
		// Next, we want to check for if this level contains any dynamic actors, because, if it does not, then we want to skip all of this extra logic and potential memory usage
		if (CurrentLevelPointer->ActorsForGC.Num() > 0) {

			// 
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("Collecting dynamic actors now from level: ")) + CurrentLevelPointer->GetOuter()->GetName());

			// Create a local dynamic actor map
			TMap<FName, TArray<uint8>> DynamicActorMap;


			// Next, loop through all currently existing dynamic actors of the current level, and...
			for (AActor * CurrentActor : CurrentLevelPointer->ActorsForGC) {

				// If the actor is invalid, then skip it and move on
				if (!IsValid(CurrentActor)) continue;

				// Save the current actor no matter what, as we are only saving its class name and know that there cannot be repeats
				if (IsLevelData(CurrentActor)) SaveActorPerLevelHelperFunction(CurrentActor, DynamicActorMap.Add(CurrentActor->GetClass()->GetFName()));
			}


			// Ignore storing the dynamic actor map if there are no savable dynamic actors, else...
			if (DynamicActorMap.Num() > 0) {

				// DEBUG ONLY
				if (GEngine) {
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("DynamicActorMap.Num() == ")) + FString::FromInt(DynamicActorMap.Num()));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("DynamicActorMap.GetAllocatedSize() == ")) + FString::FromInt(DynamicActorMap.GetAllocatedSize()));
				}

				// Find the current level's dynamic actor section of the parent TMap, create a memory writer pointing to its TArray<uint8>, and immediately serialize the DynamicActorMap into it
				TMemoryWriter<32> DynamicActorArchive{CurrentLevelMap.FindOrAdd(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_DYNAMIC_ACTORS")))), false, false, FName(TEXT("DynamicActorArchive"))};
				DynamicActorArchive << DynamicActorMap;
			}
		}

		// If this is not saving a persistent level, then...
		if constexpr (IsPersistentLevel == false) {

			// DEBUG ONLY - Draw this level's bounding box for 120 seconds
			DrawDebugBox(GetWorld(), CurrentLevelBoundingBox.GetCenter(), CurrentLevelBoundingBox.GetExtent(), FColor::Black, false, 120.0f, 0, 5.0f);

			// TODO: Change how these actors are saved, as to save their class' name rather than the actor's name
			// 
			if (LevelSavingTypes == LevelSavingTypesEnum::OnlyStreamedLevelsWithPersistentLevelActorCapturing) {

				// 
				if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("Collecting dynamic actors now from persistent level: ")) + GetWorld()->GetCurrentLevel()->GetOuter()->GetName());

				// Create a local dynamic actor map
				TMap<FName, TArray<uint8>> PersistentLevelDynamicActorMap;


				// Loop through all actors within the persistent level, and...
				for (AActor * CurrentActor : GetWorld()->GetCurrentLevel()->ActorsForGC) {

					// If the actor is invalid, then skip it and continue
					if (!CurrentActor) continue;

					// Compare the generated level bounds with the current actor's location (OR, alternatively, compare the level bounds with the actor's bounds and see if they intersect at all) and,
					//     if the location is within the level bounds, then it should be captured by this level
					FBox CurrentLevelBoundingBox2{CurrentActor->GetComponentsBoundingBox(true)};
					if (CurrentLevelBoundingBox.IsInsideOrOn(CurrentActor->GetActorLocation()) && CurrentLevelBoundingBox.Intersect(CurrentLevelBoundingBox2)) {

						// Only save the current actor IFF it has not been saved previously
						if (IsLevelData(CurrentActor) && !PersistentLevelDynamicActorMap.Contains(CurrentActor->GetClass()->GetFName())) SaveActorPerLevelHelperFunction(CurrentActor, PersistentLevelDynamicActorMap.Add(CurrentActor->GetClass()->GetFName()));
					}
				}

				// Ignore storing the dynamic actor map if there are no savable dynamic actors, else...
				if (PersistentLevelDynamicActorMap.Num() > 0) {

					// DEBUG ONLY
					if (GEngine) {
						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("PersistentLevelDynamicActorMap.Num() == ")) + FString::FromInt(PersistentLevelDynamicActorMap.Num()));
						GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("PersistentLevelDynamicActorMap.GetAllocatedSize() == ")) + FString::FromInt(PersistentLevelDynamicActorMap.GetAllocatedSize()));
					}

					// Find the current level's dynamic actor section of the parent TMap, create a memory writer pointing to its TArray<uint8>, and immediately serialize the DynamicActorMap into it
					TMemoryWriter<32> PersistentLevelDynamicActorArchive{CurrentLevelMap.FindOrAdd(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_PERSISTENT_LEVEL_DYNAMIC_ACTORS")))), false, false, FName(TEXT("PersistentLevelDynamicActorArchive")) };
					PersistentLevelDynamicActorArchive << PersistentLevelDynamicActorMap;
				}
			}
		}
	}

	// Ensure that the current level only takes up as much memory as it needs, in order to store it more appropriately
	CurrentLevelMap.Shrink();
}


// 
template<bool IsPersistentLevel>
void USaveGameSubsystem::StoreLevelHelperFunction(const ULevel * CurrentLevelPointer) {

	// 
	if (!IsValid(CurrentLevelPointer)) return;

	// Create a local map to store the current level's per actor information
	TMap<FName, TArray<uint8>> LocalLevelDataMap;

	// Save all of the current level's information and store it all into the above created map
	SaveLevelHelperFunction<IsPersistentLevel>(CurrentLevelPointer, LocalLevelDataMap);

	// If the current level failed to be stored, for whatever reason, then we should just return
	if (LocalLevelDataMap.IsEmpty()) return;

	// Create a buffer archive and store the above map's data into it
	FBufferArchive LocalMapWriter{false, FName("StoreLevelDataArchive")};
	LocalMapWriter << LocalLevelDataMap;

	// Empty out the map completely in order to get back all of that memory that we no longer need
	LocalLevelDataMap.Empty(0);

	// If the store system requires that data is compressed prior to being saved, then compress it now
	if (LevelDataCompression != CompressionOptionsEnum::None) {

		// Create a local byte buffer array for storing the compressed data
		TArray<uint8> CompressionBuffer;

		// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
		FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
		FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
		FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

		// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
		ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LocalMapWriter, TargetCompressor, TargetCompressionLevel));

		// DEBUG ONLY - Print all compression related information now
		if (GEngine) {

			// Create two local integers
			int32 Compressed, Decompressed;

			// Collect the compressed array's size information, ensuring that it is always successful
			ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

			// Print out all compression related information now, informing the user of the current map's name along with it
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
		}

		// Globally store the important compressed level data now
		StoredSavedLevelData.Add(CurrentLevelPointer->GetOuter()->GetFName(), CompressionBuffer);
	}

	// Else, globally store the uncompressed level data now
	else {
		StoredSavedLevelData.Add(CurrentLevelPointer->GetOuter()->GetFName(), LocalMapWriter);
	}
}


////


// 
void USaveGameSubsystem::StoreLevels() {

	// If we are saving persistent levels as their own level data, then...
	if (LevelSavingTypes == LevelSavingTypesEnum::OnlyPersistentLevels || LevelSavingTypes == LevelSavingTypesEnum::AllLevels) {

		// Save the current level, ensuring to treat it as a persistent level
		StoreLevelHelperFunction<true>(GetWorld()->GetCurrentLevel());
	}

	// If we are also saving streaming levels, then...
	if (LevelSavingTypes != LevelSavingTypesEnum::OnlyPersistentLevels) {

		// Loop through all current streaming levels, and...
		for (ULevelStreaming * CurrentStreamingLevel : GetWorld()->GetStreamingLevels()) {

			// If the current streaming level is invalid, or has not been loaded yet, then skip it and continue
			if (!CurrentStreamingLevel || !CurrentStreamingLevel->HasLoadedLevel()) continue;

			// Else, we save the loaded level, ensuring to treat it as a streaming level (not a persistent level)
			StoreLevelHelperFunction<false>(CurrentStreamingLevel->GetLoadedLevel());
		}
	}
}


// 
void USaveGameSubsystem::StoreLevel(const FName & LevelName) {

	// If we are saving persistent levels as their own level data, then...
	if (LevelSavingTypes == LevelSavingTypesEnum::OnlyPersistentLevels || LevelSavingTypes == LevelSavingTypesEnum::AllLevels) {

		// If the current level's name matches the one we want saved, then save the current level, ensuring to treat it as a persistent level, and return immediately
		if (LevelName == GetWorld()->GetCurrentLevel()->GetOuter()->GetFName()) {
			StoreLevelHelperFunction<true>(GetWorld()->GetCurrentLevel());
			return;
		}
	}

	// If we are also saving streaming levels, then...
	if (LevelSavingTypes != LevelSavingTypesEnum::OnlyPersistentLevels) {

		// Loop through all current streaming levels, and...
		for (ULevelStreaming * CurrentStreamingLevel : GetWorld()->GetStreamingLevels()) {

			// If the current streaming level is invalid, or has not been loaded yet, then skip it and continue
			if (!CurrentStreamingLevel || !CurrentStreamingLevel->HasLoadedLevel()) continue;

			// Else, if the current streaming level's name matches the one we wish to save, then we save the loaded level, ensuring to treat it as a streaming level (not a persistent level), and return immediately
			if (LevelName == CurrentStreamingLevel->GetLoadedLevel()->GetOuter()->GetFName()) {
				StoreLevelHelperFunction<false>(CurrentStreamingLevel->GetLoadedLevel());
				return;
			}
		}
	}

	// DEBUG ONLY
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: Could not find a currently existing level to store with the name of: ")) + LevelName.ToString());
}


////


// 
void USaveGameSubsystem::SaveLevels() {

	// Store all currently existing levels for future (as well as current) saving
	StoreLevels();

	// Loop through all stored level information, and...
	for (auto & CurrentLevelData : StoredSavedLevelData) {

		// If the stored data compression is different than the level data compression, then...
		if (LevelDataCompression != StoredDataCompression) {

			// Create a local byte buffer array to store the uncompressed data
			TArray<uint8> LocalUnCompressedData;

			// DEBUG ONLY - Print all compression related information now
			if (GEngine) {

				// Create two local integers
				int32 Compressed, Decompressed;

				// Collect the compressed array's size information, ensuring that it is always successful
				ensureAlways(FOodleCompressedArray::PeekSizes(CurrentLevelData.Value, Compressed, Decompressed) != 0);

				// Print out all compression related information now, informing the user of the current map's name along with it
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
			}

			// Uncompress the currently compressed stored data and store it within the above byte array
			FOodleCompressedArray::DecompressToTArray(LocalUnCompressedData, CurrentLevelData.Value);

			// Next, if the save system requires that level data is compressed prior to being saved, then compress it now
			if (LevelDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(LevelDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LocalUnCompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Finally, write the re-compressed data directly to file
				ensureAlways(FFileHelper::SaveArrayToFile(CompressionBuffer, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}

			// Else, just write the decompressed data directly to file
			else {
				ensureAlways(FFileHelper::SaveArrayToFile(LocalUnCompressedData, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}
		}

		// Else, just write the compressed data directly to file
		else {
			ensureAlways(FFileHelper::SaveArrayToFile(CurrentLevelData.Value, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
		}
	}

	// Lastly, completely empty the memory buffer in order to give the user back their memory, as we no longer need to locally store it
	//StoredSavedLevelData.Empty();
}


// 
void USaveGameSubsystem::SaveLevel(const FName & LevelName) {

	// Store all currently existing levels for future saving
	StoreLevel(LevelName);

	// Loop through all stored level information, and...
	for (auto & CurrentLevelData : StoredSavedLevelData) {

		// 
		if (CurrentLevelData.Key != LevelName) continue;

		// If the stored data compression is different than the level data compression, then...
		if (LevelDataCompression != StoredDataCompression) {

			// Create a local byte buffer array to store the uncompressed data
			TArray<uint8> LocalUnCompressedData;

			// DEBUG ONLY - Print all compression related information now
			if (GEngine) {

				// Create two local integers
				int32 Compressed, Decompressed;

				// Collect the compressed array's size information, ensuring that it is always successful
				ensureAlways(FOodleCompressedArray::PeekSizes(CurrentLevelData.Value, Compressed, Decompressed) != 0);

				// Print out all compression related information now, informing the user of the current map's name along with it
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
			}

			// Uncompress the currently compressed stored data and store it within the above byte array
			FOodleCompressedArray::DecompressToTArray(LocalUnCompressedData, CurrentLevelData.Value);

			// Next, if the save system requires that level data is compressed prior to being saved, then compress it now
			if (LevelDataCompression != CompressionOptionsEnum::None) {

				// Create a local byte buffer array for storing the compressed data
				TArray<uint8> CompressionBuffer;

				// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
				FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
				FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
				FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(LevelDataCompression), TargetCompressor, TargetCompressionLevel);

				// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
				ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LocalUnCompressedData, TargetCompressor, TargetCompressionLevel));

				// DEBUG ONLY - Print all compression related information now
				if (GEngine) {

					// Create two local integers
					int32 Compressed, Decompressed;

					// Collect the compressed array's size information, ensuring that it is always successful
					ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

					// Print out all compression related information now, informing the user of the current map's name along with it
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
					GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelData.Key.ToString() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
				}

				// Finally, write the re-compressed data directly to file
				ensureAlways(FFileHelper::SaveArrayToFile(CompressionBuffer, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}

			// Else, just write the decompressed data directly to file
			else {
				ensureAlways(FFileHelper::SaveArrayToFile(LocalUnCompressedData, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
			}
		}

		// Else, just write the compressed data directly to file
		else {
			ensureAlways(FFileHelper::SaveArrayToFile(CurrentLevelData.Value, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelData.Key.ToString() + FString(TEXT(".save"))).GetCharArray().GetData(), &IFileManager::Get(), EFileWrite::FILEWRITE_NoFail));
		}

		// Lastly, immediately return, as we saved our target level and that's all we care about
		return;
	}

	// DEBUG ONLY
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("ERROR: Could not find a currently existing level to save with the name of: ")) + LevelName.ToString());

	// Lastly, completely empty the memory buffer in order to give the user back their memory, as we no longer need to locally store it
	//StoredSavedLevelData.Empty();
}


////


// 
template<bool IsPersistentLevel>
void USaveGameSubsystem::LoadLevelHelperFunction(const ULevel * CurrentLevelPointer) {

	// If we are given an invalid level pointer, then just return
	if (!IsValid(CurrentLevelPointer)) return;

	// Create a local array of uncompressed level data for later loading purposes
	TArray<uint8> LevelData;

	// If we compress the data prior to storing it, then we know we need to potentially decompress it upon collection
	if (LevelDataCompression != CompressionOptionsEnum::None) {

		// Create a local buffer array to store the data directly from file
		TArray<uint8> CompressedData;

		// Load data and, if it fails, then there is none to load and we should just utilize the defaults
		if (FFileHelper::LoadFileToArray(CompressedData, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(".save"))).GetCharArray().GetData()) == false) {

			// DEBUG ONLY
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved data from level ")) + CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(", as none exists. Apologies.")));
			return;
		}

		// DEBUG ONLY - Print all compression related information now
		if (GEngine) {

			// Create two local integers
			int32 Compressed, Decompressed;

			// Collect the compressed array's size information, ensuring that it is always successful
			ensureAlways(FOodleCompressedArray::PeekSizes(CompressedData, Compressed, Decompressed) != 0);

			// Print out all compression related information now, informing the user of the current map's name along with it
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
		}

		// Uncompress the currently compressed stored data and store it within the above byte array
		FOodleCompressedArray::DecompressToTArray(LevelData, CompressedData);
	}

	// Else, we can just work directly with the retrieved file data, if there is any
	else {

		// Load data and, if it fails, then there is none to load and we should just utilize the defaults
		if (FFileHelper::LoadFileToArray(LevelData, (FString(TEXT("LevelData\\LevelData")) + CurrentSaveGameID + FString(TEXT("\\")) + CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(".save"))).GetCharArray().GetData()) == false) {

			// DEBUG ONLY
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString(TEXT("Cannot read any saved data from level ")) + CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(", as none exists. Apologies.")));
			return;
		}
	}

	// If we wish to compress the data prior to storing it locally, then...
	if (StoredDataCompression != CompressionOptionsEnum::None) {

		// Create a local byte buffer array for storing the compressed data
		TArray<uint8> CompressionBuffer;

		// Generate the wanted compression settings (could potentially be changed to simply utilize the underlying methods instead)
		FOodleDataCompression::ECompressor TargetCompressor = FOodleDataCompression::ECompressor::Mermaid;
		FOodleDataCompression::ECompressionLevel TargetCompressionLevel = FOodleDataCompression::ECompressionLevel::HyperFast2;
		FOodleDataCompression::GetCompressorAndLevelForCommonUsage(static_cast<FOodleDataCompression::ECompressionCommonUsage>(StoredDataCompression), TargetCompressor, TargetCompressionLevel);

		// Compress the array given the above settings, ensuring that the compression is always successful (crashing if it is not)
		ensureAlways(FOodleCompressedArray::CompressTArray(CompressionBuffer, LevelData, TargetCompressor, TargetCompressionLevel));

		// DEBUG ONLY - Print all compression related information now
		if (GEngine) {

			// Create two local integers
			int32 Compressed, Decompressed;

			// Collect the compressed array's size information, ensuring that it is always successful
			ensureAlways(FOodleCompressedArray::PeekSizes(CompressionBuffer, Compressed, Decompressed) != 0);

			// Print out all compression related information now, informing the user of the current map's name along with it
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Compressed Size (bytes): ")) + FString::FromInt(Compressed));
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Black, CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT(" - Decompressed Size (bytes): ")) + FString::FromInt(Decompressed));
		}

		// Globally store the important compressed level data now
		StoredSavedLevelData.Add(CurrentLevelPointer->GetOuter()->GetFName(), CompressionBuffer).Shrink();
	}

	// Else, we can just directly store the file data into the stored location and be done
	else {
		StoredSavedLevelData.Add(CurrentLevelPointer->GetOuter()->GetFName(), LevelData).Shrink();
	}

	// Create a local TMap in order to hold all level data for the current level
	TMap<FName, TArray<uint8>> CurrentLevelDataMap;

	// Convert the array of bytes back into a map of actor names and per actor saved data
	FMemoryReader LevelDataReader{ LevelData, true };
	LevelDataReader.Seek(0);
	LevelDataReader << CurrentLevelDataMap;

	// Ensure that the above array of bytes is no longer needed, as it has now been converted back into its better path
	LevelData.Empty(0);

	// Check if the current level has an actor cluster that is valid and, if so, then...
	if (CurrentLevelPointer->bActorClusterCreated && CurrentLevelPointer->ActorCluster.Get()) {

		// Loop through all actors within the actor cluster and...
		for (AActor * CurrentActor : CurrentLevelPointer->ActorCluster.Get()->Actors) {

			// Skip this current actor if it either exists within the global Actors array, exists within the global ActorsForGC array, or is invalid
			if (!IsValid(CurrentActor) || CurrentLevelPointer->Actors.Contains(CurrentActor) || CurrentLevelPointer->ActorsForGC.Contains(CurrentActor)) continue;

			// Finally, load the current actor with its data from the current level data map as long as said data exists (if it does not, then simply ignore this actor)
			if (IsLevelData(CurrentActor) && CurrentLevelDataMap.Contains(CurrentActor->GetFName())) LoadLevelPerActorHelperFunction(CurrentActor, CurrentLevelDataMap.FindChecked(CurrentActor->GetFName()));
		}
	}

	// Loop through all actors that the current level contains, and...
	for (AActor * CurrentActor : CurrentLevelPointer->Actors) {

		// Skip this current actor if it either exists within the global ActorsForGC array or is invalid
		if (!IsValid(CurrentActor) || CurrentLevelPointer->ActorsForGC.Contains(CurrentActor)) continue;

		// Finally, load the current actor with its data from the current level data map as long as said data exists (if it does not, then simply ignore this actor)
		if (IsLevelData(CurrentActor) && CurrentLevelDataMap.Contains(CurrentActor->GetFName())) LoadLevelPerActorHelperFunction(CurrentActor, CurrentLevelDataMap.FindChecked(CurrentActor->GetFName()));
	}

	// Loop through all currently existing dynamic actors and destroy them (as long as they are a savable level asset), as we need to start from a fresh slate
	for (int32 CurrentIndex = CurrentLevelPointer->ActorsForGC.Num() - 1; CurrentIndex >= 0; --CurrentIndex) {
		AActor * CurrentActor = CurrentLevelPointer->ActorsForGC[CurrentIndex];
		if (IsLevelData(CurrentActor)) CurrentActor->Destroy();
	}

	// If this level saved any dynamic actors whatsoever, then...
	if (CurrentLevelDataMap.Contains(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_DYNAMIC_ACTORS"))))) {

		// Create a memory reader and give it the [Level Name]_DYNAMIC_ACTORS' data
		FMemoryReader DynamicActorArchive{CurrentLevelDataMap.FindChecked(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_DYNAMIC_ACTORS")))), true};

		// Create a local variable to load all of the level's dynamic actor's information
		TMap<FName, TArray<uint8>> DynamicActorMap;

		// Load all of the data bytes into the original TMap format
		DynamicActorArchive << DynamicActorMap;

		// Loop through the map of actors, and...
		for (auto & CurrentDynamicActorInfo : DynamicActorMap) {

			// Generate the target class pointer now
			UClass* CurrentActorClass = GetClassFromString(CurrentDynamicActorInfo.Key.ToString());

			// Ensure that the collected class pointer is always valid
			ensureAlways(IsValid(CurrentActorClass));

			// Create a local struct for spawning an actor and set the target override level, as well as ensure that spawning cannot fail
			FActorSpawnParameters LocalSpawnParameters;
			LocalSpawnParameters.OverrideLevel = const_cast<ULevel*>(CurrentLevelPointer);
			LocalSpawnParameters.bNoFail = true;

			// Finally, spawn the actor and immediately pass the newly created actor into the load functionality to load it (duh)
			LoadLevelPerActorHelperFunction(GetWorld()->SpawnActor(CurrentActorClass, nullptr, nullptr, LocalSpawnParameters), CurrentDynamicActorInfo.Value);
		}
	}

	// Only do this functionality if we are dealing with a streaming level, in which case...
	if constexpr (IsPersistentLevel == false) {

		// If we are also storing dynamic persistent level actors within the levels that contain them temporarily, then...
		if (LevelSavingTypes == LevelSavingTypesEnum::OnlyStreamedLevelsWithPersistentLevelActorCapturing) {

			// If this level saved any persistent level dynamic actors whatsoever, then...
			if (CurrentLevelDataMap.Contains(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_PERSISTENT_LEVEL_DYNAMIC_ACTORS"))))) {

				// Create a memory reader and give it the [Level Name]_PERSISTENT_LEVEL_DYNAMIC_ACTORS' data
				FMemoryReader PersistentLevelDynamicActorArchive{CurrentLevelDataMap.FindChecked(FName(CurrentLevelPointer->GetOuter()->GetName() + FString(TEXT("_PERSISTENT_LEVEL_DYNAMIC_ACTORS")))), true};

				// Create a local variable to load all of the level's dynamic actor's information
				TMap<FName, TArray<uint8>> DynamicActorMap;

				// Load all of the data bytes into the original TMap format
				PersistentLevelDynamicActorArchive << DynamicActorMap;

				// Loop through the map of actors, and...
				for (auto & CurrentDynamicActorInfo : DynamicActorMap) {

					// Generate the target class pointer now
					UClass * CurrentActorClass = GetClassFromString(CurrentDynamicActorInfo.Key.ToString());

					// Ensure that the collected class pointer is always valid
					ensureAlways(IsValid(CurrentActorClass));

					// Create a local struct for spawning an actor and set the target override level, as well as ensure that spawning cannot fail
					FActorSpawnParameters LocalSpawnParameters;
					LocalSpawnParameters.OverrideLevel = GetWorld()->GetCurrentLevel();
					LocalSpawnParameters.bNoFail = true;

					// Finally, spawn the actor and immediately pass the newly created actor into the load functionality to load it (duh)
					LoadLevelPerActorHelperFunction(GetWorld()->SpawnActor(CurrentActorClass, nullptr, nullptr, LocalSpawnParameters), CurrentDynamicActorInfo.Value);
				}
			}
		}
	}
}


// 
void USaveGameSubsystem::LoadLevels() {

	// If we save streaming levels, then...
	if (LevelSavingTypes != LevelSavingTypesEnum::OnlyPersistentLevels) {

		// Loop through all currently existing streaming levels, and...
		for (ULevelStreaming * CurrentStreamingLevel : GetWorld()->GetStreamingLevels()) {

			// Ensure that the current streaming level is both valid and has loaded prior to anything else
			if (IsValid(CurrentStreamingLevel) && CurrentStreamingLevel->HasLoadedLevel()) {

				// Load the current level utilizing the helper function to reduce pain and code reuse
				LoadLevelHelperFunction<false>(CurrentStreamingLevel->GetLoadedLevel());
			}
		}
	}

	// If we are also loading the persistent level(s), then...
	if (LevelSavingTypes == LevelSavingTypesEnum::OnlyPersistentLevels || LevelSavingTypes == LevelSavingTypesEnum::AllLevels) {

		// Load the current persistent level utilizing the helper function to reduce pain and code reuse
		LoadLevelHelperFunction<false>(GetWorld()->GetCurrentLevel());
	}
}


// 
void USaveGameSubsystem::LoadLevel(const FName & LevelName) {

	// If we save streaming levels, then...
	if (LevelSavingTypes != LevelSavingTypesEnum::OnlyPersistentLevels) {

		// Loop through all currently existing streaming levels, and...
		for (ULevelStreaming * CurrentStreamingLevel : GetWorld()->GetStreamingLevels()) {

			// Ensure that the current streaming level is both valid and has loaded prior to anything else
			if (IsValid(CurrentStreamingLevel) && CurrentStreamingLevel->HasLoadedLevel()) {

				// If we are not dealing with the wanted level, then skip this and continue
				if (LevelName != CurrentStreamingLevel->GetLoadedLevel()->GetOuter()->GetFName()) continue;

				// Load the current level utilizing the helper function to reduce pain and code reuse
				LoadLevelHelperFunction<false>(CurrentStreamingLevel->GetLoadedLevel());

				// Now return, as we have loaded our target level and need nothing else
				return;
			}
		}
	}

	// If we are also loading the persistent level(s), then...
	if (LevelSavingTypes == LevelSavingTypesEnum::OnlyPersistentLevels || LevelSavingTypes == LevelSavingTypesEnum::AllLevels) {

		// If we ARE dealing with the wanted level, then...
		if (LevelName == GetWorld()->GetCurrentLevel()->GetOuter()->GetFName()) {

			// Load the current persistent level utilizing the helper function to reduce pain and code reuse
			LoadLevelHelperFunction<false>(GetWorld()->GetCurrentLevel());
		}
	}
}


////


// 
void USaveGameSubsystem::SaveGame() {
	this->SaveCharacters();
	this->SaveLevels();

	// DEBUG ONLY - Print out all stored data sizes (in bytes)
	if (GEngine) {

		// 
		int32 Counter = StoredSavedCharacterData.GetAllocatedSize();
		for (const auto & CurrentPair : StoredSavedCharacterData) {
			Counter += CurrentPair.Value.GetAllocatedSize();
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("StoredSavedCharacterData size (bytes): ")) + FString::FromInt(Counter));

		// 
		Counter = StoredSavedLevelData.GetAllocatedSize();
		for (const auto & CurrentPair : StoredSavedLevelData) {
			Counter += CurrentPair.Value.GetAllocatedSize();
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("StoredSavedLevelData size (bytes): ")) + FString::FromInt(Counter));

		// 
		Counter = StoredSavedStoryData.GetAllocatedSize();
		for (const auto & CurrentPair : StoredSavedStoryData) {
			Counter += CurrentPair.Value.GetAllocatedSize();
		}
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Purple, FString(TEXT("StoredSavedStoryData size (bytes): ")) + FString::FromInt(Counter));
	}
}


// 
void USaveGameSubsystem::AutoSaveGame() {
	const FString PreviousSaveID = CurrentSaveGameID;
	CurrentSaveGameID = TEXT("_AUTOSAVE");
	this->SaveGame();
	CurrentSaveGameID = PreviousSaveID;
}


// 
void USaveGameSubsystem::LoadGame() {
	this->LoadLevels();
	this->LoadCharacters();
}
