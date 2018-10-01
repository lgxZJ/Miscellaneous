///
///	@file		Window module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/13
///	@version	1.0
///

#ifndef EASY_WINDOW_H
#define EASY_WINDOW_H

#include "easyColor.h"
#include <string>

namespace EasyXPlus
{
	class BaseWindow;

	///
	///	@brief	A class providing operations in the default window.
	///	@note	Any window related operations should be called after
	///			calling Window::init().
	///
	class Window
	{
	public:
///
///	@name Initialize the default window.
///

///	@{
		static void init(const std::wstring title = L"EasyX+");
		static void init(const std::wstring title, int posX, int posY);
		static void init(const std::wstring title, int posX, int posY, unsigned width, unsigned height);
///	@}

///	@{
		static int getPosX();
		static int getPosY();
		static unsigned getWidth();
		static unsigned getHeight();
///	@}

///	@{
		static void resize(unsigned width, unsigned height);
		static void reposition(int posX, int posY);
		static void clearScreen(const Colorable& color);
///	@}

		static void checkIfInitialized();

	private:
		static bool initialized;
	};
}

#endif