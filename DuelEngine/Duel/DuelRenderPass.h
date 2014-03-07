//  [4/2/2013 OMEGA] Õ£÷π π”√SceneBlendState. 
//  [11/14/2012 OMEGA] created


#ifndef _DUELRENDERPASS_H_
#define _DUELRENDERPASS_H_

#include "DuelCommon.h"
#include "DuelRenderState.h"
#include "DuelGpuProgram.h"

namespace Duel
{

	// a single render pass in the hardware rendering pipeline, there can be
	// many passes in a DRenderTechnique, for complex rendering effect
	// no fixed pipeline supporting.
	class DUEL_API DRenderPass : public DObject
	{
	DUEL_DECLARE_RTTI(DRenderPass)
	public:
		DRenderPass(DRenderTechnique* parent, const DString& name);
		virtual ~DRenderPass();
		const DString&		getName() const; 

		// set the index of this pass in its parent.
		INTERNAL void		setIndex(uint32 index);
		// the index of this pass in its parent (DRenderTechnique)
		uint32				getIndex() const;

		// get hash value of this pass. used in sorting.
		uint32				getHash() const;

		DRenderTechnique*	getParent() const;
		
		//////////////////////////////////////////////////////////////////////////
		// NOTICE:
		// if you set vertex program or pixel program after the shader object's
		// creation, than you must recreate a shader object!
		///////////////////
		void				setShaderObject(const DShaderObjectPtr& obj);
		DShaderObjectPtr	getShaderObject();

	
		// set vertex shader for render pass, if the value is NULL, 
		// then vertex shader is diabled;
		INTERNAL void		setVertexProgram(DResourcePtr vertexShader);
		DResourcePtr		getVertexProgram() const;

		// set pixel shader for render pass, if the value is NULL, 
		// then pixel shader is diabled;
		INTERNAL void		setPixelProgram(DResourcePtr pixelShader);
		DResourcePtr		getPixelProgram() const;
		
		// used in setting state into rendering pipeline.
		void				setRasterizerStateObject(const DRasterizerStateObjectPtr& obj);
		DRasterizerStateObjectPtr	getRaseterizerStateObject();

		void				setDepthStencilStateObject(const DDepthStencilStateObjectPtr& obj);
		DDepthStencilStateObjectPtr	getDepthStencilStateObject();

		void				setBlendStateStateObject(const DBlendStateObjectPtr& obj);
		DBlendStateObjectPtr	getBlendStateStateObject();

		// make these value public for quick access/modification.
		DRasterizerState		rasterizeState;
		DDepthStencilState		depthStencilState;


		// BlendState
		DBlendState				blendState;

	protected:
		// the name of the pass.
		DString				mName;
		// used in quick sort for rendering optimization, this value is got from
		// parsing the mName
		//uint32				mHash; 
		uint32				mIndex;
		DRenderTechnique*	mParent;
		
		DRasterizerStateObjectPtr	mRasterObj;
		DDepthStencilStateObjectPtr	mDepthStencilObj;
		DBlendStateObjectPtr		mBlendObj;

		// hash value used for sorting
		uint32				mHash;

		// shader object that retrieved from render system.
		DShaderObjectPtr	mShaderObject;

		// if the pass does not contain a vertex shader, then
		// mVertexShader could be NULL;
		DResourcePtr		mVertexProgram;
		// if the pass does not contain a Pixel shader, then
		// mPixelshader could be NULL;
		DResourcePtr		mPixelProgram;

	private:
		// not allowed.
		DRenderPass();
	};
}

#endif