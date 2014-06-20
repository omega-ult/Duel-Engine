//  [11/1/2013 OMEGA] created

#ifndef _DUELGUIGEOMETRY_H_
#define _DUELGUIGEOMETRY_H_

#include "DuelGUICommon.h"
#include "Duel.h"
#include "DuelGUISize.h"

namespace Duel
{

	// geometry object use a center position and a size to describe
	// a GUI area, whichi is presented in the GUI framework.
	class DUELGUI_API DGGeometry
	{
	public:
		DGGeometry() : mCenter(DVector2(0.0f, 0.0f)), mSize(DGSize(0.0f, 0.0f)) {}
		DGGeometry(const DVector2& c, const DGSize& s) : mCenter(c), mSize(s) {}
		DGGeometry(const DGGeometry& g) : mCenter(g.mCenter), mSize(g.mSize) {}

		DGGeometry&		operator = ( const DGGeometry& g )
		{
			mCenter = g.mCenter;
			mSize = g.mSize;
			return *this;
		}

		bool			operator == ( const DGGeometry& g ) 
		{
			return (mCenter == g.mCenter && mSize == g.mSize);
		}

		bool			operator != ( const DGGeometry& g )
		{
			return !operator==(g);
		}

		void				setCenter(const DVector2& c) { mCenter = c; }
		void				setSize(const DGSize& s) { mSize = s; }
		const DVector2&		getCenter() const { return mCenter; }
		const DGSize&		getSize() const { return mSize; }

	protected:
		DVector2		mCenter;
		DGSize			mSize;
	};

}

#endif