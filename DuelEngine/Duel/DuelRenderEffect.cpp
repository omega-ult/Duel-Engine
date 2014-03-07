//  [9/9/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelCore.h"
#include "DuelRenderSystem.h"
#include "DuelRenderEffect.h"
#include "DuelRenderTechnique.h"
#include "DuelRenderResourceManager.h"
#include "DuelRenderPass.h"
#include "DuelResourceGroupManager.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DRenderEffect, DResource);

	DRenderEffect::DRenderEffect( DResourceManager* fathermanager, const DString& name, const DString& groupName ) :
		DResource(fathermanager, name, groupName, "RenderEffect")
	{

	}

	DRenderTechniquePtr DRenderEffect::addTechnique( const DString& name )
	{
		// do not check duplication
		DRenderTechnique* tech = new DRenderTechnique(name);
		DRenderTechniquePtr newTech = DRenderTechniquePtr(tech);
		mTechniques.push_back(newTech);
		return newTech;
	}

	void DRenderEffect::addTechnique( const DRenderTechniquePtr& tech )
	{
		mTechniques.push_back(tech);
	}

	DRenderTechniquePtr DRenderEffect::getTechnique( const DString& name ) 
	{
		DRenderTechniquePtr	ret;
		RenderTechniqueIterator i = getRenderTechniqueIterator();
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

	DRenderTechniquePtr DRenderEffect::getTechnique( uint32 index ) 
	{
		assert(index < mTechniques.size());
		DRenderTechniquePtr	ret;
		if (index < mTechniques.size())
		{
			ret = mTechniques.at(index);
		}
		return ret;
	}

	void DRenderEffect::removeTechnique( const DString& name )
	{
		RenderTechniques::iterator i, iend = mTechniques.end();
		for (i = mTechniques.begin(); i != iend; ++i)
		{
			if ((*i)->getName() == name)
			{
				mTechniques.erase(i);
			}
		}
	}

	void DRenderEffect::removeTechnique( uint32 index )
	{
		assert(index < mTechniques.size());
		if (index < mTechniques.size())
		{
			mTechniques.erase(mTechniques.begin()+index);
		}
	}

	void DRenderEffect::removeAllTechniques()
	{
		// SharedPtr will do release work.
		RenderTechniques::iterator i, iend = mTechniques.end();
		
		mTechniques.clear();
	}

	void DRenderEffect::prepareImpl() 
	{
		DResource::prepareImpl();
		mCodec->decode(mPreparedData.get(), (this));
		// now load the shader
		RenderTechniques::iterator i, iend = mTechniques.end();
		for (i = mTechniques.begin(); i != iend; ++i)
		{
			DRenderTechnique::RenderPassIterator ri = (*i)->getRenderPassIterator();
			while(ri.hasMoreElements())
			{
				DRenderPass* pass = ri.current()->get();
				pass->getVertexProgram()->prepare();
				pass->getPixelProgram()->prepare();
				ri.moveNext();
			}
		}
	}

	void DRenderEffect::loadImpl() 
	{
		// now load the shader
		RenderTechniques::iterator i, iend = mTechniques.end();
		for (i = mTechniques.begin(); i != iend; ++i)
		{
			DRenderTechnique::RenderPassIterator ri = (*i)->getRenderPassIterator();
			DRenderResourceManager* HResMgr = DRenderResourceManager::getSingletonPtr();
			while(ri.hasMoreElements())
			{
				DRenderPass* pass = ri.current()->get();
				// here create state objects
				pass->setRasterizerStateObject(
					HResMgr->createRasterizerStateObject(pass->rasterizeState));
				pass->setDepthStencilStateObject(
					HResMgr->createDepthStencilStateObject(pass->depthStencilState));
				pass->setBlendStateStateObject(
					HResMgr->createBlendStateObject(pass->blendState));

				pass->getVertexProgram()->load();
				pass->getPixelProgram()->load();

				pass->setShaderObject(HResMgr->createShaderObject(pass));

				ri.moveNext();
			}
		}		
	}

	void DRenderEffect::unloadImpl()
	{
		removeAllTechniques();

	}

	size_t DRenderEffect::calculateSize()
	{
		mSize = 0;
		// size is calculated in loading process.
		return mSize;
	}

	DDataStreamPtr DRenderEffect::encode()
	{
		if (mCodec)
		{
			return (mCodec)->encode(static_cast<DResource*>(this));
		}
		DDataStreamPtr ret;
		return ret;
	}

	void DRenderEffect::saveToFile( const DString& fileName )
	{
		if (mCodec)
		{
			(mCodec)->encodeToFile(fileName, static_cast<DResource*>(this));
		}
	}

}