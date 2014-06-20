//  [8/20/2012 OMEGA] created

#ifndef _DUELBOX_H_
#define _DUELBOX_H_

#include <stdlib.h>
#include "DuelMath.h"

namespace Duel
{
	// this "picture" demonstrate a concept of a box

	//                                          top   front
	//                                           |      /
	//                                     - +---------+    
	//                                 L  / /|        /| 
	//                                   / / |       / |
	//             (+)                  - +---------+  |
	//              y|  (+)|far|        | |  |      |  |
	//               |  / z      left-> H |  +------|--+  <-right
	//               | /                | | /       | / 
	//               |/                 | |/        |/
	//      (-)------#--------(+)       - +---------+    
	//              /|       x            |----W----|
	//             / |                    /     |
	//            /  |                 back   bottom
	//    |near|(-)  |
	//              (-)

	template<typename T>
	class __declspec(dllexport) TBox
	{
	public:
		TBox(){};
		TBox(T l, T r, T t, T bot, T f, T bck)
		{
			assert(l <= r && t >= bot && f >= bck);
			left	= l;
			right	= r;
			top		= t;
			bottom	= bot;
			front	= f;
			back	= bck;
		}
		~TBox(){};
		
		// query if the specified box is totally inside the current on
		bool	contain( const TBox& b)
		{
			if( b.right <= right && b.left >= left &&
				b.top <= top && b.bottom >= bottom &&
				b.front <= front && b.back >= back)
			{
				return true;
			}
			return false;
		}

		// make a box valid
		void	validate()
		{
			if (left > right)
			{
				std::swap(left, right);
			}
			if (top < bottom)
			{
				std::swap(top, bottom);
			}
			if (front < back)
			{
				std::swap(front, back);
			}
		}

		T	getWidth() { return right - left; }
		T	getHeight() { return top - bottom; }
		T	getLength() { return front - back; }

		T	left, right, top, bottom, front, back;

		
	};
	
	typedef	TBox<int>		IBox;
	typedef	TBox<uint32>	UBox;
	typedef	TBox<float>		FBox;	

	
}
#endif