//  [12/6/2013 OMEGA] created
#ifndef _DUELGUISIZEADJUSTMENT_H_
#define _DUELGUISIZEADJUSTMENT_H_

#include "DuelGUICommon.h"
#include "Duel.h"

namespace Duel
{
	//  把margin和padding统一放到这里.

	class DUELGUI_API DGMargin
	{
	public:
		DGMargin() : mLeft(0.0f), mTop(0.0f), mRight(0.0f), mBottom(0.0f) {}
		DGMargin(DReal l, DReal t, DReal r, DReal b) : mLeft(l), mTop(t), mRight(r), mBottom(b) {}
		DGMargin(DReal l_r, DReal t_b) : mLeft(l_r), mTop(t_b), mRight(l_r), mBottom(t_b) {}
		inline bool		isNull() const 
		{
			return mLeft == 0.0f && mTop == 0.0f && mRight == 0.0f && mBottom == 0.0f;
		}
		inline DReal	getLeft() const
		{
			return mLeft;
		}
		inline DReal	getTop() const
		{
			return mBottom;
		}
		inline DReal	getRight() const
		{
			return mRight;
		}
		inline DReal	getBottom() const
		{
			return mBottom;
		}
		inline void		setLeft(DReal left)
		{
			mLeft = left;
		}
		inline void		setTop(DReal top)
		{
			mTop = top;
		}
		inline void		setRight(DReal right)
		{
			mRight = right;
		}
		inline void		setBottom(DReal bottom)
		{
			mBottom = bottom;
		}
	protected:
		DReal	mLeft, mTop, mRight, mBottom;
	};

	class DUELGUI_API DGPadding
	{
	public:
		DGPadding() : mLeft(0.0f), mTop(0.0f), mRight(0.0f), mBottom(0.0f) {}
		DGPadding(DReal l, DReal t, DReal r, DReal b) : mLeft(l), mTop(t), mRight(r), mBottom(b) {}
		DGPadding(DReal l_r, DReal t_b) : mLeft(l_r), mTop(t_b), mRight(l_r), mBottom(t_b) {}
		inline bool		isNull() const 
		{
			return mLeft == 0.0f && mTop == 0.0f && mRight == 0.0f && mBottom == 0.0f;
		}
		inline DReal	getLeft() const
		{
			return mLeft;
		}
		inline DReal	getTop() const
		{
			return mBottom;
		}
		inline DReal	getRight() const
		{
			return mRight;
		}
		inline DReal	getBottom() const
		{
			return mBottom;
		}
		inline void		setLeft(DReal left)
		{
			mLeft = left;
		}
		inline void		setTop(DReal top)
		{
			mTop = top;
		}
		inline void		setRight(DReal right)
		{
			mRight = right;
		}
		inline void		setBottom(DReal bottom)
		{
			mBottom = bottom;
		}
	protected:
		DReal	mLeft, mTop, mRight, mBottom;
	};

}


#endif