// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbstractSettingsWidget.h"
#include "SettingsMenuOption.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API USettingsMenuOption final : public UUserWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAbstractSettingsWidget> Type;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSettingsOptionsStruct SettingsOptions;

	// 
	TObjectPtr<UAbstractSettingsWidget> SettingsWidget;

	// Optional settings menu option widget that 
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USettingsMenuOption * OptionalRelySetting;

	// An enum to handle the type of the underlying data for ease of use
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(EditCondition="OptionalRelySetting!=nullptr", EditConditionHides))
	USettingDataTypeEnum RelySettingDataType = USettingDataTypeEnum::ERROR;

	// USettingDataTypeEnum::Boolean equate information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="RelySettingDataType==USettingDataTypeEnum::Boolean", EditConditionHides))
	bool RelySettingVisibleBoolState = false;

	// USettingDataTypeEnum::Integer equate information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="RelySettingDataType==USettingDataTypeEnum::Integer", EditConditionHides))
	TArray<int32> RelySettingVisibleIntegerStates;

	// USettingDataTypeEnum::Float equate information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="RelySettingDataType==USettingDataTypeEnum::Float", EditConditionHides))
	float RelySettingVisibleFloatMinRange = 0.0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="RelySettingDataType==USettingDataTypeEnum::Float", EditConditionHides))
	float RelySettingVisibleFloatMaxRange = 2.0;

	// USettingDataTypeEnum::String equate information
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="RelySettingDataType==USettingDataTypeEnum::String", EditConditionHides))
	TArray<FString> RelySettingVisibleStringStates;


// 
public:

	// 
	//USettingsMenuOption(const FObjectInitializer & ObjectInitializer);

	// 
	virtual bool Initialize() override;

	// 
	virtual void NativeConstruct() override;

	// 
	const TUnion<bool, int32, float, FString> & CollectDataContainer() const;
};
