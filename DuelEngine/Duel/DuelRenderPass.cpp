//  [11/14/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelRenderState.h"
#include "DuelRenderPass.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DRenderPass, DObject);

	DRenderPass::DRenderPass( DRenderTechnique* parent, const DString& name) :
		mParent(parent),
		mName(name)
	{
		_StringHash hash;	
		mHash = hash(mName);
	}

	DRenderPass::DRenderPass()
	{

	}

	Duel::DBlendStateObjectPtr DRenderPass::getBlendStateStateObject()
	{
		return mBlendObj;
	}

	void DRenderPass::setBlendStateStateObject( const DBlendStateObjectPtr& obj )
	{
		mBlendObj = obj;
	}

	Duel::DDepthStencilStateObjectPtr DRenderPass::getDepthStencilStateObject()
	{
		return mDepthStencilObj;
	}

	void DRenderPass::setDepthStencilStateObject( const DDepthStencilStateObjectPtr& obj )
	{
		mDepthStencilObj = obj;
	}

	Duel::DRasterizerStateObjectPtr DRenderPass::getRaseterizerStateObject()
	{
		return mRasterObj;
	}

	void DRenderPass::setRasterizerStateObject( const DRasterizerStateObjectPtr& obj )
	{
		mRasterObj = obj;
	}

	Duel::DResourcePtr DRenderPass::getPixelProgram() const
	{
		return mPixelProgram;
	}

	INTERNAL void DRenderPass::setPixelProgram( DResourcePtr pixelShader )
	{
		mPixelProgram = pixelShader;
	}

	Duel::DResourcePtr DRenderPass::getVertexProgram() const
	{
		return mVertexProgram;
	}

	INTERNAL void DRenderPass::setVertexProgram( DResourcePtr vertexShader )
	{
		mVertexProgram = vertexShader;
	}

	Duel::DShaderObjectPtr DRenderPass::getShaderObject()
	{
		return mShaderObject;
	}

	void DRenderPass::setShaderObject( const DShaderObjectPtr& obj )
	{
		mShaderObject = obj;
	}

	DRenderTechnique* DRenderPass::getParent() const
	{
		return mParent;
	}

	Duel::uint32 DRenderPass::getHash() const
	{
		return mHash;
	}

	Duel::uint32 DRenderPass::getIndex() const
	{
		return mIndex;
	}

	INTERNAL void DRenderPass::setIndex( uint32 index )
	{
		mIndex = index;
	}

	const DString& DRenderPass::getName() const
	{
		return mName;
	}

	DRenderPass::~DRenderPass()
	{

	}

	// 
// 	void DRenderPass::addTextureState( const TextureState& texState )
// 	{
// 		mTextureStates.push_back(texState);
// 	}

// 	uint32 DRenderPass::getTextureStateCount() const
// 	{
// 		return mTextureStates.size();
// 	}
// 
// 	TextureState* DRenderPass::getTextureState( uint32 index )
// 	{
// 		assert(index < mTextureStates.size());
// 		TextureState*	ret = NULL;
// 		if (index < mTextureStates.size())
// 		{
// 			ret = &(mTextureStates.at(index));
// 		}
// 		return ret;
// 	}
// 
// 	void DRenderPass::removeTextureState( uint32 index )
// 	{
// 		assert(index < mTextureStates.size());
// 		if (index < mTextureStates.size())
// 		{
// 			mTextureStates.erase(mTextureStates.begin()+index);
// 		}
// 	}
// 
// 	void DRenderPass::removeAllTextureState()
// 	{
// 		mTextureStates.clear();
// 	}


}