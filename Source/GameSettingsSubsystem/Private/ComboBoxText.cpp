// Copyright Epic Games, Inc. All Rights Reserved.

#include "ComboBoxText.h"

#include "Widgets/SNullWidget.h"
#include "UObject/EditorObjectVersion.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/Font.h"
#include "Styling/DefaultStyleCache.h"
#include "Styling/UMGCoreStyle.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ComboBoxText)

/////////////////////////////////////////////////////
// UComboBoxText

UComboBoxText::UComboBoxText(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetComboBoxStyle();
	ItemStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetTableRowStyle();
	ScrollBarStyle = UE::Slate::Private::FDefaultStyleCache::GetRuntime().GetScrollBarStyle();

#if WITH_EDITOR 
	if (IsEditorWidget())
	{
		WidgetStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetComboBoxStyle();
		ItemStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetTableRowStyle();
		ScrollBarStyle = UE::Slate::Private::FDefaultStyleCache::GetEditor().GetScrollBarStyle();

		// The CDO isn't an editor widget and thus won't use the editor style, call post edit change to mark difference from CDO
		PostEditChange();
	}
#endif
	
	ForegroundColor = ItemStyle.TextColor;
	bIsFocusable = true;

	ContentPadding = FMargin(4.0, 2.0);
	MaxListHeight = 450.0f;
	HasDownArrow = true;
	EnableGamepadNavigationMode = true;
	// We don't want to try and load fonts on the server.
	if ( !IsRunningDedicatedServer() )
	{
		static ConstructorHelpers::FObjectFinder<UFont> RobotoFontObj(*UWidget::GetDefaultFontName());
		Font = FSlateFontInfo(RobotoFontObj.Object, 16, FName("Bold"));
	}
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UComboBoxText::PostInitProperties()
{
	Super::PostInitProperties();

	// Initialize the set of options from the default set only once.
	for (const FText & DefaultOption : DefaultOptions)
	{
		Options.Emplace(MakeShareable(new FText(DefaultOption)));
	}

	//
	RefreshOptions();
}

void UComboBoxText::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyComboBox.Reset();
	ComboBoxContent.Reset();
}

void UComboBoxText::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar.UsingCustomVersion(FEditorObjectVersion::GUID);
}

void UComboBoxText::PostLoad()
{
	Super::PostLoad();

	// Initialize the set of options from the default set only once.
	for (const FText & DefaultOption : DefaultOptions)
	{
		Options.Emplace(MakeShareable(new FText(DefaultOption)));
	}

	//
	RefreshOptions();

	if (GetLinkerCustomVersion(FEditorObjectVersion::GUID) < FEditorObjectVersion::ComboBoxControllerSupportUpdate)
	{
PRAGMA_DISABLE_DEPRECATION_WARNINGS
		EnableGamepadNavigationMode = false;
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	}
}

TSharedRef<SWidget> UComboBoxText::RebuildWidget()
{
	//
	if (Options.IsValidIndex(SelectedIndex) == false)
		SelectedIndex = 0;

	// 
	if (Options.IsValidIndex(SelectedIndex))
		CurrentOptionPtr = Options[SelectedIndex];
	
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	MyComboBox =
		SNew(SComboBox< TSharedPtr<FText> >)
		.ComboBoxStyle(&WidgetStyle)
		.ItemStyle(&ItemStyle)
		.ForegroundColor(ForegroundColor)
		.OptionsSource(&Options)
		.InitiallySelectedItem(CurrentOptionPtr)
		.ContentPadding(ContentPadding)
		.MaxListHeight(MaxListHeight)
		.HasDownArrow(HasDownArrow)
		.EnableGamepadNavigationMode(EnableGamepadNavigationMode)
		.OnGenerateWidget(BIND_UOBJECT_DELEGATE(SComboBox< TSharedPtr<FText> >::FOnGenerateWidget, HandleGenerateWidget))
		.OnSelectionChanged(BIND_UOBJECT_DELEGATE(SComboBox< TSharedPtr<FText> >::FOnSelectionChanged, HandleSelectionChanged))
		.OnComboBoxOpening(BIND_UOBJECT_DELEGATE(FOnComboBoxOpening, HandleOpening))
	    .ScrollBarStyle(&ScrollBarStyle)
		.IsFocusable(bIsFocusable)
		[
			SAssignNew(ComboBoxContent, SBox)
		];
PRAGMA_ENABLE_DEPRECATION_WARNINGS
	if (Options.IsValidIndex(SelectedIndex))
	{
		// Generate the widget for the initially selected widget if needed
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}

	return MyComboBox.ToSharedRef();
}

void UComboBoxText::AddOption(const FText& Option)
{
	Options.Emplace(MakeShareable(new FText(Option)));

	RefreshOptions();
}

bool UComboBoxText::RemoveOption(const FText & Option)
{
	if (const int32 OptionIndex = FindOptionIndex(Option); OptionIndex >= 0)
	{
		if (OptionIndex == SelectedIndex)
		{
			ClearSelection();
		}

		Options.RemoveAt(OptionIndex);

		SelectedIndex = FindOptionIndex(Option);

		RefreshOptions();

		return true;
	}

	return false;
}

int32 UComboBoxText::FindOptionIndex(const FText & Option) const
{
	for (int32 OptionIndex = 0; OptionIndex < Options.Num(); ++OptionIndex)
	{
		if (Options[OptionIndex]->EqualTo(Option))
		{
			return OptionIndex;
		}
	}

	return -1;
}

FText UComboBoxText::GetOptionAtIndex(int32 Index) const
{
	return (Options.IsValidIndex(Index) ? *(Options[Index]) : FText::GetEmpty());
}

void UComboBoxText::ClearOptions()
{
	ClearSelection();

	Options.Empty();

	if ( MyComboBox.IsValid() )
	{
		MyComboBox->RefreshOptions();
	}
}

void UComboBoxText::ClearSelection()
{
	CurrentOptionPtr.Reset();
	SelectedIndex = -1;

	if ( MyComboBox.IsValid() )
	{
		MyComboBox->ClearSelection();
	}

	if ( ComboBoxContent.IsValid() )
	{
		ComboBoxContent->SetContent(SNullWidget::NullWidget);
	}
}

void UComboBoxText::RefreshOptions()
{
	if ( MyComboBox.IsValid() )
	{
		MyComboBox->RefreshOptions();
	}
}

void UComboBoxText::SetSelectedOption(FText Option)
{
	SetSelectedIndex(FindOptionIndex(Option));
}

void UComboBoxText::SetSelectedIndex(const int32 Index)
{
	if (Options.IsValidIndex(Index))
	{
		CurrentOptionPtr = Options[Index];
		// Don't select item if its already selected
		if (SelectedIndex != Index)
		{
			BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedIndex);
			SelectedIndex = Index;

			if (ComboBoxContent.IsValid())
			{
				MyComboBox->SetSelectedItem(CurrentOptionPtr);
				UpdateOrGenerateWidget(CurrentOptionPtr);
			}		
			else
			{
				HandleSelectionChanged(CurrentOptionPtr, ESelectInfo::Direct);
			}
		}
	}
}
FText UComboBoxText::GetSelectedOption() const
{
	return (CurrentOptionPtr.IsValid() ? *CurrentOptionPtr : FText::GetEmpty());
}

int32 UComboBoxText::GetSelectedIndex() const
{
	return SelectedIndex;
}

int32 UComboBoxText::GetOptionCount() const
{
	return Options.Num();
}

bool UComboBoxText::IsOpen() const
{
	return MyComboBox.IsValid() && MyComboBox->IsOpen();
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
void  UComboBoxText::SetContentPadding(FMargin InPadding)
{
	ContentPadding = InPadding;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetButtonContentPadding(InPadding);
	}
}

FMargin  UComboBoxText::GetContentPadding() const
{
	return ContentPadding;
}

void UComboBoxText::SetEnableGamepadNavigationMode(bool InEnableGamepadNavigationMode)
{
	EnableGamepadNavigationMode = InEnableGamepadNavigationMode;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetEnableGamepadNavigationMode(EnableGamepadNavigationMode);
	}
}

bool UComboBoxText::IsEnableGamepadNavigationMode() const
{
	return EnableGamepadNavigationMode;
}

bool UComboBoxText::IsHasDownArrow() const
{
	return HasDownArrow;
}

void UComboBoxText::SetHasDownArrow(bool InHasDownArrow)
{
	HasDownArrow = InHasDownArrow;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetHasDownArrow(HasDownArrow);
	}
}

float UComboBoxText::GetMaxListHeight() const
{
	return MaxListHeight;
}

void UComboBoxText::SetMaxListHeight(float InMaxHeight)
{
	MaxListHeight = InMaxHeight;
	if (MyComboBox.IsValid())
	{
		MyComboBox->SetMaxHeight(MaxListHeight);
	}
}

const FSlateFontInfo&  UComboBoxText::GetFont() const
{
	return Font;
}

const FComboBoxStyle& UComboBoxText::GetWidgetStyle() const
{
	return WidgetStyle;
}

const FTableRowStyle& UComboBoxText::GetItemStyle() const
{
	return ItemStyle;
}

const FScrollBarStyle& UComboBoxText::GetScrollBarStyle() const
{
	return ScrollBarStyle;
}

bool UComboBoxText::IsFocusable() const
{
	return bIsFocusable;
}

FSlateColor UComboBoxText::GetForegroundColor() const
{
	return ForegroundColor;
}

void UComboBoxText::SetWidgetStyle(const FComboBoxStyle& InWidgetStyle)
{
	WidgetStyle = InWidgetStyle;
	if (MyComboBox.IsValid())
	{
		MyComboBox->InvalidateStyle();
	}
}

void UComboBoxText::SetItemStyle(const FTableRowStyle& InItemStyle)
{
	ItemStyle = InItemStyle;
	if (MyComboBox.IsValid())
	{
		MyComboBox->InvalidateItemStyle();
	}
}

void UComboBoxText::InitScrollBarStyle(const FScrollBarStyle& InScrollBarStyle)
{
	ensureMsgf(!MyComboBox.IsValid(), TEXT("The widget is already created."));
	ScrollBarStyle = InScrollBarStyle;
}

void UComboBoxText::InitFont(FSlateFontInfo InFont)
{
	ensureMsgf(!MyComboBox.IsValid(), TEXT("The widget is already created."));
	Font = InFont;
}

void UComboBoxText::InitIsFocusable(bool InIsFocusable)
{
	ensureMsgf(!MyComboBox.IsValid(), TEXT("The widget is already created."));
	bIsFocusable = InIsFocusable;
}

void UComboBoxText::InitForegroundColor(FSlateColor InForegroundColor)
{
	ensureMsgf(!MyComboBox.IsValid(), TEXT("The widget is already created."));
	ForegroundColor = InForegroundColor;
}

PRAGMA_ENABLE_DEPRECATION_WARNINGS

void UComboBoxText::UpdateOrGenerateWidget(TSharedPtr<FText> Item)
{
	// If no custom widget was supplied and the default STextBlock already exists,
	// just update its text instead of rebuilding the widget.
	if (DefaultComboBoxContent.IsValid() && (IsDesignTime() || OnGenerateWidgetEvent.IsBound()))
	{
		DefaultComboBoxContent.Pin()->SetText(Item.IsValid() ? *Item : FText::GetEmpty());
	}
	else
	{
		DefaultComboBoxContent.Reset();
		ComboBoxContent->SetContent(HandleGenerateWidget(Item));
	}
}

TSharedRef<SWidget> UComboBoxText::HandleGenerateWidget(TSharedPtr<FText> Item) const
{
	const FText & TextItem = Item.IsValid() ? *Item : FText::GetEmpty();

	// Call the user's delegate to see if they want to generate a custom widget bound to the data source.
	if ( !IsDesignTime() && OnGenerateWidgetEvent.IsBound() )
	{
		if (UWidget * Widget = OnGenerateWidgetEvent.Execute(TextItem))
		{
			return Widget->TakeWidget();
		}
	}

	// If a row wasn't generated just create the default one, a simple text block of the item's name.
PRAGMA_DISABLE_DEPRECATION_WARNINGS
	return SNew(STextBlock)
		.Text(TextItem)
		.Font(Font);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
}

void UComboBoxText::HandleSelectionChanged(TSharedPtr<FText> Item, ESelectInfo::Type SelectionType)
{
	CurrentOptionPtr = Item;
	BroadcastFieldValueChanged(FFieldNotificationClassDescriptor::SelectedIndex);

	const FText & TextItem = Item.IsValid() ? *Item : FText::GetEmpty();
	SelectedIndex = FindOptionIndex(TextItem);

	// When the selection changes we always generate another widget to represent the content area of the combobox.
	if ( ComboBoxContent.IsValid() )
	{
		UpdateOrGenerateWidget(CurrentOptionPtr);
	}

	if ( !IsDesignTime() )
	{
		OnSelectionChanged.Broadcast(TextItem, SelectionType);
	}
}

void UComboBoxText::HandleOpening()
{
	OnOpening.Broadcast();
}

#if WITH_EDITOR

const FText UComboBoxText::GetPaletteCategory()
{
	return NSLOCTEXT("UMG", "Input", "Input");
}

#endif

/////////////////////////////////////////////////////
