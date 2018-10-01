#include "easyKeyboard.h"
#include "easyBaseWindow.h"
#include "easyExcept.h"

#include <cassert>
#include <queue>

//	variables in other units
extern std::queue<EasyXPlus::Key> g_keysPressed;

namespace EasyXPlus
{
	///////////////////////////////////////////////////////////////////////////
	//								Keyboard

	bool Keyboard::isPressing(Key key)
	{
		int virtualKey = convertToVirtualKey(key);
		return GetAsyncKeyState(virtualKey) != 0;
	}

	int Keyboard::convertToVirtualKey(Key key)
	{
		switch (key)
		{
		case Key::Key0:		return 0x30;
		case Key::Key1:		return 0x31;
		case Key::Key2:		return 0x32;
		case Key::Key3:		return 0x33;
		case Key::Key4:		return 0x34;
		case Key::Key5:		return 0x35;
		case Key::Key6:		return 0x36;
		case Key::Key7:		return 0x37;
		case Key::Key8:		return 0x38;
		case Key::Key9:		return 0x39;

		case Key::KeyA:		return 0x41;
		case Key::KeyB:		return 0x42;
		case Key::KeyC:		return 0x43;
		case Key::KeyD:		return 0x44;
		case Key::KeyE:		return 0x45;
		case Key::KeyF:		return 0x46;
		case Key::KeyG:		return 0x47;
		case Key::KeyH:		return 0x48;
		case Key::KeyI:		return 0x49;
		case Key::KeyJ:		return 0x4A;
		case Key::KeyK:		return 0x4B;
		case Key::KeyL:		return 0x4C;
		case Key::KeyM:		return 0x4D;
		case Key::KeyN:		return 0x4E;
		case Key::KeyO:		return 0x4F;
		case Key::KeyP:		return 0x50;
		case Key::KeyQ:		return 0x51;
		case Key::KeyR:		return 0x52;
		case Key::KeyS:		return 0x53;
		case Key::KeyT:		return 0x54;
		case Key::KeyU:		return 0x55;
		case Key::KeyV:		return 0x56;
		case Key::KeyW:		return 0x57;
		case Key::KeyX:		return 0x58;
		case Key::KeyY:		return 0x59;
		case Key::KeyZ:		return 0x5A;

		case Key::KeyShift:		return VK_SHIFT;
		case Key::KeyCtrl:		return VK_CONTROL;

		case Key::KeyUpArrow:		return VK_UP;
		case Key::KeyDownArrow:		return VK_DOWN;
		case Key::KeyLeftArrow:		return VK_LEFT;
		case Key::KeyRightArrow:	return VK_RIGHT;

		case Key::KeyEsc:			return VK_ESCAPE;
		case Key::KeyTab:			return VK_TAB;
		case Key::KeySpace:			return VK_SPACE;
		case Key::KeyEnter:			return VK_RETURN;
		case Key::KeyBackspace:		return VK_BACK;
		default:
			throw EasyExcept("Invalid key!");
		}
	}

	/////////////////////////////////////////////////////////////////////

	Key Keyboard::tryGetPressed()
	{
		if (g_keysPressed.empty())
			return Key::KeyNone;
		else
		{
			Key key = g_keysPressed.front();
			g_keysPressed.pop();
			return key;
		}
	}

	Key Keyboard::convertToKeyEnum(int virtualKey)
	{
		switch (virtualKey)
		{
		case 0x30:		return Key::Key0;
		case 0x31:		return Key::Key1;
		case 0x32:		return Key::Key2;
		case 0x33:		return Key::Key3;
		case 0x34:		return Key::Key4;
		case 0x35:		return Key::Key5;
		case 0x36:		return Key::Key6;
		case 0x37:		return Key::Key7;
		case 0x38:		return Key::Key8;
		case 0x39:		return Key::Key9;

		case 0x41:		return Key::KeyA;
		case 0x42:		return Key::KeyB;
		case 0x43:		return Key::KeyC;
		case 0x44:		return Key::KeyD;
		case 0x45:		return Key::KeyE;
		case 0x46:		return Key::KeyF;
		case 0x47:		return Key::KeyG;
		case 0x48:		return Key::KeyH;
		case 0x49:		return Key::KeyI;
		case 0x4A:		return Key::KeyJ;
		case 0x4B:		return Key::KeyK;
		case 0x4C:		return Key::KeyL;
		case 0x4D:		return Key::KeyM;
		case 0x4E:		return Key::KeyN;
		case 0x4F:		return Key::KeyO;
		case 0x50:		return Key::KeyP;
		case 0x51:		return Key::KeyQ;
		case 0x52:		return Key::KeyR;
		case 0x53:		return Key::KeyS;
		case 0x54:		return Key::KeyT;
		case 0x55:		return Key::KeyU;
		case 0x56:		return Key::KeyV;
		case 0x57:		return Key::KeyW;
		case 0x58:		return Key::KeyX;
		case 0x59:		return Key::KeyY;
		case 0x5A:		return Key::KeyZ;

		case VK_SHIFT:			return Key::KeyShift;
		case VK_CONTROL:		return Key::KeyCtrl;

		case VK_UP:				return Key::KeyUpArrow;
		case VK_DOWN:			return Key::KeyDownArrow;
		case VK_LEFT:			return Key::KeyLeftArrow;
		case VK_RIGHT:			return Key::KeyRightArrow;

		case VK_ESCAPE:			return Key::KeyEsc;
		case VK_TAB:			return Key::KeyTab;
		case VK_SPACE:			return Key::KeySpace;
		case VK_RETURN:			return Key::KeyEnter;
		case VK_BACK:			return Key::KeyBackspace;

		default:				return Key::KeyNotSupport;
		}
	}
}