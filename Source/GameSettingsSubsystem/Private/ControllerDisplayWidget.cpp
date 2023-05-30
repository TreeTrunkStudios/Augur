// TreeTrunkStudios (c) 2023


// 
#include "ControllerDisplayWidget.h"
#include "Blueprint/WidgetTree.h"


// 
bool UControllerDisplayWidget::Initialize() {

	// 
	return Super::Initialize();
}


// 
void UControllerDisplayWidget::NativeConstruct() {

	// 
	Super::NativeConstruct();
}


// 
TSharedRef<SWidget> UControllerDisplayWidget::RebuildWidget() {

	// 
	TSharedRef<SWidget> LocalSharedWidget = Super::RebuildWidget();

	// 
	RootCanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("CanvasPanel"));

	// 
	ensureAlways(IsValid(RootCanvasPanel));

	// If we have a root widget already, then just add as a child widget
	if (IsValid(Cast<UPanelWidget>(WidgetTree->RootWidget))) {
		Cast<UPanelWidget>(WidgetTree->RootWidget)->AddChild(RootCanvasPanel);
	}

	// Else, we can make the new widget as the root, which is much better
	else if (RootCanvasPanel->IsSafeForRootSet()) {
		WidgetTree->RootWidget = RootCanvasPanel;
	}

	// 
	RootCanvasPanel->SetRenderShear(FVector2D(20.0, 0.0));

	// 
	return LocalSharedWidget;
}
