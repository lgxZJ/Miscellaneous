#ifndef EASY_BASEWINDOW_H
#define EASY_BASEWINDOW_H

///////////////////////////////////////////////////////////////////////////////////
//
//		This header is not intended to be used by users. Instead, use easyWindow.h
//
///////////////////////////////////////////////////////////////////////////////////

#include <Windows.h>
#include "easyColor.h"
#include <string>

namespace EasyXPlus
{
	class BaseWindow
	{
	private:
		friend class Geometry;
		friend class Text;

		struct GeometryAttribute;
		struct TextAttribute;

	public:
		static BaseWindow* getDefaultBaseWindow();
		static HWND getDefaultWindowHandle();
		static GeometryAttribute* getDefaultGeometryAttribute();
		static TextAttribute* getDefaultTextAttribute();

		BaseWindow(const std::wstring title, int posX, int posY, unsigned width, unsigned height);

		virtual ~BaseWindow();

		//	forbid copy constructor
		BaseWindow operator = (BaseWindow& window) = delete;

		int getPosX() const;
		int getPosY() const;
		unsigned getWidth() const;
		unsigned getHeight() const;

		void resize(unsigned width, unsigned height);
		void reposition(int posX, int posY);
		void setAsDefault();

		void clear(const Colorable& color);

	private:
		void realCtor(const std::wstring title, int posX, int posY, unsigned width, unsigned height);
		void registerWindowClass();
		void createWindow(const std::wstring title, unsigned posX, unsigned posY, unsigned width, unsigned height);
		void createDC();
		RECT getWindowRect() const;
		void releaseDCResources();
		void releaseGeometryResources();
		void releaseTextResources();

	private:
		struct GeometryAttribute
		{
			GeometryAttribute();
			~GeometryAttribute();

			HDC hdc;
			HPEN pen;			//	created pen, need to delete
			HBRUSH brush;		//	created brush, need to delete
			Rgb dotColor, lineColor, fillColor;
		};

		struct TextAttribute
		{
			TextAttribute();
			~TextAttribute();

			HDC hdc;
			HFONT font;
			std::wstring fontName;
			bool isBold;
			bool isItalic;
			bool isUnderline;
			unsigned pointSize;
			Rgb textColor;
			Rgb bkColor;

			//	Since window handle is created in Outer-Class-Ctor body,
			//	i cannot initialize this member in initialzation lists.
			//	However, i decide to initialize it when setAsDefault() is called.
			void realCtor();

			void TextAttribute::releaseFont();
			void TextAttribute::createFont();
			void TextAttribute::applyFont();
			void TextAttribute::changeFont();
		};


	protected:
		static bool registered;
		static BaseWindow* defaultBaseWindow;
		static HWND defaultWindowHandle;
		static GeometryAttribute* defaultGeometryAttribute;
		static TextAttribute* defaultTextAttribute;

		HWND windowHandle;
		HDC hdc;
		GeometryAttribute geometryAttribute;
		TextAttribute textAttribute;
	};

}

#endif