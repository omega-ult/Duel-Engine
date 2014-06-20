//  [12/2/2012 OMEGA] created

#ifndef _DUELRENDERTECHNIQUE_H_
#define _DUELRENDERTECHNIQUE_H_

#include "DuelCommon.h"

namespace Duel
{

	class DUEL_API DRenderTechnique : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderTechnique)
	public:
		// every render techinique should have a parent.
		DRenderTechnique(const DString& name);
		virtual ~DRenderTechnique();

		void			setName(const DString& name) { mName = name; }
		const DString&	getName() const { return mName; }
	
		// 有很多很多pass
		typedef	std::vector<DRenderPassPtr>		RenderPassList;
		typedef	VectorIterator<RenderPassList>	RenderPassIterator;
		RenderPassIterator	getRenderPassIterator() { return RenderPassIterator(mPasses.begin(), mPasses.end()); }

		// get the count of the passes.
		size_t			getPassCount() { return mPasses.size(); }
		// add a new pass, append it to the list. the name should be unique
		DRenderPassPtr	addPass(const DString& name);
		// add a new pass, append it to the list.
		void			addPass(DRenderPassPtr pass);
		// get pass using name
		DRenderPassPtr	getPass(const DString& name);
		// get pass at specified index, the index should range form 0 to
		// the count of the pass list, otherwise a NULL will be returned.
		DRenderPassPtr	getPass(uint32 index);
		// remove a pass at specified index, if it existed, 
		void			removePass(uint32 index);
		// remove all passes.
		void			removeAllPasses();	

	protected:
		DString	mName;

		RenderPassList	mPasses;

	private:
		// not allowed.
		DRenderTechnique() {}
	};


}



#endif