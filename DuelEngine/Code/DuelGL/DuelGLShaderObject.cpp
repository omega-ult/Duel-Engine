//  [8/22/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLGpuParameters.h"
#include "DuelGLShaderObject.h"
#include "DuelGLGpuProgram.h"
#include "DuelGLTranslator.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(GLShaderObject, DShaderObject);
	GLShaderObject::GLShaderObject() :
		mProgramID(0),
		mGLVSShaderID(0),
		mGLPSShaderID(0),
		mLinkedParam(NULL)
	{

	}

	GLShaderObject::~GLShaderObject()
	{
		if (mProgramID != 0)
		{
			glDeleteProgram(mProgramID);
		}
		if (mGLVSShaderID != 0)
		{
			glDeleteShader(mGLVSShaderID);
		}
		if (mGLPSShaderID != 0)
		{
			glDeleteShader(mGLPSShaderID);
		}
		if (mLinkedParam)
		{
			delete mLinkedParam;
		}
	}

	void GLShaderObject::build( DRenderPass* pass )
	{
		if (pass == NULL)
		{
			return;
		}
		if ((pass->getVertexProgram() == NULL || !pass->getVertexProgram()->isLoaded())
			|| (pass->getPixelProgram() == NULL || !pass->getPixelProgram()->isLoaded()))
		{
			return;
		}
		DShaderObject::build(pass);

		if (mProgramID != 0)
		{
			glDeleteProgram(mProgramID);
			mProgramID = 0;
		}
		if (mGLVSShaderID != 0)
		{
			glDeleteShader(mGLVSShaderID);
			mGLVSShaderID = 0;
		}
		if (mGLPSShaderID != 0)
		{
			glDeleteShader(mGLPSShaderID);
			mGLPSShaderID = 0;
		}
		GLGpuProgram* vs = pass->getVertexProgram()->getAs<GLGpuProgram>();
		GLGpuProgram* ps = pass->getPixelProgram()->getAs<GLGpuProgram>();

		mProgramID = glCreateProgram();
		mGLVSShaderID = glCreateShader(GL_VERTEX_SHADER);
		mGLPSShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		const char* vsSrc[2] = { vs->getPreProcessor().c_str(), vs->getSourceCode().c_str()};

		const char* psSrc[2] = { ps->getPreProcessor().c_str(), ps->getSourceCode().c_str()};
		glShaderSource(mGLVSShaderID, 2, vsSrc, NULL);
		glShaderSource(mGLPSShaderID, 2, psSrc, NULL);
		glAttachShader(mProgramID, mGLVSShaderID);
		glAttachShader(mProgramID, mGLPSShaderID);

		// vs-------
		glCompileShader(mGLVSShaderID);
		GLint	err = 0;
		glGetShaderiv(mGLVSShaderID,GL_COMPILE_STATUS, &err);
		if (err == GL_FALSE)
		{
			// ah failed in compilation.
			vs->setCompileErrorFlag(true);
			glGetShaderiv(mGLVSShaderID,GL_INFO_LOG_LENGTH, &err);
			uint32 errLength = err;
			char* errBuf = new char[errLength+1];
			glGetShaderInfoLog(mGLVSShaderID, err, &err, errBuf);
			errBuf[errLength] = '\0';
			vs->setCompileError(errBuf);
			delete[] errBuf;
			DLogManager::getSingleton().logMessage("Failed to load GpuProgram(GLSL) resource: " + 
				vs->getGroupName() + "/" + vs->getName());

#ifdef DUEL_DEBUG
			std::cout << vs->getName() << ":\n" << vs->getCompileError() + "\n";
#endif // DUEL_DEBUG

		}
		else
		{
			vs->setCompileErrorFlag(false);
		}
		// ps------
		glCompileShader(mGLPSShaderID);
		glGetShaderiv(mGLPSShaderID,GL_COMPILE_STATUS, &err);
		if (err == GL_FALSE)
		{
			// ah failed in compilation.
			ps->setCompileErrorFlag(true);
			glGetShaderiv(mGLPSShaderID,GL_INFO_LOG_LENGTH, &err);
			uint32 errLength = err;
			char* errBuf = new char[errLength+1];
			glGetShaderInfoLog(mGLPSShaderID, err, &err, errBuf);
			errBuf[errLength] = '\0';
			ps->setCompileError(errBuf);
			delete[] errBuf;
			DLogManager::getSingleton().logMessage("Failed to load GpuProgram(GLSL) resource: " + 
				ps->getGroupName() + "/" + ps->getName());

#ifdef DUEL_DEBUG
			std::cout << ps->getName() << ":\n" << ps->getCompileError() + "\n";
#endif // DUEL_DEBUG

		}
		else
		{
			ps->setCompileErrorFlag(false);
		}
		// if any of them failed in compile, just return
		if ( vs->hasCompileError() || ps->hasCompileError())
		{
			return;
		}

		glLinkProgram(mProgramID);
		buildParameters();

	}

	void GLShaderObject::buildParameters()
	{
		if (mLinkedParam)
		{
			delete mLinkedParam;
			mLinkedParam = NULL;
		}
		mLinkedParam = new DGpuParameters("glProgram_" + 
			DStringTool::toString((uint32) mProgramID) + "_parameters");
		
		int32 count;
		GLsizei actualLen;
		GLint arrsize;
		// 		GLint uniArrayStride;
		GLenum type;
		char *name = NULL;
		int32 maxUniLength;

		glGetProgramiv(mProgramID, GL_ACTIVE_UNIFORMS, &count);

		glGetProgramiv(mProgramID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniLength);

		if (maxUniLength != 0)
		{
			name = (char *)malloc(sizeof(char) * maxUniLength);
		}

		GpuConstantDefinition constDef;
		for (int32 i = 0; i < count; ++i)
		{
			glGetActiveUniform(mProgramID, i, maxUniLength, &actualLen, &arrsize, &type, name);
			constDef.constType = GLTranslator::getGpuConstantTypeFrom(type);
			constDef.elementSize = GpuConstantDefinition::getElementSize(constDef.constType, false); // glsl does not pad.
			constDef.arraySize = arrsize;
			GLint loc = glGetUniformLocation(mProgramID, name);
			constDef.logicalIndex = loc;
			// the logic index is determined in run-time. so we do not process it here.
			mLinkedParam->addConstant(name, constDef);
			// add for sampler.
			if (GpuConstantDefinition::isTexture(constDef.constType))
			{
				GpuConstantDefinition sampConstDef = constDef;
				sampConstDef.constType = GCT_Sampler;
				mLinkedParam->addConstant(name, constDef);
			}
		}

		if (name != NULL)
		{
			free(name);
		}
	}

	DGpuParameters* GLShaderObject::getVertexProgramParameters()
	{
		return mLinkedParam;
	}

	DGpuParameters* GLShaderObject::getPixelProgramParameters()
	{
		return mLinkedParam;
	}

}