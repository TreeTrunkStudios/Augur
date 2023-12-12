// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include <InputMappingContext.h>
#include "CoreMinimal.h"
#include "InputAction.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "InputImageInputPreprocessor.h"
#include "InputImageSubsystem.generated.h"


// 
enum class EInputType : uint8 {
	KeyboardAndMouse,
	Xbox360Controller,
	XboxOneController,
	XboxSeriesController,
	NintendoSwitchController,
	NintendoSwitchProController,
	SteamDeckController,
	Playstation3Controller,
	Playstation4Controller,
	Playstation5Controller,
	AmazonLunaController,
	OuyaController,
	Num
};


// 
namespace ECustomKeys {

	static const FKey F13("F13");
	static const FKey F14("F14");
	static const FKey F15("F15");
	static const FKey F16("F16");
	static const FKey F17("F17");
	static const FKey F18("F18");
	static const FKey F19("F19");
	static const FKey F20("F20");
	static const FKey F21("F21");
	static const FKey F22("F22");
	static const FKey F23("F23");
	static const FKey F24("F24");

	static const FKey U_AccentGrave("U_AccentGrave");
}


//// TODO: Implement all console commands for settings system
// 
static TAutoConsoleVariable<int32> CVarGamepadInputTypeOverride(
	TEXT("Input.Image.GamepadTypeOverride"),
	-1,
	TEXT(""),
	ECVF_Set_NoSinkCall_Unsafe
);


//
static const TSoftObjectPtr<UTexture2D> InvalidInputImage{FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Invalid/RedX.RedX'"))};


// Create a constant static array of maps in order to convert 
static const TMap<FKey, TSoftObjectPtr<UTexture2D>> GlobalInputImageMapArray[static_cast<uint8>(EInputType::Num)] {

	// KeyboardAndMouse
	{
		{EKeys::MouseX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/X.X'")))},
		{EKeys::MouseY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/Y.Y'")))},
		{EKeys::Mouse2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/2D.2D'")))},

		{EKeys::MouseScrollUp, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/ScrollUp.ScrollUp'")))},
		{EKeys::MouseScrollDown, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/ScrollDown.ScrollDown'")))},
		{EKeys::MouseWheelAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/WheelAxis.WheelAxis'")))},

		{EKeys::LeftMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/LeftButton.LeftButton'")))},
		{EKeys::RightMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/RightButton.RightButton'")))},
		{EKeys::MiddleMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/MiddleButton.MiddleButton'")))},
		{EKeys::ThumbMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/ThumbButton.ThumbButton'")))},
		{EKeys::ThumbMouseButton2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Mouse/ThumbButton2.ThumbButton2'")))},

		{EKeys::BackSpace, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Backspace.Backspace'")))},
		{EKeys::Tab, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Tab.Tab'")))},
		{EKeys::Enter, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Enter.Enter'")))},
		{EKeys::Pause, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/PauseBreak.PauseBreak'")))},

		{EKeys::CapsLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/CapsLock.CapsLock'")))},
		{EKeys::Escape, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Escape.Escape'")))},
		{EKeys::SpaceBar, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Spacebar.Spacebar'")))},
		{EKeys::PageUp, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/PageUp.PageUp'")))},
		{EKeys::PageDown, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/PageDown.PageDown'")))},
		{EKeys::End, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/End.End'")))},
		{EKeys::Home, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Home.Home'")))},

		{EKeys::Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowLeft.ArrowLeft'")))},
		{EKeys::Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowUp.ArrowUp'")))},
		{EKeys::Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowRight.ArrowRight'")))},
		{EKeys::Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowDown.ArrowDown'")))},

		{EKeys::Insert, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Insert.Insert'")))},
		{EKeys::Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Delete.Delete'")))},

		{EKeys::Zero, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/0.0'")))},
		{EKeys::One, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/1.1'")))},
		{EKeys::Two, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/2.2'")))},
		{EKeys::Three, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/3.3'")))},
		{EKeys::Four, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/4.4'")))},
		{EKeys::Five, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/5.5'")))},
		{EKeys::Six, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/6.6'")))},
		{EKeys::Seven, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/7.7'")))},
		{EKeys::Eight, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/8.8'")))},
		{EKeys::Nine, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/9.9'")))},

		{EKeys::NumPadZero, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/0.0'")))},
		{EKeys::NumPadOne, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/1.1'")))},
		{EKeys::NumPadTwo, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/2.2'")))},
		{EKeys::NumPadThree, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/3.3'")))},
		{EKeys::NumPadFour, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/4.4'")))},
		{EKeys::NumPadFive, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/5.5'")))},
		{EKeys::NumPadSix, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/6.6'")))},
		{EKeys::NumPadSeven, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/7.7'")))},
		{EKeys::NumPadEight, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/8.8'")))},
		{EKeys::NumPadNine, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/9.9'")))},

		{EKeys::Multiply, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Asterix.Asterix'")))},
		{EKeys::Add, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Plus.Plus'")))},
		{EKeys::Subtract, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Minus.Minus'")))},
		{EKeys::Decimal, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Period.Period'")))},
		{EKeys::Divide, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashForward.SlashForward'")))},

		{EKeys::F1, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F1.F1'")))},
		{EKeys::F2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F2.F2'")))},
		{EKeys::F3, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F3.F3'")))},
		{EKeys::F4, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F4.F4'")))},
		{EKeys::F5, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F5.F5'")))},
		{EKeys::F6, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F6.F6'")))},
		{EKeys::F7, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F7.F7'")))},
		{EKeys::F8, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F8.F8'")))},
		{EKeys::F9, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F9.F9'")))},
		{EKeys::F10, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F10.F10'")))},
		{EKeys::F11, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F11.F11'")))},
		{EKeys::F12, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F12.F12'")))},

		{ECustomKeys::F13, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F13.F13'")))},
		{ECustomKeys::F14, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F14.F14'")))},
		{ECustomKeys::F15, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F15.F15'")))},
		{ECustomKeys::F16, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F16.F16'")))},
		{ECustomKeys::F17, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F17.F17'")))},
		{ECustomKeys::F18, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F18.F18'")))},
		{ECustomKeys::F19, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F19.F19'")))},
		{ECustomKeys::F20, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F20.F20'")))},
		{ECustomKeys::F21, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F21.F21'")))},
		{ECustomKeys::F22, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F22.F22'")))},
		{ECustomKeys::F23, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F23.F23'")))},
		{ECustomKeys::F24, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F24.F24'")))},

		{EKeys::NumLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/NumLock.NumLock'")))},
		{EKeys::ScrollLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ScrollLock.ScrollLock'")))},

		{EKeys::LeftShift, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ShiftLeft.ShiftLeft'")))},
		{EKeys::RightShift, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ShiftRight.ShiftRight'")))},
		{EKeys::LeftControl, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ControlLeft.ControlLeft'")))},
		{EKeys::RightControl, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ControlRight.ControlRight'")))},
		{EKeys::LeftAlt, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/AltLeft.AltLeft'")))},
		{EKeys::RightAlt, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/AltRight.AltRight'")))},

// If we are utilizing windows, then the command keys get a unique windows exclusive texture, else they stay as the default command texture
#if PLATFORM_WINDOWS
		{EKeys::LeftCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Windows.Windows'")))},
		{EKeys::RightCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Windows.Windows'")))},
#else
		{EKeys::LeftCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Command.Command'")))},
		{EKeys::RightCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Command.Command'")))},
#endif

		{EKeys::Semicolon, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/SemiColon.SemiColon'")))},
		{EKeys::Equals, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Equals.Equals'")))},
		{EKeys::Comma, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Comma.Comma'")))},
		{EKeys::Underscore, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Underscore.Underscore'")))},
		{EKeys::Hyphen, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Minus.Minus'")))},
		{EKeys::Period, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Period.Period'")))},
		{EKeys::Slash, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashForward.SlashForward'")))},
		{EKeys::Tilde, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Tilde.Tilde'")))},
		{EKeys::LeftBracket, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/BracketLeft.BracketLeft'")))},
		{EKeys::LeftParantheses, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ParenthesisLeft.ParenthesisLeft'")))},
		{EKeys::Backslash, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashBackward.SlashBackward'")))},
		{EKeys::RightBracket, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/BracketRight.BracketRight'")))},
		{EKeys::RightParantheses, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ParenthesisRight.ParenthesisRight'")))},
		{EKeys::Apostrophe, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Apostrophe.Apostrophe'")))},
		{EKeys::Quote, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/QuotationMarks.QuotationMarks'")))},

		{EKeys::Asterix, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Asterix.Asterix'")))},
		{EKeys::Ampersand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Ampersand.Ampersand'")))},
		{EKeys::Caret, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Caret.Caret'")))},
		{EKeys::Dollar, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/DollarSign.DollarSign'")))},
		{EKeys::Exclamation, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/ExclamationMark.ExclamationMark'")))},
		{EKeys::Colon, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Colon.Colon'")))},

		{EKeys::A_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveA.GraveA'")))},
		{EKeys::E_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveE.GraveE'")))},
		{ECustomKeys::U_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveU.GraveU'")))},
		{EKeys::E_AccentAigu, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Aigu.Aigu'")))},
		{EKeys::C_Cedille, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Cedilla.Cedilla'")))},
		{EKeys::Section, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Section.Section'")))},

#if PLATFORM_MAC
		{EKeys::Platform_Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Backspace.Backspace'")))},
#else
		{EKeys::Platform_Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Delete.Delete'")))},
#endif

		{EKeys::A, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/A.A'")))},
		{EKeys::B, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/B.B'")))},
		{EKeys::C, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/C.C'")))},
		{EKeys::D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/D.D'")))},
		{EKeys::E, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/E.E'")))},
		{EKeys::F, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/F.F'")))},
		{EKeys::G, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/G.G'")))},
		{EKeys::H, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/H.H'")))},
		{EKeys::I, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/I.I'")))},
		{EKeys::J, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/J.J'")))},
		{EKeys::K, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/K.K'")))},
		{EKeys::L, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/L.L'")))},
		{EKeys::M, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/M.M'")))},
		{EKeys::N, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/N.N'")))},
		{EKeys::O, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/O.O'")))},
		{EKeys::P, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/P.P'")))},
		{EKeys::Q, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Q.Q'")))},
		{EKeys::R, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/R.R'")))},
		{EKeys::S, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/S.S'")))},
		{EKeys::T, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/T.T'")))},
		{EKeys::U, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/U.U'")))},
		{EKeys::V, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/V.V'")))},
		{EKeys::W, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/W.W'")))},
		{EKeys::X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/X.X'")))},
		{EKeys::Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Y.Y'")))},
		{EKeys::Z, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Keyboard/Z.z'")))}
	},

	// Xbox360Controller
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadLeft.DPadLeft'")))}
	},

	// XboxOneController
	{
		//{EKeys::Global_Menu, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_View, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Pause, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Play, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Back, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadLeft.DPadLeft'")))}
	},

	// XboxSeriesController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadLeft.DPadLeft'")))}
	},

	// NintendoSwitchController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/DpadLeft.DPadLeft'")))}
	},

	// NintendoSwitchProController (uses the same as the nintendo switch controller, but with a different d-pad)
	{
{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadLeft.DPadLeft'")))}
	},

	// SteamDeckController
	{
		{EKeys::Steam_Touch_0, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftExtraUpper.LeftExtraUpper'")))},
		{EKeys::Steam_Touch_1, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftExtraLower.LeftExtraLower'")))},
		{EKeys::Steam_Touch_2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightExtraUpper.RightExtraUpper'")))},
		{EKeys::Steam_Touch_3, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightExtraLower.RightExtraLower'")))},

		{EKeys::Steam_Back_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/TouchPadLeft.TouchPadLeft'")))},
		{EKeys::Steam_Back_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/TouchPadRight.TouchPadRight'")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Steam/DpadLeft.DPadLeft'")))}
	},

	// Playstation3Controller
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS3/DpadLeft.DPadLeft'")))}
	},

	// Playstation4Controller
	{
		{EKeys::Gamepad_Special_Left_X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/TouchPad.TouchPad'")))},
		{EKeys::Gamepad_Special_Left_Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/TouchPad.TouchPad'")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS4/DpadLeft.DPadLeft'")))}
	},

	// Playstation5Controller
	{
		{EKeys::Gamepad_Special_Left_X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/TouchPad.TouchPad'")))},
		{EKeys::Gamepad_Special_Left_Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/TouchPad.TouchPad'")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/PS5/DpadLeft.DPadLeft'")))}
	},

	// AmazonLunaController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Luna/DpadLeft.DPadLeft'")))}
	},

	// OuyaController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/Left2D.Left2D'")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftX.LeftX'")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftY.LeftY'")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/Right2D.Right2D'")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightX.RightX'")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightY.RightY'")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightTrigger.RightTrigger'")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftStick.LeftStick'")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightStick.RightStick'")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftSpecialButton.LeftSpecialButton'")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightSpecialButton.RightSpecialButton'")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonBottom.FaceButtonBottom'")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonRight.FaceButtonRight'")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonLeft.FaceButtonLeft'")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonTop.FaceButtonTop'")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftBumper.LeftBumper'")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightBumper.RightBumper'")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/LeftTrigger.LeftTrigger'")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/RightTrigger.RightTrigger'")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/DpadUp.DPadUp'")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/DpadDown.DPadDown'")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/DpadRight.DPadRight'")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Script/Engine.Texture2D'/Game/UserInterfaces/ButtonPrompts/Ouya/DpadLeft.DPadLeft'")))}
	}
};


//
DECLARE_MULTICAST_DELEGATE_OneParam(FOnControllerTypeChanged, UInputImageSubsystem*);


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class INPUTIMAGESUBSYSTEM_API UInputImageSubsystem final : public ULocalPlayerSubsystem {
	GENERATED_BODY()

//
public:

	//
	FOnControllerTypeChanged OnControllerTypeChanged;


// 
public:

	// 
	TObjectPtr<UTexture2D> GetFirstInputImage(const UInputMappingContext * GivenMapping, const UInputAction * GivenAction) const;

	// 
	TArray<TObjectPtr<UTexture2D>> GetAllInputImages(const UInputMappingContext * GivenMapping, const UInputAction * GivenAction) const;

	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	virtual void Deinitialize() override;

	//
	void SetCurrentInputType(EBaseInputTypes GivenInputType);

	//
	FName GetCurrentGamepadName() const;

	//
	void SetGamepadInputType(FName GivenInputTypeName);


// 
protected:

	// 
	EBaseInputTypes CurrentInputType;

	//
	FName GamepadInputType = NAME_None;

	//
	TSharedPtr<FInputImageInputPreprocessor> InputImageInputPreprocessor;

	//
	FTSTicker::FDelegateHandle TickHandle;

	/** Used to broadcast when input method should be considered changed based on external systems, such as on enhanced input mapping context application*/
	FScriptDelegate BroadcastInputMethodChangedEvent;


//
protected:

	//
	UFUNCTION()
	void BroadcastInputMethodChanged();

	//
	bool Tick(float DeltaTime);

	// 
	uint8 CollectTargetInputType(const int32 & GivenInputTypeOverride) const;
};
