#include "easyClip.h"
#include "easyExcept.h"
#include "easyBaseWindow.h"

using namespace std;

namespace EasyXPlus
{
    ///////////////////////////////////////////////////////////////////////
    //                      RectClipRegion

	RectClipRegion::ClipUnion::ClipUnion(const RectRegion& region)
		: region(region)
	{
	}

	RectClipRegion::ClipUnion::ClipUnion(int offsetX, int offsetY)
		: offsets{ offsetX, offsetY }
	{
	}

	RectClipRegion::ClipUnion::ClipUnion(int scale)
		: scaleFactor(scale)
	{
	}

	/////////////////////////////

	RectClipRegion::RectClipRegion(const RectRegion& region)
	{
		regionHandle = createRgnFromRegion(region);
	}

	RectClipRegion::~RectClipRegion()
	{
		if (regionHandle != NULL)
		{
			SelectClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc, NULL);
			DeleteObject((HGDIOBJ)regionHandle);
		}
	}

	/////////////////////////////

	void RectClipRegion::exclude(const RectRegion& region)
	{
		actions.push_back(make_pair(ClipType::Exclude, ClipUnion(region)));
	}

	///////////////////////////

	void RectClipRegion::intersect(const RectRegion& region)
	{
		actions.push_back(make_pair(ClipType::Intersect, ClipUnion(region)));
	}

	/////////////////////////////

	void RectClipRegion::unionn(const RectRegion& region)
	{
		actions.push_back(make_pair(ClipType::Union, ClipUnion(region)));
	}

	///////////////////////////

	void RectClipRegion::move(int horizontalOffset, int verticalOffset)
	{
		actions.push_back(make_pair(ClipType::Move, ClipUnion(horizontalOffset, verticalOffset)));
	}

	//////////////////////////

	void RectClipRegion::set(const RectRegion& region)
	{
		clear();
		regionHandle = createRgnFromRegion(region);
	}

	//////////////////////////

	void RectClipRegion::clear()
	{
		if (regionHandle != NULL)
			DeleteObject((HGDIOBJ)regionHandle);

		regionHandle = NULL;
		actions.clear();
	}

	/////////////////////////

	void RectClipRegion::associateToWindow()
	{
		if (regionHandle == NULL)
			throw EasyExcept("clip region not set!");

		if (ERROR == SelectClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc, regionHandle))
			throw EasyExcept("SelectClipRgn error");

		for (auto actionPair : actions)
		{
			switch (actionPair.first)
			{
			case ClipType::Exclude:		excludeRegion(actionPair.second.region);	break;
			case ClipType::Intersect:	intersectRegion(actionPair.second.region);	break;
			case ClipType::Move:		moveRegion(actionPair.second.offsets);		break;
			case ClipType::Union:		unionRegion(actionPair.second.region);		break;
			default:					throw EasyExcept("Not supported ClipType");
			}
		}
	}

	//////////////////////////

	void RectClipRegion::cancelAssociation()
	{
		if (regionHandle == NULL)
			throw EasyExcept("clip region not set!");

		if (ERROR == SelectClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc, NULL))
			throw EasyExcept("SelectClipRgn error");
	}

	//////////////////////////

	void RectClipRegion::excludeRegion(const RectRegion& region)
	{
		//	in GDI, th lower and right edges of the specified rectangle are
		//	not excluded from the clipping region, add 1 here to make it
		//	excluded
		if (ERROR == ExcludeClipRect(BaseWindow::getDefaultGeometryAttribute()->hdc,
									 region.getLeftTop().getX(),
									 region.getLeftTop().getY(),
									 region.getRightBottom().getX() + 1,
									 region.getRightBottom().getY() + 1))
			throw EasyExcept("ExcludeClipRect error");
	}
    
	////////////////////////////

	void RectClipRegion::intersectRegion(const RectRegion& region)
	{
		if (ERROR == IntersectClipRect(BaseWindow::getDefaultGeometryAttribute()->hdc,
									   region.getLeftTop().getX(),
									   region.getLeftTop().getY(),
									   region.getRightBottom().getX() + 1,
									   region.getRightBottom().getY() + 1))
			throw EasyExcept("IntersectClipRect error");
	}

	/////////////////////////////

	void RectClipRegion::moveRegion(int offsets[2])
	{
		if (ERROR == OffsetClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc,
								   offsets[0],
								   offsets[1]))
			throw EasyExcept("OffsetClipRgn error");
	}

	/////////////////////////////

	void RectClipRegion::unionRegion(const RectRegion& region)
	{
		HRGN currentRegion = createRgnFromRegion(region);
		if (1 != GetClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc, currentRegion))
			throw EasyExcept("GetClipRgn error");

		HRGN paramRegion = createRgnFromRegion(region);
		HRGN resultRegion = createRgnFromRegion(region);
		if (ERROR == CombineRgn(resultRegion, currentRegion, paramRegion, RGN_OR))
			throw EasyExcept("CombineRgn error");

		if (ERROR == SelectClipRgn(BaseWindow::getDefaultGeometryAttribute()->hdc, resultRegion))
			throw EasyExcept("SelectClipRgn error");

		DeleteObject((HGDIOBJ)currentRegion);
		DeleteObject((HGDIOBJ)paramRegion);
		DeleteObject((HGDIOBJ)resultRegion);
	}

	/////////////////////////////

	HRGN RectClipRegion::createRgnFromRegion(const RectRegion& region)
	{
		//	in GDI, th lower and right edges of the specified rectangle are
		//	not excluded from the clipping region, add 1 here to make it
		//	excluded
		HRGN ret = CreateRectRgn(region.getLeftTop().getX(),
								 region.getLeftTop().getY(),
								 region.getRightBottom().getX() + 1,
								 region.getRightBottom().getY() + 1);
		if (ret == NULL)
			throw EasyExcept("CreateRectRgn error");

		return ret;
	}
}