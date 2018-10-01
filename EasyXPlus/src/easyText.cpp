#include "easyText.h"
#include "easyBaseWindow.h"
#include "easyExcept.h"
#include "easyColor.h"

using namespace std;

namespace EasyXPlus
{
	///////////////////////////////////////////////////////////////////////////
	//									Macros

#define APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(oneField)							\
	BaseWindow::TextAttribute* textAttribute = BaseWindow::getDefaultTextAttribute();	\
	textAttribute->oneField = oneField;											\
																				\
	textAttribute->changeFont();

	///////////////////////////////////////////////////////////////////////////
	//									Text

	void Text::setFont(wstring fontName)
	{
		APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(fontName);
	}

	////////////////////////////////////

	void Text::setBold(bool isBold)
	{
		APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(isBold);
	}

	////////////////////////////////////

	void Text::setItalic(bool isItalic)
	{
		APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(isItalic);
	}

	////////////////////////////////////

	void Text::setUnderline(bool isUnderline)
	{
		APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(isUnderline);
	}

	////////////////////////////////////

	void Text::setFontSize(unsigned pointSize)
	{
		if (pointSize > 7200)	//	100 inch
			throw EasyExcept("Too big the size!");

		APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE(pointSize);
	}

	////////////////////////////////////

	void Text::setTextColor(const Colorable& color)
	{
		BaseWindow::TextAttribute* attribute = BaseWindow::getDefaultTextAttribute();
		attribute->textColor = Rgb(color.toColorref());

		if (CLR_INVALID ==
				SetTextColor(attribute->hdc, attribute->textColor.toColorref()))

			throw EasyExcept("System call error!");
	}

	////////////////////////////////////

	void Text::setBkColor(const Colorable& color)
	{
		BaseWindow::TextAttribute* attribute = BaseWindow::getDefaultTextAttribute();
		attribute->bkColor = Rgb(color.toColorref());

		if (CLR_INVALID ==
				SetBkColor(attribute->hdc, attribute->bkColor.toColorref()))

			throw EasyExcept("System call error!");
	}

	////////////////////////////////////

	wstring Text::getCurrentFontName()
	{
		return BaseWindow::getDefaultTextAttribute()->fontName;
	}

	////////////////////////////////////

	bool Text::isCurrentBold()
	{
		return BaseWindow::getDefaultTextAttribute()->isBold;
	}

	///////////////////////////////////

	bool Text::isCurrentItalic()
	{
		return BaseWindow::getDefaultTextAttribute()->isItalic;
	}

	////////////////////////////////////

	bool Text::isCurrentUnderline()
	{
		return BaseWindow::getDefaultTextAttribute()->isUnderline;
	}

	////////////////////////////////////

	unsigned Text::getCurrentFontSize()
	{
		return BaseWindow::getDefaultTextAttribute()->pointSize;
	}

	////////////////////////////////////

	Rgb Text::getCurrentTextColor()
	{
		return BaseWindow::getDefaultTextAttribute()->textColor;
	}

	////////////////////////////////////

	Rgb Text::getCurrentBkColor()
	{
		return BaseWindow::getDefaultTextAttribute()->bkColor;
	}

	////////////////////////////////////

	Rect Text::getOutputSize(std::wstring text)
	{
		BaseWindow::TextAttribute* attribute = BaseWindow::getDefaultTextAttribute();

		SIZE sizeRet;
		BOOL ret = GetTextExtentPoint32W(
			attribute->hdc,
			text.c_str(),
			text.size(),
			&sizeRet);
		if (ret == 0)	throw EasyExcept("System call error!");

		return Rect(sizeRet.cx, sizeRet.cy);
	}

	////////////////////////////////////

	void Text::output(const Point pos, wstring text)
	{
		BaseWindow::TextAttribute* textAttribute = BaseWindow::getDefaultTextAttribute();

		BOOL ret = TextOutW(
			textAttribute->hdc,
			pos.getX(),
			pos.getY(),
			text.c_str(),
			text.size());
		if (ret == 0)	throw EasyExcept("System call error!");

		//	it seems the last text output is not drawn until next call, 
		//	call UpdateWindow to make it draw immediately
		if (UpdateWindow(BaseWindow::getDefaultWindowHandle()) == 0)
			throw EasyExcept("UpdateWindow call error!");
	}

#undef APPLY_ONE_FONT_ATTRIBUTE_FIELD_CHANGE

	///////////////////////////////		END		///////////////////////////////
}