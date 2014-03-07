//  [6/29/2013 OMEGA] created

#ifndef _DUELGUISIZE_H_
#define _DUELGUISIZE_H_

#include "DuelCommon.h"
#include "DuelGUICommon.h"


namespace Duel
{
	/*
		Constant						Value	Description
		ARM_IgnoreAspectRatio			0		The size is scaled freely. The aspect ratio is not preserved.
		ARM_KeepAspectRatio				1		The size is scaled to a rectangle as large as possible inside a given rectangle, preserving the aspect ratio.
		ARM_KeepAspectRatioByExpanding	2		The size is scaled to a rectangle as small as possible outside a given rectangle, preserving the aspect ratio.
	*/
	enum AspectRatioMode {
		ARM_Ignore,
		ARM_Keep,
		ARM_KeepByExpanding
	};

	class DUELGUI_API DGSize
	{
	public:
		DGSize() : mWidth(-1.0f), mHeight(-1.0f) {}
		DGSize(const DGSize& s) : mWidth(s.mWidth), mHeight(s.mHeight) {}
		DGSize(DReal w, DReal h) : mWidth(w), mHeight(h) {}

		inline bool		isNull() const { return DMath::realEqual(mWidth, 0.0f) && DMath::realEqual(mHeight, 0.0f); }
		inline bool		isEmpty() const { return mWidth < 1.0f && mHeight < 1.0f; }
		inline bool		isValid() const { return mWidth >= 0.0f && mHeight >= 0.0f; }

		inline DReal	getWidth() const { return mWidth; }
		inline DReal	getHeight() const { return mHeight; }
		inline void		setWidth(DReal w) { mWidth = w; }
		inline void		setHeight(DReal h) { mHeight = h; }
		inline void		transpose() { std::swap(mWidth, mHeight); }

		void		scale(DReal w, DReal h, AspectRatioMode mode)
		{
			scale(DGSize(w, h), mode);
		}
		void		scale(const DGSize& s, AspectRatioMode mode)
		{
			if (mode == ARM_Ignore || DMath::realEqual(mWidth, 0.0f)|| DMath::realEqual(mHeight, 0.0f)) {
				mWidth = s.mWidth;
				mHeight = s.mHeight;
			} else {
				bool useHeight;
				DReal rw = s.mHeight * mWidth / mHeight;

				if (mode == ARM_Keep) {
					useHeight = (rw <= s.mWidth);
				} else { // mode == Qt::KeepAspectRatioByExpanding
					useHeight = (rw >= s.mWidth);
				}

				if (useHeight) {
					mWidth = rw;
					mHeight = s.mHeight;
				} else {
					mHeight = s.mWidth * mHeight / mWidth;
					mWidth = s.mWidth;
				}
			}
		}

		inline DGSize		expandedTo(const DGSize& s) const
		{
			return DGSize(DMath::Max<DReal>(mWidth, s.mWidth), DMath::Max<DReal>(mHeight, s.mHeight));
		}
		inline DGSize		boundedTo(const DGSize& s) const
		{
			return DGSize(DMath::Min<DReal>(mWidth, s.mWidth), DMath::Min<DReal>(mHeight, s.mHeight));
		}

		inline DGSize&		operator+=(const DGSize& s)
		{
			mWidth += s.mWidth; mHeight += s.mHeight; return *this; 
		}
		inline DGSize&		operator-=(const DGSize& s)
		{
			mWidth -= s.mWidth; mHeight -= s.mHeight; return *this; 
		}
		inline DGSize&		operator*=(DReal c)
		{
			mWidth *= c; mHeight *= c; return *this;
		}
		inline DGSize&		operator/=(DReal c)
		{
			assert(!DMath::realEqual(c, 0.0f));
			mWidth /= c; mHeight /= c; return *this;
		}

		friend inline bool operator==(const DGSize& a, const DGSize& b)
		{
			return DMath::realEqual(a.mWidth, b.mWidth) && DMath::realEqual(b.mHeight, b.mHeight);
		}
		friend inline bool operator!=(const DGSize& a, const DGSize& b)
		{
			return !DMath::realEqual(a.mWidth, b.mWidth) || !DMath::realEqual(b.mHeight, b.mHeight);
		}
		friend inline const DGSize operator+(const DGSize& a, const DGSize& b)
		{
			return DGSize(a.mWidth+b.mWidth, a.mHeight+b.mHeight); 
		}
		friend inline const DGSize operator-(const DGSize& a, const DGSize& b)
		{
			return DGSize(a.mWidth-b.mWidth, a.mHeight-b.mHeight); 
		}
		friend inline const DGSize operator*(const DGSize& a, DReal s)
		{
			return DGSize(a.mWidth*s, a.mHeight*s);
		}
		friend inline const DGSize operator*(DReal s, const DGSize& a)
		{
			return DGSize(a.mWidth*s, a.mHeight*s);
		}
		friend inline const DGSize operator/(const DGSize& a, DReal s)
		{
			assert(!DMath::realEqual(s, 0.0f));
			return DGSize(a.mWidth/s, a.mHeight/s);
		}

	private:
		DReal		mWidth;
		DReal		mHeight;
	};


}


#endif