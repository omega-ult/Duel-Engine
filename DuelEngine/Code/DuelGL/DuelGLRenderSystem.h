//  [4/7/2013 OMEGA] created

#ifndef _DUELGLRENDERSYSTEM_H_
#define _DUELGLRENDERSYSTEM_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLRenderSystem : public DRenderSystem
	{
	DUEL_DECLARE_RTTI(GLRenderSystem)
	public:
		GLRenderSystem();
		~GLRenderSystem();

		virtual const DString& getName() const;


		// override : DRenderSystem---------------------------------
		virtual void	initialize();
		// override : DRenderSystem---------------------------------
		virtual void	shutdown();

		// override : DRenderSystem---------------------------------
		virtual void	setRasterizerState( DRasterizerStateObject* rs );

		// override : DRenderSystem---------------------------------
		virtual void	setBlendState( DBlendStateObject* sbs, const DColor& blendFactor );

		// override : DRenderSystem---------------------------------
		virtual void	setDepthStencilState( DDepthStencilStateObject* dss, uint32 frontStencilRef = 0, uint32 backStencilRef = 0  );
		
		// override : DRenderSystem---------------------------------
		virtual	void	bindShaderObject(DShaderObject* so);

		// override : DRenderSystem---------------------------------
		virtual void	render( DRenderLayout* layout );


		// override : DRenderSystem---------------------------------
		virtual void	fillDeviceCaps();
		// override : DRenderSystem---------------------------------
		virtual DRasterizerStateObjectPtr		getCurrentRaseterizerState();

		// override : DRenderSystem---------------------------------
		virtual DDepthStencilStateObjectPtr		getCurrentDepthStencilStateObject();

		// override : DRenderSystem---------------------------------
		virtual DBlendStateObjectPtr			getCurrentBlendState();
		
		// override : DRenderSystem---------------------------------
		virtual void			bindFrameBuffer( DFrameBuffer* buf );

		// override : DRenderSystem---------------------------------
		virtual DFrameBuffer*	getCurrentFrameBuffer();


		GLuint			getCurGLFBO();


	protected:
		void			initRenderStates();

		// current frame buffer binded to the pipe line. on which all draw calls happen.
		DFrameBuffer*	mCurFrameBuffer;

		// use a pointer to indicate whether the current render states are available.
		DRasterizerState			mCurRasState;
		DDepthStencilState			mCurDepState;
		uint32						mCurFrontStencilRef;
		uint32						mCurBackStencilRef;
		DBlendState					mCurBlendState;
		DColor						mCurBlendFactor;
		

		// used in binding shader
		GLuint				mGLProgram;
		

		// used in bug tracing.
		DGpuProgram*		mVSProgram;
		DGpuProgram*		mPSProgram;
		// just a reference pointer.
		DGpuParameters*		mVSParams;
		DGpuParameters*		mPSParams;

		// the render system name.
		static DString		msGLSystemName;

	};

}


#endif
