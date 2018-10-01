#ifndef EASY_KEYBOARD_H
#define EASY_KEYBOARD_H

#include <windows.h>

namespace EasyXPlus
{
	enum class Key
	{
		KeyNone, KeyNotSupport,

		KeyA, KeyB, KeyC, KeyD, KeyE, KeyF, KeyG, KeyH, KeyI,
		KeyJ, KeyK, KeyL, KeyM, KeyN, KeyO, KeyP, KeyQ, KeyR,
		KeyS, KeyT, KeyU, KeyV, KeyW, KeyX, KeyY, KeyZ,

		Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8,
		Key9,

		KeyShift, KeyCtrl,
		KeyUpArrow, KeyDownArrow, KeyLeftArrow, KeyRightArrow,
		KeyEsc, KeyTab, KeySpace, KeyEnter, KeyBackspace
	};

	class Keyboard
	{
	public:
		static bool isPressing(Key key);
		static Key tryGetPressed();

		static int convertToVirtualKey(Key key);
		static Key convertToKeyEnum(int virtualKey);
	};
}

#endif