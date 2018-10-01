#include "easyBitmap.h"
#include "easyExcept.h"
#include "easyBaseWindow.h"

using namespace std;

namespace EasyXPlus
{
	//////////////////////////////////////////////////////////////////////
	//							Bitmap

	Bitmap::Bitmap(wstring bitmapFilename)
		: isVerticalFlip(false)
		, isHorizontalFlip(false)
	{
		bitmapHandle = (HBITMAP)LoadImageW(NULL, bitmapFilename.c_str(),
								 IMAGE_BITMAP,
								 0, 0,
								 LR_LOADFROMFILE);
		if (bitmapHandle == NULL)
			throw EasyExcept("LoadImage error");

		if (0 == GetObject((HGDIOBJ)bitmapHandle, sizeof(bitmapStruct), &bitmapStruct))
			throw EasyExcept("GetObject error");

		memoryDC = CreateCompatibleDC(BaseWindow::getDefaultGeometryAttribute()->hdc);
		if (memoryDC == NULL)
			throw EasyExcept("CreateCompatibleDC error");
		if (SelectObject(memoryDC, (HGDIOBJ)bitmapHandle) == NULL)
			throw EasyExcept("CreateCompatibleDC error");
	}

	//////////////////////////

	Bitmap::~Bitmap()
	{
		if (bitmapHandle != NULL)
			DeleteObject((HGDIOBJ)bitmapHandle);
		if (memoryDC != NULL)
			DeleteDC(memoryDC);
	}

	/////////////////////////

	unsigned Bitmap::getWidth() const
	{
		return bitmapStruct.bmWidth;
	}

	/////////////////////////

	unsigned Bitmap::getHeight() const
	{
		return bitmapStruct.bmHeight;
	}

	/////////////////////////

	void Bitmap::verticalFlip()
	{
		isVerticalFlip = !isVerticalFlip;
	}

	/////////////////////////

	void Bitmap::horizontalFlip()
	{
		isHorizontalFlip = !isHorizontalFlip;
	}

	/////////////////////////

	void Bitmap::resetFlip()
	{
		isVerticalFlip = false;
		isHorizontalFlip = false;
	}

	/////////////////////////

	void Bitmap::draw(const Point& leftTop, int width, int height)
	{
		if (width <= 0 || height <= 0)
			throw EasyExcept("width or height invalid");

		draw_(leftTop.getX(), leftTop.getY(), width, height);
	}

	void Bitmap::draw(const Point& leftTop, const Rect& rect)
	{
		draw_(leftTop.getX(), leftTop.getY(), rect.getWidth(), rect.getHeight());
	}

	void Bitmap::draw(const RectRegion& region)
	{
		draw_(region.getLeftTop().getX(),
			  region.getLeftTop().getY(),
			  region.getRightBottom().getX() - region.getLeftTop().getX(),
			  region.getRightBottom().getY() - region.getLeftTop().getY());
	}

	///////////////////////////

	void Bitmap::draw_(int x, int y, int width, int height)
	{
		if (isVerticalFlip)
		{
			y = (y + height - 1);	//	0-line not changed when flipping
			height = -height;
		}
		if (isHorizontalFlip)
		{
			x = (x + width - 1);	//	0-column not changed when flipping
			width = -width;
		}

		if (0 == StretchBlt(BaseWindow::getDefaultGeometryAttribute()->hdc,
							x, y, width, height,
							memoryDC,
							0, 0, getWidth(), getHeight(),
							SRCCOPY))
			throw EasyExcept("StretchBlt error");
	}
}