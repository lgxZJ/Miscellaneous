#include "easyBaseWindow.h"
#include "easyKeyboard.h"
#include "easyMouse.h"
#include "easyExcept.h"

#include <cassert>
#include <queue>

//	pseudo main function used for developers
int main(int argc, wchar_t* argv[]);

///////////////////////////////////////////////////////////////////////////////////

using namespace EasyXPlus;

///////////////////////////////////////////////////////////////////////////////////
//							globals

HANDLE g_threadHandle = NULL;
std::queue<MouseEvent> g_mouseEvents;
std::queue<Key> g_keysPressed;

int WINAPI wWinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	BaseWindow window(L"easyX+", 0, 0, 640, 480);
	window.setAsDefault();
	window.clear(Rgb::Black());

	MSG msg;
	BOOL fGotMessage;
	while ((fGotMessage = GetMessage(&msg, (HWND)NULL, 0, 0)) != 0 && fGotMessage != -1)
	{
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

DWORD WINAPI MainThread(LPVOID param)
{
	if (0 == SetThreadPriority(
			GetCurrentThread(), THREAD_MODE_BACKGROUND_BEGIN))
		throw EasyExcept("Thread priority set error!");

	int argc = 0;
	LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);

	main(argc, argv);

	ExitProcess(0);
}

LRESULT CALLBACK EasyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_PAINT:
		{
			static bool called = false;

			if (!called)
			{
				g_threadHandle = CreateThread(NULL, NULL, MainThread, nullptr, 0, NULL);
				
				if (g_threadHandle == NULL)
					throw EasyExcept("Thread create error!");
				called = true;
			}
			return 0;
		}

	case WM_KEYUP:
		g_keysPressed.push(Keyboard::convertToKeyEnum(wParam));
		return 0;

	case WM_LBUTTONDOWN:
		g_mouseEvents.push(MouseEvent::LeftDown);
		return 0;
	case WM_LBUTTONUP:
		g_mouseEvents.push(MouseEvent::LeftUp);
		return 0;
	case WM_RBUTTONDOWN:
		g_mouseEvents.push(MouseEvent::RightDown);
		return 0;
	case WM_RBUTTONUP:
		g_mouseEvents.push(MouseEvent::RightUp);
		return 0;
	case WM_MOUSEMOVE:
		g_mouseEvents.push(MouseEvent::Move);
		return 0;

	case WM_ERASEBKGND:
		return TRUE;	//	no erase drawing

	case WM_CLOSE:
		ExitProcess(0);
		DestroyWindow(hwnd);
		return 0;

	default:
		return DefWindowProcW(hwnd, msg, wParam, lParam);
	}
}