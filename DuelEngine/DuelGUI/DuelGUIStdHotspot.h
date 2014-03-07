//  [11/1/2013 OMEGA] created

#ifndef _DUELGUISTDHOTSPOT_H_
#define _DUELGUISTDHOTSPOT_H_

#include "DuelGUICommon.h"
#include "DuelGUIHotSpot.h"

namespace Duel
{

	// standard hotspot is a rectangle area calculated from parent widget's geometry. with 3d-transform applied.
	class DGStdHotspot : public DGHotspot
	{
		DUEL_DECLARE_RTTI(DGStdHotspot)
	public:
		DGStdHotspot(DGWidget* parent = NULL);


		// override : DGHotspot
		virtual	bool			response(const DEventBase* e) { return false; }
		// used to update properties.
		virtual void			update() {}


	protected:
		DVector3	mTLPnt;
		DVector3	mTRPnt;
		DVector3	mBLPnt;
		DVector3	mBRPnt;
	};

}

#endif