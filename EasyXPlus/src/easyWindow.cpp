#include "easyBaseWindow.h"
#include "easyWindow.h"
#include "easyExcept.h"

#include <cassert>
#include <climits>	//	for INT_MAX

using namespace std;

//	it will be used only after its initialization
extern EasyXPlus::BaseWindow* g_baseWindow;

//	forwards
LRESULT CALLBACK EasyWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace EasyXPlus
{
	/////////////////////////////////////////////////////////////////////////////////////
	//								Static BaseWindow variables

	bool BaseWindow::registered = false;
	BaseWindow* BaseWindow::defaultBaseWindow = nullptr;
	HWND BaseWindow::defaultWindowHandle = NULL;
	BaseWindow::TextAttribute* BaseWindow::defaultTextAttribute = nullptr;
	BaseWindow::GeometryAttribute* BaseWindow::defaultGeometryAttribute = nullptr;

	/////////////////////////////////////////////////////////////////////////////////////
	//								Static BaseWindow functions

	BaseWindow* BaseWindow::getDefaultBaseWindow()
	{
		Window::checkIfInitialized();

		if (defaultBaseWindow == nullptr)
			throw EasyExcept("No default window set!");

		return defaultBaseWindow;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	HWND BaseWindow::getDefaultWindowHandle()
	{
		Window::checkIfInitialized();

		if (defaultWindowHandle == NULL)
			throw EasyExcept("No default window set!");

		return defaultWindowHandle;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	BaseWindow::TextAttribute* BaseWindow::getDefaultTextAttribute()
	{
		Window::checkIfInitialized();

		if (defaultTextAttribute == nullptr)
			throw EasyExcept("No default window set!");

		return defaultTextAttribute;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	BaseWindow::GeometryAttribute* BaseWindow::getDefaultGeometryAttribute()
	{
		Window::checkIfInitialized();

		if (defaultGeometryAttribute == nullptr)
			throw EasyExcept("No default window set!");

		return defaultGeometryAttribute;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//									BaseWindow class

	BaseWindow::BaseWindow(const wstring title, int posX, int posY, unsigned width, unsigned height)
	{
		realCtor(title, posX, posY, width, height);
	}

	BaseWindow::~BaseWindow()
	{
		if (hdc != NULL)
			assert(0 != ReleaseDC(windowHandle, hdc));

		if (defaultBaseWindow = this)
			defaultBaseWindow = nullptr;
		if (defaultWindowHandle = defaultWindowHandle)
			defaultWindowHandle = NULL;
		if (defaultGeometryAttribute ==  &geometryAttribute)
			defaultGeometryAttribute = nullptr;
		if (defaultTextAttribute == &textAttribute)
			defaultTextAttribute = nullptr;
	}

	///////////////////////////////////////

	void BaseWindow::realCtor(const wstring title, int posX, int posY, unsigned width, unsigned height)
	{
		registerWindowClass();
		createWindow(title.c_str(), posX, posY, width, height);
		createDC();
		ShowWindow(windowHandle, SW_SHOW);
		UpdateWindow(windowHandle); 
	}

	void BaseWindow::registerWindowClass()
	{
		if (!registered)
		{
			WNDCLASSW wndclass = { 0 };
			wndclass.lpszClassName = L"EasyXPlus::WindowClassName";
			wndclass.hInstance = GetModuleHandleW(NULL);			//	current .exe's module handle
			wndclass.lpfnWndProc = EasyWndProc;						//	default window procedure
			wndclass.hCursor = LoadCursorW(NULL, IDC_ARROW);		//	default arrow cursor
			wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DROPSHADOW;//	default window style
			wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

			if (RegisterClassW(&wndclass) == 0)
				throw EasyExcept("Cannot register window class!");
			registered = true;
		}
	}

	void BaseWindow::createWindow(const wstring title, unsigned posX, unsigned posY, unsigned width, unsigned height)
	{
		windowHandle = CreateWindowW(
			L"EasyXPlus::WindowClassName",
			title.c_str(),
			WS_OVERLAPPEDWINDOW,
			posX, posY,
			width, height,
			NULL, NULL,
			NULL, NULL);

		//	disable unwanted window buttons
		SetWindowLong(windowHandle, GWL_STYLE,
			GetWindowLong(windowHandle, GWL_STYLE) & ~WS_MINIMIZEBOX);
		SetWindowLong(windowHandle, GWL_STYLE,
			GetWindowLong(windowHandle, GWL_STYLE) & ~WS_MAXIMIZEBOX);
		SetWindowLong(windowHandle, GWL_STYLE,
			GetWindowLong(windowHandle, GWL_STYLE) & ~WS_SIZEBOX);

		if (windowHandle == NULL)
			throw EasyExcept("Cannot create window!");
	}

	void BaseWindow::createDC()
	{
		if (NULL == (hdc = GetDC(windowHandle)))
			throw EasyExcept("System call error!");

		geometryAttribute.hdc = textAttribute.hdc = hdc;
		textAttribute.realCtor();

		if (0 == SetArcDirection(geometryAttribute.hdc, AD_CLOCKWISE))
			throw EasyExcept("System call error!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void BaseWindow::clear(const Colorable& color)
	{
		HBRUSH brush = CreateSolidBrush(color.toColorref());
		if (brush == NULL)
			goto error;

		RECT clientRect;
		if (0 == GetClientRect(windowHandle, &clientRect))
			goto error;
		
		if (!FillRect(GetDC(windowHandle), &clientRect, brush))
			goto error;

		DeleteObject((HGDIOBJ)brush);
		return;

	error:
		throw EasyExcept("BaseWindow clear error!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void BaseWindow::resize(unsigned width, unsigned height)
	{
		if (width > INT_MAX || height > INT_MAX)
			throw EasyExcept("Too big width or height!");

		RECT oldWindowRect;
		if ( 0 == GetWindowRect(windowHandle, &oldWindowRect))
			goto call_error;

		unsigned widthPlusBorder =
			width + 2 * GetSystemMetrics(SM_CXSIZEFRAME);
		unsigned heightPlusBorderAndTitle =
			height + 2 * GetSystemMetrics(SM_CYSIZEFRAME) + GetSystemMetrics(SM_CYCAPTION);
		if (0 == MoveWindow(
					windowHandle,
					oldWindowRect.left, oldWindowRect.top,
					widthPlusBorder, heightPlusBorderAndTitle,
					FALSE))
			goto call_error;
		return;

	call_error:
		throw EasyXPlus::EasyExcept("resize error!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void BaseWindow::reposition(int posX, int posY)
	{
		RECT windowRect;
		if (0 == GetWindowRect(windowHandle, &windowRect))
			goto call_error;

		if (0 == MoveWindow(
			windowHandle,
			posX, posY,
			windowRect.right - windowRect.left,
			windowRect.bottom - windowRect.top,
			FALSE))

			goto call_error;
		return;

	call_error:
		throw EasyXPlus::EasyExcept("reposition error!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void BaseWindow::setAsDefault()
	{
		defaultBaseWindow = this;
		defaultWindowHandle=  windowHandle;
		defaultGeometryAttribute = &geometryAttribute;
		defaultTextAttribute = &textAttribute;
	}

	/////////////////////////////////////////////////////////////////////////////////////

	int BaseWindow::getPosX() const
	{
		return getWindowRect().left;
	}

	int BaseWindow::getPosY() const
	{
		return getWindowRect().top;
	}

	unsigned BaseWindow::getWidth() const
	{
		RECT rect = getWindowRect();
		return rect.right - rect.left;
	}

	unsigned BaseWindow::getHeight() const
	{
		RECT rect = getWindowRect();
		return rect.bottom - rect.top;
	}

	RECT BaseWindow::getWindowRect() const
	{
		RECT windowRect;
		if (0 == GetWindowRect(windowHandle, &windowRect))
			throw EasyExcept("Get BaseWindow Region error!");

		return windowRect;
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//								GeometryAttribute struct

	BaseWindow::GeometryAttribute::GeometryAttribute() :
		hdc(NULL), pen(NULL), brush(NULL),
		dotColor(Rgb::Red()), lineColor(Rgb::Black()), fillColor(Rgb::White())
	{}

	BaseWindow::GeometryAttribute::~GeometryAttribute()
	{
		if (pen != NULL)
			assert(0 != DeleteObject((HGDIOBJ)pen));
		if (brush != NULL)
			assert(0 != DeleteObject((HGDIOBJ)brush));
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//								TextAttribute struct

	BaseWindow::TextAttribute::TextAttribute() :
		hdc(NULL),
		fontName(L"Arial"), isBold(false), isItalic(false), isUnderline(false),
		pointSize(14), textColor(Rgb::White()), bkColor(Rgb::Black())
	{
	}

	BaseWindow::TextAttribute::~TextAttribute()
	{
		if (font != NULL)
			assert (0 == DeleteObject((HGDIOBJ)font));
	}

	////////////////////////////////

	void BaseWindow::TextAttribute::realCtor()
	{
		createFont();

		if (NULL == SelectObject(hdc, font))
			throw EasyExcept("System call error!");
		if (CLR_INVALID == SetTextColor(hdc, textColor.toColorref()))
			throw EasyExcept("System call error!");
		if (CLR_INVALID == SetBkColor(hdc, bkColor.toColorref()))
			throw EasyExcept("System call error!");
	}

	////////////////////////////////////////////////////////////////////
	//					static TextAttribute functions

	void BaseWindow::TextAttribute::releaseFont()
	{
		if (0 == DeleteObject(font))
			throw EasyExcept("System call error!");
	}

	/////////////////////////////////

	void BaseWindow::TextAttribute::createFont()
	{
		font = CreateFont(
			-MulDiv(pointSize, GetDeviceCaps(hdc, LOGPIXELSY), 72),
			0, 0, 0,
			isBold ? FW_BOLD : FW_NORMAL,
			isItalic,
			isUnderline,
			FALSE,
			CHINESEBIG5_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			CLEARTYPE_QUALITY,
			DEFAULT_PITCH,
			fontName.c_str());
		if (font == NULL)	throw EasyExcept("System call error!");
	}

	///////////////////////////////////////

	void BaseWindow::TextAttribute::applyFont()
	{
		if (NULL == SelectObject(hdc, font))
			throw EasyExcept("System call error!");
	}

	//////////////////////////////////////

	void BaseWindow::TextAttribute::changeFont()
	{
		TextAttribute::releaseFont();
		TextAttribute::createFont();
		TextAttribute::applyFont();
	}

	/////////////////////////////////////////////////////////////////////////////////////
	//										Window

	/////////////////////////////////////////////////////////////////////////////////////
	//								Static variables

	bool Window::initialized = false;

	/////////////////////////////////////////////////////////////////////////////////////
	//								Static functions

	void Window::init(const std::wstring title)
	{
		Window::init(title, 0, 0, 640, 480);
	}

	void Window::init(const std::wstring title, int posX, int posY)
	{
		Window::init(title, posX, posY, 640, 480);
	}

	void Window::init(const std::wstring title, int posX, int posY, unsigned width, unsigned height)
	{
		initialized = true;

		BaseWindow::getDefaultBaseWindow()->reposition(posX, posY);
		BaseWindow::getDefaultBaseWindow()->resize(width, height);
		//	if width and height is greater than the real init window size,
		//	there will be a black area, call clearScreen() to make it black.
		clearScreen(Rgb::Black());

		assert(
			SetWindowText(BaseWindow::getDefaultWindowHandle(), title.c_str()));
		ShowWindow(BaseWindow::getDefaultWindowHandle(), SW_SHOW);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	int Window::getPosX()
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->getPosX();
	}

	void Window::checkIfInitialized()
	{
		if (initialized == false)
			throw EasyExcept("Window::init() not called!");
	}

	/////////////////////////////////////////////////////////////////////////////////////

	int Window::getPosY()
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->getPosY();
	}

	////////////////////////////////////////////////////////////////////////////////////

	unsigned Window::getWidth()
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->getWidth();
	}

	/////////////////////////////////////////////////////////////////////////////////////

	unsigned Window::getHeight()
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->getHeight();
	}

	////////////////////////////////////////////////////////////////////////////////////

	void Window::resize(unsigned width, unsigned height)
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->resize(width, height);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void Window::reposition(int posX, int posY)
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->reposition(posX, posY);
	}

	/////////////////////////////////////////////////////////////////////////////////////

	void Window::clearScreen(const Colorable& color)
	{
		checkIfInitialized();
		return BaseWindow::getDefaultBaseWindow()->clear(color);
	}

	////////////////////////////////////////  END  //////////////////////////////////////
}