#include "..\src\easyBitmap.h"
#include "..\src\easyExcept.h"

#include "SU.h"

using namespace EasyXPlus;

////////////////////////////////	Mocks	////////////////////////////////

class MockBitmap : public Bitmap
{
public:
	explicit MockBitmap(const Bitmap& bitmap)
		: Bitmap(bitmap)
	{}

	HBITMAP getBitmapHandle() { return bitmapHandle; }
	HDC getInnerDC() { return memoryDC; }
};

////////////////////////////////	Tests	/////////////////////////////////

void Ctor_ByDefault_InitMembers()
{
	Bitmap bitmap(L"pic.bmp");

	MockBitmap mock(bitmap);
	assert ( mock.getBitmapHandle() != NULL);
	assert ( mock.getInnerDC() != NULL);
}

void Getters_ByDefault_ReturnBitmapSize()
{
	//	it's the size of bitmap "pic.bmp"
	unsigned expectedWidth = 593;
	unsigned expectedHeight = 222;

	Bitmap bitmap(L"pic.bmp");

	assert(bitmap.getWidth() == expectedWidth);
	assert(bitmap.getHeight() == expectedHeight);
}

void DrawThreeParam_PassAnyNoPositiveParam_ThrowExcept()
{
	Bitmap bitmap(L"pic.bmp");
	
	SU_ASSERT_THROW( bitmap.draw(Point(0, 0), -1, 90), EasyExcept );
	SU_ASSERT_THROW(bitmap.draw(Point(0, 0), 0, 90), EasyExcept);
	SU_ASSERT_THROW(bitmap.draw(Point(0, 0), 90, -1), EasyExcept);
	SU_ASSERT_THROW(bitmap.draw(Point(0, 0), 90, 0), EasyExcept);
	SU_ASSERT_THROW(bitmap.draw(Point(0, 0), -1, 0), EasyExcept);
}