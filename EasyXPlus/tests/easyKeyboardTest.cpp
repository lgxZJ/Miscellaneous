#include "..\src\easyKeyboard.h"
#include "..\src\easyWindow.h"
#include "..\src\easyBaseWindow.h"
#include "..\src\easyExcept.h"

#include <cassert>

#include "SU.h"

using namespace EasyXPlus;

///////////////////////////////////////////////////////////////////////////////
//									NOTE:
//		While running these tests, please do not press any key in your keyboard
//	!!!!!
//

///////////////////////////////////////////////////////////////////////////////
//									Tests

int g_virtualKeys[] = {
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
	0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52, 0x53, 0x54,
	0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,

	0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,

	0x10, 0x11,
	0x25, 0x26, 0x27, 0x28,
	0x1B, 0x09, 0x20, 0x0D, 0x08
};

Key g_keys[] = {
	Key::KeyA, Key::KeyB, Key::KeyC, Key::KeyD, Key::KeyE, Key::KeyF, Key::KeyG, Key::KeyH, Key::KeyI,
	Key::KeyJ, Key::KeyK, Key::KeyL, Key::KeyM, Key::KeyN, Key::KeyO, Key::KeyP, Key::KeyQ, Key::KeyR,
	Key::KeyS, Key::KeyT, Key::KeyU, Key::KeyV, Key::KeyW, Key::KeyX, Key::KeyY, Key::KeyZ,

	Key::Key0, Key::Key1, Key::Key2, Key::Key3, Key::Key4, Key::Key5, Key::Key6, Key::Key7, Key::Key8,
	Key::Key9,

	Key::KeyShift, Key::KeyCtrl,
	Key::KeyLeftArrow, Key::KeyUpArrow, Key::KeyRightArrow, Key::KeyDownArrow,
	Key::KeyEsc, Key::KeyTab, Key::KeySpace, Key::KeyEnter, Key::KeyBackspace
};
const int g_count = sizeof(g_keys) / sizeof(g_keys[0]);

void simulateKeyStrikes()
{
	INPUT inputs[g_count];

	for (int i = 0; i < g_count; ++i)
		inputs[i].type = INPUT_KEYBOARD,
		inputs[i].ki.wVk = g_virtualKeys[i],
		inputs[i].ki.wScan = 0,
		inputs[i].ki.dwFlags = 0,
		inputs[i].ki.time = 0,
		inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
	SendInput(g_count, inputs, sizeof(inputs[0]));
}

void sendKeyDownMsgs()
{
	for (int i = 0; i < g_count; ++i)
		SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, g_virtualKeys[i], 0);
}

/*
	NOTE:	
		This test had been tested and passed, but it caused a system problem. Maybe
	the test method is not very good.
	
void IsPressed_ByDefault_CanDetected()
{
	

	simulateKeyStrikes();
	for (int i = 0; i < g_count; ++i)
		assert(Keyboard::isPressing(g_keys[i]) == true);
	for (int i = 0; i < g_count; ++i)
		assert(Keyboard::isPressing(g_keys[i]) == false);
}
*/

void TryGetPressed_NoPressed_ReturnKeyNone()
{
	Window::clearScreen(Rgb::Black());

	assert(Keyboard::tryGetPressed() == Key::KeyNone);
}

void TryGetPressed_SupportedKeyPressed_ReturnKey()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_ESCAPE, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyEsc);
}

void TryGetPressed_ManyKeyDownOneKeyUp_MiddleMsgsIgnored()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_ESCAPE, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyEsc);
}

void TryGetPressed_KeyMsgsAfterCharMsgs_ReturnKey()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_CHAR, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_UNICHAR, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_SHIFT, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyShift);
}

void TryGetPressed_KeyMsgsBeforeCharMsgs_ReturnKey()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_CHAR, VK_ESCAPE, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_UNICHAR, VK_ESCAPE, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyShift);
}

void TryGetPressed_OnlyKeyDownMsg_ReturnKeyNone()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_SHIFT, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyNone);
}

void TryGetPressed_TwoKeyPressed_GetTwo()
{
	Window::clearScreen(Rgb::Black());
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_CHAR, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYDOWN, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_KEYUP, VK_SHIFT, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_CHAR, VK_SHIFT, 0);

	assert(Keyboard::tryGetPressed() == Key::KeyShift);
	assert(Keyboard::tryGetPressed() == Key::KeyShift);
	assert(Keyboard::tryGetPressed() == Key::KeyNone);
}