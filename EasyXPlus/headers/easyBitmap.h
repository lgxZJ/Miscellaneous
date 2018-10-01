///
///	@file		Bitmap module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/12
///	@version	1.0
///

#ifndef EASY_BITMAP_H
#define EASY_BITMAP_H

#include "easyGeometry.h"

#include <string>

namespace EasyXPlus
{
	///
	///	@brief	A bitmap class.
	///
	class Bitmap
	{
	public:
		///
		///	@brief	Creates a bitmap from the given filename.
		///	@throw	EasyExcept	If an error occurs, it throws an exception. The error
		///			is most probably caused by a invalid bitmap filename.
		///
		explicit Bitmap(std::wstring bitmapFilename);
		~Bitmap();

///	@{
		///
		///	@brief Get bitmap width in pixel unit.
		///
		unsigned getWidth() const;
		///
		///	@brief Get bitmap height in pixel unit.
		///
		unsigned getHeight() const;
///	@}

///	@{
		///
		///	@brief	Flip the bitmap vertically.
		///
		void verticalFlip();
		///
		///	@brief	Flip the bitmap horizontally.
		///
		void horizontalFlip();
		///
		///	@brief	Reset(clear) all flip actions.
		///
		void resetFlip();
///	@}

///	@{
		///
		///	@brief	Draw the bitmap in the given region.
		///
		void draw(const RectRegion& region);
		///
		///	@brief	Draw the bitmap with the given left-top point and rectangle.
		///	@param	leftTop	The left-top point from which the bitmap starts to be drawn.
		///	@param	rect	A rectangle which points out the size of the drawing
		///			size(width and height).
		///
		void draw(const Point& leftTop, const Rect& rect);
		///
		///	@brief	Draw the bitmap with the given left-top point, width and height.
		///	@param	leftTop	The left-top point from which the bitmap starts to be drawn.
		///	@param	width	A positive width with which the bitmap is to be drawn.
		///	@param	height	A positive height with which the bitmap is to be drawn.
		///	@throw	EasyExcept	If width or height passed is negative, it throws an exception.
		///
		void draw(const Point& leftTop, int width, int height);
///	@}

	private:
		void draw_(int x, int y, int width, int height);

	protected:
		HBITMAP bitmapHandle;
		BITMAP bitmapStruct;
		HDC memoryDC;

		bool isVerticalFlip;
		bool isHorizontalFlip;
	};
}

#endif