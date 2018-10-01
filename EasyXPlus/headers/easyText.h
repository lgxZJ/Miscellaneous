///
///	@file		Text module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/13
///	@version	1.0
///

#ifndef EASY_TEXT_H
#define EASY_TEXT_H

#include "easyGeometry.h"
#include <string>

namespace EasyXPlus
{
	///
	///	@brief	A class for text operations.
	///	@todo	Add a setFontSize() overload that uses pixel size to set the font.
	///
	class Text
	{
	public:
///	@{
		///
		///	@brief	Set the text font to the given fontname.
		///	@throw	EasyExcept	If the font specified by the filename
		///			cannot be find, it throws an exception.
		///
		static void setFont(std::wstring fontName);
		static void setBold(bool isBold);
		static void setItalic(bool isItalic);
		static void setUnderline(bool isUnderline);
		///
		///	@brief	Set the font size to the given size in point size unit.
		///
		static void setFontSize(unsigned pointSize);
		static void setTextColor(const Colorable& color);
		static void setBkColor(const Colorable& color);
///	@}

///	@{
		static std::wstring getCurrentFontName();
		static bool isCurrentBold();
		static bool isCurrentItalic();
		static bool isCurrentUnderline();
		static unsigned getCurrentFontSize();
		static Rgb getCurrentTextColor();
		static Rgb getCurrentBkColor();
///	@}

///	@{
		///
		///	@brief	Get the string size if it is to be drawn.
		///	@remark	The string size is only affected by the font size
		///			and the string content.
		///
		static Rect getOutputSize(std::wstring text);
		///
		///	@brief	Output the string at the given position in the default
		///			window.
		///	@throw	EasyExcept	If Window::int() is not called, it throws an exception.
		///
		static void output(const Point pos,  std::wstring text);
///	@}
	};
}

#endif