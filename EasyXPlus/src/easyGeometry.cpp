#include "easyGeometry.h"
#include "easyBaseWindow.h"
#include "easyExcept.h"

#include <cassert>

using namespace EasyXPlus;

namespace EasyXPlus
{
	////////////////////////////////////////////////////////////////////////////////
	//									Point

	bool Point::operator == (const Point& another)const
	{
		return	this->getX() == another.getX() &&
				this->getY() == another.getY();
	}
	
	bool Point::operator != (const Point& another)const
	{
		return !(*this == another);
	}

	////////////////////////////////////////////////////////////////////////////////
	//									RectRegion

	RectRegion::RectRegion(Point leftTopPoint, Point rightBottomPoint)
		: leftTop(leftTopPoint), rightBottom(rightBottomPoint)
	{
		//	invalid rectangular-region
		if (leftTop.getX() >= rightBottom.getX() ||
			leftTop.getY() >= rightBottom.getY())

			throw EasyExcept("Invalid RectRegion!");
	}

	////////////////////////////////////

	Point RectRegion::getCenter()const
	{
		return Point(
			(leftTop.getX() + rightBottom.getX()) / 2,
			(leftTop.getY() + rightBottom.getY())  / 2);
	}

	////////////////////////////////////////////////////////////////////////////////
	//									PointArray

	PointArray::PointArray(std::initializer_list<Point> points)
		: points(points)
	{}

	///////////////////////////////////

	inline bool PointArray::isEmpty() const
	{
		return points.empty();
	}

	///////////////////////////////////

	inline unsigned PointArray::getSize() const
	{
		return points.size();
	}

	///////////////////////////////////

	Point PointArray::getPoint(unsigned index) const
	{
		try
		{
			return points.at(index);
		}
		catch (std::out_of_range exception)
		{
			throw EasyExcept("Index error!");
		}
	}

	///////////////////////////////////

	void PointArray::appendPoint(Point point)
	{
		points.push_back(point);
	}

	//////////////////////////////////

	void PointArray::insertPoint(unsigned indexBefore, Point point)
	{
		if (indexBefore > points.size())
			throw EasyExcept("Index error");

		points.insert(points.begin() + indexBefore, point);
	}

	//////////////////////////////////

	void PointArray::assignPoint(unsigned index, Point point)
	{
		points.at(index) = point;
	}

	//////////////////////////////////

	void PointArray::erasePoint(unsigned index)
	{
		points.erase(points.begin() + index);
	}

	//////////////////////////////////

	void PointArray::clearAll()
	{
		points.clear();
	}

	///////////////////////////////////////////////////////////////////////////
	//								Geometry

	Rgb Geometry::getCurrentDotColor()
	{
		return BaseWindow::getDefaultGeometryAttribute()->dotColor;
	}

	/////////////////////////////////

	Rgb Geometry::getCurrentLineColor()
	{
		return BaseWindow::getDefaultGeometryAttribute()->lineColor;
	}

	/////////////////////////////////

	Rgb Geometry::getCurrentFillColor()
	{
		return BaseWindow::getDefaultGeometryAttribute()->fillColor;
	}

	/////////////////////////////////

	void Geometry::setDotColor(const Colorable& color)
	{
		BaseWindow::getDefaultGeometryAttribute()->dotColor = Rgb(color.toColorref());
	}

	/////////////////////////////////

	void Geometry::setLineColor(const Colorable& color)
	{
		BaseWindow::GeometryAttribute* geometryAttribute = BaseWindow::getDefaultGeometryAttribute();
		if (geometryAttribute->lineColor.toColorref() == color.toColorref())
			return;

		HPEN newPen = CreatePen(PS_SOLID, 0, color.toColorref());
		if (NULL == newPen)
			throw EasyExcept("System call error!");

		if (NULL == SelectObject(geometryAttribute->hdc, (HGDIOBJ)newPen))
			throw EasyExcept("System call error!");

		//	if created already, delete it
		if (geometryAttribute->pen != NULL)
			if (0 == DeleteObject((HGDIOBJ)geometryAttribute->pen))
				throw EasyExcept("System call error!");
		geometryAttribute->pen = newPen;
		geometryAttribute->lineColor = Rgb(color.toColorref());
	}

	/////////////////////////////////////

	void Geometry::setFillColor(const Colorable& color)
	{
		BaseWindow::GeometryAttribute* geometryAttribute = BaseWindow::getDefaultGeometryAttribute();
		if (geometryAttribute->fillColor.toColorref() == color.toColorref())
			return;

		HBRUSH newBrush = CreateSolidBrush(color.toColorref());
		if (NULL == newBrush)
			throw EasyExcept("System call error!");

		HGDIOBJ objRet = SelectObject(
			BaseWindow::getDefaultGeometryAttribute()->hdc, (HGDIOBJ)newBrush);
		if (NULL == objRet)
			throw EasyExcept("System call error!");

		//	if created, delete it
		if (geometryAttribute->brush != NULL)
			if (0 == DeleteObject((HGDIOBJ)geometryAttribute->brush))
				throw EasyExcept("System call error!");
		geometryAttribute->brush = newBrush;
		geometryAttribute->fillColor = Rgb(color.toColorref());
	}

	///////////////////////////////////////

	Rgb Geometry::getPointColor(Point point)
	{
		COLORREF retColor = GetPixel(
			BaseWindow::getDefaultGeometryAttribute()->hdc, point.getX(), point.getY());

		if (retColor == CLR_INVALID)
			throw EasyExcept("System call error!");
		else
			return Rgb(retColor);
	}

	///////////////////////////////////////

	void Geometry::drawDot(Point point)
	{
		BaseWindow::GeometryAttribute* geometryAttribute = BaseWindow::getDefaultGeometryAttribute();
		COLORREF colorRet = SetPixel(
			geometryAttribute->hdc,
			point.getX(), point.getY(),
			geometryAttribute->dotColor.toColorref());

		//	colorRet == -1 when the dot is on the window border,
		//	skip this failure since it does not matter
		if (colorRet == ERROR_INVALID_PARAMETER)
			throw EasyExcept("System call error!");
	}

	///////////////////////////////////////

	void Geometry::drawLine(Point from, Point to)
	{
		HDC hdc = BaseWindow::getDefaultGeometryAttribute()->hdc;

		if (0 == MoveToEx(hdc, from.getX(), from.getY(), NULL))
			throw EasyExcept("System call error!");

		if (0 == LineTo(hdc, to.getX(), to.getY()))
			throw EasyExcept("System call error!");

		//	in GDI, the last pixel is not drew
		COLORREF oldColorValue = Geometry::getCurrentDotColor().toColorref();
		Geometry::setDotColor(Geometry::getCurrentLineColor());
		Geometry::drawDot(to);
		Geometry::setDotColor(Rgb(GetRValue(oldColorValue), GetGValue(oldColorValue), GetBValue(oldColorValue)));
	}

	///////////////////////////////////////

	void Geometry::drawArc(RectRegion bound, Point start, Point end)
	{
		checkTwoEndPoints(bound, start, end);

		BOOL ret = Arc(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			bound.getLeftTop().getX(),
			bound.getLeftTop().getY(),
			bound.getRightBottom().getX(),
			bound.getRightBottom().getY(),
			start.getX(),
			start.getY(),
			end.getX(),
			end.getY());
		if (ret == 0)	throw EasyExcept("System call error!");

		//	Since the intersect point is hard to calculate and points in mathematics
		//	cannot be strictly represented in displays, i choose to let the two-end
		//	points go.
	}

	void Geometry::checkTwoEndPoints(const RectRegion& bound, const Point& start, const Point& end)
	{
		Point centerPoint = bound.getCenter();

		//	one point is center, another not, invalid!!
		if ( start != end &&
			(centerPoint == start || centerPoint == end))

			throw EasyExcept("Two points invalid!");
	}

	///////////////////////////////////////

	void Geometry::drawBeziers(PointArray points)
	{
		//	bezier curves need (3n + 1) point to draw itself
		if ( (points.getSize() % 3) != 1 )
			throw EasyExcept("The number of points is invalid!");

		POINT* pointArray = convertToPOINTs(points);
		BOOL ret = PolyBezier(
			BaseWindow::getDefaultGeometryAttribute()->hdc, pointArray, points.getSize());

		delete[] pointArray;
		if (ret == 0)	throw EasyExcept("System call error");

		//	in GDI, the end point is not drew. Use drawLine() bacause drawDot()
		//	uses different drawing color
		const Point& lastPoint = points.getPoint(points.getSize() - 1);
		Geometry::drawLine(lastPoint, lastPoint);
	}

	POINT* Geometry::convertToPOINTs(const PointArray& points)
	{
		POINT* pointArray = new POINT[points.getSize()];
		for (unsigned i = 0; i < points.getSize(); ++i)
			pointArray[i].x = points.getPoint(i).getX(),
			pointArray[i].y = points.getPoint(i).getY();

		return pointArray;
	}

	///////////////////////////////////////

	void Geometry::drawPolyline(PointArray points)
	{
		//	polyline -> point number greater equal than 2
		if (points.getSize() < 2)
			throw EasyExcept("System call error!");

		POINT* pointArray = convertToPOINTs(points);
		BOOL ret = Polyline(
			BaseWindow::getDefaultGeometryAttribute()->hdc, pointArray, points.getSize());

		delete[] pointArray;
		if (ret == 0)	throw EasyExcept("System call error!");

		//	in GDI, the last point is not drew. Use drawLine() bacause drawDot()
		//	uses different drawing color
		const Point& lastPoint = points.getPoint(points.getSize() - 1);
		Geometry::drawLine(lastPoint, lastPoint);
	}

	/////////////////////////////////////////

	void Geometry::drawChord(RectRegion bound, Point start, Point end)
	{
		checkTwoEndPoints(bound, start, end);

		BOOL ret = Chord(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			bound.getLeftTop().getX(),
			bound.getLeftTop().getY(),
			bound.getRightBottom().getX(),
			bound.getRightBottom().getY(),
			start.getX(),
			start.getY(),
			end.getX(),
			end.getY());
		if (ret == 0)	throw EasyExcept("System call error!");
	}

	////////////////////////////////////////

	void Geometry::drawEllipse(RectRegion bound)
	{
		BOOL ret = Ellipse(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			bound.getLeftTop().getX(),
			bound.getLeftTop().getY(),
			bound.getRightBottom().getX(),
			bound.getRightBottom().getY());

		if (ret == 0)	throw EasyExcept("System call error!");
	}

	//////////////////////////////////////////

	void Geometry::drawRectangle(RectRegion rectRegion)
	{
		BOOL ret = Rectangle(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			rectRegion.getLeftTop().getX(),
			rectRegion.getLeftTop().getY(),
			rectRegion.getRightBottom().getX(),
			rectRegion.getRightBottom().getY());
		if (ret == 0)	throw EasyExcept("System call error!");

		Geometry::drawLine(
			Point(rectRegion.getLeftTop().getX(), rectRegion.getRightBottom().getY()),
			Point(rectRegion.getRightBottom()));
		Geometry::drawLine(
			Point(rectRegion.getRightBottom().getX(), rectRegion.getLeftTop().getY()),
			Point(rectRegion.getRightBottom()));
	}

	///////////////////////////////////////////

	void Geometry::drawRoundRectangle(RectRegion rectRegion, Rect rectangle)
	{
		unsigned regionWidth = 
			rectRegion.getRightBottom().getX() - rectRegion.getLeftTop().getX();
		unsigned regionHeight =
			rectRegion.getRightBottom().getY() - rectRegion.getLeftTop().getY();

		if (rectangle.getWidth() > regionWidth ||
			rectangle.getHeight() > regionHeight)

			throw EasyExcept("Rect invalid!");

		BOOL ret = RoundRect(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			rectRegion.getLeftTop().getX(),
			rectRegion.getLeftTop().getY(),
			rectRegion.getRightBottom().getX(),
			rectRegion.getRightBottom().getY(),
			rectangle.getWidth(),
			rectangle.getHeight());

		if (ret == 0)	throw EasyExcept("System call error!");
	}

	///////////////////////////////////////////

	void Geometry::drawPie(RectRegion bound, Point start, Point end)
	{
		checkTwoEndPoints(bound, start, end);

		BOOL ret = Pie(
			BaseWindow::getDefaultGeometryAttribute()->hdc,
			bound.getLeftTop().getX(),
			bound.getLeftTop().getY(),
			bound.getRightBottom().getY(),
			bound.getRightBottom().getY(),
			start.getX(),
			start.getY(),
			end.getX(),
			end.getY());
		if (ret == 0)	throw EasyExcept("System call error!");
	}

	/////////////////////////////////////////////

	void Geometry::drawPolygon(PointArray points)
	{
		if (points.getSize() < 3)
			throw EasyExcept("System call error!");

		POINT* pointArray = convertToPOINTs(points);
		BOOL ret = Polygon(
			BaseWindow::getDefaultGeometryAttribute()->hdc, pointArray, points.getSize());

		delete[] pointArray;
		if (ret == 0)	throw EasyExcept("System call error!");
	}

}