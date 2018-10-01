#ifndef EASY_CLIP_H
#define EASY_CLIP_H

#include <windows.h>
#include "easyGeometry.h"

namespace EasyXPlus
{
    class ClipRegion
    {
    public:
        virtual void associateToWindow() = 0;
        virtual void cancelAssociation() = 0;
    };

    class RectClipRegion : public ClipRegion
    {
    public:
        explicit RectClipRegion(const RectRegion& region);
		~RectClipRegion();

        void exclude(const RectRegion& region);
        void intersect(const RectRegion& region);
		void unionn(const RectRegion& region);
        void move(int horizontalOffset, int verticalOffset);

		void set(const RectRegion& region);
		void clear();

        virtual void associateToWindow();
        virtual void cancelAssociation();

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