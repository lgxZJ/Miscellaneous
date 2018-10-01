#include "easyMouse.h"
#include "easyBaseWindow.h"
#include "easyExcept.h"

#include <algorithm>
#include <queue>

using namespace std;

extern std::queue<EasyXPlus::MouseEvent> g_mouseEvents;

namespace EasyXPlus
{
	//////////////////////////////////////////////////////////////////////////
	//								Mouse class
	
	Point Mouse::getCurrentPos()
	{
		POINT pos;
		if (0 == GetCursorPos(&pos))
			throw EasyExcept("System call error!");

		if (0 == ScreenToClient(BaseWindow::getDefaultWindowHandle(), &pos))
			throw EasyExcept("System call error!");

		return Point(pos.x, pos.y);
	}

	//////////////////////////////////////////////////////////////////////////

	MouseEvent Mouse::tryGetEvent()
	{
		if (g_mouseEvents.empty())
			return MouseEvent::None;
		else
		{
			MouseEvent key = g_mouseEvents.front();
			g_mouseEvents.pop();
			return key;
		}
	}
}