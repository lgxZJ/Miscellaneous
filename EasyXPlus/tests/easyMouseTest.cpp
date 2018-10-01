#include "..\src\easyMouse.h"
#include "..\src\easyExcept.h"
#include "..\src\easyWindow.h"
#include "..\src\easyBaseWindow.h"

#include "SU.h"

using namespace EasyXPlus;

///////////////////////////////////////////////////////////////////////////////
//									Tests

void flushMouseEvnets()
{
	while (Mouse::tryGetEvent() != MouseEvent::None)
		;
}

//	!!! Do not move mouse when running this test
void GetCurrentPos_ByDefault_GetWhereItIs()
{
	Window::clearScreen(Rgb::Black());
	Point expectedPoint(12, 32);

	SetCursorPos(expectedPoint.getX(), expectedPoint.getY());

	POINT point;
	Point returnPoint = Mouse::getCurrentPos();
	point.x = returnPoint.getX(), point.y = returnPoint.getY();
	ClientToScreen(BaseWindow::getDefaultWindowHandle(), &point);
	assert(Point(point.x, point.y) == expectedPoint);
}

//	Do not move mouse!!!
void TryGetEvent_NoEvent_ReturnNone()
{
	Window::clearScreen(Rgb::Black());
	flushMouseEvnets();

	MouseEvent ev = Mouse::tryGetEvent();
	assert(ev == MouseEvent::None);
}

void TryGetEvent_SupportedEvent_ReturnEvent()
{
	Window::clearScreen(Rgb::Black());
	flushMouseEvnets();

	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_LBUTTONDOWN, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_LBUTTONUP, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_RBUTTONDOWN, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_RBUTTONUP, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_MOUSEMOVE, 0, 0);

	auto it = Mouse::tryGetEvent();
	assert(it == MouseEvent::LeftDown);
	assert(Mouse::tryGetEvent() == MouseEvent::LeftUp);
	assert(Mouse::tryGetEvent() == MouseEvent::RightDown);
	assert(Mouse::tryGetEvent() == MouseEvent::RightUp);
	assert(Mouse::tryGetEvent() == MouseEvent::Move);
}

void TryGetEvent_NotSupportedEvent_ReturnNone()
{
	Window::clearScreen(Rgb::Black());
	flushMouseEvnets();
	PostMessageW(BaseWindow::getDefaultWindowHandle(), WM_LBUTTONDBLCLK, 0, 0);

	assert(Mouse::tryGetEvent() == MouseEvent::None);
}

void TryGetEvent_SupportedMixedWithUnsupported_ReturnSupported()
{
	Window::clearScreen(Rgb::Black());
	flushMouseEvnets();

	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_LBUTTONDOWN, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_NCLBUTTONDBLCLK, 0, 0);
	SendMessageW(BaseWindow::getDefaultWindowHandle(), WM_MOUSEMOVE, 0, 0);

	assert(Mouse::tryGetEvent() == MouseEvent::LeftDown);
	assert(Mouse::tryGetEvent() == MouseEvent::Move);
	assert(Mouse::tryGetEvent() == MouseEvent::None);
}