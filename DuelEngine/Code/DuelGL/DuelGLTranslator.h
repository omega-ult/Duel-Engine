//  [4/7/2013 OMEGA] created

#ifndef _DUELGLTRANSLATOR_H_
#define _DUELGLTRANSLATOR_H_

#include "Duel.h"
#include "DuelGLCommon.h"


namespace Duel
{
	struct GLFormatGroup
	{
	public:
		GLFormatGroup(GLint interFmt, GLenum fmt, GLenum type) :
			internalFormat(interFmt), glFormat(fmt), glFormatType(type) {}
		GLint	internalFormat;
		GLenum	glFormat;
		GLenum	glFormatType;
	};

	class DUELGL_API GLTranslator
	{
	public:
		static GLenum			getGLBlendFactor(BlendFactor bf);
		static GLenum			getGLBlendOperation(BlendOperation bo);
		static GLenum			getGLCompareFunction(CompareFunction cf);
		static GLenum			getGLPolygonMode(PolygonMode pm);
		static GLenum			getGLShadeMode(ShadeMode sm);
		static GLint			getGLTextureAddressMode(TextureAddressMode tam);
		static GLenum			getGLStencilOperation(StencilOperation so);
		static GLFormatGroup	getGLFormat(DPixelFormat pf);
		static GLenum			getGLUsage(HardwareBufferUsage usage);
		static GLenum			getGLVertexElementType(VertexElementType t);
		static void				getGLVertexFormat(GLuint& size, GLenum& gltype, GLboolean& normalized, VertexElementType vf);
		static GLenum			getGLTopologyMode(PrimitiveTopology topo);
		static GpuConstantType	getGpuConstantTypeFrom(GLenum type);
	protected:
		typedef	std::map<DPixelFormat, GLFormatGroup>	GLFormatMap;
		static GLFormatMap		msGLFormatMap;
		static GLFormatMap		initGLFormatMap();
	};
	
}

#endif