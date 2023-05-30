// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SettingsOptionsStruct.generated.h"


// Enum which contains all possible types of setting variable data
UENUM(BlueprintType)
enum class USettingDataTypeEnum : uint8 {
	Boolean,
	Integer,
	Float,
	String,
	ERROR
};


// 
USTRUCT(BlueprintType)
struct FSettingsOptionsStruct {
	GENERATED_BODY()


// 
public:

	// Text type to store the setting's localized name
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	// Text type to store the setting's localized description
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Description;

	// A boolean check to see whether this setting, when focused, should move the main camera's world transform
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool ShouldUpdateCameraWorldTransform = false;

	// If the camera should move, then this is its target transform
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="ShouldUpdateCameraWorldTransform", EditConditionHides))
	FTransform CameraWorldTransform = FTransform::Identity;

	// Section which the variable is stored into within the config file
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ConfigSectionName;

	// Name of the variable that is actively being stored (Note: If you want this config variable to directly affect an existing console command, then insert the command here, as all Variable Names are treated as console commands)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ToolTip ="Note: If you want this config variable to directly affect an existing console command, then insert the command here, as all Variable Names are treated as console commands."))
	FName ConfigVariableName;

	// An enum to handle the type of the underlying data for ease of use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USettingDataTypeEnum SettingDataType = USettingDataTypeEnum::ERROR;

	// Optional text array which is only utilized in certain settings menu options, such as ComboBoxes
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="SettingDataType==USettingDataTypeEnum::Integer", EditConditionHides))
	TArray<FText> SettingLevelLabels;
};


// 
USTRUCT(BlueprintType)
struct FUnionDataStruct {
	GENERATED_BODY()


// 
public:

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USettingDataTypeEnum DataType;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(EditCondition="DataType==USettingDataTypeEnum::Boolean", EditConditionHides))
	bool BooleanData;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(EditCondition="DataType==USettingDataTypeEnum::Integer", EditConditionHides))
	int32 IntegerData;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(EditCondition="DataType==USettingDataTypeEnum::Float", EditConditionHides))
	float FloatData;

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta=(EditCondition="DataType==USettingDataTypeEnum::String", EditConditionHides))
	FString StringData;


// 
public:

	// 
	FUnionDataStruct() {
		DataType = USettingDataTypeEnum::ERROR;
	}

	// 
	FUnionDataStruct(bool NewValue) {
		DataType = USettingDataTypeEnum::Boolean;
		BooleanData = NewValue;
	}

	// 
	FUnionDataStruct(int32 NewValue) {
		DataType = USettingDataTypeEnum::Integer;
		IntegerData = NewValue;
	}

	// 
	FUnionDataStruct(float NewValue) {
		DataType = USettingDataTypeEnum::Float;
		FloatData = NewValue;
	}

	// 
	FUnionDataStruct(FString NewValue) {
		DataType = USettingDataTypeEnum::String;
		StringData = NewValue;
	}
};
