//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include <GL/glew.h>
#include "DuelRenderSystem.h"
#include "DuelRenderPass.h"
#include "DuelGLRenderSystem.h"
#include "DuelGLRenderWindow.h"
#include "DuelGLRenderState.h"
#include "DuelGLGpuProgram.h"
#include "DuelGLGpuParameters.h"
#include "DuelGLTexture.h"
#include "DuelGLTranslator.h"
#include "DuelGLVertexBuffer.h"
#include "DuelGLIndexBuffer.h"
#include "DuelGLRenderLayout.h"
#include "DuelGLShaderObject.h"
#include "DuelGLFrameBuffer.h"
#include "DuelGLRenderView.h"
#include "DuelLogManager.h"
#ifdef DUEL_PLATFORM_WINDOWS
#pragma comment(lib, "glu32.lib")
#endif


namespace Duel
{
	DString GLRenderSystem::msGLSystemName = "OpenGL";
	DUEL_IMPLEMENT_RTTI_1(GLRenderSystem, DRenderSystem);


	void	bindGLParameters(GLuint glProg, DGpuParameters* params)
	{
		DGpuParameters::GpuConstantIterator pi = params->getAutoGpuConstantIterator();
		uint32 texUnit = 0;
		while (pi.hasMoreElements())
		{
			GLint loc = pi.current()->second.logicalIndex;
			if (loc != -1)
			{
				DGpuTextureConstantPtr tex;
				const GpuConstantDefinition& def = pi.current()->second;
				switch (def.constType)
				{
				case GCT_Float1:
					glProgramUniform1fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float2:
					glProgramUniform2fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float3:
					glProgramUniform3fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float4:
					glProgramUniform4fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Texture1D:
				case GCT_Texture2D:
				case GCT_Texture3D:
				case GCT_TextureCube:
					tex = params->getTextureConstant(pi.current()->first);
					if (tex != NULL && tex->isValid())
					{
						glActiveTexture(GL_TEXTURE0 + texUnit);
						texUnit++;
						GLGpuTextureConstant* glTexPtr = tex->getAs<GLGpuTextureConstant>();
						glTexPtr->setSamplerState(params->getSamplerConstant(pi.current()->first));
						glBindTexture(glTexPtr->getGLTextureTarget(), glTexPtr->getTextureID());
						glProgramUniform1ui(glProg, loc, glTexPtr->getTextureID());
					}
					else
					{
						glProgramUniform1ui(glProg, loc, 0);
					}
					break;
				case GCT_Int1:
					glProgramUniform1iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int2:
					glProgramUniform2iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int3:
					glProgramUniform3iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int4:
					glProgramUniform4iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x2:
					glProgramUniformMatrix2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x3:
					glProgramUniformMatrix2x3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x4:
					glProgramUniformMatrix2x4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x2:
					glProgramUniformMatrix3x2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x3:
					glProgramUniformMatrix3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x4:
					glProgramUniformMatrix3x4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x2:
					glProgramUniformMatrix4x2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x3:
					glProgramUniformMatrix4x3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x4:
					glProgramUniformMatrix4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				default:
					// for sampler, do nothing.
					break;
				}
			}
			pi.moveNext();
		}
		pi = params->getCustomGpuConstantIterator();
		while (pi.hasMoreElements())
		{
			GLint loc = pi.current()->second.logicalIndex;
			if (loc != -1)
			{
				DGpuTextureConstantPtr tex;
				const GpuConstantDefinition& def = pi.current()->second;
				switch (def.constType)
				{
				case GCT_Float1:
					glProgramUniform1fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float2:
					glProgramUniform2fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float3:
					glProgramUniform3fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Float4:
					glProgramUniform4fv(glProg, loc, def.arraySize, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Texture1D:
				case GCT_Texture2D:
				case GCT_Texture3D:
				case GCT_TextureCube:
					tex = params->getTextureConstant(pi.current()->first);
					if (tex != NULL && tex->isValid())
					{
						glActiveTexture(GL_TEXTURE0 + texUnit);
						texUnit++;
						GLGpuTextureConstant* glTexPtr = tex->getAs<GLGpuTextureConstant>();
						glTexPtr->setSamplerState(params->getSamplerConstant(pi.current()->first));
						glBindTexture(glTexPtr->getGLTextureTarget(), glTexPtr->getTextureID());
						glProgramUniform1ui(glProg, loc, glTexPtr->getTextureID());
					}
					else
					{
						glProgramUniform1ui(glProg, loc, 0);
					}
					break;
				case GCT_Int1:
					glProgramUniform1iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int2:
					glProgramUniform2iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int3:
					glProgramUniform3iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Int4:
					glProgramUniform4iv(glProg, loc, def.arraySize, params->getIntValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x2:
					glProgramUniformMatrix2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x3:
					glProgramUniformMatrix2x3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix2x4:
					glProgramUniformMatrix2x4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x2:
					glProgramUniformMatrix3x2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x3:
					glProgramUniformMatrix3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix3x4:
					glProgramUniformMatrix3x4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x2:
					glProgramUniformMatrix4x2fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x3:
					glProgramUniformMatrix4x3fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				case GCT_Matrix4x4:
					glProgramUniformMatrix4fv(glProg, loc, def.arraySize, GL_FALSE, params->getFloatValuePtr(def.physicalIndex));
					break;
				default:
					// for sampler, do nothing.
					break;
				}
			}
			pi.moveNext();
		}
	}

	GLRenderSystem::GLRenderSystem() :
		mCurFrameBuffer(NULL),
		mCurFrontStencilRef(0),
		mCurBackStencilRef(0),
		mCurBlendFactor(0.0f, 0.0f, 0.0f, 0.0f),
		mGLProgram(0),
		mVSParams(NULL),
		mPSParams(NULL),
		mVSProgram(NULL),
		mPSProgram(NULL)
	{

	}

	GLRenderSystem::~GLRenderSystem()
	{

	}

	const DString& GLRenderSystem::getName() const
	{
		return msGLSystemName;
	}

	void GLRenderSystem::initialize()
	{
		InitializationOptions	op;
		initialize(op);

	}

	void GLRenderSystem::initialize( const InitializationOptions& options )
	{

		// initialize render states

		fillDeviceCaps();
		initRenderStates();
		
	}



	void GLRenderSystem::shutdown()
	{
		mGLProgram = 0;
		mVSProgram = NULL;
		mPSProgram = NULL;
		mVSParams = NULL;
		mPSParams = NULL;

	}


	void GLRenderSystem::setRasterizerState( DRasterizerStateObject* rs )
	{
		if (rs == NULL)
		{
			return;
		}
		GLRasterizerStateObject* stateObj = rs->getAs<GLRasterizerStateObject>(true);
		
		if (stateObj->mState.polygonMode != mCurRasState.polygonMode)
		{
			glPolygonMode(GL_FRONT_AND_BACK, stateObj->mGLPolygonMode);
		}
		if (stateObj->mState.shadeMode != mCurRasState.shadeMode)
		{
			glShadeModel(stateObj->mGLShadeMode);
		}
		if (stateObj->mState.cullingMode != mCurRasState.cullingMode)
		{
			if (stateObj->mbCulling)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
				glFrontFace(stateObj->mGLFrontFace);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}
		if (stateObj->mState.polygonOffsetFactor != mCurRasState.polygonOffsetFactor ||
			stateObj->mState.polygonOffsetUnit != mCurRasState.polygonOffsetUnit)
		{
			// Bias is in {0, 16}, scale the unit addition appropriately
			glPolygonOffset(stateObj->mState.polygonOffsetFactor, stateObj->mState.polygonOffsetUnit);
		}
		if (stateObj->mState.scissorEnable != mCurRasState.scissorEnable)
		{
			if (stateObj->mState.scissorEnable)
			{
				glEnable(GL_SCISSOR_TEST);
			}
			else
			{
				glDisable(GL_SCISSOR_TEST);
			}
		}
		if (stateObj->mState.multisampleEnable != mCurRasState.multisampleEnable)
		{
			if (stateObj->mState.multisampleEnable)
			{
				glEnable(GL_MULTISAMPLE);
			}
			else
			{
				glDisable(GL_MULTISAMPLE);
			}
		}
		mCurRasState = stateObj->mState;
	}


	void GLRenderSystem::setDepthStencilState( DDepthStencilStateObject* dss, uint32 frontStencilRef /*= 0*/, uint32 backStencilRef/* = 0 */)
	{
		if (dss == NULL)
		{
			return;
		}
		GLDepthStencilStateObject* stateObj = dss->getAs<GLDepthStencilStateObject>(true);

		if (stateObj->mState.depthEnable != mCurDepState.depthEnable)
		{
			if (stateObj->mState.depthEnable)
			{
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}
		if (stateObj->mState.depthWriteEnable != mCurDepState.depthWriteEnable)
		{
			glDepthMask(stateObj->mGLDepthWriteMask);
		}
		if (stateObj->mState.depthComparison != mCurDepState.depthComparison)
		{
			glDepthFunc(stateObj->mGLDepthFunc);
		}
		// front.
		if ( mCurFrontStencilRef != frontStencilRef ||
			stateObj->mState.frontStencilComparison != mCurDepState.frontStencilComparison ||
			stateObj->mState.frontStencilReadMask != mCurDepState.frontStencilReadMask)
		{
			mCurFrontStencilRef = frontStencilRef;
			glStencilFuncSeparate(GL_FRONT, stateObj->mGLFrontStencilFunc, frontStencilRef, stateObj->mState.frontStencilReadMask);
		}
		if (stateObj->mState.frontStencilFail != mCurDepState.frontStencilFail ||
			stateObj->mState.frontStencilDepthFail != mCurDepState.frontStencilDepthFail ||
			stateObj->mState.frontStencilPass != mCurDepState.frontStencilPass)
		{
			glStencilOpSeparate(GL_FRONT, stateObj->mGLFrontStencilFail,stateObj->mGLFrontStencilDepthFail,stateObj->mGLFrontStencilPass);
		}
		if (stateObj->mState.frontStencilWriteMask != mCurDepState.frontStencilWriteMask)
		{
			glStencilMaskSeparate(GL_FRONT, stateObj->mState.frontStencilWriteMask);
		}
		
		// back.
		if ( mCurBackStencilRef != backStencilRef ||
			stateObj->mState.backStencilComparison != mCurDepState.backStencilComparison ||
			stateObj->mState.backStencilReadMask != mCurDepState.backStencilReadMask)
		{
			mCurBackStencilRef = backStencilRef;
			glStencilFuncSeparate(GL_BACK, stateObj->mGLBackStencilFunc, backStencilRef, stateObj->mState.backStencilReadMask);
		}
		if (stateObj->mState.backStencilFail != mCurDepState.backStencilFail ||
			stateObj->mState.backStencilDepthFail != mCurDepState.backStencilDepthFail ||
			stateObj->mState.backStencilPass != mCurDepState.backStencilPass)
		{
			glStencilOpSeparate(GL_BACK, stateObj->mGLBackStencilFail,stateObj->mGLBackStencilDepthFail,stateObj->mGLBackStencilPass);
		}
		if (stateObj->mState.backStencilWriteMask != mCurDepState.backStencilWriteMask)
		{
			glStencilMaskSeparate(GL_BACK, stateObj->mState.backStencilWriteMask);
		}

		//
		if (stateObj->mState.frontStencilEnable != mCurDepState.frontStencilEnable ||
			stateObj->mState.backStencilEnable != mCurDepState.backStencilEnable)
		{
			if(stateObj->mState.frontStencilEnable || stateObj->mState.backStencilEnable)
			{
				glEnable(GL_STENCIL_TEST);
			}
			else
			{
				glDisable(GL_STENCIL_TEST);
			}
		}

		mCurDepState = stateObj->mState;
	}

	void GLRenderSystem::setBlendState( DBlendStateObject* sbs, const DColor& blendFactor )
	{
		if (sbs == NULL)
		{
			return;
		}
		GLBlendStateObject* stateObj = sbs->getAs<GLBlendStateObject>(true);

		if (stateObj->mState.alphaToCoverageEnable != mCurBlendState.alphaToCoverageEnable)
		{
			if (stateObj->mState.alphaToCoverageEnable)
			{
				glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
			else
			{
				glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
			}
		}
		for (uint32 i = 0; i < 8; ++ i)
		{
			if (stateObj->mState.targetStates[i].blendEnable != mCurBlendState.targetStates[i].blendEnable)
			{
				if (stateObj->mState.targetStates[i].blendEnable)
				{
					glEnablei(GL_BLEND, i);
				}
				else
				{
					glDisablei(GL_BLEND, i);
				}
			}
			if ((stateObj->mState.targetStates[i].colorBlendOpeartion != mCurBlendState.targetStates[i].colorBlendOpeartion)
				|| stateObj->mState.targetStates[i].alphaBlendOpeartion != mCurBlendState.targetStates[i].alphaBlendOpeartion)
			{
				glBlendEquationSeparatei(i,
					stateObj->mGLColorBlendOp[i], stateObj->mGLAlphaBlendOp[i]);
			}
			if ((stateObj->mState.targetStates[i].colorSrcBlend != mCurBlendState.targetStates[i].colorSrcBlend)
				|| (stateObj->mState.targetStates[i].colorDestBlend != mCurBlendState.targetStates[i].colorDestBlend)
				|| (stateObj->mState.targetStates[i].alphaSrcBlend != mCurBlendState.targetStates[i].alphaSrcBlend)
				|| (stateObj->mState.targetStates[i].alphaDestBlend != mCurBlendState.targetStates[i].alphaDestBlend))
			{
				glBlendFuncSeparatei(i,
					stateObj->mGLColorSrcBlend[i], stateObj->mGLColorDstBlend[i], 
					stateObj->mGLAlphaSrcBlend[i], stateObj->mGLAlphaDstBlend[i]);
			}
			if (stateObj->mState.targetStates[i].colorWriteMask != mCurBlendState.targetStates[i].colorWriteMask)
			{
				uint8 mask = stateObj->mState.targetStates[i].colorWriteMask;
				glColorMaski(i, (mask & CWM_Red) != 0,
					(mask & CWM_Green) != 0,
					(mask & CWM_Blue) != 0,
					(mask & CWM_Alpha) != 0);

			}
		}

		if (mCurBlendFactor != blendFactor)
		{
			glBlendColor(blendFactor.r, blendFactor.g, blendFactor.b, blendFactor.a);
		}

		mCurBlendFactor = blendFactor;
		mCurBlendState = stateObj->mState;
	}


	void GLRenderSystem::bindShaderObject( DShaderObject* so )
	{
		if (so == NULL || !so->isValid())
		{
			mVSProgram = NULL;
			mPSProgram = NULL;
			mVSParams = NULL;
			mPSProgram = NULL;
			return;
		}

		mVSProgram = so->getVertexProgram();
		mPSProgram = so->getPixelProgram();
		
		mVSParams = so->getVertexProgramParameters();
		mPSParams = so->getPixelProgramParameters();

		mGLProgram = so->getAs<GLShaderObject>()->getProgramID();
	}




	void GLRenderSystem::render( DRenderLayout* layout )
	{
		if (layout == NULL || mCurFrameBuffer == NULL ||
			mVSProgram == NULL || mPSProgram == NULL ||
			mVSParams == NULL || mPSParams == NULL)
		{
			return;
		}
		glValidateProgram(mGLProgram);
		GLint state = GL_FALSE;
		glGetProgramiv(mGLProgram, GL_VALIDATE_STATUS, &state);
		if (state == GL_FALSE)
		{
			DString vsName, psName;
			if (mVSProgram != NULL)
			{
				vsName = mVSProgram->getName();
			}
			else
			{
				vsName = "NULL";
			}
			if (mPSProgram != NULL)
			{
				psName = mPSProgram->getName();
			}
			else
			{
				psName = "NULL";
			}
			DUEL_EXCEPT_BRIEF(DException::ET_RenderFailed,
				"failed to validate gpu program with:\n VS: " + vsName + "\n " + "PS: " + psName,
				"Duel::GLRenderSystem::render");
			return;
		}
		glUseProgram(mGLProgram);

		//////////////////////////////////////////////////////////////////////////
		// 下面绑定gpu参数.

		// 由于oepngl的参数都是一块的...
		if (mVSParams == mPSParams)
		{
			bindGLParameters(mGLProgram, mVSParams);
		}
		else
		{
			if (mVSParams != NULL)
			{
				bindGLParameters(mGLProgram, mVSParams);
			}
			if (mPSParams != NULL)
			{
				bindGLParameters(mGLProgram, mPSParams);
			}
		}


		glBindVertexArray(layout->getAs<GLRenderLayout>()->getVAO()); 
		DIndexData idxData = layout->getIndexData();
		if (idxData.getIndexBuffer() != NULL)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idxData.getIndexBuffer()->getAs<GLIndexBuffer>()->getGLIndexBufferID());
			GLenum mode = GLTranslator::getGLTopologyMode(layout->getTopologyType());
			GLenum indxType;
			uint32 stepSize;
			if (idxData.getIndexBuffer()->getType() == IT_32Bit)
			{
				stepSize = sizeof(uint32);
				indxType = GL_UNSIGNED_INT;
			}
			else
			{
				stepSize = sizeof(uint16);
				indxType = GL_UNSIGNED_SHORT;
			}

			if (idxData.getIndexStart() < idxData.getIndexEnd())
			{
				void* ptr = (void*)(0 + stepSize * idxData.getIndexStart());
				uint32 elemCount = idxData.getIndexEnd() - idxData.getIndexStart();
				glDrawElements(mode, elemCount, indxType, ptr);  
			}


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);  

		//////////////////////////////////////////////////////////////////////////
		glUseProgram(0);
	}


	void GLRenderSystem::fillDeviceCaps()
	{
		//throw std::exception("The method or operation is not implemented.");
		//mCaps.

	}


	Duel::DRasterizerStateObjectPtr GLRenderSystem::getCurrentRaseterizerState()
	{
		return DRenderResourceManager::getSingleton().
			createRasterizerStateObject(mCurRasState);
	}

	Duel::DDepthStencilStateObjectPtr GLRenderSystem::getCurrentDepthStencilStateObject()
	{
		return DRenderResourceManager::getSingleton().
			createDepthStencilStateObject(mCurDepState);
	}

	Duel::DBlendStateObjectPtr GLRenderSystem::getCurrentBlendState()
	{
		return DRenderResourceManager::getSingleton().
			createBlendStateObject(mCurBlendState);
	}

	void GLRenderSystem::bindFrameBuffer( DFrameBuffer* buf )
	{
		if(buf == NULL)
		{
			mCurFrameBuffer = NULL;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return;
		}
		mCurFrameBuffer = buf;
		GLuint fbo = getCurGLFBO();
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		if (fbo != 0)
		{
			uint32 maxTargetCount = 0;
			GLenum targetList[8];
			for (uint32 i = 0; i < 8; ++i)
			{
				DRenderColorView* v = mCurFrameBuffer->getRenderColorView((ElementAttachment)(EA_Color0 + i));
				if (v != NULL)
				{
					maxTargetCount = i;
				}
				if (v != NULL && v->isEnabled())
				{
					targetList[i] = GL_COLOR_ATTACHMENT0 + i;
				}
				else
				{
					targetList[i] = GL_NONE;
				}
			}
			glDrawBuffers(maxTargetCount+1, targetList);
		}
		DViewport vp = buf->getViewport();

		// because opengl use lower-left corner as the screen origin, we must do a convertion
		GLint fboH = buf->getHeight();
		GLint low = fboH - (vp.getTop() + vp.getHeight());
		glViewport(vp.getLeft(), low, vp.getWidth(), vp.getHeight());
	}

	DFrameBuffer* GLRenderSystem::getCurrentFrameBuffer()
	{
		return mCurFrameBuffer;
	}

	GLuint GLRenderSystem::getCurGLFBO()
	{
		GLuint	fbo = 0;
		if (mCurFrameBuffer != NULL)
		{
			if ((mCurFrameBuffer->getAs<GLRenderWindow>(false)) != NULL)
			{
				fbo = mCurFrameBuffer->getAs<GLRenderWindow>()->getFBO();
			}
			else if ((mCurFrameBuffer->getAs<GLFrameBuffer>(false)) != NULL)
			{		
				fbo = mCurFrameBuffer->getAs<GLFrameBuffer>()->getFBO();
			}
		}
		return fbo;
	}


	void GLRenderSystem::initRenderStates()
	{
		DRenderResourceManager* resMgr = DRenderResourceManager::getSingletonPtr();
		DRasterizerStateObjectPtr rasPtr = resMgr->createRasterizerStateObject(mCurRasState);
		DDepthStencilStateObjectPtr depPtr = resMgr->createDepthStencilStateObject(mCurDepState);
		DBlendStateObjectPtr blendPtr = resMgr->createBlendStateObject(mCurBlendState);
		GLRasterizerStateObject* rasObj = rasPtr->getAs<GLRasterizerStateObject>();
		GLDepthStencilStateObject* depObj = depPtr->getAs<GLDepthStencilStateObject>();
		GLBlendStateObject* blendObj = blendPtr->getAs<GLBlendStateObject>();

		// rasterization-------------------------
		glPolygonMode(GL_FRONT_AND_BACK, rasObj->mGLPolygonMode);
		glShadeModel(rasObj->mGLShadeMode);
		if (rasObj->mbCulling)
		{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
			glFrontFace(rasObj->mGLFrontFace);
		}
		else
		{
			glDisable(GL_CULL_FACE);
		}
		glPolygonOffset(rasObj->mState.polygonOffsetFactor, rasObj->mState.polygonOffsetUnit);
		if (rasObj->mState.scissorEnable)
		{
			glEnable(GL_SCISSOR_TEST);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
		if (rasObj->mState.multisampleEnable)
		{
			glEnable(GL_MULTISAMPLE);
		}
		else
		{
			glDisable(GL_MULTISAMPLE);
		}

		// depth stencil----------------------------
		if (depObj->mState.depthEnable)
		{
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}
		// front 
		glDepthMask(depObj->mGLDepthWriteMask);
		glDepthFunc(depObj->mGLDepthFunc);
		glStencilFuncSeparate(GL_FRONT, depObj->mGLFrontStencilFunc, mCurFrontStencilRef, depObj->mState.frontStencilReadMask);
		glStencilOpSeparate(GL_FRONT, depObj->mGLFrontStencilFail, depObj->mGLFrontStencilDepthFail,depObj->mGLFrontStencilPass);
		glStencilMaskSeparate(GL_FRONT, depObj->mState.frontStencilWriteMask);
		// back
		glStencilFuncSeparate(GL_BACK, depObj->mGLBackStencilFunc, mCurBackStencilRef, depObj->mState.backStencilReadMask);
		glStencilOpSeparate(GL_BACK, depObj->mGLBackStencilFail, depObj->mGLBackStencilDepthFail,depObj->mGLBackStencilPass);
		glStencilMaskSeparate(GL_BACK, depObj->mState.backStencilWriteMask);
		if(depObj->mState.frontStencilEnable || depObj->mState.backStencilEnable)
		{
			glEnable(GL_STENCIL_TEST);
		}
		else
		{
			glDisable(GL_STENCIL_TEST);
		}

		// blend state-------------------------------------
		if (blendObj->mState.alphaToCoverageEnable)
		{
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		else
		{
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		}
		for (uint32 i = 0; i < 8; ++ i)
		{
			if (blendObj->mState.targetStates[i].blendEnable)
			{
				glEnablei(GL_BLEND, i);
			}
			else
			{
				glDisablei(GL_BLEND, i);
			}
			glBlendEquationSeparatei(i,
				blendObj->mGLColorBlendOp[i], blendObj->mGLAlphaBlendOp[i]);
			glBlendFuncSeparatei(i,
					blendObj->mGLColorSrcBlend[i], blendObj->mGLColorDstBlend[i], 
					blendObj->mGLAlphaSrcBlend[i], blendObj->mGLAlphaDstBlend[i]);
			
			uint8 mask = blendObj->mState.targetStates[i].colorWriteMask;
				glColorMaski(i, (mask & CWM_Red) != 0,
					(mask & CWM_Green) != 0,
					(mask & CWM_Blue) != 0,
					(mask & CWM_Alpha) != 0);
		}
		glBlendColor(mCurBlendFactor.r, mCurBlendFactor.g, mCurBlendFactor.b, mCurBlendFactor.a);
	}

}
