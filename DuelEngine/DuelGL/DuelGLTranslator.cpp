//  [4/12/2013 OMEGA] created

#include "DuelGLCommon.h"
#include "DuelGLTranslator.h"

namespace Duel
{
	GLTranslator::GLFormatMap	GLTranslator::msGLFormatMap = GLTranslator::initGLFormatMap();

	GLenum GLTranslator::getGLBlendFactor( BlendFactor bf )
	{
		switch (bf)
		{
		case Duel::BF_Zero:
			return GL_ZERO;
			break;
		case Duel::BF_One:
			return GL_ONE;
			break;
		case Duel::BF_SrcAlpha:
			return GL_SRC_ALPHA;
			break;
		case Duel::BF_DstAlpha:
			return GL_DST_ALPHA;
			break;
		case Duel::BF_InvSrcAlpha:
			return GL_ONE_MINUS_SRC_ALPHA;
			break;
		case Duel::BF_InvDstAlpha:
			return GL_ONE_MINUS_DST_ALPHA;
			break;
		case Duel::BF_SrcColor:
			return GL_SRC_COLOR;
			break;
		case Duel::BF_DstColor:
			return GL_DST_COLOR;
			break;
		case Duel::BF_InvSrcColor:
			return GL_ONE_MINUS_SRC_COLOR;
			break;
		case Duel::BF_InvDstColor:
			return GL_ONE_MINUS_DST_COLOR;
			break;
		case Duel::BF_SrcAlphaSat:
			return GL_SRC_ALPHA_SATURATE;
			break;
		default:
			assert(false);
			return GL_ZERO;
			break;
		}

	}

	GLenum GLTranslator::getGLBlendOperation( BlendOperation bo )
	{
		switch (bo)
		{
		case Duel::BO_Add:
			return GL_FUNC_ADD;
			break;
		case Duel::BO_Sub:
			return GL_FUNC_SUBTRACT;
			break;
		case Duel::BO_RevSub:
			return GL_FUNC_REVERSE_SUBTRACT;
			break;
		case Duel::BO_Min:
			return GL_MIN;
			break;
		case Duel::BO_Max:
			return GL_MAX;
			break;
		default:
			assert(false);
			return GL_FUNC_ADD;
			break;
		}

	}

	GLenum GLTranslator::getGLCompareFunction( CompareFunction cf )
	{
		switch (cf)
		{
		case Duel::CF_AlwaysFail:
			return GL_NEVER;
			break;
		case Duel::CF_AlwaysPass:
			return GL_ALWAYS;
			break;
		case Duel::CF_Less:
			return GL_LESS;
			break;
		case Duel::CF_LessEqual:
			return GL_LEQUAL;
			break;
		case Duel::CF_Equal:
			return GL_EQUAL;
			break;
		case Duel::CF_NotEqual:
			return GL_NOTEQUAL;
			break;
		case Duel::CF_GreaterEqual:
			return GL_GEQUAL;
			break;
		case Duel::CF_Greater:
			return GL_GREATER;
			break;
		default:
			assert(false);
			return GL_EQUAL;
			break;
		}
	}

	GLenum GLTranslator::getGLPolygonMode( PolygonMode pm )
	{
		switch (pm)
		{
		case Duel::PM_Points:
			return GL_POINT;
			break;
		case Duel::PM_Wireframe:
			return GL_LINE;
			break;
		case Duel::PM_Solid:
			return GL_FILL;
			break;
		default:
			assert(false);
			return GL_FILL;
			break;
		}
	}

	GLenum GLTranslator::getGLShadeMode( ShadeMode sm )
	{
		switch (sm)
		{
		case Duel::SM_Flat:
			return GL_FLAT;
			break;
		case Duel::SM_Gouraud:
			return GL_SMOOTH;
			break;
		case Duel::SM_Phong:
			return GL_SMOOTH;
			break;
		default:
			assert(false);
			return GL_FLAT;
			break;
		}
	}

	GLint GLTranslator::getGLTextureAddressMode( TextureAddressMode tam )
	{
		switch (tam)
		{
		case Duel::TAM_Wrap:
			return GL_REPEAT;
			break;
		case Duel::TAM_Mirror:
			return GL_MIRRORED_REPEAT;
			break;
		case Duel::TAM_Clamp:
			return GL_CLAMP_TO_EDGE;
			break;
		case Duel::TAM_Border:
			return GL_CLAMP_TO_BORDER;
			break;
		default:
			assert(false);
			return GL_REPEAT;
			break;
		}
	}

	GLenum GLTranslator::getGLStencilOperation( StencilOperation so )
	{
		switch (so)
		{
		case Duel::SO_Keep:
			return GL_KEEP;
			break;
		case Duel::SO_Zero:
			return GL_ZERO;
			break;
		case Duel::SO_Replace:
			return GL_REPLACE;
			break;
		case Duel::SO_Increment:
			return GL_INCR;
			break;
		case Duel::SO_Decrement:
			return GL_DECR;
			break;
		case Duel::SO_IncrementWrap:
			return GL_INCR_WRAP;
			break;
		case Duel::SO_DecrementWrap:
			return GL_DECR_WRAP;
			break;
		case Duel::SO_Invert:
			return GL_INVERT;
			break;
		default:
			assert(false);
			return GL_KEEP;
			break;
		}
	}

	GLTranslator::GLFormatMap GLTranslator::initGLFormatMap()
	{
		GLFormatMap retMap;

		retMap.insert(std::make_pair(
			PF_R32G32B32A32, GLFormatGroup(GL_RGBA32UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT)));
		retMap.insert(std::make_pair(
			PF_R32G32B32A32_Float, GLFormatGroup(GL_RGBA32F_ARB, GL_RGBA, GL_FLOAT)));

		retMap.insert(std::make_pair(
			PF_R32G32B32, GLFormatGroup(GL_RGB32UI, GL_RGB_INTEGER, GL_UNSIGNED_INT)));
		retMap.insert(std::make_pair(
			PF_R32G32B32_Float, GLFormatGroup(GL_RGB32F_ARB, GL_RGB, GL_FLOAT)));


		retMap.insert(std::make_pair(
			PF_R16G16B16A16, GLFormatGroup(GL_RGBA16UI, GL_RGBA_INTEGER, GL_UNSIGNED_SHORT)));
		retMap.insert(std::make_pair(
			PF_Q16W16V16U16, GLFormatGroup(GL_RGBA16I, GL_RGBA_INTEGER, GL_SHORT)));
		retMap.insert(std::make_pair(
			PF_R16G16B16A16_Float, GLFormatGroup(GL_RGBA16F_ARB, GL_RGBA, GL_HALF_FLOAT_ARB)));

		retMap.insert(std::make_pair(
			PF_G32R32, GLFormatGroup(GL_RG32UI, GL_RG_INTEGER, GL_UNSIGNED_INT)));
		retMap.insert(std::make_pair(
			PF_G32R32_Float, GLFormatGroup(GL_RG32F, GL_RG, GL_FLOAT)));

		retMap.insert(std::make_pair(
			PF_A2B10G10R10, GLFormatGroup(GL_RGB10_A2, GL_RGBA, GL_UNSIGNED_INT_2_10_10_10_REV)));
		retMap.insert(std::make_pair(
			PF_A2R10G10B10, GLFormatGroup(GL_RGB10_A2, GL_BGRA, GL_UNSIGNED_INT_2_10_10_10_REV)));

		retMap.insert(std::make_pair(
			PF_G16R16, GLFormatGroup(GL_RG16, GL_RG, GL_UNSIGNED_SHORT)));
		retMap.insert(std::make_pair(
			PF_G16R16_Float, GLFormatGroup(GL_RG16F, GL_RG, GL_HALF_FLOAT_ARB)));

		retMap.insert(std::make_pair(
			PF_R32, GLFormatGroup(GL_R32UI, GL_RED_INTEGER_EXT, GL_UNSIGNED_INT)));
		retMap.insert(std::make_pair(
			PF_D32_Float, GLFormatGroup(GL_R32F, GL_RED, GL_FLOAT)));
		retMap.insert(std::make_pair(
			PF_R32_Float, GLFormatGroup(GL_R32F, GL_RED, GL_FLOAT)));

#ifdef DUEL_BIG_ENDIAN
		retMap.insert(std::make_pair(
			PF_A8B8G8R8, GLFormatGroup(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_INT_8_8_8_8_REV)));
		retMap.insert(std::make_pair(
			PF_A8R8G8B8, GLFormatGroup(GL_RGBA8, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV)));
		retMap.insert(std::make_pair(
			PF_B8G8R8A8, GLFormatGroup(GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_R8G8B8A8, GLFormatGroup(GL_RGBA8UI, GL_RGBA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_X8R8G8B8, GLFormatGroup(GL_RGB8, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8_REV)));
		retMap.insert(std::make_pair(
			PF_X8B8G8R8, GLFormatGroup(GL_RGB8, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV)));

		// Formats are in native endian, so R8G8B8 on little endian is
		// BGR, on big endian it is RGB.
		retMap.insert(std::make_pair(
			PF_R8G8B8, GLFormatGroup(GL_ALPHA8, GL_RGB, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_B8G8R8, GLFormatGroup(GL_ALPHA8, GL_BGR, GL_UNSIGNED_BYTE)));
#else
		retMap.insert(std::make_pair(
			PF_A8B8G8R8, GLFormatGroup(GL_RGBA8UI, GL_RGBA_INTEGER, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_A8R8G8B8, GLFormatGroup(GL_RGBA8, GL_BGRA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_B8G8R8A8, GLFormatGroup(GL_RGBA8, GL_BGRA, GL_UNSIGNED_INT_8_8_8_8)));
		retMap.insert(std::make_pair(
			PF_R8G8B8A8, GLFormatGroup(GL_RGBA8UI, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8)));
		retMap.insert(std::make_pair(
			PF_X8R8G8B8, GLFormatGroup(GL_RGB8, GL_BGRA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_X8B8G8R8, GLFormatGroup(GL_RGB8, GL_RGBA, GL_UNSIGNED_BYTE)));

		retMap.insert(std::make_pair(
			PF_R8G8B8, GLFormatGroup(GL_ALPHA8, GL_BGR, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_B8G8R8, GLFormatGroup(GL_ALPHA8, GL_RGB, GL_UNSIGNED_BYTE)));
#endif // DUEL_BIG_ENDIAN

		retMap.insert(std::make_pair(
			PF_A1R5G5B5, GLFormatGroup(GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV)));
		retMap.insert(std::make_pair(
			PF_X1R5G5B5, GLFormatGroup(GL_RGB5_A1, GL_BGRA, GL_UNSIGNED_SHORT_1_5_5_5_REV)));
		retMap.insert(std::make_pair(
			PF_R5G6B5, GLFormatGroup(GL_RGB5, GL_RGB, GL_UNSIGNED_SHORT_5_6_5)));
		retMap.insert(std::make_pair(
			PF_B5G6R5, GLFormatGroup(GL_RGB5, GL_BGR, GL_UNSIGNED_SHORT_5_6_5)));

		retMap.insert(std::make_pair(
			PF_A4R4G4B4, GLFormatGroup(GL_RGBA4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV)));
		retMap.insert(std::make_pair(
			PF_X4R4G4B4, GLFormatGroup(GL_RGBA4, GL_BGRA, GL_UNSIGNED_SHORT_4_4_4_4_REV)));

		retMap.insert(std::make_pair(
			PF_R16, GLFormatGroup(GL_R16, GL_RED, GL_UNSIGNED_SHORT)));
		retMap.insert(std::make_pair(
			PF_R16_Float, GLFormatGroup(GL_R16F, GL_RED, GL_HALF_FLOAT_ARB)));
		retMap.insert(std::make_pair(
			PF_D16, GLFormatGroup(GL_R16F, GL_RED, GL_HALF_FLOAT_ARB)));
		retMap.insert(std::make_pair(
			PF_A8L8, GLFormatGroup(GL_LUMINANCE8_ALPHA8, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_L16, GLFormatGroup(GL_LUMINANCE16, GL_LUMINANCE, GL_UNSIGNED_SHORT)));

		retMap.insert(std::make_pair(
			PF_R8, GLFormatGroup(GL_R8UI, GL_RED, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_A8, GLFormatGroup(GL_ALPHA8, GL_ALPHA, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_L8, GLFormatGroup(GL_LUMINANCE8, GL_LUMINANCE, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_A4L4, GLFormatGroup(GL_LUMINANCE4_ALPHA4, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE)));

		retMap.insert(std::make_pair(
			PF_BC1, GLFormatGroup(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_COMPRESSED_RGBA_S3TC_DXT1_EXT, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_BC2, GLFormatGroup(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_COMPRESSED_RGBA_S3TC_DXT3_EXT, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_BC3, GLFormatGroup(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_BC4, GLFormatGroup(GL_COMPRESSED_LUMINANCE_LATC1_EXT, GL_COMPRESSED_LUMINANCE, GL_UNSIGNED_BYTE)));
		retMap.insert(std::make_pair(
			PF_BC5, GLFormatGroup(GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT, GL_COMPRESSED_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE)));

		return retMap;
	}

	Duel::GLFormatGroup GLTranslator::getGLFormat( DPixelFormat pf )
	{
		GLFormatMap::iterator i = msGLFormatMap.find(pf);
		if (i == msGLFormatMap.end())
		{
			return GLFormatGroup(GL_NONE, GL_NONE, GL_NONE);
		}
		return i->second;
	}

	GLenum GLTranslator::getGLUsage( HardwareBufferUsage usage )
	{
		switch (usage)
		{
		case Duel::HBU_Static:
		case Duel::HBU_StaticWriteOnly:
			return GL_STATIC_DRAW;
			break;
		case Duel::HBU_Dynamic:
		case Duel::HBU_DynamicWriteOnly:
			return GL_DYNAMIC_DRAW;
			break;
		case Duel::HBU_DynamicWriteOnlyDiscardable:
			return GL_STREAM_DRAW;
			break;
		default:
			return GL_DYNAMIC_DRAW;
			break;
		}
	}

	GLenum GLTranslator::getGLVertexElementType( VertexElementType t )
	{
		switch(t)
		{
		case VET_Float1:
		case VET_Float2:
		case VET_Float3:
		case VET_Float4:
			return GL_FLOAT;
		case VET_Short1:
		case VET_Short2:
		case VET_Short3:
		case VET_Short4:
			return GL_SHORT;
		case VET_ARGB:
		case VET_ABGR:
		case VET_UByte4:
			return GL_UNSIGNED_BYTE;
		default:
			return 0;
		}

	}

	void GLTranslator::getGLVertexFormat( GLuint& size, GLenum& gltype, GLboolean& normalized, VertexElementType vf )
	{
		switch (vf)
		{
		case Duel::VET_ARGB:
			size = 4;
			gltype = GL_UNSIGNED_BYTE;
			normalized = GL_TRUE;
			break;
		case Duel::VET_ABGR:
			size = 4;
			gltype = GL_UNSIGNED_BYTE;
			normalized = GL_TRUE;
			break;
		case Duel::VET_Byte4:
			size = 4;
			gltype = GL_BYTE;
			normalized = GL_FALSE;
		case Duel::VET_UByte4:
			size = 4;
			gltype = GL_UNSIGNED_BYTE;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Float1:
			size = 1;
			gltype = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Float2:
			size = 2;
			gltype = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Float3:
			size = 3;
			gltype = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Float4:
			size = 4;
			gltype = GL_FLOAT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Short1:
			size = 1;
			gltype = GL_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Short2:
			size = 2;
			gltype = GL_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Short3:
			size = 3;
			gltype = GL_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Short4:
			size = 4;
			gltype = GL_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UShort1:
			size = 1;
			gltype = GL_UNSIGNED_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UShort2:
			size = 2;
			gltype = GL_UNSIGNED_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UShort3:
			size = 3;
			gltype = GL_UNSIGNED_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UShort4:
			size = 4;
			gltype = GL_UNSIGNED_SHORT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Int1:
			size = 1;
			gltype = GL_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Int2:
			size = 2;
			gltype = GL_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Int3:
			size = 3;
			gltype = GL_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_Int4:
			size = 4;
			gltype = GL_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UInt1:
			size = 1;
			gltype = GL_UNSIGNED_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UInt2:
			size = 2;
			gltype = GL_UNSIGNED_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UInt3:
			size = 3;
			gltype = GL_UNSIGNED_INT;
			normalized = GL_FALSE;
			break;
		case Duel::VET_UInt4:
			size = 4;
			gltype = GL_UNSIGNED_INT;
			normalized = GL_FALSE;
			break;
// 		case Duel::VET_FORCE_UINT:
// 			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
// 				"invalid parameter with VertexElementType",
// 				"Duel::getGLVertexFormat")
// 				break;
		default:
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"invalid parameter with VertexElementType",
				"Duel::getGLVertexFormat")
				break;
		}
	}

	GLenum GLTranslator::getGLTopologyMode( PrimitiveTopology topo )
	{
		switch (topo)
		{
		case Duel::PT_PointList:
			return GL_POINT;
			break;
		case Duel::PT_LineList:
			return GL_LINES;
			break;
		case Duel::PT_LineStrip:
			return GL_LINE_STRIP;
			break;
		case Duel::PT_TriangleList:
			return GL_TRIANGLES;
			break;
		case Duel::PT_TriangleStrip:
			return GL_TRIANGLE_STRIP;
			break;
		case Duel::PT_TriangleFan:
			return GL_TRIANGLE_FAN;
			break;
		default:
			return GL_POINT;
			break;
		}
		return GL_POINT;
	}

	Duel::GpuConstantType GLTranslator::getGpuConstantTypeFrom( GLenum type )
	{
		switch (type)
		{
		case GL_FLOAT:
			return GCT_Float1;
		case GL_FLOAT_VEC2:
			return GCT_Float2;
		case GL_FLOAT_VEC3:
			return GCT_Float3;
		case GL_FLOAT_VEC4:
			return GCT_Float4;
		case GL_INT:
			return GCT_Int1;
		case GL_INT_VEC2:
			return GCT_Int2;
		case GL_INT_VEC3:
			return GCT_Int3;
		case GL_INT_VEC4:
			return GCT_Int4;
		case GL_FLOAT_MAT2:
			return GCT_Matrix2x2;
		case GL_FLOAT_MAT3:
			return GCT_Matrix3x3;
		case GL_FLOAT_MAT4:
			return GCT_Matrix4x4;
		case GL_FLOAT_MAT2x3:
			return GCT_Matrix2x3;
		case GL_FLOAT_MAT2x4:
			return GCT_Matrix2x4;
		case GL_FLOAT_MAT3x2:
			return GCT_Matrix3x2;
		case GL_FLOAT_MAT3x4:
			return GCT_Matrix3x4;
		case GL_FLOAT_MAT4x2:
			return GCT_Matrix4x2;
		case GL_FLOAT_MAT4x3:
			return GCT_Matrix4x3;
		case GL_SAMPLER_1D:
			return GCT_Texture1D;
		case GL_SAMPLER_2D:
			return GCT_Texture2D;
		case GL_SAMPLER_3D:
			return GCT_Texture3D;
		case GL_SAMPLER_CUBE:
			return GCT_TextureCube;
		default:
			return GCT_Unknown;
			break;
		}
		return GCT_Unknown;

	}


}