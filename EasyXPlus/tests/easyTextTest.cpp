#include "..\src\easyText.h"
#include "..\src\easyExcept.h"
#include "..\src\easyWindow.h"

#include "SU.h"

using namespace std;
using namespace EasyXPlus;

/////////////////////////////////////////////////////////////////////////////
//									Tests

void Text_FunctionCalls_DefaultWindowNotSet_ThrowExcept()
{
	SU_ASSERT_THROW(Text::setBkColor(Rgb::Black()), EasyExcept);
	SU_ASSERT_THROW(Text::setBold(true), EasyExcept);
	SU_ASSERT_THROW(Text::setFont(L"Century Gothic"), EasyExcept);
	SU_ASSERT_THROW(Text::setFontSize(12), EasyExcept);
	SU_ASSERT_THROW(Text::setItalic(true), EasyExcept);
	SU_ASSERT_THROW(Text::setTextColor(Rgb::Coral()), EasyExcept);
	SU_ASSERT_THROW(Text::setUnderline(true), EasyExcept);
	SU_ASSERT_THROW(Text::getCurrentBkColor(), EasyExcept);
	SU_ASSERT_THROW(Text::getCurrentFontName(), EasyExcept);
	SU_ASSERT_THROW(Text::getCurrentFontSize(), EasyExcept);
	SU_ASSERT_THROW(Text::getCurrentTextColor(), EasyExcept);
	SU_ASSERT_THROW(Text::getOutputSize(wstring(L"12121212")), EasyExcept);
	SU_ASSERT_THROW(
		Text::output(Point(0, 0), wstring(L"test string")),
		EasyExcept); 
}

void Text_SetFont_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	wstring fontName = L"Arial";
	Text::setFont(fontName);

	assert(Text::getCurrentFontName() == fontName);
}

void Text_SetBold_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setBold(true);
	assert(Text::isCurrentBold() == true);
	Text::setBold(false);
	assert(Text::isCurrentBold() == false);
}

void Text_SetItalic_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setItalic(true);
	assert(Text::isCurrentItalic() == true);
	Text::setItalic(false);
	assert(Text::isCurrentItalic() == false);
}

void Text_SetUnderline_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setUnderline(true);
	assert(Text::isCurrentUnderline() == true);
	Text::setUnderline(false);
	assert(Text::isCurrentUnderline() == false);
}

void Text_SetFontSize_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setFontSize(10);

	assert(Text::getCurrentFontSize() == 10);
}

void Text_SetFontSize_LargerThen7200_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW( Text::setFontSize(7201), EasyExcept );
}

void Text_SetTextColor_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setTextColor(Rgb::Black());

	assert(Text::getCurrentTextColor() == Rgb::Black());
}

void Text_SetBkColor_ByDefault_GetWhatSet()
{
	Window::clearScreen(Rgb::Black());

	Text::setBkColor(Rgb::Black());

	assert(Text::getCurrentBkColor() == Rgb::Black());
}