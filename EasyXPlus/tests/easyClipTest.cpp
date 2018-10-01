#include "..\src\easyClip.h"
#include "..\src\easyWindow.h"
#include "..\src\easyExcept.h"

#include "SU.h"
#include <cassert>

using namespace EasyXPlus;

/////////////////////////////	Mocks	////////////////////////////////////////

class MockRectClipRegion : public RectClipRegion
{
public:
	explicit MockRectClipRegion(const RectClipRegion& region)
		: RectClipRegion(region)
	{
	}

	HRGN getRgnHandle()	{ return regionHandle; }
	unsigned getActionSize() { return actions.size(); }
};

/////////////////////////////    Tests  ////////////////////////////////////////

void Ctor_ByDefault_CreateRegionHandle()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	MockRectClipRegion mockRegion(region);

	assert(mockRegion.getRgnHandle() != NULL);
}

void Ctor_ByDefault_ZeroActions()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	MockRectClipRegion mockRegion(region);

	assert(mockRegion.getActionSize() == 0);
}

void Exclude_ByDefault_AddOneAction()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	region.exclude(RectRegion(Point(0, 0), Point(100, 100)));
	unsigned actionSizeBefore = MockRectClipRegion(region).getActionSize();

	region.exclude(RectRegion(Point(0, 0), Point(100, 100)));

	unsigned actionSizeAfter = MockRectClipRegion(region).getActionSize();
	assert((actionSizeAfter = actionSizeBefore) == 1);
}

void Intersect_ByDefault_AddOneAction()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	region.intersect(RectRegion(Point(0, 0), Point(100, 100)));
	unsigned actionSizeBefore = MockRectClipRegion(region).getActionSize();

	region.intersect(RectRegion(Point(0, 0), Point(100, 100)));

	unsigned actionSizeAfter = MockRectClipRegion(region).getActionSize();
	assert((actionSizeAfter = actionSizeBefore) == 1);
}

void Union_ByDefault_AddOneAction()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	region.unionn(RectRegion(Point(0, 0), Point(100, 100)));
	unsigned actionSizeBefore = MockRectClipRegion(region).getActionSize();

	region.unionn(RectRegion(Point(0, 0), Point(100, 100)));

	unsigned actionSizeAfter = MockRectClipRegion(region).getActionSize();
	assert((actionSizeAfter = actionSizeBefore) == 1);
}

void Move_ByDefault_AddOneAction()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));
	region.move(12, 12);
	unsigned actionSizeBefore = MockRectClipRegion(region).getActionSize();

	region.move(12, 12);

	unsigned actionSizeAfter = MockRectClipRegion(region).getActionSize();
	assert((actionSizeAfter = actionSizeBefore) == 1);
}

void Clear_ByDefault_SetActionsAndHandleToNull()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.clear();

	MockRectClipRegion mockRegion(region);
	assert (mockRegion.getRgnHandle() == NULL);
	assert (mockRegion.getActionSize() == 0);
}

void Set_ByDefault_CreateNewRgnHandle()
{
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.set(RectRegion(Point(0, 0), Point(1, 1)));

	MockRectClipRegion mockRegion(region);
	assert (mockRegion.getRgnHandle() != NULL);
}

void AssociateToWindow_ByDefault_NotThrowExcept()
{
	Window::init(L"asa", 0, 0);
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.associateToWindow();
}

void AssociateToWindow_AfterSet_NotThrowExcept()
{
	Window::init(L"asa", 0, 0);
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.set(RectRegion(Point(0, 0), Point(12, 12)));

	region.associateToWindow();
}

void AssociateToWindow_AfterClear_ThrowExcept()
{
	Window::init(L"asa", 0, 0);
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.clear();

	SU_ASSERT_THROW(region.associateToWindow(), EasyExcept);
}

void CancelAssociation_AfterClear_ThrowExcept()
{
	Window::init(L"asa", 0, 0);
	RectClipRegion region(RectRegion(Point(0, 0), Point(100, 100)));

	region.clear();

	SU_ASSERT_THROW(region.cancelAssociation(), EasyExcept);
}