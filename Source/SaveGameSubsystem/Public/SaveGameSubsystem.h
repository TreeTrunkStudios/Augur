// Sidney Towers
// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SaveGameSubsystem.generated.h"


// 
UENUM()
enum LevelSavingTypesEnum {
	OnlyPersistentLevels = 0,
	OnlyStreamedLevels = 1,
	OnlyStreamedLevelsWithPersistentLevelActorCapturing = 2,
	AllLevels = 3
};


// 
UENUM()
enum CompressionOptionsEnum {
	NoBias = 0,
	SpeedBias = 1,
	SizeBias = 2,
	TinyBiasDONOTUSE = 3,
	None = 4
};


// 
UCLASS()
class SAVEGAMESUBSYSTEM_API USaveGameSubsystem final : public UWorldSubsystem {
	GENERATED_BODY()
	

// 
private:

	// 
	FString CurrentSaveGameID{TEXT("_0")};

	// 
	TMap<uint8, TArray<uint8>> StoredSavedCharacterData;
	TMap<FName, TArray<uint8>> StoredSavedLevelData;
	TMap<FName, TArray<uint8>> StoredSavedStoryData;


// 
private:

	// 
	LevelSavingTypesEnum LevelSavingTypes = LevelSavingTypesEnum::OnlyStreamedLevelsWithPersistentLevelActorCapturing;

	// 
	CompressionOptionsEnum CharacterDataCompression = CompressionOptionsEnum::SpeedBias;

	// 
	CompressionOptionsEnum LevelDataCompression = CompressionOptionsEnum::SpeedBias;

	// 
	CompressionOptionsEnum StoryDataCompression = CompressionOptionsEnum::SpeedBias;

	// 
	CompressionOptionsEnum StoredDataCompression = CompressionOptionsEnum::SizeBias;


// 
public:

	// 
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// 
	virtual void Deinitialize() override;


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	void SetCurrentGameID(const FString & NewGameID);

	// 
	UFUNCTION(BlueprintCallable)
	void SetLevelSavingTypes(const LevelSavingTypesEnum NewLevelSavingTypes);

	// 
	UFUNCTION(BlueprintCallable)
	void SetCharacterDataCompression(const CompressionOptionsEnum NewCharacterDataCompression);

	// 
	UFUNCTION(BlueprintCallable)
	void SetLevelDataCompression(const CompressionOptionsEnum NewLevelDataCompression);

	// 
	UFUNCTION(BlueprintCallable)
	void SetStoryDataCompression(const CompressionOptionsEnum NewStoryDataCompression);

	// 
	UFUNCTION(BlueprintCallable)
	void SetStoredDataCompression(const CompressionOptionsEnum NewStoredDataCompression);


// 
public:

	// 
	void StoreCharacters();

	// 
	void StoreCharacter(const uint8 & CharacterIndex);

	// 
	void SaveCharacters();

	// 
	void SaveCharacter(const uint8 & CharacterIndex);

	// 
	void LoadCharacters();

	// 
	void LoadCharacter(const uint8 & CharacterIndex);


// 
public:

	// 
	void StoreLevels();

	// 
	void StoreLevel(const FName & LevelName);

	// 
	void SaveLevels();

	// 
	void SaveLevel(const FName & LevelName);

	// 
	void LoadLevels();

	// 
	void LoadLevel(const FName & LevelName);


// 
public:

	// 
	UFUNCTION(BlueprintCallable)
	void SaveGame();

	// 
	UFUNCTION(BlueprintCallable)
	void AutoSaveGame();

	// 
	UFUNCTION(BlueprintCallable)
	void LoadGame();


//
private:

	// 
	template<bool IsPersistentLevel>
	void SaveLevelHelperFunction(const ULevel * CurrentLevelPointer, TMap<FName, TArray<uint8>> & CurrentLevelMap);

	// 
	template<bool IsPersistentLevel>
	void StoreLevelHelperFunction(const ULevel * CurrentLevelPointer);

	// 
	template<bool IsPersistentLevel>
	void LoadLevelHelperFunction(const ULevel * CurrentLevelPointer);
};
