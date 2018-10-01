#ifndef SAMPLE_APP_H
#define SAMPLE_APP_H

#include "easyKeyboard.h"
#include "easyGeometry.h"
#include "easyMouse.h"

#include <array>
#include <map>

using namespace EasyXPlus;

class App
{
public:
	explicit App();

	void exec();

private:
	using StringCollection = std::vector<std::wstring>;
	using MemberFuncPtr = void (App::*)(const EasyXPlus::Rgb& color);

	void drawStringInRect(const EasyXPlus::RectRegion& region, StringCollection strs);
	void drawRect(const EasyXPlus::RectRegion& region);
	void drawStrings(const EasyXPlus::RectRegion& region, StringCollection strs);
	void drawOneString(EasyXPlus::Point* leftTop, const EasyXPlus::Rect& stirngRect, std::wstring str);
	void applyPaddings(EasyXPlus::Point* point);
	void adjustToCenter(Point* point, std::wstring str, Rect boundingRect);
	unsigned findAppropriateFontSize(EasyXPlus::Rect rect, std::wstring str);

	void drawMouseBlocks();
	void drawLeftButtonBlock(const Rgb& fillColor);
	void drawRightButtonBlock(const Rgb& fillColor);
	void drawMoveMouseBlock(const Rgb& fillColor);

	void drawPressedKeyBlock(Key keyPressed);
	void drawKeyBlockWithColor(Key keyPressed, Rgb color);
	void drawClickedMouseBlocks(EasyXPlus::MouseEvent event);
	void drawUserEvent(MemberFuncPtr func);

private:
	const unsigned	windowWidth;
	const unsigned	windowHeight;
	const unsigned	horizontalCellNum;
	const unsigned	verticalCellNum;
	const unsigned	paddingPixel;
	const unsigned	marginPixel;
	const unsigned  durationOfHighlight;

	const EasyXPlus::Rgb defaultLineColor;
	const EasyXPlus::Rgb defaultFillColor;
	const EasyXPlus::Rgb defaultTextColor;
	const EasyXPlus::Rgb highlightFillColor;

	unsigned		cellUnit;

	const std::array<unsigned, 84> cellLengths = {
		1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
		1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
		1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
		1,1,1,1,1, 1,1,1,1,1, 1,1,2,1,
		2,1,1,1,   1,1,1,1,1, 1,1,1,1,1,
		1,1,1,1,		 5,1, 1,1,1,1,1
	};
	const std::array<StringCollection, 84> stringCollections = {
		StringCollection{ L"Esc" },	{ L"F1" },	{ L"F2" },	{ L"F3" },	{ L"F4" },	{ L"F5" },	{ L"F6" },	{ L"F7" },
		{ L"F8" },	{ L"F9" },	{ L"F10" },	{ L"F11" },	{ L"F12" },	{ L"PrtSc" },	{ L"Del" },

		{ L"~",	L"`" },	{ L"!", L"1" },	{ L"@", L"2" },	{ L"#", L"3" },	{ L"$", L"4" },	{ L"%", L"5" },
		{ L"^", L"6" },	{ L"&", L"7" },	{ L"*", L"8" },	{ L"(", L"9" },	{ L")", L"0" },	{ L"_", L"-" },
		{ L"+", L"=" },	{ L"Back" },	{ L"Home" },
		
		{ L"Tab" },	{ L"Q" },	{ L"W" },	{ L"E" },	{ L"R" },	{ L"T" },	{ L"Y" },	{ L"U" },
		{ L"I" },	{ L"O" },	{ L"P" },	{ L"{", L"[" },	{ L"}", L"]" },	{ L"|", L"\\" },	{ L"End" },
		
		{ L"CapsLk" },	{ L"A" },	{ L"S" },	{ L"D" },	{ L"F" }, { L"G" },	{ L"H" },	{ L"J" },
		{ L"K" },	{ L"L" },	{ L":", L";" },	{ L"\"", L"'" },	{ L"Enter" },	{ L"PgUp" },

		{ L"Shift" },	{ L"Z" },	{ L"X" },	{ L"C" },	{ L"V" },	{ L"B" },	{ L"N" },	{ L"M" },
		{ L"<", L"," },	{ L">", L"." },	{ L"?", L"/" },	{ L"Shift" },	{ L"¡ü" },	{ L"PgDn" },

		{ L"Ctrl" },	{ L"Fn" },	{ L"Win" },	{ L"Alt" },	{ L"SpaceBar" },	{ L"Alt" },	{ L"Menu" },
		{ L"Ctrl" },	{ L"¡û" },	{ L"¡ý" },	{ L"¡ú" }
	};
	const std::array<EasyXPlus::Key, 84> keys = {
		Key::KeyEsc,		Key::KeyNotSupport, Key::KeyNotSupport, Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyNotSupport, Key::KeyNotSupport,	Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyNotSupport, Key::KeyNotSupport,	Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyNotSupport, Key::KeyNotSupport,

		Key::KeyNotSupport,	Key::Key1,	Key::Key2,	Key::Key3,	Key::Key4,	Key::Key5,
		Key::Key6,	Key::Key7,	Key::Key8,	Key::Key9,	Key::Key0, Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyBackspace,	Key::KeyNotSupport,

		Key::KeyTab,	Key::KeyQ,	Key::KeyW,	Key::KeyE,	Key::KeyR,	Key::KeyT,	Key::KeyY,
		Key::KeyU,		Key::KeyI,	Key::KeyO,	Key::KeyP,	Key::KeyNotSupport,	Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyNotSupport,

		Key::KeyNotSupport,	Key::KeyA,	Key::KeyS,	Key::KeyD,	Key::KeyF,	Key::KeyG,
		Key::KeyH,	Key::KeyJ,	Key::KeyK,	Key::KeyL,	Key::KeyNotSupport,
		Key::KeyNotSupport,	Key::KeyEnter,	Key::KeyNotSupport,

		Key::KeyShift,	Key::KeyZ,	Key::KeyX,	Key::KeyC,	Key::KeyV,	Key::KeyB,	Key::KeyN,
		Key::KeyM,	Key::KeyNotSupport,	Key::KeyNotSupport,	Key::KeyNotSupport,	Key::KeyShift,
		Key::KeyUpArrow,	Key::KeyNotSupport,

		Key::KeyCtrl,	Key::KeyNotSupport,	Key::KeyNotSupport,	Key::KeyNotSupport,	Key::KeySpace,
		Key::KeyNotSupport,	Key::KeyNotSupport,	Key::KeyCtrl,	Key::KeyLeftArrow,	Key::KeyDownArrow,
		Key::KeyRightArrow
	};
};

#endif
