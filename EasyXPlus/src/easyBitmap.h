#ifndef EASY_BITMAP_H
#define EASY_BITMAP_H

#include "easyGeometry.h"

#include <string>

namespace EasyXPlus
{
	class Bitmap
	{
	public:
		explicit Bitmap(std::wstring bitmapFilename);
		~Bitmap();

		unsigned getWidth() const;
		unsigned getHeight() const;

		void verticalFlip();
		void horizontalFlip();
		void resetFlip();

		void draw(const RectRegion& region);
		void draw(const Point& leftTop, const Rect& rect);
		void draw(const Point& leftTop, int width, int height);

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