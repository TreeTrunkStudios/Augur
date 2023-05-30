// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "ControllerDisplayWidget.generated.h"


// 
UCLASS()
class GAMESETTINGSSUBSYSTEM_API UControllerDisplayWidget : public UUserWidget {
	GENERATED_BODY()
	

// 
public:

	// 
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UCanvasPanel * RootCanvasPanel;

	//
	virtual bool Initialize() override;

	// 
	virtual void NativeConstruct() override;

	// 
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
