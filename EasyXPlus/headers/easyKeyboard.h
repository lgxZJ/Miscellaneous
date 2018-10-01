///
///	@file		Keyboard module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/13
///	@version	1.0
///

#ifndef EASY_KEYBOARD_H
#define EASY_KEYBOARD_H

#include <windows.h>

namespace EasyXPlus
{
	///
	///	@brief	Key enumerations.
	///
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

	///
	///	@brief	A class for keyboard support.
	///
	class Keyboard
	{
	public:
		///
		///	@brief	Check if the given key is being pressed at present.
		///
		static bool isPressing(Key key);
		///
		///	@brief	Try get the enumeration of the pressed key.
		///	@remark	If no key is pressed, it returns Key::KeyNone.
		///
		static Key tryGetPressed();

		static int convertToVirtualKey(Key key);
		static Key convertToKeyEnum(int virtualKey);
	};
}

#endif