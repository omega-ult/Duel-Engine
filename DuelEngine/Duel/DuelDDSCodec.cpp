//  [8/24/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelDDSCodec.h"
#include "DuelImageDesc.h"
#include "DuelException.h"
#include "DuelBitTool.h"

#ifdef DUEL_PLATFORM_WINDOWS
#include <dxgi.h>
#else
enum DXGI_FORMAT
{
	DXGI_FORMAT_UNKNOWN	                    = 0,
	DXGI_FORMAT_R32G32B32A32_TYPELESS       = 1,
	DXGI_FORMAT_R32G32B32A32_FLOAT          = 2,
	DXGI_FORMAT_R32G32B32A32_UINT           = 3,
	DXGI_FORMAT_R32G32B32A32_SINT           = 4,
	DXGI_FORMAT_R32G32B32_TYPELESS          = 5,
	DXGI_FORMAT_R32G32B32_FLOAT             = 6,
	DXGI_FORMAT_R32G32B32_UINT              = 7,
	DXGI_FORMAT_R32G32B32_SINT              = 8,
	DXGI_FORMAT_R16G16B16A16_TYPELESS       = 9,
	DXGI_FORMAT_R16G16B16A16_FLOAT          = 10,
	DXGI_FORMAT_R16G16B16A16_UNORM          = 11,
	DXGI_FORMAT_R16G16B16A16_UINT           = 12,
	DXGI_FORMAT_R16G16B16A16_SNORM          = 13,
	DXGI_FORMAT_R16G16B16A16_SINT           = 14,
	DXGI_FORMAT_R32G32_TYPELESS             = 15,
	DXGI_FORMAT_R32G32_FLOAT                = 16,
	DXGI_FORMAT_R32G32_UINT                 = 17,
	DXGI_FORMAT_R32G32_SINT                 = 18,
	DXGI_FORMAT_R32G8X24_TYPELESS           = 19,
	DXGI_FORMAT_D32_FLOAT_S8X24_UINT        = 20,
	DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS    = 21,
	DXGI_FORMAT_X32_TYPELESS_G8X24_UINT     = 22,
	DXGI_FORMAT_R10G10B10A2_TYPELESS        = 23,
	DXGI_FORMAT_R10G10B10A2_UNORM           = 24,
	DXGI_FORMAT_R10G10B10A2_UINT            = 25,
	DXGI_FORMAT_R11G11B10_FLOAT             = 26,
	DXGI_FORMAT_R8G8B8A8_TYPELESS           = 27,
	DXGI_FORMAT_R8G8B8A8_UNORM              = 28,
	DXGI_FORMAT_R8G8B8A8_UNORM_SRGB         = 29,
	DXGI_FORMAT_R8G8B8A8_UINT               = 30,
	DXGI_FORMAT_R8G8B8A8_SNORM              = 31,
	DXGI_FORMAT_R8G8B8A8_SINT               = 32,
	DXGI_FORMAT_R16G16_TYPELESS             = 33,
	DXGI_FORMAT_R16G16_FLOAT                = 34,
	DXGI_FORMAT_R16G16_UNORM                = 35,
	DXGI_FORMAT_R16G16_UINT                 = 36,
	DXGI_FORMAT_R16G16_SNORM                = 37,
	DXGI_FORMAT_R16G16_SINT                 = 38,
	DXGI_FORMAT_R32_TYPELESS                = 39,
	DXGI_FORMAT_D32_FLOAT                   = 40,
	DXGI_FORMAT_R32_FLOAT                   = 41,
	DXGI_FORMAT_R32_UINT                    = 42,
	DXGI_FORMAT_R32_SINT                    = 43,
	DXGI_FORMAT_R24G8_TYPELESS              = 44,
	DXGI_FORMAT_D24_UNORM_S8_UINT           = 45,
	DXGI_FORMAT_R24_UNORM_X8_TYPELESS       = 46,
	DXGI_FORMAT_X24_TYPELESS_G8_UINT        = 47,
	DXGI_FORMAT_R8G8_TYPELESS               = 48,
	DXGI_FORMAT_R8G8_UNORM                  = 49,
	DXGI_FORMAT_R8G8_UINT                   = 50,
	DXGI_FORMAT_R8G8_SNORM                  = 51,
	DXGI_FORMAT_R8G8_SINT                   = 52,
	DXGI_FORMAT_R16_TYPELESS                = 53,
	DXGI_FORMAT_R16_FLOAT                   = 54,
	DXGI_FORMAT_D16_UNORM                   = 55,
	DXGI_FORMAT_R16_UNORM                   = 56,
	DXGI_FORMAT_R16_UINT                    = 57,
	DXGI_FORMAT_R16_SNORM                   = 58,
	DXGI_FORMAT_R16_SINT                    = 59,
	DXGI_FORMAT_R8_TYPELESS                 = 60,
	DXGI_FORMAT_R8_UNORM                    = 61,
	DXGI_FORMAT_R8_UINT                     = 62,
	DXGI_FORMAT_R8_SNORM                    = 63,
	DXGI_FORMAT_R8_SINT                     = 64,
	DXGI_FORMAT_A8_UNORM                    = 65,
	DXGI_FORMAT_R1_UNORM                    = 66,
	DXGI_FORMAT_R9G9B9E5_SHAREDEXP          = 67,
	DXGI_FORMAT_R8G8_B8G8_UNORM             = 68,
	DXGI_FORMAT_G8R8_G8B8_UNORM             = 69,
	DXGI_FORMAT_BC1_TYPELESS                = 70,
	DXGI_FORMAT_BC1_UNORM                   = 71,
	DXGI_FORMAT_BC1_UNORM_SRGB              = 72,
	DXGI_FORMAT_BC2_TYPELESS                = 73,
	DXGI_FORMAT_BC2_UNORM                   = 74,
	DXGI_FORMAT_BC2_UNORM_SRGB              = 75,
	DXGI_FORMAT_BC3_TYPELESS                = 76,
	DXGI_FORMAT_BC3_UNORM                   = 77,
	DXGI_FORMAT_BC3_UNORM_SRGB              = 78,
	DXGI_FORMAT_BC4_TYPELESS                = 79,
	DXGI_FORMAT_BC4_UNORM                   = 80,
	DXGI_FORMAT_BC4_SNORM                   = 81,
	DXGI_FORMAT_BC5_TYPELESS                = 82,
	DXGI_FORMAT_BC5_UNORM                   = 83,
	DXGI_FORMAT_BC5_SNORM                   = 84,
	DXGI_FORMAT_B5G6R5_UNORM                = 85,
	DXGI_FORMAT_B5G5R5A1_UNORM              = 86,
	DXGI_FORMAT_B8G8R8A8_UNORM              = 87,
	DXGI_FORMAT_B8G8R8X8_UNORM              = 88,
	DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM  = 89,
	DXGI_FORMAT_B8G8R8A8_TYPELESS           = 90,
	DXGI_FORMAT_B8G8R8A8_UNORM_SRGB         = 91,
	DXGI_FORMAT_B8G8R8X8_TYPELESS           = 92,
	DXGI_FORMAT_B8G8R8X8_UNORM_SRGB         = 93,
	DXGI_FORMAT_BC6H_TYPELESS               = 94,
	DXGI_FORMAT_BC6H_UF16                   = 95,
	DXGI_FORMAT_BC6H_SF16                   = 96,
	DXGI_FORMAT_BC7_TYPELESS                = 97,
	DXGI_FORMAT_BC7_UNORM                   = 98,
	DXGI_FORMAT_BC7_UNORM_SRGB              = 99,
	DXGI_FORMAT_FORCE_UINT                  = 0xffffffff
};
#endif // DUEL_PLATFORM_WINDOWS


namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DDSCodec, DMediaCodec);
	
	// internal method used for getting dxgiformat
	DXGI_FORMAT	getDXGIFormat(DPixelFormat pf)
	{
		switch(pf)
		{
		case PF_R32G32B32A32:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		case PF_R32G32B32A32_Float:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case PF_R32G32B32:
			return DXGI_FORMAT_R32G32B32_UINT;
		case PF_R32G32B32_Float:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case PF_R16G16B16A16:
			return DXGI_FORMAT_R16G16B16A16_UNORM;
		case PF_Q16W16V16U16:
			return DXGI_FORMAT_R16G16B16A16_SNORM;
		case PF_R16G16B16A16_Float:
			return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case PF_G32R32:
			return DXGI_FORMAT_R32G32_UINT;
		case PF_G32R32_Float:
			return DXGI_FORMAT_R32G32_FLOAT;
		case PF_A2B10G10R10:
			return DXGI_FORMAT_R10G10B10A2_UNORM;
		case PF_A2R10G10B10:
			return DXGI_FORMAT_UNKNOWN;
		case PF_G16R16:
			return DXGI_FORMAT_R16G16_UNORM;
		case PF_G16R16_Float:
			return DXGI_FORMAT_R16G16_FLOAT;
		case PF_R32:
			return DXGI_FORMAT_R32_UINT;
		case PF_D32_Float:
			return DXGI_FORMAT_D32_FLOAT;
		case PF_R32_Float:
			return DXGI_FORMAT_R32_FLOAT;
		case PF_A8B8G8R8:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case PF_A8R8G8B8:
			return DXGI_FORMAT_B8G8R8A8_UNORM;
		case PF_B8G8R8A8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R8G8B8A8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_X8R8G8B8:
			return DXGI_FORMAT_B8G8R8X8_UNORM;
		case PF_X8B8G8R8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R8G8B8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_B8G8R8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A1R5G5B5:
			return DXGI_FORMAT_B5G5R5A1_UNORM;
		case PF_X1R5G5B5:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R5G6B5:
			return DXGI_FORMAT_B5G6R5_UNORM;
		case PF_B5G6R5:
			return DXGI_FORMAT_UNKNOWN;
		case PF_A4R4G4B4:
			return DXGI_FORMAT_UNKNOWN;
		case PF_X4R4G4B4:
			return DXGI_FORMAT_UNKNOWN;
		case PF_R16:
			return DXGI_FORMAT_R16_UNORM;
		case PF_R16_Float:
			return DXGI_FORMAT_R16_FLOAT;
		case PF_D16:
			return DXGI_FORMAT_D16_UNORM;
		case PF_A8L8:
			return DXGI_FORMAT_UNKNOWN;
		case PF_L16:
			return DXGI_FORMAT_R16_UNORM;
		case PF_R8:
			return DXGI_FORMAT_R8_UNORM;
		case PF_A8:
			return DXGI_FORMAT_A8_UNORM;
		case PF_L8:
			return DXGI_FORMAT_R8_UNORM;
		case PF_A4L4:
			return DXGI_FORMAT_UNKNOWN;
		case PF_BC1:
			return DXGI_FORMAT_BC1_UNORM;
		case PF_BC2:
			return DXGI_FORMAT_BC2_UNORM;
		case PF_BC3:
			return DXGI_FORMAT_BC3_UNORM;
		case PF_BC4:
			return DXGI_FORMAT_BC4_UNORM;
		case PF_BC5:
			return DXGI_FORMAT_BC5_UNORM;
		default:
			return DXGI_FORMAT_UNKNOWN;
		}
	}
	DPixelFormat	getPixelFormat(DXGI_FORMAT pf)
	{
		switch(pf)
		{
		case DXGI_FORMAT_R32G32B32A32_UINT:
			return PF_R32G32B32A32;
		case DXGI_FORMAT_R32G32B32A32_FLOAT:
			return PF_R32G32B32A32_Float;
		case DXGI_FORMAT_R32G32B32_UINT:
			return PF_R32G32B32;
		case DXGI_FORMAT_R32G32B32_FLOAT:
			return PF_R32G32B32_Float;
		case DXGI_FORMAT_R16G16B16A16_UNORM:
			return PF_R16G16B16A16;
		case DXGI_FORMAT_R16G16B16A16_SNORM:
			return PF_Q16W16V16U16;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return PF_R16G16B16A16_Float;
		case DXGI_FORMAT_R32G32_UINT:
			return PF_G32R32;
		case DXGI_FORMAT_R32G32_FLOAT:
			return PF_G32R32_Float;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			return PF_A2B10G10R10;
		case DXGI_FORMAT_UNKNOWN:
			return PF_A2R10G10B10;
		case DXGI_FORMAT_R16G16_UNORM:
			return PF_G16R16;
		case DXGI_FORMAT_R16G16_FLOAT:
			return PF_G16R16_Float;
		case DXGI_FORMAT_R32_UINT:
			return PF_R32;
		case DXGI_FORMAT_D32_FLOAT:
			return PF_D32_Float;
		case DXGI_FORMAT_R32_FLOAT:
			return PF_R32_Float;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return PF_A8B8G8R8;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return PF_A8R8G8B8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_B8G8R8A8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_R8G8B8A8;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			return PF_X8R8G8B8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_X8B8G8R8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_R8G8B8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_B8G8R8;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return PF_A1R5G5B5;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_X1R5G5B5;
		case DXGI_FORMAT_B5G6R5_UNORM:
			return PF_R5G6B5;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_B5G6R5;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_A4R4G4B4;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_X4R4G4B4;
		case DXGI_FORMAT_R16_UNORM:
			return PF_R16;
		case DXGI_FORMAT_R16_FLOAT:
			return PF_R16_Float;
		case DXGI_FORMAT_D16_UNORM:
			return PF_D16;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_A8L8;
// 		case DXGI_FORMAT_R16_UNORM:
// 			return PF_L16;
		case DXGI_FORMAT_R8_UNORM:
			return PF_R8;
		case DXGI_FORMAT_A8_UNORM:
			return PF_A8;
// 		case DXGI_FORMAT_R8_UNORM:
// 			return PF_L8;
// 		case DXGI_FORMAT_UNKNOWN:
// 			return PF_A4L4;
		case DXGI_FORMAT_BC1_UNORM:
			return PF_BC1;
// 		case DXGI_FORMAT_BC2_UNORM:
// 			return PF_DXT2;
		case DXGI_FORMAT_BC2_UNORM:
			return PF_BC3;
// 		case DXGI_FORMAT_BC3_UNORM:
// 			return PF_DXT4;
		case DXGI_FORMAT_BC3_UNORM:
			return PF_BC5;
		default:
			return PF_Unknown;
		}
	}
// for enum value alignment.
#pragma pack(push, 1)

#ifndef FOURCC
#define FOURCC(c0, c1, c2, c3) (c0 | (c1 << 8) | (c2 << 16) | (c3 << 24))
#endif

	struct DDSPixelFormat
	{
		uint32	size;		// size of structure
		uint32	flags;		// pixel format flags
		uint32	fourCC;		// (FOURCC code)
		uint32	rgbBits;	// how many bits per pixel
		uint32	rMask;		// mask for red bit
		uint32	gMask;		// mask for green bits
		uint32	bMask;		// mask for blue bits
		uint32	aMask;		// mask for alpha channels
	};

	struct DDSCaps
	{
		uint32	caps1;		// caps that surface needed.
		uint32	caps2;
		uint32	reserved[2];
	};

	// main header of dds file
	struct DDSHeader
	{
		uint32	size;					// size of the DDSURFACEDESC structure
		uint32	flags;					// determines what fields are valid
		uint32	height;					// height of surface to be created
		uint32	width;					// width of input surface
		union
		{
			int32	pitch;				// distance to start of next line (return value only)
			uint32	linearSize;			// Formless late-allocated optimized surface size
		};
		uint32			depth;				// the depth if this is a volume texture
		uint32			mipMapCount;		// number of mip-map levels requestde
		uint32			reserved1[11];		// reserved
		DDSPixelFormat	pixelFormat;		// pixel format description of the surface
		DDSCaps			ddsCaps;			// direct draw surface capabilities
		uint32			reserved2;
	};

	enum D3D10ResouceDimension
	{
		D3D10_RESOURCE_DIMENSION_UNKNOWN	= 0,
		D3D10_RESOURCE_DIMENSION_BUFFER		= 1,
		D3D10_RESOURCE_DIMENSION_TEXTURE1D	= 2,
		D3D10_RESOURCE_DIMENSION_TEXTURE2D	= 3,
		D3D10_RESOURCE_DIMENSION_TEXTURE3D	= 4,
	};

	enum D3D10ResourceMiscFlag
	{
		D3D10_RESOURCE_MISC_GENERATE_MIPS		= 0x00000001,
		D3D10_RESOURCE_MISC_SHARED				= 0x00000002,
		D3D10_RESOURCE_MISC_TEXTURECUBE			= 0x00000004,
		D3D10_RESOURCE_MISC_SHARED_KEYEDMUTEX	= 0x00000010,
		D3D10_RESOURCE_MISC_GDI_COMPATIBLE		= 0x00000020
	};

	struct DDSHeaderDXT10
	{
		uint32 dxgiFormat;
		D3D10ResouceDimension resDimension;
		uint32 miscFlag;
		uint32 arraySize;
		uint32 reserved;
	};

	//////////////////////////////////////////////////////////////////////////
	// const defination, grouped in enum type, referd KlayGE
	enum
	{
		DDSD_CAPS			= 0x00000001,	// default, dds_caps field is valid.
		DDSD_HEIGHT			= 0x00000002,	// height field is valid.
		DDSD_WIDTH			= 0x00000004,	// width field is valid.
		DDSD_PITCH			= 0x00000008,	// pitch is valid.
		DDSD_PIXELFORMAT	= 0x00001000,	// pixel_format is valid.
		DDSD_MIPMAPCOUNT	= 0x00020000,	// mip_map_count is valid.
		DDSD_LINEARSIZE		= 0x00080000,	// linear_size is valid
		DDSD_DEPTH			= 0x00800000,	// depth is valid
	};

	enum
	{
		// The surface has alpha channel information in the pixel format.
		DDSPF_ALPHAPIXELS = 0x00000001,

		// The pixel format contains alpha only information
		DDSPF_ALPHA = 0x00000002,

		// The FourCC code is valid.
		DDSPF_FOURCC = 0x00000004,

		// The RGB data in the pixel format structure is valid.
		DDSPF_RGB = 0x00000040,

		// Luminance data in the pixel format is valid.
		// Use this flag for luminance-only or luminance+alpha surfaces,
		// the bit depth is then ddpf.dwLuminanceBitCount.
		DDSPF_LUMINANCE = 0x00020000,

	};

	enum
	{
		// Indicates a complex surface structure is being described.  A
		// complex surface structure results in the creation of more than
		// one surface.  The additional surfaces are attached to the root
		// surface.  The complex structure can only be destroyed by
		// destroying the root.
		DDSCAPS_COMPLEX		= 0x00000008,

		// Indicates that this surface can be used as a 3D texture.  It does not
		// indicate whether or not the surface is being used for that purpose.
		DDSCAPS_TEXTURE		= 0x00001000,

		// Indicates surface is one level of a mip-map. This surface will
		// be attached to other DDSCAPS_MIPMAP surfaces to form the mip-map.
		// This can be done explicitly, by creating a number of surfaces and
		// attaching them with AddAttachedSurface or by implicitly by CreateSurface.
		// If this bit is set then DDSCAPS_TEXTURE must also be set.
		DDSCAPS_MIPMAP		= 0x00400000,
	};

	enum
	{
		// This flag is used at CreateSurface time to indicate that this set of
		// surfaces is a cubic environment map
		DDSCAPS2_CUBEMAP	= 0x00000200,

		// These flags preform two functions:
		// - At CreateSurface time, they define which of the six cube faces are
		//   required by the application.
		// - After creation, each face in the cubemap will have exactly one of these
		//   bits set.
		DDSCAPS2_CUBEMAP_POSITIVEX	= 0x00000400,
		DDSCAPS2_CUBEMAP_NEGATIVEX	= 0x00000800,
		DDSCAPS2_CUBEMAP_POSITIVEY	= 0x00001000,
		DDSCAPS2_CUBEMAP_NEGATIVEY	= 0x00002000,
		DDSCAPS2_CUBEMAP_POSITIVEZ	= 0x00004000,
		DDSCAPS2_CUBEMAP_NEGATIVEZ	= 0x00008000,

		// Indicates that the surface is a volume.
		// Can be combined with DDSCAPS_MIPMAP to indicate a multi-level volume
		DDSCAPS2_VOLUME		= 0x00200000,
	};

	//////////////////////////////////////////////////////////////////////////

#pragma pack(pop)

	DDataStreamPtr DDSCodec::encode( DDataStream* input, DMediaDesc* imageDesc )
	{
		// refered KlayGE and:
		// http://msdn.microsoft.com/en-us/library/bb943991.aspx
		// check magic number

		uint32	magic = FOURCC('D','D','S',' ');
		NativeToLittle(&magic);

		// used to record file size.
		size_t		memorySize = 0;
		memorySize	+= sizeof(magic);

		DDSHeader	header;
		ZeroMemory(&header, sizeof(header));

		DImageDesc *srcDesc = imageDesc->getAs<DImageDesc>(false);
		if (srcDesc == NULL)
		{
			return DDataStreamPtr();
		}

		header.size	= sizeof(header);
		memorySize	+= header.size;

		header.flags	|= DDSD_WIDTH;
		header.flags	|= DDSD_HEIGHT;

		header.width	= srcDesc->width;
		header.height	= srcDesc->height;

		if (srcDesc->mipMapCount != 0)
		{
			header.flags |= DDSD_MIPMAPCOUNT;
			header.mipMapCount = srcDesc->mipMapCount;
		}

		header.pixelFormat.size = sizeof(header.pixelFormat);

		if (srcDesc->hasProperty(IP_SRGB))
		{
			header.reserved1[0] = 1;
		}

		if (srcDesc->arraySize != 1)
		{
			header.pixelFormat.flags |= DDSPF_FOURCC;
			header.pixelFormat.fourCC  = FOURCC('D','X','1','0');
		}

		// indicate if the format spcification using DDSPF_FOUCC / RGB format
		// if the format pass the first format check(all false) 
		// it should be a fourcc format.
		bool isFourccFormat = true;
		bool isRGBFormat	= true;
		switch(srcDesc->pixelFormat)
		{
		case PF_A8B8G8R8:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0xff000000;
			header.pixelFormat.rMask	= 0x000000ff;
			header.pixelFormat.gMask	= 0x0000ff00;
			header.pixelFormat.bMask	= 0x00ff0000;
			isRGBFormat		= true;
			break;
		case PF_G16R16:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0x00000000;
			header.pixelFormat.rMask	= 0x0000ffff;
			header.pixelFormat.gMask	= 0xffff0000;
			header.pixelFormat.bMask	= 0x00000000;
			isRGBFormat		= true;
			break;
		case PF_A2B10G10R10:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0x00000000;
			header.pixelFormat.rMask	= 0x000003ff;
			header.pixelFormat.gMask	= 0x000ffc00;
			header.pixelFormat.bMask	= 0x3ff00000;
			isRGBFormat		= true;
			break;
		case PF_A8R8G8B8:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0xff000000;
			header.pixelFormat.rMask	= 0x00ff0000;
			header.pixelFormat.gMask	= 0x0000ff00;
			header.pixelFormat.bMask	= 0x000000ff;
			isRGBFormat		= true;
			break;
		case PF_X8R8G8B8:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0x00000000;
			header.pixelFormat.rMask	= 0x00ff0000;
			header.pixelFormat.gMask	= 0x0000ff00;
			header.pixelFormat.bMask	= 0x000000ff;
			isRGBFormat		= true;
			break;
		case PF_X8B8G8R8:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0x00000000;
			header.pixelFormat.rMask	= 0x000000ff;
			header.pixelFormat.gMask	= 0x0000ff00;
			header.pixelFormat.bMask	= 0x00ff0000;
			isRGBFormat		= true;
			break;
		case PF_A2R10G10B10:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 32;
			header.pixelFormat.aMask	= 0xc0000000;
			header.pixelFormat.rMask	= 0x3ff00000;
			header.pixelFormat.gMask	= 0x000ffc00;
			header.pixelFormat.bMask	= 0x000003ff;
			isRGBFormat		= true;
			break;
		case PF_R8G8B8:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 24;
			header.pixelFormat.aMask	= 0x000000;
			header.pixelFormat.rMask	= 0xff0000;
			header.pixelFormat.gMask	= 0x00ff00;
			header.pixelFormat.bMask	= 0x0000ff;
			isRGBFormat		= true;
			break;
		case PF_A1R5G5B5:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0x8000;
			header.pixelFormat.rMask	= 0x7c00;
			header.pixelFormat.gMask	= 0x03e0;
			header.pixelFormat.bMask	= 0x001f;
			isRGBFormat		= true;
			break;
		case PF_R5G6B5:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0x0000;
			header.pixelFormat.rMask	= 0xf800;
			header.pixelFormat.gMask	= 0x07e0;
			header.pixelFormat.bMask	= 0x001f;
			isRGBFormat		= true;
			break;
		case PF_X1R5G5B5:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0x0000;
			header.pixelFormat.rMask	= 0x7c00;
			header.pixelFormat.gMask	= 0x03e0;
			header.pixelFormat.bMask	= 0x001f;
			isRGBFormat		= true;
			break;
		case PF_A4R4G4B4:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0xf000;
			header.pixelFormat.rMask	= 0x0f00;
			header.pixelFormat.gMask	= 0x00f0;
			header.pixelFormat.bMask	= 0x000f;
			isRGBFormat		= true;
			break;
		case PF_X4R4G4B4:
			header.pixelFormat.flags	|= DDSPF_RGB;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0x0000;
			header.pixelFormat.rMask	= 0x0f00;
			header.pixelFormat.gMask	= 0x00f0;
			header.pixelFormat.bMask	= 0x000f;
			isRGBFormat		= true;
			break;
		case PF_A8L8:
			header.pixelFormat.flags	|= DDSPF_LUMINANCE;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0xff00;
			header.pixelFormat.rMask	= 0x00ff;
			header.pixelFormat.gMask	= 0x0000;
			header.pixelFormat.bMask	= 0x0000;
			isRGBFormat		= true;
			break;
		case PF_L16:
			header.pixelFormat.flags	|= DDSPF_LUMINANCE;
			header.pixelFormat.rgbBits	= 16;
			header.pixelFormat.aMask	= 0x0000;
			header.pixelFormat.rMask	= 0xffff;
			header.pixelFormat.gMask	= 0x0000;
			header.pixelFormat.bMask	= 0x0000;
			isRGBFormat		= true;
			break;
		case PF_A8:
			header.pixelFormat.flags	|= DDSPF_ALPHA;
			header.pixelFormat.rgbBits	= 8;
			header.pixelFormat.aMask	= 0xff;
			header.pixelFormat.rMask	= 0x00;
			header.pixelFormat.gMask	= 0x00;
			header.pixelFormat.bMask	= 0x00;
			isRGBFormat		= true;
			break;
		case PF_L8:
			header.pixelFormat.flags	|= DDSPF_LUMINANCE;
			header.pixelFormat.rgbBits	= 8;
			header.pixelFormat.aMask	= 0x00;
			header.pixelFormat.rMask	= 0xff;
			header.pixelFormat.gMask	= 0x00;
			header.pixelFormat.bMask	= 0x00;
			isRGBFormat		= true;
			break;
		case PF_A4L4:
			header.pixelFormat.flags	|= DDSPF_LUMINANCE;
			header.pixelFormat.flags	|= DDSPF_ALPHAPIXELS;
			header.pixelFormat.rgbBits	= 8;
			header.pixelFormat.aMask	= 0xf0;
			header.pixelFormat.rMask	= 0x0f;
			header.pixelFormat.gMask	= 0x00;
			header.pixelFormat.bMask	= 0x00;
			isRGBFormat		= true;
			break;
		default:
			// ah... maybe it is a fourcc format.
			isRGBFormat		= false;
		} // RGB format check.
		if (!isRGBFormat)
		{
			switch(srcDesc->pixelFormat)
			{
			case PF_BC1:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','T','1');
				isFourccFormat	= true;
				break;
			case PF_BC2:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','T','2');
				isFourccFormat	= true;
				break;
			case PF_BC3:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','T','3');
				isFourccFormat	= true;
				break;
			case PF_BC4:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','T','4');
				isFourccFormat	= true;
				break;
			case PF_BC5:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','T','5');
				isFourccFormat	= true;
				break;
			case PF_Q16W16V16U16:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 110;
				isFourccFormat	= true;
				break;
			case PF_R16_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 111;
				isFourccFormat	= true;
				break;
			case PF_G16R16_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 112;
				isFourccFormat	= true;
				break;
			case PF_R16G16B16A16_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 113;
				isFourccFormat	= true;
				break;
			case PF_R32_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 114;
				isFourccFormat	= true;
				break;
			case PF_G32R32_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 115;
				isFourccFormat	= true;
				break;
			case PF_R32G32B32A32_Float:
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= 116;
				isFourccFormat	= true;
				break;
			default:
				// ah.. maybe another type.... but we may not support all.
				// just assume it is a DXGI format.
				header.pixelFormat.flags	|= DDSPF_FOURCC;
				header.pixelFormat.fourCC	= FOURCC('D','X','1','0');
				isFourccFormat	= true;
			}
		} // fourcc format check

		// may never occur!
		if((!isRGBFormat) && (!isFourccFormat))
		{
			DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
				"Error format",
				"can not find a matched texture format",
				"Duel::DDSCodec::codeToFile");
		}

		if (header.pixelFormat.fourCC != FOURCC('D','X','1','0'))
		{
			header.flags	|= DDSD_CAPS;
			header.flags	|= DDSD_PIXELFORMAT;

			header.ddsCaps.caps1		= DDSCAPS_TEXTURE;
			if (srcDesc->mipMapCount != 1)
			{
				header.ddsCaps.caps1	|= DDSCAPS_MIPMAP;
				header.ddsCaps.caps1	|= DDSCAPS_COMPLEX;
			}
			if (srcDesc->imageType == IT_Image3D)
			{
				header.ddsCaps.caps1	|= DDSCAPS_COMPLEX;
				header.ddsCaps.caps2	|= DDSCAPS2_VOLUME;
				header.flags			|= DDSD_DEPTH;
				header.depth			=  srcDesc->depth;
			}
			else if (srcDesc->imageType == IT_ImageCube)
			{
				header.ddsCaps.caps1	|= DDSCAPS_COMPLEX;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_POSITIVEX;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_NEGATIVEX;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_POSITIVEY;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_NEGATIVEY;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_POSITIVEZ;
				header.ddsCaps.caps2	|= DDSCAPS2_CUBEMAP_NEGATIVEZ;
			}
		}
		DDSHeader	headerLE;
		memcpy(&headerLE, &header, sizeof(DDSHeader));

		NativeToLittle(&headerLE.size);
		NativeToLittle(&headerLE.flags);
		NativeToLittle(&headerLE.height);
		NativeToLittle(&headerLE.width);
		NativeToLittle(&headerLE.pitch);
		NativeToLittle(&headerLE.depth);
		NativeToLittle(&headerLE.mipMapCount);
		for (int i = 0; i < sizeof(headerLE.reserved1)/sizeof(headerLE.reserved1[0]); ++i)
		{
			NativeToLittle(&headerLE.reserved1[i]);
		}
		NativeToLittle(&headerLE.pixelFormat.size);
		NativeToLittle(&headerLE.pixelFormat.flags);
		NativeToLittle(&headerLE.pixelFormat.fourCC);
		NativeToLittle(&headerLE.pixelFormat.rgbBits);
		NativeToLittle(&headerLE.pixelFormat.rMask);
		NativeToLittle(&headerLE.pixelFormat.gMask);
		NativeToLittle(&headerLE.pixelFormat.bMask);
		NativeToLittle(&headerLE.pixelFormat.aMask);
		NativeToLittle(&headerLE.ddsCaps.caps1);
		NativeToLittle(&headerLE.ddsCaps.caps2);
		for (int i = 0; i < sizeof(headerLE.ddsCaps.reserved)/sizeof(headerLE.ddsCaps.reserved[0]); ++i)
		{
			NativeToLittle(&headerLE.ddsCaps.reserved[i]);
		}
		NativeToLittle(&headerLE.reserved2);
		

		DDSHeaderDXT10	headerDXT10;
		ZeroMemory(&headerDXT10, sizeof(DDSHeaderDXT10));
		if (header.pixelFormat.fourCC == FOURCC('D','X','1','0'))
		{
			headerDXT10.dxgiFormat	= getDXGIFormat(srcDesc->pixelFormat);
			headerDXT10.miscFlag	= 0;
			switch(srcDesc->imageType)
			{
			case IT_Image1D:
				headerDXT10.resDimension	= D3D10_RESOURCE_DIMENSION_TEXTURE1D;
				headerDXT10.arraySize		= srcDesc->arraySize;
				break;
			case IT_Image2D:
				headerDXT10.resDimension	= D3D10_RESOURCE_DIMENSION_TEXTURE2D;
				headerDXT10.arraySize		= srcDesc->arraySize;
				break;
			case IT_Image3D:
				headerDXT10.resDimension	= D3D10_RESOURCE_DIMENSION_TEXTURE3D;
				headerDXT10.arraySize		= srcDesc->arraySize;
				break;
			case IT_ImageCube:
				headerDXT10.resDimension	= D3D10_RESOURCE_DIMENSION_TEXTURE2D;
				headerDXT10.miscFlag		= D3D10_RESOURCE_MISC_TEXTURECUBE;
				headerDXT10.arraySize		*= 6;
				break;
			default:
				assert(false);
				headerDXT10.resDimension	= D3D10_RESOURCE_DIMENSION_UNKNOWN;
			}

			NativeToLittle(&headerDXT10.dxgiFormat);
			NativeToLittle(&headerDXT10.resDimension);
			NativeToLittle(&headerDXT10.miscFlag);
			NativeToLittle(&headerDXT10.arraySize);
			NativeToLittle(&headerDXT10.reserved);
			memorySize	+= sizeof(DDSHeaderDXT10);
		} // if (header.pixelFormat.fourCC == FOURCC('D','X','1','0'))

		// just write to file directly.
		size_t		imageSize = srcDesc->getImageSize();
		memorySize	+= imageSize;

		// create a new memory stream, remember to release it after use.
		DMemoryStream	*resultStream	= new DMemoryStream(memorySize,DA_Write,true);
		
		resultStream->write(&magic, sizeof(magic));
		resultStream->write(&headerLE, sizeof(DDSHeader));
		if (header.pixelFormat.fourCC == FOURCC('D','X','1','0'))
		{
			resultStream->write(&headerDXT10, sizeof(DDSHeaderDXT10));
		}

		char*	memPtr	= resultStream->getChunkPtr();
		memPtr	+=	resultStream->tell();
		input->read(memPtr, imageSize);
		
		return DMemoryStreamPtr(resultStream);
	}

	void DDSCodec::encodeToFile( DDataStream* input, const DString& fileName, DMediaDesc* imageDesc )
	{

		DMemoryStream* memData = static_cast<DMemoryStream*>(encode(input, imageDesc).get());
		// output 
		std::ofstream output(fileName);
		output.write(memData->getChunkPtr(), memData->getSize());
		output.flush();

	}

	DDataStreamPtr DDSCodec::decode( DDataStream* input, DMediaDesc* imageDesc )
	{
		// refered KlayGE and:
		// http://msdn.microsoft.com/en-us/library/bb943991.aspx
		// check magic number
		uint32	magic;
		input->read(&magic,sizeof(magic));
		LittleToNative(&magic);

		if (magic != FOURCC('D','D','S',' '))
		{
			DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
				"Error file type",
				"Excepting file is not a DDS file",
				"Duel::DDSCodec::decode");
		}

		// used for recording decode result.
		DImageDesc* resultDesc = imageDesc->getAs<DImageDesc>(false);
		if (resultDesc == NULL)
		{
			return DDataStreamPtr();
		}
		resultDesc->setFormat(CF_DDS);

		// formalization of the header
		DDSHeader header;
		input->read(&header,sizeof(DDSHeader));
		LittleToNative(&header.size);
		LittleToNative(&header.flags);
		LittleToNative(&header.height);
		LittleToNative(&header.width);
		LittleToNative(&header.pitch);
		LittleToNative(&header.depth);
		LittleToNative(&header.mipMapCount);
		for (int i = 0; i < sizeof(header.reserved1)/sizeof(header.reserved1[0]); ++i)
		{
			LittleToNative(&header.reserved1[i]);
		}
		LittleToNative(&header.pixelFormat.size);
		LittleToNative(&header.pixelFormat.flags);
		LittleToNative(&header.pixelFormat.fourCC);
		LittleToNative(&header.pixelFormat.rgbBits);
		LittleToNative(&header.pixelFormat.rMask);
		LittleToNative(&header.pixelFormat.gMask);
		LittleToNative(&header.pixelFormat.bMask);
		LittleToNative(&header.pixelFormat.aMask);
		LittleToNative(&header.ddsCaps.caps1);
		LittleToNative(&header.ddsCaps.caps2);
		for (int i = 0; i < sizeof(header.ddsCaps.reserved)/sizeof(header.ddsCaps.reserved[0]); ++i)
		{
			LittleToNative(&header.ddsCaps.reserved[i]);
		}
		LittleToNative(&header.reserved2);

		
		DDSHeaderDXT10	headerDXT10;
		if(header.pixelFormat.fourCC == FOURCC('D','X','1','0'))
		{
			input->read(&headerDXT10, sizeof(DDSHeaderDXT10));
			LittleToNative(&headerDXT10.dxgiFormat);
			LittleToNative(&headerDXT10.resDimension);
			LittleToNative(&headerDXT10.miscFlag);
			LittleToNative(&headerDXT10.arraySize);
			LittleToNative(&headerDXT10.reserved);

			// For a 2D texture that is also a cube-map texture, 
			// this number represents the number of cubes.
			resultDesc->arraySize = headerDXT10.arraySize;
		}
		else
		{
			ZeroMemory(&headerDXT10, sizeof(DDSHeaderDXT10));
			resultDesc->arraySize = 1;

			assert((header.flags & DDSD_CAPS) != 0);
			assert((header.flags & DDSD_PIXELFORMAT) != 0);
		}

		assert((header.flags & DDSD_WIDTH) != 0);
		assert((header.flags & DDSD_HEIGHT) != 0);

		if((header.flags & DDSD_MIPMAPCOUNT) == 0)
		{
			resultDesc->mipMapCount = 1;
		}
		else
		{
			resultDesc->mipMapCount = header.mipMapCount;
		}

		// query the format of the dds file
		resultDesc->pixelFormat = PF_Unknown;
		if((header.pixelFormat.flags & DDSPF_FOURCC) != 0)
		{
			switch(header.pixelFormat.fourCC)
			{
			case 36: // D3DFMT_A16B16G16R16F
				resultDesc->pixelFormat = PF_R16G16B16A16_Float;
				resultDesc->enableProperty(IP_Alpha);
				break;
			case 111: // D3DFMT_R16F
				resultDesc->pixelFormat = PF_R16_Float;
				break;
			case 112: // D3DFMT_G16R16F
				resultDesc->pixelFormat = PF_G16R16_Float;
				break;
			case 113: // D3DFMT_A16B16G16R16F
				resultDesc->pixelFormat = PF_R16G16B16A16_Float;
				resultDesc->enableProperty(IP_Alpha);
				break;
			case 114: // D3DFMT_R32F
				resultDesc->pixelFormat = PF_D32_Float;
				break;
			case 115: // D3DFMT_G32R32F
				resultDesc->pixelFormat = PF_G32R32_Float;
				break;
			case 116: // D3DFMT_A32B32G32R32F
				resultDesc->pixelFormat = PF_R32G32B32A32_Float;
				resultDesc->enableProperty(IP_Alpha);
				break;
			case FOURCC('D','X','T','1'):
				resultDesc->pixelFormat = PF_BC1;
				resultDesc->enableProperty(IP_Compressed);
				break;
			case FOURCC('D','X','T','2'):
				resultDesc->pixelFormat = PF_BC2;
				resultDesc->enableProperty(IP_Compressed);
				break;
			case FOURCC('D','X','T','3'):
				resultDesc->pixelFormat = PF_BC3;
				resultDesc->enableProperty(IP_Compressed);
				break;
			case FOURCC('D','X','T','4'):
				resultDesc->pixelFormat = PF_BC4;
				resultDesc->enableProperty(IP_Compressed);
				break;
			case FOURCC('D','X','T','5'):
				resultDesc->pixelFormat = PF_BC5;
				resultDesc->enableProperty(IP_Compressed);
				break;
			// not support yets
// 			case FOURCC('B','C','4','U'):
// 			case FOURCC('A','T','I','1'):
// 				resultDesc->pixelFormat = PF_BC4_UNORM;
// 				resultDesc->enableProperty(IP_COMPRESSED);
// 				break;
// 			case FOURCC('B','C','4','S'):
// 				resultDesc->pixelFormat = PF_BC4_SNORM;
// 				resultDesc->enableProperty(IP_COMPRESSED);
// 				break;
// 			case FOURCC('A','T','I','2'):
// 				resultDesc->pixelFormat = PF_BC5_UNORM;
// 				resultDesc->enableProperty(IP_COMPRESSED);
// 				break;
// 			case FOURCC('B','C','5','S'):
// 				resultDesc->pixelFormat = PF_BC5_SNORM;
// 				resultDesc->enableProperty(IP_COMPRESSED);
// 				break;
			case FOURCC('D','X','1','0'):
				resultDesc->pixelFormat = getPixelFormat((DXGI_FORMAT)headerDXT10.dxgiFormat);
				break;
			default:
				DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
					"FourCC flag error",
					"FourCC flag specified but FourCC value incorrect",
					"Duel::DDSCodec::decode");
			}
		}
		else // not a FourCC type
		{
			// is a rgb format
			if((header.pixelFormat.flags & DDSPF_RGB) != 0)
			{
				switch(header.pixelFormat.rgbBits)
				{
				case 16:
					if( (0xf000 == header.pixelFormat.aMask) &&
						(0x0f00 == header.pixelFormat.rMask) &&
						(0x00f0 == header.pixelFormat.gMask) &&
						(0x000f == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_A4R4G4B4;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x0000 == header.pixelFormat.aMask) &&
							(0x0f00 == header.pixelFormat.rMask) &&
							(0x00f0 == header.pixelFormat.gMask) &&
							(0x000f == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_X4R4G4B4;
					}
					else if((0x8000 == header.pixelFormat.aMask) &&
							(0x7c00 == header.pixelFormat.rMask) &&
							(0x03e0 == header.pixelFormat.gMask) &&
							(0x001f == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_A1R5G5B5;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x0000 == header.pixelFormat.aMask) &&
							(0x7c00 == header.pixelFormat.rMask) &&
							(0x03e0 == header.pixelFormat.gMask) &&
							(0x001f == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_X1R5G5B5;
					}
					else if((0x0000 == header.pixelFormat.aMask) &&
							(0xf800 == header.pixelFormat.rMask) &&
							(0x07e0 == header.pixelFormat.gMask) &&
							(0x001f == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_R5G6B5;
					}
					else
					{
						// a broken file
						DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
							"Format error",
							"RGB flag/16 Bits specified but mask value incorrect",
							"Duel::DDSCodec::decode");
					}
					break;
				case 24:
					if( (0xff0000 == header.pixelFormat.rMask) &&
						(0x00ff00 == header.pixelFormat.gMask) &&
						(0x0000ff == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_R8G8B8;
					}
					else
					{
						// a broken file
						DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
							"Format error",
							"RGB flag/16 Bits specified but mask value incorrect",
							"Duel::DDSCodec::decode");
					}
					break;
				case 32:
					if( (0xff000000 == header.pixelFormat.aMask) &&
						(0x00ff0000 == header.pixelFormat.rMask) &&
						(0x0000ff00 == header.pixelFormat.gMask) &&
						(0x000000ff == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_A8R8G8B8;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x00000000 == header.pixelFormat.aMask) &&
							(0x00ff0000 == header.pixelFormat.rMask) &&
							(0x0000ff00 == header.pixelFormat.gMask) &&
							(0x000000ff == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_X8R8G8B8;
					}
					else if((0xff000000 == header.pixelFormat.aMask) &&
							(0x000000ff == header.pixelFormat.rMask) &&
							(0x0000ff00 == header.pixelFormat.gMask) &&
							(0x00ff0000 == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_A8B8G8R8;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x00000000 == header.pixelFormat.aMask) &&
							(0x000000ff == header.pixelFormat.rMask) &&
							(0x0000ff00 == header.pixelFormat.gMask) &&
							(0x00ff0000 == header.pixelFormat.bMask) )
					{
						resultDesc->pixelFormat = PF_X8B8G8R8;
					}
					else if((0xc0000000 == header.pixelFormat.aMask) &&
							(0x3ff00000 == header.pixelFormat.rMask) &&
							(0x000ffc00 == header.pixelFormat.gMask) &&
							(0x000003ff == header.pixelFormat.bMask) )
					{
						// seems awkward, but it is correct, maybe
						// dx do it on purpose.
						resultDesc->pixelFormat = PF_A2R10G10B10;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x00000000 == header.pixelFormat.aMask) &&
							(0x000003ff == header.pixelFormat.rMask) &&
							(0x000ffc00 == header.pixelFormat.gMask) &&
							(0x3ff00000 == header.pixelFormat.bMask) )
					{
						// seems awkward, but it is correct, maybe
						// dx do it on purpose.
						resultDesc->pixelFormat = PF_A2B10G10R10;
						resultDesc->enableProperty(IP_Alpha);
					}
					else if((0x00000000 == header.pixelFormat.aMask) &&
							(0x0000ffff == header.pixelFormat.rMask) &&
							(0xffff0000 == header.pixelFormat.gMask) &&
							(0x00000000 == header.pixelFormat.bMask) )
					{
						// reversed, too
						resultDesc->pixelFormat = PF_G16R16_Float;
					}
					else
					{
						// broken file or something else.
						DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
							"Format error",
							"RGB flag/32 Bits specified but matched format can not be found",
							"Duel::DDSCodec::decode");
					}
					break; // case 32.
				default:
					// can't get a correct format from input.
					DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
						"Format error",
						"RGB flag specified but bits is not 16/32",
						"Duel::DDSCodec::decode");
				} // switch (bits)
			} // if((header.pixelFormat.flags & DDSPF_RGB) != 0)
			else if ((header.pixelFormat.flags & DDSPF_LUMINANCE) != 0)
			{
				switch(header.pixelFormat.rgbBits)
				{
				case 8:
					if((header.pixelFormat.rMask == 0xff))
					{
						resultDesc->pixelFormat = PF_L8;
					}
					else if((header.pixelFormat.rMask == 0x0f) &&
							(header.pixelFormat.aMask == 0xf0))
					{
						resultDesc->pixelFormat = PF_A4L4;
						resultDesc->enableProperty(IP_Alpha);
					}
					else
					{
						DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
							"Format error",
							"LUMINANCE flag/8 Bits but matched format can not be found",
							"Duel::DDSCodec::decode");
					}
					break;
				case 16:
					if((header.pixelFormat.rMask == 0xffff))
					{
						resultDesc->pixelFormat = PF_L16;
					}
					else if((header.pixelFormat.rMask == 0x00ff) &&
							(header.pixelFormat.aMask == 0xff00))
					{
						resultDesc->pixelFormat = PF_A8L8;
						resultDesc->enableProperty(IP_Alpha);
					}
					else
					{
						DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
							"Format error",
							"LUMINANCE flag/16 Bits but matched format can not be found",
							"Duel::DDSCodec::decode");
					}
					break;
				default:
					// can't get a correct format from input.
					DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
						"Format error",
						"LUMINANCE flag specified but bits is not 8/16",
						"Duel::DDSCodec::decode");
				}
			} // if ((header.pixelFormat.flags & DDSPF_LUMINANCE) != 0)
			else if ((header.pixelFormat.flags & DDSPF_ALPHA) != 0 )
			{
				if((header.pixelFormat.aMask == 0xff))
				{
					resultDesc->pixelFormat = PF_A8;
					resultDesc->enableProperty(IP_Alpha);
				}
				else
				{
					// can't get a correct format from input.
					DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
						"Format error",
						"ALPHA flag specified but alpha mask incorrect",
						"Duel::DDSCodec::decode");
				}
			} // if (header.pixelFormat.flags & DDSPF_ALPHA) != 0)
		} // format detecting.
		

		if (header.reserved1[0] != 0)
		{
			// indicate is a SRGB format.
			resultDesc->enableProperty(IP_SRGB);
		}

		resultDesc->width = header.width;
		if(FOURCC('D','X','1','0') == header.pixelFormat.fourCC)
		{
			if(headerDXT10.miscFlag == D3D10_RESOURCE_MISC_TEXTURECUBE)
			{
				resultDesc->imageType = IT_ImageCube;
				resultDesc->arraySize	/= 6;
				resultDesc->height		= resultDesc->width;
				resultDesc->depth = 1;
			}
			else
			{
				switch(headerDXT10.resDimension)
				{
				case D3D10_RESOURCE_DIMENSION_TEXTURE1D:
					resultDesc->imageType = IT_Image1D;
					resultDesc->height		= 1;
					resultDesc->depth		= 1;
					break;
				case D3D10_RESOURCE_DIMENSION_TEXTURE2D:
					resultDesc->imageType = IT_Image2D;
					resultDesc->height		= header.height;
					resultDesc->depth		= 1;
					break;
				case D3D10_RESOURCE_DIMENSION_TEXTURE3D:
					resultDesc->imageType = IT_Image3D;
					resultDesc->height		= header.height;
					resultDesc->depth		= header.depth;
					break;
				default:
					// can't get a correct texture type from input.
					DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
						"Texture type error",
						"DX10 resource specified but texture type incorrect",
						"Duel::DDSCodec::decode");
				}
			} // if(headerDXT10.miscFlag == D3D10_RESOURCE_MISC_TEXTURECUBE)
		} // if(FOURCC('D','X','1','0') == header.pixelFormat.fourCC)
		else // not a DX10 Texture Type.
		{
			if((header.ddsCaps.caps2 & DDSCAPS2_CUBEMAP) != 0)
			{
				resultDesc->imageType = IT_ImageCube;
				resultDesc->height		= header.width;
				resultDesc->depth		= 1;
			}
			else if ((header.ddsCaps.caps2 & DDSCAPS2_VOLUME) != 0)
			{
				resultDesc->imageType = IT_Image3D;
				resultDesc->height		= header.height;
				resultDesc->depth		= header.depth;
			}
			else
			{
				resultDesc->imageType = IT_Image2D;
				resultDesc->height		= header.height;
				resultDesc->depth		= 1;
			}
		}

		// Calculate total size from number of mipmaps, faces and size
		size_t	imageSize = resultDesc->getImageSize();
		
		// do not uncompress.
		// remember to release the memory stream.
		DMemoryStream* resultStream = new DMemoryStream(imageSize, DA_Write, true);
		char*		memPtr = resultStream->getChunkPtr();
		
		if(DPixelFormatTool::isCompressed(resultDesc->pixelFormat))
		{
			// load directly
			input->read(memPtr, imageSize);
		}
		else // not a compressed format, then we must take pitch into consideration.
		{
			// what's 'pitch':
			// every scan line of a image may contains some unused bytes,
			// the total bytes of a scan line is called pitch, not width, which
			// defines used bytes that contains color information.
			
			// an image:
			// |------------pitch------------|
			// |------width------|
			// |*****************++++++++++++| <--- a scan line of a image, '*' means
			// |*****************++++++++++++|      byte contains color information,
			// |*****************++++++++++++|      '+' means padding byte.
			// |...........the same..........|
			
			// calculate total face.
			int faceCount = (resultDesc->imageType == IT_ImageCube) ? 6 : 1;
			// read every surfcace(mipmap included)
			for (uint32 i = 0; i < resultDesc->arraySize; ++i)
			{
				for(int face = 0; face < faceCount; ++face)
				{
					// first mip:
					uint32 mipWidth	= resultDesc->width;
					uint32 mipHeight	= resultDesc->height;
					uint32 mipDepth	= resultDesc->depth;
					for (uint32 mip = 0; mip < resultDesc->mipMapCount; ++mip)
					{
						size_t validBytes = static_cast<uint32>(mipWidth * DPixelFormatTool::getFormatBytes(resultDesc->pixelFormat));
						size_t mipPitch = validBytes;
						
						if ((header.flags & DDSD_PITCH) != 0)
						{
							mipPitch = header.pitch / ((mip == 0) ? 1 : mip * 2);
						}
						assert(mipPitch >= validBytes);
						size_t padBytes = mipPitch - validBytes;

						for(uint32 d = 0; d < mipDepth; ++d)
						{
							for (uint32 h = 0; h < mipHeight; ++h)
							{
								input->read(memPtr, validBytes);
								input->skip(padBytes);
								// move forward and read next line.
								memPtr = (memPtr + validBytes);
							}
						}

						// ready for next mip.
						if(mipWidth != 1)
						{
							mipWidth /= 2;
						}
						if(mipHeight != 1)
						{
							mipHeight /= 2;
						}
						if(mipDepth != 1)
						{
							mipDepth /= 2;
						}

					} // for (mip < resultDesc->mipMapCount; ++mip)
				} // for(face < faceCount; ++face)
			} // for(i < resultDesc->arraySize; ++i)
		}
		
		return DMemoryStreamPtr(resultStream);
	}


}