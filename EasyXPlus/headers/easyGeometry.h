///
///	@file		Geometry module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/12
///	@version	1.0
///

#ifndef EASY_GEOMETRY_H
#define EASY_GEOMETRY_H

#include "easyColor.h"
#include <vector>

namespace EasyXPlus
{
	///
	///	@brief	A point class.
	///
	class Point
	{
	public:
		///
		///	@brief	Creates a point with the given x and y coordinates.
		///
		///			The coordinates are relative to the left-top point of
		///			drawable area of the default window. The X-axis grows
		///			rightwards and the Y-axis grows downwards.
		///
		Point(unsigned X, unsigned Y) : x(X), y(Y)
		{}

///	@{
		///
		///	@brief	Compares if two points are equal.
		///
		bool operator == (const Point& rhs)const;
		///
		///	@sa	operator==()
		///
		bool operator != (const Point& rhs)const;
///	@}

///	@{
		int getX() const	{ return x; }
		int getY() const	{ return y; }
///	@}

	private:
		unsigned x, y;
	};

	///
	///	@brief	A class represents a rectangle area.
	///
	class RectRegion
	{
	public:
		///
		///	@brief	Creates a valid rectangle area with the given left-top point and
		///			right-bottom point.
		///
		///			A Rect-Region is a rectangular area in the coordinate system.
		///	@throw	EasyExcept	If the left-top point is "non-lefter" or "non-topper"
		///			than the right-bottom point, it throws an exception.
		///
		RectRegion(Point leftTopPoint, Point rightBottomPoint);

///	@{
		Point getLeftTop() const		{ return leftTop; }
		Point getRightBottom() const	{ return rightBottom; }
		Point getCenter() const;
///	@}

	private:
		Point leftTop, rightBottom;
	};

	///
	///	@brief	A class represents rectangle.
	///
	//	
	class Rect
	{
	public:
		///
		///	@brief	Creates a rectangle with the given width and height.
		///
		///			A rectangle is an "RectRegion" without coordinate information.
		///			Thus, it is only limited by width and height.
		///
		Rect(unsigned width, unsigned height) : width(width), height(height)
		{}

///	@{
		unsigned getWidth() const	{ return width; }
		unsigned getHeight() const	{ return height; }
///	@}

	private:
		unsigned width, height;
	};

	///
	///	@brief	A class holding some points.
	///
	class PointArray
	{
	public:
		///
		///	@brief	Creates a point array with the given points.
		///
		PointArray(std::initializer_list<Point> points);

///	@{
		bool isEmpty() const;
		unsigned getSize() const;
///	@}

///
///	@name	Point operations by index, index ranges from 0 to "size - 1".
///			If the index parameter is invalid(out of range), then these
///			functions throw an exception.
///
///	@{
		///
		///	@brief	Get the point with the given index.
		///	@throw	EasyExcept If the index is out of range, it throws an exception.
		///
		Point getPoint(unsigned index) const;
		///
		///	@brief	Append the given point at the end of points.
		///	@throw	EasyExcept If the index is out of range, it throws an exception.
		///
		void appendPoint(Point point);
		///
		///	@brief	Erase a point with the given index.
		///	@throw	EasyExcept If the index is out of range, it throws an exception.
		///
		void erasePoint(unsigned index);
		///
		///	@brief	Assign the point with the given index to the new point.
		///	@throw	EasyExcept If the index is out of range, it throws an exception.
		///
		void assignPoint(unsigned index, Point point);
		///
		///	@brief	Insert a point before the given index.
		///	@param	indexBefore	An index before which the new point is inserted.
		///	@param	point		A point.
		///	@throw	EasyExcept If the index is out of range, it throws an exception.
		///
		void insertPoint(unsigned indexBefore, Point point);
		///
		///	@brief	Clear all points.
		///
		void clearAll();
///	@}

	private:
		std::vector<Point>	points;
	};

	///
	///	@brief	A class providing geometry operations.
	///
	///			When doing geometry operations, you need to know three common attributes:
	///				Color Types		|	Related Operations
	///				-----------	  	|	------------
	///				Dot color		|	doing "Dot" Operations.
	///				Line color		|	doing "Line" Operations such as drawing lines and the outer lines of shapes.
	///				Fill color		|	doing "Filled Shape" Operations such as drawing rectangles and ellipses.
	///
	class Geometry
	{
	public:
///	@{
		static Rgb getCurrentDotColor();
		static Rgb getCurrentLineColor();
		static Rgb getCurrentFillColor();
		static void setDotColor(const Colorable& color);
		static void setLineColor(const Colorable& color);
		static void setFillColor(const Colorable& color);
///	@}

		///
		///	@brief Get the Rgb color in the given point.
		///
		static Rgb getPointColor(Point point);

///
///	@name	Dot Operations. Dot operations draw themselves using the dot color set.
///
///	@{
		///
		///	@brief	Draw a dot at the given point with the dot color set.
		///
		static void drawDot(Point point);
///	@}

///
///	@name	Line Operations. Line operations draw themselves using the line color set.
///
///	@{
		///
		///	@brief	Draw a line which starts from the "from" point and ends at the
		///			"to" point.
		///
		static void drawLine(Point from, Point to);
		///
		///	@brief	Draw an arc which is bounded in the "bound" region, starts from
		///			the "start" point and ends at the "end" point in clockwise order.
		///	@param	bound	A region bounding the arc.
		///	@param	start	A point which forms a radial with region center together.
		///	@param	end		A point which forms another radial with region center together.
		///	@remark	The region "bound" specify the bounding rectangle. An ellipse formed
		///			by the specified bounding rectangle defines the curve of the arc. The
		///			arc extends in the current drawing direction from the point where it
		///			intersects the radial from the center of the bounding rectangle to the
		///			"start" point. The arc ends where it intersects the radial from the
		///			center of the bounding rectangle to the "end" point. If the starting
		///			point and ending point are the same, a complete ellipse is drawn.
		///	@throw	If one point is the region center and another not, than it throws an
		///			exception.
		///
		static void drawArc(RectRegion bound, Point start, Point end);
		///
		///	@brief	Draw some bezier curves.
		///	@param	points	Some points from which the bezier curves are drawn. The 
		///			number of points must be one more than three times the number of
		///			curves to be drawn, because each Bézier curve requires two control
		///			points and an endpoint, and the initial curve requires an additional
		///			starting point.
		///	@throw 	EasyExcept	If the point number is invalid, it throws an exception.
		///
		static void drawBeziers(PointArray points);
		///
		///	@brief	Draw some polylines.
		///	@param	points	Some endpoints from which the polyline is drawn. The number
		///			of points must be greater or equal than 2.
		///	@throw EasyExcept	If the point number is less than 2, it throws an exception.
		///
		static void drawPolyline(PointArray points);
///	@}

///
///	@name	Filled Shape Operations. Fill operations draw themselves using the line
///			color and fill color set.
///
///	@{
		///
		///	@brief	Draw a chord which is bounded in the "bound" region, starts from
		///			the "start" point and ends at the "end" point in clockwise order.
		///	@param	bound	A region bounding the chord.
		///	@param	start	A point which forms a radial with region center together.
		///	@param	end		A point which forms another radial with region center together.
		///	@remark	The region "bound" specify the bounding rectangle. An ellipse formed
		///			by the specified bounding rectangle defines the curve of the arc. The
		///			arc extends in the current drawing direction from the point where it
		///			intersects the radial from the center of the bounding rectangle to the
		///			"start" point. The arc ends where it intersects the radial from the
		///			center of the bounding rectangle to the "end" point. If the starting
		///			point and ending point are the same, a complete ellipse is drawn.
		///	@throw	If one point is the region center and another not, than it throws an
		///			exception.
		///
		static void drawChord(RectRegion bound, Point start, Point end);

		///
		///	@brief	Draw an ellipse with the given bound.
		///
		static void drawEllipse(RectRegion bound);
		///
		///	@brief	Draw a rectangle with the given rectangle region.
		///
		static void drawRectangle(RectRegion rectRegion);
		///
		///	@brief	Draw a round rectangle with the given rectangle region and
		///			round rectangle.
		///	@param	rectRegion		A rectangle region which bounds the rectangle.
		///	@param	ellipseBBound	A rectangle which bounds the round part of the rectangle.
		///	@throw	EasyExcept	If the "rectRegion" is not bigger than the "ellipseBound",
		///			it throws an exception.
		///
		static void drawRoundRectangle(RectRegion rectRegion, Rect ellipseBound);
		///
		///	@brief	Draw a pie with the given bound region, start point and end point.
		///	@sa		drawChord()
		///
		static void drawPie(RectRegion bound, Point start, Point end);
		///
		///	@brief	Draw a polygon with the given points.
		///
		///			Polygons are similar to polylines except that polygons are filled
		///			shapes. If the first point and the last point is not the same point,
		///			then a line formed by these two points will be drawn.
		///	@param	points	The points to form the polygon, the number of points must
		///			be greater or equal to 3.
		///	@throw	If the number of points is invalid, it throws an exception.
		///
		static void drawPolygon(PointArray points);
///	@}

	private:
		static POINT* convertToPOINTs(const PointArray& points);
		static void checkTwoEndPoints(const RectRegion& bound, const Point& lhs, const Point& rhs);
	};
}

#endif