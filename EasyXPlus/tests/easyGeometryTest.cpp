#include "..\src\easyGeometry.h"
#include "..\src\easyWindow.h"
#include "..\src\easyColor.h"
#include "..\src\easyExcept.h"

#include "SU.h"

#include <string>

using namespace std;
using namespace EasyXPlus;

////////////////////////////////////////////////////////////////////////////
//							Tests for PointArray

void RectRegion_Ctor_InvalidRectRegion_ThrowExcept()
{
	SU_ASSERT_THROW(RectRegion(Point(2, 5), Point(2, 6)), EasyExcept);
	SU_ASSERT_THROW(RectRegion(Point(2, 5), Point(3, 5)), EasyExcept);
	SU_ASSERT_THROW(RectRegion(Point(3, 5), Point(3, 5)), EasyExcept);
	SU_ASSERT_THROW(RectRegion(Point(4, 5), Point(1, 2)), EasyExcept);
}

////////////////////////////////////////////////////////////////////////////
//							Tests for PointArray

void PointArray_GetPoint_InvalidIndex_ThrowExcept()
{
	PointArray points{Point(0, 0)};

	SU_ASSERT_THROW(points.getPoint(2), EasyExcept);
}

void PointArray_InsertPoint_InvalidIndex_ThrowExcept()
{
	PointArray points{ Point(0, 0) };

	SU_ASSERT_THROW(points.insertPoint(2, Point(0, 0)), EasyExcept);
}

/////////////////////////////////////////////////////////////////////////////
//							Tests for Geometry

void Geometry_SetDotColor_ByDefault_DrawDotWithThisColor()
{
	Rgb color = Rgb(23, 45, 89);
	Window::clearScreen(Rgb::Black());

	Point point(23, 23);
	Geometry::setDotColor(color);
	Geometry::drawDot(point);

	assert(Geometry::getPointColor(point) == color);
}

void Geometry_SetLineColor_ByDefault_DrawLineWithThisColor()
{
	Rgb color = Rgb(23, 34, 23);
	Window::clearScreen(Rgb::Black());

	Geometry::setLineColor(color);
	Geometry::drawLine(Point(0, 0), Point(40, 40));

	assert(Geometry::getPointColor(Point(20, 20)) == color);
}

void Geometry_SetLineColor_SameColor_ColorNotChange()
{
	Window::clearScreen(Rgb::Black());
	Rgb oldColorValue = Geometry::getCurrentLineColor();

	Geometry::setLineColor(Geometry::getCurrentLineColor());

	assert(Geometry::getCurrentLineColor() == oldColorValue);
}

void Geometry_SetFillColor_ByDefault_DrawShapesWithThisColor()
{
	Rgb color = Rgb(11, 22, 33);
	Window::clearScreen(Rgb::Black());

	Geometry::setFillColor(color);
	Geometry::drawRectangle(RectRegion(Point(0, 0), Point(60, 60)));

	assert(Geometry::getPointColor(Point(30, 30)) == color);
}

void Geometry_SetFillColor_SameColor_ColorNotChange()
{
	Window::clearScreen(Rgb::Black());
	const Colorable& oldColorValue = Geometry::getCurrentFillColor();

	Geometry::setFillColor(Geometry::getCurrentFillColor());

	assert(Geometry::getCurrentFillColor().toColorref() == oldColorValue.toColorref());
}

void Geometry_DrawDot_ByDefault_DrawDotWithDotColor()
{
	Window::clearScreen(Rgb::Black());
	Point dot(23, 23);
	Rgb dotColor = Rgb::Coral();
	Geometry::setDotColor(dotColor);

	Geometry::drawDot(dot);

	assert(Geometry::getPointColor(dot) == dotColor);
}

void Geometry_DrawLine_AfterDraw_DotColorAndLineColorNotChange()
{
	Window::clearScreen(Rgb::Black());

	Geometry::setDotColor(Rgb::Black());
	Geometry::setLineColor(Rgb::Blue());
	Geometry::setFillColor(Rgb::Coral());

	Geometry::drawLine(Point(1, 1), Point(2, 2));

	assert(Geometry::getCurrentDotColor().toColorref() != Geometry::getCurrentLineColor().toColorref());
}

void Geometry_DrawLine_ByDefault_IncludeTwoEndPoint()
{
	Window::clearScreen(Rgb::Black());
	Point from(0, 0), to(30, 20), middle(15, 10);
	Rgb lineColor(12, 22, 43);
	Geometry::setLineColor(lineColor);

	Geometry::drawLine(from, to);

	assert(Geometry::getPointColor(from) == lineColor);
	assert(Geometry::getPointColor(middle) == lineColor);
	assert(Geometry::getPointColor(to) == lineColor);
}

void Geometry_DrawLine_TwoSameEnd_DrawOnePoint()
{
	Window::clearScreen(Rgb::Black());
	Point from(0, 0), to = from;
	Rgb lineColor(12, 22, 43);
	Geometry::setLineColor(lineColor);

	Geometry::drawLine(from, to);

	assert(Geometry::getPointColor(from) == lineColor);
}

void Geometry_DrawArc_TwoPointsOneCenterOneNot_ThrowExcep()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW(
		Geometry::drawArc(RectRegion(Point(0, 0), Point(40, 40)), Point(20, 20), Point(23, 45)),
		EasyExcept);
}

void Geometry_DrawArc_SameStartAndEndPoint_DrawEntireEllipse()
{
	Window::clearScreen(Rgb::Black());
	Rgb lineColor = Rgb::Blue();
	Geometry::setLineColor(lineColor);

	RectRegion bound(Point(0, 0), Point(40, 40));
	Geometry::drawArc(bound, Point(0, 0), Point(0, 0));

	assert(Geometry::getPointColor(Point(20, 0)) == lineColor);
	assert(Geometry::getPointColor(Point(0, 20)) == lineColor);
}

void Geometry_DrawBezier_PointNumberNotThreeTimePlusOne_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());
	Rgb lineColor = Rgb::Blue();
	Geometry::setLineColor(lineColor);

	SU_ASSERT_THROW(
		Geometry::drawBeziers(PointArray{Point(0, 0)}),
		EasyExcept);
	SU_ASSERT_THROW(
		Geometry::drawBeziers(PointArray{ Point(0, 0), Point(1, 2) }),
		EasyExcept);
	SU_ASSERT_THROW(
		Geometry::drawBeziers(PointArray{ Point(0, 0), Point(1, 2), Point(0, 4) }),
		EasyExcept);
}

void Geometry_DrawBezier_ByDefault_IncludeTwoEndAndTwoControlPoints()
{
	Window::clearScreen(Rgb::Black());
	Rgb lineColor = Rgb::Blue();
	Geometry::setLineColor(lineColor);
	PointArray points{ Point(0, 0), Point(10, 10), Point(20, 20), Point(30, 30) };

	Geometry::drawBeziers(points);

	assert(
		Geometry::getPointColor(points.getPoint(0)) == lineColor);
	assert(
		Geometry::getPointColor(points.getPoint(1)) == lineColor);
	assert(
		Geometry::getPointColor(points.getPoint(2)) == lineColor);
	assert(
		Geometry::getPointColor(points.getPoint(3)) == lineColor);
}

void Geometry_DrawPolyline_PointNumberNotGreaterOne_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW(
		Geometry::drawPolyline(PointArray{}),
		EasyExcept);
	SU_ASSERT_THROW(
		Geometry::drawPolyline(PointArray{Point(0, 0)}),
		EasyExcept);
}

void Geometry_DrawPolyline_ByDefault_IncludeAllStartAndEndsPoints()
{
	Window::clearScreen(Rgb::Black());
	Rgb lineColor = Rgb::Blue();
	Geometry::setLineColor(lineColor);
	PointArray points{ Point(0, 0), Point(10, 10), Point(20, 20)};

	Geometry::drawPolyline(points);

	assert(
		Geometry::getPointColor(points.getPoint(0)) == lineColor);
	assert(
		Geometry::getPointColor(points.getPoint(1)) == lineColor);
	assert(
		Geometry::getPointColor(points.getPoint(2)) == lineColor);
}

void Geometry_DrawChord_EndPointsOneCenterAnotherNot_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW(
		Geometry::drawChord(RectRegion(Point(0, 0), Point(10, 10)), Point(5, 5), Point(1, 2)),
		EasyExcept);
}

void Geometry_DrawRectangle_ByDegault_IncludeBottomLineAndRightLine()
{
	Window::clearScreen(Rgb::Black());
	Rgb lineColor = Rgb::Blue();
	Geometry::setLineColor(lineColor);

	Geometry::drawRectangle(RectRegion(Point(0, 0), Point(10, 10)));

	assert(
		Geometry::getPointColor(Point(0, 0)) == lineColor);
	assert(
		Geometry::getPointColor(Point(0, 10)) == lineColor);
	assert(
		Geometry::getPointColor(Point(10, 0)) == lineColor);
	assert(
		Geometry::getPointColor(Point(10, 10)) == lineColor);
}

void Geometry_DrawPie_TwoEndsOneCanterOneAnoter_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW(
		Geometry::drawPie(RectRegion(Point(0, 0), Point(10, 10)), Point(5, 5), Point(1, 2)),
		EasyExcept);
}

void Geometry_DrawPolygon_PointNumberLessThree_ThrowExcept()
{
	Window::clearScreen(Rgb::Black());

	SU_ASSERT_THROW(
		Geometry::drawPolygon(PointArray{ Point(0, 0) }),
		EasyExcept);
	SU_ASSERT_THROW(
		Geometry::drawPolygon(PointArray{ Point(0, 0), Point(23, 34) }),
		EasyExcept);
}