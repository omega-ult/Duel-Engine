//  [12/2/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderTechnique.h"
#include "DuelRenderPass.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderTechnique, DObject);

	DRenderTechnique::DRenderTechnique( const DString& name ) :
		mName(name)
	{

	}

	DRenderTechnique::~DRenderTechnique()
	{
		removeAllPasses();
	}

	DRenderPassPtr DRenderTechnique::getPass( uint32 index )
	{
		assert(index < mPasses.size());
		DRenderPassPtr	ret;
		if (index < mPasses.size())
		{
			ret = mPasses.at(index);
		}
		return ret;
	}

	DRenderPassPtr DRenderTechnique::getPass( const DString& name )
	{
		DRenderPassPtr	ret;
		RenderPassIterator i = getRenderPassIterator();
		while(i.hasMoreElements())
		{
			if (i.current()->get()->getName() == name)
			{
				return *(i.current());
			}
			i.moveNext();
		}
		return ret;
	}

	DRenderPassPtr DRenderTechnique::addPass( const DString& name )
	{
		// do not check duplication
		DRenderPass* pass = new DRenderPass(this, name);
		DRenderPassPtr newPass = DRenderPassPtr(pass);
		newPass->setIndex(mPasses.size());
		mPasses.push_back(newPass);
		return newPass;
	}

	void DRenderTechnique::addPass( DRenderPassPtr pass )
	{
		pass->setIndex(mPasses.size());
		mPasses.push_back(pass);
	}

	void DRenderTechnique::removePass( uint32 index )
	{
		assert(index < mPasses.size());
		if (index < mPasses.size())
		{
			RenderPassList::iterator i = mPasses.begin() + index;
			uint32 tmp = index-1;
			while (i != mPasses.end())
			{
				(*i)->setIndex(tmp);
				++i;
				++tmp;
			}
			mPasses.erase(mPasses.begin()+index);
		}
	}

	void DRenderTechnique::removeAllPasses()
	{
		// SharedPtr will do release work.
		mPasses.clear();
	}

}