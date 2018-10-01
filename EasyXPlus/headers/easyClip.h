///
///	@file		Clip module.
///	@author		lgxZJ@outlook.com
///	@date		2017/6/12
///	@version	1.0
///

#ifndef EASY_CLIP_H
#define EASY_CLIP_H

#include <windows.h>
#include "easyGeometry.h"

namespace EasyXPlus
{
	///
	///	@brief	A clip region interface.
	///
    class ClipRegion
    {
    public:
        virtual void associateToWindow() = 0;
        virtual void cancelAssociation() = 0;
    };

	///
	///	@brief	A clip region class in rectangle shape.
	///	@remark	Although the basic shape of this clip region is rectangle,
	///			complex clip regions that are not strictly rectangles can also
	///			be creaetd from applying a lot of logic operations(exclude,
	///			intersect and so on).
	///
    class RectClipRegion : public ClipRegion
    {
    public:
		///
		///	@brief	Creates a rectangle clip region with the given region.
		///
        explicit RectClipRegion(const RectRegion& region);
		~RectClipRegion();

///
///	@name	Logic operations of clip regions. These opertions will not be performed
///			immediately until the associateToWindow() function is called. Logic
///			operations are performed in order.
///
///	@{
		///
		///	@brief	Exclude the given region from the inner clip region.
		///
        void exclude(const RectRegion& region);
		///
		///	@brief	Intersect the inner clip region with the given region.
		///
        void intersect(const RectRegion& region);
		///
		///	@brief	Union the inner clip region with the given region.
		///
		void unionn(const RectRegion& region);
		///
		///	@brief	Move the inner clip region with the given x-y offsets.
		///
        void move(int horizontalOffset, int verticalOffset);
		///
		///	@brief	Set the inner clip region to the given region and clear all
		///			logic operations.
		///
		void set(const RectRegion& region);
		///
		///	@brief	Clear the inner clip region and all logic operations on the
		///			inner clip region.
		///	@note	Generally, users should call set() after this function call.
		///
		void clear();
///	@}

///	@{
		///
		///	@brief	Apply this clip region and all its logic operations to
		///			the default window.
		///	@throw	EasyExcept	If Window::init() is not called, it throws an excecption.
		///			If this function is called after calling clear() but before set(), it
		///			also throws an exception.
		///	@note	Users can call this function many times so long as the inner clip region
		///			is valid.
		///
        virtual void associateToWindow();
		///
		///	@brief	Cancel applying the clip region.
		///
		///			After canceling association, the clip region is remove from the default
		///			window.
		///	@throw	EasyExcept	If this function is called after calling clear() but before
		///			set(), it throw an exception.
		///	@note	Users can call this function many times so long as the inner clip region
		///			is valid.
		///
        virtual void cancelAssociation();
///	@}

	private:
		void excludeRegion(const RectRegion& region);
		void intersectRegion(const RectRegion& region);
		void unionRegion(const RectRegion& region);
		void moveRegion(int offsets[2]);
		HRGN createRgnFromRegion(const RectRegion& region);

    protected:
		enum class ClipType { Exclude, Intersect, Move, Union };
		union ClipUnion {
			explicit ClipUnion(const RectRegion& region);
			explicit ClipUnion(int horizontalOffset, int verticalOffset);
			explicit ClipUnion(int scaleFactor);

			RectRegion region; 
			int offsets[2]; 
			int scaleFactor; 
		};

        HRGN regionHandle;
		std::vector<std::pair<ClipType, ClipUnion>> actions;	
    };
}

#endif