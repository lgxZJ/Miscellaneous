#include "App.h"
#include "easyWindow.h"
#include "easyText.h"
#include "easyExcept.h"
#include "easyOthers.h"

#include <algorithm>

using namespace EasyXPlus;
using namespace std;

App::App()
	: windowWidth(960)
	, windowHeight(355)
	, horizontalCellNum(17)
	, verticalCellNum(6)
	, paddingPixel(5)
	, marginPixel(3)
	, durationOfHighlight(50)
	, defaultLineColor(Rgb::LightBlue())
	, defaultFillColor(Rgb::DarkCyan())
	, defaultTextColor(Rgb::Coral())
	, highlightFillColor(Rgb::DarkMagenta())
{
	Window::init(L"EasyXPlus Sample", 100, 100, windowWidth, windowHeight);
	Geometry::setLineColor(defaultLineColor);
	Geometry::setFillColor(defaultFillColor);
	Text::setTextColor(defaultTextColor);
	Text::setBkColor(Geometry::getCurrentFillColor());

	cellUnit = min(	windowWidth / horizontalCellNum,
					windowHeight / verticalCellNum);

	for (int i = 0; i < cellLengths.size(); ++i)
	{
		static int row = 0;
		static int column = 0;

		Point leftTop(column * cellUnit, row * cellUnit);
		Point rightBottom((column + cellLengths[i]) * cellUnit, (row + 1) * cellUnit);

		auto region = RectRegion(leftTop, rightBottom);
		drawStringInRect(region, stringCollections[i]);

		column += cellLengths[i];
		if (column == (horizontalCellNum - 2))
			column = 0, ++row;
	}
	drawMouseBlocks();
}

void App::drawStringInRect(const RectRegion& region, StringCollection strs)
{
	if (strs.size() > 2)	throw EasyExcept("too much strings");

	drawRect(region);
	drawStrings(region, strs);
}

void App::drawRect(const RectRegion& region)
{
	if ((region.getRightBottom().getX() - region.getLeftTop().getX()) < marginPixel ||
		(region.getRightBottom().getY() - region.getLeftTop().getY()) < marginPixel)

		throw EasyExcept("window too small!");

	Point newLeftTop(	region.getLeftTop().getX() + marginPixel,
						region.getLeftTop().getY() + marginPixel);
	Point newRightBottom(	region.getRightBottom().getX() + marginPixel,
							region.getRightBottom().getY() + marginPixel);
	Geometry::drawRectangle(RectRegion(newLeftTop, newRightBottom));
}

void App::drawStrings(const RectRegion& region, StringCollection strs)
{
	if (strs.size() == 1)
	{
		Rect stringRect(region.getRightBottom().getX() - region.getLeftTop().getX() - 2 * marginPixel,
						region.getRightBottom().getY() - region.getLeftTop().getY() - 2 * marginPixel);
		Point leftTop(	region.getLeftTop().getX(),
						region.getLeftTop().getY());

		drawOneString(&leftTop, stringRect, strs[0]);
	}
	else
	{
		Rect stringRect((region.getRightBottom().getX() - region.getLeftTop().getX() - 2 * marginPixel) / 2,
						(region.getRightBottom().getY() - region.getLeftTop().getY() - 2 * marginPixel) / 2);
		Point leftTopOne(region.getLeftTop());
		Point leftTopTwo(region.getLeftTop().getX(),
			region.getLeftTop().getY() + stringRect.getHeight());

		drawOneString(&leftTopOne, stringRect, strs[0]);
		drawOneString(&leftTopTwo, stringRect, strs[1]);
	}
}

void App::drawOneString(Point* leftTop, const Rect& stringRect, wstring str)
{
	Text::setFontSize(findAppropriateFontSize(stringRect, str));
	applyPaddings(leftTop);
	adjustToCenter(leftTop, str, stringRect);
	Text::output(*leftTop, str);
}

void App::applyPaddings(Point* point)
{
	*point = Point(	point->getX() + paddingPixel,
					point->getY() + paddingPixel );
}

void App::adjustToCenter(Point* point, wstring str, Rect boundingRect)
{
	Rect strRect = Text::getOutputSize(str);

	*point = Point(point->getX() + (boundingRect.getWidth() - strRect.getWidth()) / 2,
					point->getY() + (boundingRect.getHeight() - strRect.getHeight()) / 2);
}

unsigned App::findAppropriateFontSize(Rect rect, wstring str)
{
	for (int size = 60; size > 0; --size)
	{
		Text::setFontSize(size);
		Rect result = Text::getOutputSize(str);
		if (result.getWidth() <= (rect.getWidth() - 2 * paddingPixel) &&
			result.getHeight() <= (rect.getHeight() - 2 * paddingPixel))

			return size;
	}
	
	throw EasyExcept("Cannot find a appropriate font size!");
}

void App::drawMouseBlocks()
{
	Rgb fillColor = Geometry::getCurrentFillColor();

	drawLeftButtonBlock(fillColor);
	drawRightButtonBlock(fillColor);
	drawMoveMouseBlock(fillColor);
}

void App::drawLeftButtonBlock(const Rgb& fillColor)
{
	Geometry::setFillColor(fillColor);
	Text::setBkColor(fillColor);

	RectRegion leftRegion(	Point((horizontalCellNum - 2) * cellUnit, static_cast<unsigned>(cellUnit * 1.5)),
							Point((horizontalCellNum - 1) * cellUnit, static_cast<unsigned>(cellUnit * 2.5)));
	drawStringInRect(leftRegion, StringCollection{ L"Left" });
}

void App::drawRightButtonBlock(const Rgb& fillColor)
{
	Geometry::setFillColor(fillColor);
	Text::setBkColor(fillColor);

	RectRegion rightRegion(	Point((horizontalCellNum - 1) * cellUnit, static_cast<unsigned>(cellUnit * 1.5)),
							Point((horizontalCellNum - 0) * cellUnit, static_cast<unsigned>(cellUnit * 2.5)));
	drawStringInRect(rightRegion, StringCollection{ L"Right" });
}

void App::drawMoveMouseBlock(const Rgb& fillColor)
{
	Geometry::setFillColor(fillColor);
	Text::setBkColor(fillColor);

	RectRegion mouseRegion(	Point((horizontalCellNum - 2) * cellUnit, static_cast<unsigned>(cellUnit * 2.5)),
							Point((horizontalCellNum - 0) * cellUnit, static_cast<unsigned>(cellUnit * 4.5)));
	drawStringInRect(mouseRegion, StringCollection{ L"==Mouse==" });
}

///////////////////////////////////////////////////////////////////////////////

void App::exec()
{
	while (true)
	{
		Key keyPressed = Keyboard::tryGetPressed();
		if (keyPressed != Key::KeyNone)
			drawPressedKeyBlock(keyPressed);

		drawClickedMouseBlocks(Mouse::tryGetEvent());
	}
}

void App::drawPressedKeyBlock(Key keyPressed)
{
	drawKeyBlockWithColor(keyPressed, highlightFillColor);
	Others::pause(durationOfHighlight);
	drawKeyBlockWithColor(keyPressed, defaultFillColor);
}

void App::drawKeyBlockWithColor(Key keyPressed, Rgb color)
{
	Geometry::setFillColor(color);
	Text::setBkColor(color);

	int row = 0, column = 0;
	for (int i = 0; i < cellLengths.size(); ++i)
	{
		if (keys[i] == keyPressed)
		{
			Point leftTop(column * cellUnit, row * cellUnit);
			Point rightBottom((column + cellLengths[i]) * cellUnit, (row + 1) * cellUnit);

			auto region = RectRegion(leftTop, rightBottom);
			drawStringInRect(region, stringCollections[i]);
		}

		column += cellLengths[i];
		if (column == (horizontalCellNum - 2))
			column = 0, ++row;
	}
}

void App::drawClickedMouseBlocks(MouseEvent event)
{
	if (event == MouseEvent::None)
		return;

	if (event == MouseEvent::LeftUp || event == MouseEvent::LeftDown)
		drawUserEvent(&App::drawLeftButtonBlock);
	else if (event == MouseEvent::RightUp || event == MouseEvent::RightDown)
		drawUserEvent(&App::drawRightButtonBlock);
	else if (event == MouseEvent::Move)
		drawUserEvent(&App::drawMoveMouseBlock);
}

void App::drawUserEvent(MemberFuncPtr func)
{
	(this->*func)(highlightFillColor);
	Others::pause(durationOfHighlight);
	(this->*func)(defaultFillColor);
}