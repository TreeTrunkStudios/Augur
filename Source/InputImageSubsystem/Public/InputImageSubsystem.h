// TreeTrunkStudios (c) 2023


// 
#pragma once


// 
#include "CoreMinimal.h"
#include "InputAction.h"
#include "CommonInputSubsystem.h"
#include "Subsystems/LocalPlayerSubsystem.h"
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
struct ECustomKeys {

	static INPUTIMAGESUBSYSTEM_API const FKey F13;
	static INPUTIMAGESUBSYSTEM_API const FKey F14;
	static INPUTIMAGESUBSYSTEM_API const FKey F15;
	static INPUTIMAGESUBSYSTEM_API const FKey F16;
	static INPUTIMAGESUBSYSTEM_API const FKey F17;
	static INPUTIMAGESUBSYSTEM_API const FKey F18;
	static INPUTIMAGESUBSYSTEM_API const FKey F19;
	static INPUTIMAGESUBSYSTEM_API const FKey F20;
	static INPUTIMAGESUBSYSTEM_API const FKey F21;
	static INPUTIMAGESUBSYSTEM_API const FKey F22;
	static INPUTIMAGESUBSYSTEM_API const FKey F23;
	static INPUTIMAGESUBSYSTEM_API const FKey F24;
	
	static INPUTIMAGESUBSYSTEM_API const FKey U_AccentGrave;
};


// Create a constant static array of maps in order to convert 
static const TMap<FKey, TSoftObjectPtr<UTexture2D>> GlobalInputImageMapArray[static_cast<uint8>(EInputType::Num)] {

	// KeyboardAndMouse
	{
		{EKeys::MouseX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/X")))},
		{EKeys::MouseY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/Y")))},
		{EKeys::Mouse2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/2D")))},

		{EKeys::MouseScrollUp, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/ScrollUp")))},
		{EKeys::MouseScrollDown, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/ScrollDown")))},
		{EKeys::MouseWheelAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/WheelAxis")))},

		{EKeys::LeftMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/LeftButton")))},
		{EKeys::RightMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/RightButton")))},
		{EKeys::MiddleMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/MiddleButton")))},
		{EKeys::ThumbMouseButton, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/ThumbButton")))},
		{EKeys::ThumbMouseButton2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Mouse/ThumbButton2")))},

		{EKeys::BackSpace, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Backspace")))},
		{EKeys::Tab, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Tab")))},
		{EKeys::Enter, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Enter")))},
		{EKeys::Pause, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/PauseBreak")))},

		{EKeys::CapsLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/CapsLock")))},
		{EKeys::Escape, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Escape")))},
		{EKeys::SpaceBar, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Spacebar")))},
		{EKeys::PageUp, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/PageUp")))},
		{EKeys::PageDown, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/PageDown")))},
		{EKeys::End, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/End")))},
		{EKeys::Home, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Home")))},

		{EKeys::Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowLeft")))},
		{EKeys::Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowUp")))},
		{EKeys::Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowRight")))},
		{EKeys::Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ArrowDown")))},

		{EKeys::Insert, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Insert")))},
		{EKeys::Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Delete")))},

		{EKeys::Zero, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/0")))},
		{EKeys::One, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/1")))},
		{EKeys::Two, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/2")))},
		{EKeys::Three, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/3")))},
		{EKeys::Four, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/4")))},
		{EKeys::Five, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/5")))},
		{EKeys::Six, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/6")))},
		{EKeys::Seven, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/7")))},
		{EKeys::Eight, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/8")))},
		{EKeys::Nine, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/9")))},

		{EKeys::NumPadZero, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/0")))},
		{EKeys::NumPadOne, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/1")))},
		{EKeys::NumPadTwo, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/2")))},
		{EKeys::NumPadThree, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/3")))},
		{EKeys::NumPadFour, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/4")))},
		{EKeys::NumPadFive, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/5")))},
		{EKeys::NumPadSix, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/6")))},
		{EKeys::NumPadSeven, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/7")))},
		{EKeys::NumPadEight, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/8")))},
		{EKeys::NumPadNine, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/9")))},

		{EKeys::Multiply, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Asterix")))},
		{EKeys::Add, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Plus")))},
		{EKeys::Subtract, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Minus")))},
		{EKeys::Decimal, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Period")))},
		{EKeys::Divide, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashForward")))},

		{EKeys::F1, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F1")))},
		{EKeys::F2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F2")))},
		{EKeys::F3, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F3")))},
		{EKeys::F4, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F4")))},
		{EKeys::F5, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F5")))},
		{EKeys::F6, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F6")))},
		{EKeys::F7, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F7")))},
		{EKeys::F8, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F8")))},
		{EKeys::F9, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F9")))},
		{EKeys::F10, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F10")))},
		{EKeys::F11, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F11")))},
		{EKeys::F12, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F12")))},

		{ECustomKeys::F13, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F13")))},
		{ECustomKeys::F14, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F14")))},
		{ECustomKeys::F15, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F15")))},
		{ECustomKeys::F16, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F16")))},
		{ECustomKeys::F17, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F17")))},
		{ECustomKeys::F18, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F18")))},
		{ECustomKeys::F19, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F19")))},
		{ECustomKeys::F20, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F20")))},
		{ECustomKeys::F21, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F21")))},
		{ECustomKeys::F22, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F22")))},
		{ECustomKeys::F23, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F23")))},
		{ECustomKeys::F24, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F24")))},

		{EKeys::NumLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/NumLock")))},
		{EKeys::ScrollLock, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ScrollLock")))},

		{EKeys::LeftShift, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ShiftLeft")))},
		{EKeys::RightShift, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ShiftRight")))},
		{EKeys::LeftControl, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ControlLeft")))},
		{EKeys::RightControl, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ControlRight")))},
		{EKeys::LeftAlt, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/AltLeft")))},
		{EKeys::RightAlt, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/AltRight")))},

// If we are utilizing windows, then the command keys get a unique windows exclusive texture, else they stay as the default command texture
#if PLATFORM_WINDOWS
		{EKeys::LeftCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Windows")))},
		{EKeys::RightCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Windows")))},
#else
		{EKeys::LeftCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Command")))},
		{EKeys::RightCommand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Command")))},
#endif

		{EKeys::Semicolon, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/SemiColon")))},
		{EKeys::Equals, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Equals")))},
		{EKeys::Comma, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Comma")))},
		{EKeys::Underscore, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Underscore")))},
		{EKeys::Hyphen, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Minus")))},
		{EKeys::Period, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Period")))},
		{EKeys::Slash, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashForward")))},
		{EKeys::Tilde, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Tilde")))},
		{EKeys::LeftBracket, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/BracketLeft")))},
		{EKeys::LeftParantheses, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ParenthesisLeft")))},
		{EKeys::Backslash, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/SlashBackward")))},
		{EKeys::RightBracket, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/BracketRight")))},
		{EKeys::RightParantheses, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ParenthesisRight")))},
		{EKeys::Apostrophe, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Apostrophe")))},
		{EKeys::Quote, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/QuotationMarks")))},

		{EKeys::Asterix, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Asterix")))},
		{EKeys::Ampersand, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Ampersand")))},
		{EKeys::Caret, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Caret")))},
		{EKeys::Dollar, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/DollarSign")))},
		{EKeys::Exclamation, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/ExclamationMark")))},
		{EKeys::Colon, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Colon")))},

		{EKeys::A_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveA")))},
		{EKeys::E_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveE")))},
		{ECustomKeys::U_AccentGrave, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/GraveU")))},
		{EKeys::E_AccentAigu, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Aigu")))},
		{EKeys::C_Cedille, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Cedilla")))},
		{EKeys::Section, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Section")))},

#if PLATFORM_MAC
		{EKeys::Platform_Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Backspace")))},
#else
		{EKeys::Platform_Delete, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Delete")))},
#endif

		{EKeys::A, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/A")))},
		{EKeys::B, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/B")))},
		{EKeys::C, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/C")))},
		{EKeys::D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/D")))},
		{EKeys::E, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/E")))},
		{EKeys::F, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/F")))},
		{EKeys::G, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/G")))},
		{EKeys::H, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/H")))},
		{EKeys::I, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/I")))},
		{EKeys::J, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/J")))},
		{EKeys::K, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/K")))},
		{EKeys::L, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/L")))},
		{EKeys::M, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/M")))},
		{EKeys::N, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/N")))},
		{EKeys::O, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/O")))},
		{EKeys::P, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/P")))},
		{EKeys::Q, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Q")))},
		{EKeys::R, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/R")))},
		{EKeys::S, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/S")))},
		{EKeys::T, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/T")))},
		{EKeys::U, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/U")))},
		{EKeys::V, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/V")))},
		{EKeys::W, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/W")))},
		{EKeys::X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/X")))},
		{EKeys::Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Y")))},
		{EKeys::Z, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Keyboard/Z")))}
	},

	// Xbox360Controller
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Xbox360/DpadLeft")))}
	},

	// XboxOneController
	{
		//{EKeys::Global_Menu, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_View, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Pause, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Play, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		//{EKeys::Global_Back, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxOne/DpadLeft")))}
	},

	// XboxSeriesController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/XboxSeries/DpadLeft")))}
	},

	// NintendoSwitchController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/DpadLeft")))}
	},

	// NintendoSwitchProController (uses the same as the nintendo switch controller, but with a different dpad)
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Switch/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/SwitchPro/DpadLeft")))}
	},

	// SteamDeckController
	{
		{EKeys::Steam_Touch_0, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftExtraUpper")))},
		{EKeys::Steam_Touch_1, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftExtraLower")))},
		{EKeys::Steam_Touch_2, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightExtraUpper")))},
		{EKeys::Steam_Touch_3, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightExtraLower")))},

		{EKeys::Steam_Back_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/TouchPadLeft")))},
		{EKeys::Steam_Back_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/TouchPadRight")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Steam/DpadLeft")))}
	},

	// Playstation3Controller
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS3/DpadLeft")))}
	},

	// Playstation4Controller
	{
		{EKeys::Gamepad_Special_Left_X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/TouchPad")))},
		{EKeys::Gamepad_Special_Left_Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/TouchPad")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS4/DpadLeft")))}
	},

	// Playstation5Controller
	{
		{EKeys::Gamepad_Special_Left_X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/TouchPad")))},
		{EKeys::Gamepad_Special_Left_Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/TouchPad")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/PS5/DpadLeft")))}
	},

	// AmazonLunaController
	{
		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Luna/DpadLeft")))}
	},

	// OuyaController
	{
		{EKeys::Gamepad_Special_Left_X, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/TouchPad")))},
		{EKeys::Gamepad_Special_Left_Y, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/TouchPad")))},

		{EKeys::Gamepad_Left2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/Left2D")))},
		{EKeys::Gamepad_LeftX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftX")))},
		{EKeys::Gamepad_LeftY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftY")))},
		{EKeys::Gamepad_Right2D, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/Right2D")))},
		{EKeys::Gamepad_RightX, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightX")))},
		{EKeys::Gamepad_RightY, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightY")))},

		{EKeys::Gamepad_LeftTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftTrigger")))},
		{EKeys::Gamepad_RightTriggerAxis, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightTrigger")))},

		{EKeys::Gamepad_LeftThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftStick")))},
		{EKeys::Gamepad_RightThumbstick, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightStick")))},
		{EKeys::Gamepad_Special_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftSpecialButton")))},
		{EKeys::Gamepad_Special_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightSpecialButton")))},
		{EKeys::Gamepad_FaceButton_Bottom, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonBottom")))},
		{EKeys::Gamepad_FaceButton_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonRight")))},
		{EKeys::Gamepad_FaceButton_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonLeft")))},
		{EKeys::Gamepad_FaceButton_Top, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/FaceButtonTop")))},
		{EKeys::Gamepad_LeftShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftBumper")))},
		{EKeys::Gamepad_RightShoulder, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightBumper")))},
		{EKeys::Gamepad_LeftTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/LeftTrigger")))},
		{EKeys::Gamepad_RightTrigger, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/RightTrigger")))},
		{EKeys::Gamepad_DPad_Up, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/DpadUp")))},
		{EKeys::Gamepad_DPad_Down, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/DpadDown")))},
		{EKeys::Gamepad_DPad_Right, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/DpadRight")))},
		{EKeys::Gamepad_DPad_Left, TSoftObjectPtr<UTexture2D>(FSoftObjectPath(TEXT("/Game/UserInterfaces/ButtonPrompts/Ouya/DpadLeft")))}
	}
};


// Subsystem which handles all game settings, both in setting and getting them, in a simplistic, thread-safe singleton way
UCLASS()
class INPUTIMAGESUBSYSTEM_API UInputImageSubsystem final : public ULocalPlayerSubsystem {
	GENERATED_BODY()


// 
public:

	// 
	TObjectPtr<class UTexture2D> GetFirstInputImage(UInputAction * GivenAction);

	// 
	TArray<TObjectPtr<class UTexture2D>> GetAllInputImages(UInputAction * GivenAction);

	
//
public:

	// Function which collects all saved settings information and defaults what could not be found
	virtual void Initialize(FSubsystemCollectionBase & Collection) override;

	// Function which forcibly saves all save setting information prior to destruction
	//virtual void Deinitialize() override;


// 
protected:

	// 
	const uint8 CollectTargetInputType(const int32 && GivenInputTypeOverride, const ECommonInputType & CurrentInputType) const;
};
