//  [7/15/2012 OMEGA] created

#ifndef	_DUELRECTANGLE_H_
#define _DUELRECTANGLE_H_

#include "DuelMath.h"

namespace Duel
{
	
	// (xBegin,yBegin)---------------->xExtend
	//		 |
	//		 |
	//		 |
	//		 |
	//		 |
	//		 v
	//    yExtend

	template<typename T>
	class __declspec(dllexport) TRectangle
	{
	public:
		TRectangle()
		{
		}
		TRectangle(T x, T y, T xExt, T yExt)
		{
			xBegin	= x;
			yBegin	= y;
			xExtend	= xExt;
			yExtend	= yExt;
		}

		TRectangle	adjust(T l, T t, T r, T b)
		{
			TRectangle ret;
			ret.xBegin = xBegin + l;
			ret.xExtend = xExtend - l;
			ret.yBegin = yBegin + t;
			ret.yExtend = yExtend - t;
			ret.xExtend += r;
			ret.yExtend += b;
			return ret;
		}
		bool	operator != (const TRectangle& r)
		{
			return !operator==(r);
		}
		bool	operator == (const TRectangle& r)
		{
			return (r.xBegin == xBegin && 
					r.yBegin == yBegin && 
					r.xExtend == xExtend && 
					r.yExtend == yExtend);
		}
		bool	isInside(T x, T y)
		{
			return (x >= xBegin && x <= xBegin + xExtend) && (y >= yBegin && y <= yBegin + yExtend);
		}

		bool	isIntersect(const TRectangle& rRect)
		{
			if((rRect.xBegin >= xBegin + xExtend) ||
				(rRect.yBegin >= yBegin + yExtend)  ||
				(rRect.xBegin + xExtend <= xBegin) ||
				(rRect.yBegin + yExtend <= yBegin))
			{
				return false;
			}
			return true;
		}
		// return a intersected rectangle between two rectangles
		// if they are not intersect, return a zero rectangle.
		TRectangle	intersect(const TRectangle& rRect)
		{
			TRectangle<T> rResult;

			// I don't use min(x, y), because it can sometimes return 
			// a wrong number, I encountered, but I can't say why, 
			// before I make it clear, I will use some conventional methods.
			if(isIntersect(rRect))
			{
				rResult.xBegin	= xBegin	> rRect.xBegin ? xBegin : rRect.xBegin;
				rResult.yBegin		= yBegin	> rResult.yBegin ? yBegin : rRect.yBegin;

				DReal rRight;
				rRight = xBegin + xExtend < rRect.xBegin + rRect.xExtend ?
					xBegin + xExtend : rRect.xBegin + rRect.xExtend;
				rResult.xExtend	= rRight - rResult.xBegin;

				DReal rBottom;
				rBottom = yBegin + yExtend < rRect.yBegin + rRect.yExtend ?
					yBegin + yExtend : rRect.yBegin + rRect.yExtend;
				rResult.yExtend	= rBottom - rResult.yBegin;
			}

			return rResult;
		}

		T	xBegin;
		T	yBegin;
		T	xExtend;
		T	yExtend;
	};

	typedef	TRectangle<uint32>	URect;
	typedef	TRectangle<int>		IRect;
	typedef	TRectangle<float>	FRect;
}


#endif