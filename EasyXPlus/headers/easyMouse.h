///
///	@file		Mouse module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/13
///	@version	1.0
///

#ifndef EASY_MOUSE_H
#define EASY_MOUSE_H

#include "easyGeometry.h"

namespace EasyXPlus
{
	///
	///	@brief	Mouse event enumerations.
	///
	enum class MouseEvent
	{
		None, Move, LeftDown, LeftUp, RightDown, RightUp
	};

	///
	///	@brief 	A class for mouse support.
	///
	class Mouse
	{
	public:
		///
		///	@brief	Get the current mouse cursor position.
		///
		static Point getCurrentPos();

		///
		///	@brief	Try get the triggered mouse event.
		///	@remark	If no mouse event is triggered, it returns MouseEvent::None.
		///
		static MouseEvent tryGetEvent();
	};
}

#endif