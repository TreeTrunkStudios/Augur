// TreeTrunkStudios (c) 2023


// 
#include "AbstractItemUseSubMenuWidget.h"
#include "AbstractInventoryWidget.h"
#include <Blueprint/WidgetTree.h>
#include "Components/CanvasPanelSlot.h"
#include "Slate/SGameLayerManager.h"


// 
bool UAbstractItemUseSubMenuWidget::Initialize() {

	// 
	const bool ReturnValue = Super::Initialize();

	//
	if (IsValid(RemoveButton) == false || IsValid(InteractButton) == false || IsValid(CombineButton) == false || IsValid(MoveButton) == false || IsValid(ExamineButton) == false || IsValid(DiscardButton) == false)
		return ReturnValue;

	//
	RemoveButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenuButton);
	InteractButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::InteractItemButton);
	CombineButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::CombineItemButton);
	MoveButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::MoveItemButton);
	ExamineButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::ExamineItemButton);
	DiscardButton->OnClicked.AddDynamic(this, &UAbstractItemUseSubMenuWidget::DiscardItemButton);

	// Create new, defaulted objects for each and every single button's navigation systems
	InteractButton->Navigation = NewObject<UWidgetNavigation>(InteractButton);
	CombineButton->Navigation = NewObject<UWidgetNavigation>(CombineButton);
	MoveButton->Navigation = NewObject<UWidgetNavigation>(MoveButton);
	ExamineButton->Navigation = NewObject<UWidgetNavigation>(ExamineButton);
	DiscardButton->Navigation = NewObject<UWidgetNavigation>(DiscardButton);

	// Ensure that left and right movement does nothing, as we need to stay within this widget now
	InteractButton->Navigation->Left.Rule = EUINavigationRule::Stop;
	InteractButton->Navigation->Right.Rule = EUINavigationRule::Stop;
	CombineButton->Navigation->Left.Rule = EUINavigationRule::Stop;
	CombineButton->Navigation->Right.Rule = EUINavigationRule::Stop;
	MoveButton->Navigation->Left.Rule = EUINavigationRule::Stop;
	MoveButton->Navigation->Right.Rule = EUINavigationRule::Stop;
	ExamineButton->Navigation->Left.Rule = EUINavigationRule::Stop;
	ExamineButton->Navigation->Right.Rule = EUINavigationRule::Stop;
	DiscardButton->Navigation->Left.Rule = EUINavigationRule::Stop;
	DiscardButton->Navigation->Right.Rule = EUINavigationRule::Stop;

	// Initialize the use rules to utilize the custom functionality depending on what button is focused
	InteractButton->Navigation->Next.Rule = EUINavigationRule::Custom;
	InteractButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::InteractItem);
	CombineButton->Navigation->Next.Rule = EUINavigationRule::Custom;
	CombineButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::CombineItem);
	MoveButton->Navigation->Next.Rule = EUINavigationRule::Custom;
	MoveButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::MoveItem);
	ExamineButton->Navigation->Next.Rule = EUINavigationRule::Custom;
	ExamineButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExamineItem);
	DiscardButton->Navigation->Next.Rule = EUINavigationRule::Custom;
	DiscardButton->Navigation->Next.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::DiscardItem);

	// Initialize the back button rules to always exit the menu regardless of what button we are focused on
	InteractButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
	InteractButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenu);
	CombineButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
	CombineButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenu);
	MoveButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
	MoveButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenu);
	ExamineButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
	ExamineButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenu);
	DiscardButton->Navigation->Previous.Rule = EUINavigationRule::Custom;
	DiscardButton->Navigation->Previous.CustomDelegate.BindDynamic(this, &UAbstractItemUseSubMenuWidget::ExitMenu);
	
	// Initialize all fixed up and down movement (namely, the three last buttons interactions)
	// Interact button's up always goes to the discard button, but it's down movement is not fixed
	InteractButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
	InteractButton->Navigation->Up.Widget = DiscardButton;
	InteractButton->Navigation->Up.WidgetToFocus = FName("DiscardButton");

	// Combine button always goes down to the move button, but it's up movement is not fixed
	CombineButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
	CombineButton->Navigation->Down.Widget = MoveButton;
	CombineButton->Navigation->Down.WidgetToFocus = FName("MoveButton");

	// The move button always goes down to the examine button, but it's up movement is not fixed
	MoveButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
	MoveButton->Navigation->Down.Widget = ExamineButton;
	MoveButton->Navigation->Down.WidgetToFocus = FName("ExamineButton");

	// The examine button always goes up to the move button and down to the discard button
	ExamineButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
	ExamineButton->Navigation->Up.Widget = MoveButton;
	ExamineButton->Navigation->Up.WidgetToFocus = FName("MoveButton");
	ExamineButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
	ExamineButton->Navigation->Down.Widget = DiscardButton;
	ExamineButton->Navigation->Down.WidgetToFocus = FName("DiscardButton");

	// The discard button always goes up to the examine button, but it's down movement is not fixed
	DiscardButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
	DiscardButton->Navigation->Up.Widget = ExamineButton;
	DiscardButton->Navigation->Up.WidgetToFocus = FName("ExamineButton");

	// Lastly, we need to build every button's navigation now that it has all been setup
	//     NOTE: Technically this is useless, as the setup system finishes and rebuilds the navigation anyways
	InteractButton->BuildNavigation();
	CombineButton->BuildNavigation();
	MoveButton->BuildNavigation();
	ExamineButton->BuildNavigation();
	DiscardButton->BuildNavigation();

	// Finally, return the parent generated return value
	return ReturnValue;
}


// 
void UAbstractItemUseSubMenuWidget::BeginDestroy() {

	// Call the parent functionality
	Super::BeginDestroy();

	// Remove all instances of this object to within the viewport resized event (for pointer security)
	FViewport::ViewportResizedEvent.RemoveAll(this);
}


// 
void UAbstractItemUseSubMenuWidget::UpdateViewportPosition(FViewport * ChangedViewport, uint32 Unknown) {

	// Only update this widget's position IFF it is visible by...
	if (this->GetVisibility() == ESlateVisibility::Visible) {

		// Locally store our newly wanted 2D screen position in local space based on the given attachment panel widget
		//USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), GivenAttachment->GetCachedWidget()->GetCachedGeometry().GetAbsolutePosition(), Pixels, Viewport);
		const FVector2D Viewport{GetWorld()->GetGameViewport()->GetGameLayerManager()->GetViewportWidgetHostGeometry().AbsoluteToLocal(CachedGivenAttachment->GetCachedWidget()->GetCachedGeometry().GetAbsolutePosition())};

		// Move the sub menu widget to the wanted 2D screen position now
		Cast<UCanvasPanelSlot>(ContainerBase->Slot)->SetPosition(Viewport);
	}
}


//
void UAbstractItemUseSubMenuWidget::Setup(const UPanelWidget * GivenAttachment, UAbstractInventoryItemWidget * GivenInventoryItem) {

	// Store the given inventory item as a weak object pointer to a constant (read-only) inventory item
	FocusedInventoryItem = GivenInventoryItem;

	// Sadly have to cache the pointer to the given attachment location in-case the viewport's resolution changes while this widget is still active
	CachedGivenAttachment = GivenAttachment;

	// Locally store whether the given item type is combinable or not
	const bool IsCombinable = FocusedInventoryItem->ItemData->IsCombinable();

	// Setup a function callback for if this menu is on screen and the viewport changes resolution, so that we move it to its newly accurate position
	FViewport::ViewportResizedEvent.AddUObject(this, &UAbstractItemUseSubMenuWidget::UpdateViewportPosition);
	
	// Locally store our newly wanted 2D screen position in local space based on the given attachment panel widget
	//USlateBlueprintLibrary::AbsoluteToViewport(GetWorld(), GivenAttachment->GetCachedWidget()->GetCachedGeometry().GetAbsolutePosition(), Pixels, Viewport);
	const FVector2D Viewport{GetWorld()->GetGameViewport()->GetGameLayerManager()->GetViewportWidgetHostGeometry().AbsoluteToLocal(GivenAttachment->GetCachedWidget()->GetCachedGeometry().GetAbsolutePosition())};

	// Move the sub menu widget to the wanted 2D screen position now
	Cast<UCanvasPanelSlot>(ContainerBase->Slot)->SetPosition(Viewport);

	// If this item is interactable, then display the interact item button, else hide it
	CombineButton->SetVisibility((FocusedInventoryItem->ItemData->ItemData.ItemUseType != EInventoryUseType::None) ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	
	// If this item can combine, then display the combine button, else hide it
	CombineButton->SetVisibility(IsCombinable ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);

	// If this item is interactable, then...
	if (FocusedInventoryItem->ItemData->ItemData.ItemUseType != EInventoryUseType::None) {

		// Set the interact button to be the focus, since it's the top of the list now
		InteractButton->SetFocus();

		// Ensure that the interact button text is updated, either via an overridden blueprint function or the native functionality
		this->UpdateInteractButtonText(FocusedInventoryItem->ItemData->ItemData.ItemUseType);

		// If this item is combinable, then...
		if (IsCombinable) {

			// 
			InteractButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			InteractButton->Navigation->Down.Widget = CombineButton;
			InteractButton->Navigation->Down.WidgetToFocus = FName("CombineButton");

			// 
			CombineButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			CombineButton->Navigation->Up.Widget = InteractButton;
			CombineButton->Navigation->Up.WidgetToFocus = FName("InteractButton");

			// 
			MoveButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			MoveButton->Navigation->Up.Widget = CombineButton;
			MoveButton->Navigation->Up.WidgetToFocus = FName("CombineButton");

			// 
			DiscardButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			DiscardButton->Navigation->Down.Widget = InteractButton;
			DiscardButton->Navigation->Down.WidgetToFocus = FName("InteractButton");
		}

		// Else, this item is not combinable, so...
		else {

			// 
			InteractButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			InteractButton->Navigation->Down.Widget = MoveButton;
			InteractButton->Navigation->Down.WidgetToFocus = FName("MoveButton");

			// 
			MoveButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			MoveButton->Navigation->Up.Widget = InteractButton;
			MoveButton->Navigation->Up.WidgetToFocus = FName("InteractButton");

			// 
			DiscardButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			DiscardButton->Navigation->Down.Widget = InteractButton;
			DiscardButton->Navigation->Down.WidgetToFocus = FName("InteractButton");
		}
	}

	// Else, if this item is NOT interactable, then...
	else {

		// If this item is combinable, then...
		if (IsCombinable) {

			// Set the combine button to be the focus, since it's the top of the list now
			CombineButton->SetFocus();

			// Set it so that the combine button's up wraps around to the bottom of the list, the discard button
			CombineButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			CombineButton->Navigation->Up.Widget = DiscardButton;
			CombineButton->Navigation->Up.WidgetToFocus = FName("DiscardButton");

			// Set the move button to go up to the combine button now
			MoveButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			MoveButton->Navigation->Up.Widget = CombineButton;
			MoveButton->Navigation->Up.WidgetToFocus = FName("CombineButton");

			// Set this discard button's down to wraps around to the top of the list, the combine button
			DiscardButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			DiscardButton->Navigation->Down.Widget = CombineButton;
			DiscardButton->Navigation->Down.WidgetToFocus = FName("CombineButton");
		}

		// Else, this item is not combinable, so...
		else {

			// Set the move button to be the focus, since it's the top of the list now
			MoveButton->SetFocus();

			// Set it so that the move button's up wraps around to the bottom of the list, the discard button
			MoveButton->Navigation->Up.Rule = EUINavigationRule::Explicit;
			MoveButton->Navigation->Up.Widget = DiscardButton;
			MoveButton->Navigation->Up.WidgetToFocus = FName("DiscardButton");

			// Set it so that the discard button's down wraps around to the top of the list, the move button
			DiscardButton->Navigation->Down.Rule = EUINavigationRule::Explicit;
			DiscardButton->Navigation->Down.Widget = MoveButton;
			DiscardButton->Navigation->Down.WidgetToFocus = FName("MoveButton");
		}
	}

	// Lastly, we need to rebuild every changed button's navigation now that it has all been setup
	InteractButton->BuildNavigation();
	CombineButton->BuildNavigation();
	MoveButton->BuildNavigation();
	ExamineButton->BuildNavigation();
	DiscardButton->BuildNavigation();

	//
	this->SetVisibility(ESlateVisibility::Visible);
}


// 
UWidget * UAbstractItemUseSubMenuWidget::InteractItem(EUINavigation GivenNavigation) {
	this->SetVisibility(ESlateVisibility::Collapsed);
	return FocusedInventoryItem->InteractItem();
}


// 
UWidget * UAbstractItemUseSubMenuWidget::MoveItem(EUINavigation GivenNavigation) {
	this->SetVisibility(ESlateVisibility::Collapsed);
	return FocusedInventoryItem->MoveItem();
}


// 
UWidget * UAbstractItemUseSubMenuWidget::CombineItem(EUINavigation GivenNavigation) {
	this->SetVisibility(ESlateVisibility::Collapsed);
	return FocusedInventoryItem->CombineItem();
}


// 
UWidget * UAbstractItemUseSubMenuWidget::ExamineItem(EUINavigation GivenNavigation) {

	// Create the newly wanted examine menu widget
	ExamineMenuToForceClose = WidgetTree->ConstructWidget<UAbstractExamineMenuWidget>(ExamineMenuClass.Get());

	//
	const FInternalInventoryItemData & CurrentItemData = FocusedInventoryItem->ItemData->ItemDataArray[FocusedInventoryItem->GetItemIndex()];

	//
	const int32 LocalOffsetY = (ExamineMenuToForceClose->GiveExtraInformation(FocusedInventoryItem->ItemData->ItemName, CurrentItemData.UpgradeBitmask, CurrentItemData.CurrentStackCount) ? -120 : 0);
	
	//
	ExamineMenuToForceClose->Setup(ExamineButton, FocusedInventoryItem->ItemData->ItemData.ExamineMesh.LoadSynchronous(), LocalOffsetY/*, FocusedInventoryItem->ItemData->ItemData.ExamineTargetRotations*/);

	//
	if (UCanvasPanelSlot * NewCanvasPanelSlot = Cast<UCanvasPanelSlot>(ContainerBase->GetParent()->AddChild(ExamineMenuToForceClose.Get()))) {

		//
		FAnchorData LocalAnchorData;
		LocalAnchorData.Anchors = FAnchors{0.0, 0.0, 1.0, 1.0};
		LocalAnchorData.Alignment = FVector2D::ZeroVector;
		LocalAnchorData.Offsets = FMargin{0.0, 0.0, 0.0, 0.0};

		//
		NewCanvasPanelSlot->SetLayout(LocalAnchorData);

		//
		NewCanvasPanelSlot->SetZOrder(10);
	}

	// 
	return ExamineMenuToForceClose->GetWidgetToFocus();
}


// 
UWidget * UAbstractItemUseSubMenuWidget::DiscardItem(EUINavigation GivenNavigation) {
	this->SetVisibility(ESlateVisibility::Collapsed);
	return FocusedInventoryItem->DiscardItem();
}


// 
UWidget * UAbstractItemUseSubMenuWidget::ExitMenu(EUINavigation GivenNavigation) {
	this->SetVisibility(ESlateVisibility::Collapsed);
	const TWeakObjectPtr<UAbstractInventoryItemWidget> LocalFocusWidget = FocusedInventoryItem;
	FocusedInventoryItem = nullptr;
	return (LocalFocusWidget.IsValid() ? LocalFocusWidget->GetDesiredFocusWidget() : nullptr);
}


// 
void UAbstractItemUseSubMenuWidget::ForceCloseSubMenu() {

	//
	if (ExamineMenuToForceClose.IsValid())
		ExamineMenuToForceClose->ForceClose();

	//
	if(UWidget * LocalWidget = ExitMenu(EUINavigation::Previous))
		LocalWidget->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::InteractItemButton() {
	InteractItem(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::MoveItemButton() {
	MoveItem(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::CombineItemButton() {
	CombineItem(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::ExamineItemButton() {
	ExamineItem(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::DiscardItemButton() {
	DiscardItem(EUINavigation::Next)->SetFocus();
}


// 
void UAbstractItemUseSubMenuWidget::ExitMenuButton() {
	ExitMenu(EUINavigation::Previous)->SetFocus();
}
