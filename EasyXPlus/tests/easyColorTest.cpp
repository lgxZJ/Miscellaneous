#include "..\src\easyColor.h"
#include <cassert>

using namespace EasyXPlus;

////////////////////////////////////////////////////////////////////////////
//								Faker

class FakeRgb : public Rgb
{
public:

	FakeRgb(Rgb rgb) : Rgb(rgb)	{}
	virtual COLORREF toColorref() const { return colorValue; }
};

////////////////////////////////////////////////////////////////////////////
//								Tests

void Ctor_ByDefault_ConstructColorValue()
{
	FakeRgb fakeRgb(Rgb{ 12, 34, 56 });

	assert( fakeRgb.toColorref() == RGB(12, 34, 56));
}

void Getters_ByDefault_GetRGBValue()
{
	const unsigned char R = 12, G = 90, B = 34;
	Rgb rgb(R, G, B);

	assert(R == rgb.getR());
	assert(G == rgb.getG());
	assert(B == rgb.getB());
}