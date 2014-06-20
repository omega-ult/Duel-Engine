//  [8/13/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelBitTool.h"

namespace Duel
{
	//-----------------------------------------------------------------------
    /**
    * A record that describes a pixel format in detail.
    */
    struct PixelFormatDescription {
		DPixelFormat format;
        // Name of the format, as in the enum
        DString	name;
        // Number of bits one element (colour value) takes.
        uint8 elemBits;
		// a flag indicate the format's properties
		uint8 elemFlag;
        // Component type
        uint8 componentType;
		// Component count
        uint8 componentCount;
		// Number of bits for red(or luminance), green, blue, alpha
        uint8 rbits,gbits,bbits,abits; 
		// channel mask, 
		uint32	rMask, gMask, bMask, aMask;
		// channel shift, for unpack/pack color, corresponding to the
		// number of bits of the channel to the rightmost
		uint8	rShift, gShift, bShift, aShift;

    };
	//-----------------------------------------------------------------------
	/** Pixel format database */
	const PixelFormatDescription _pixelFormats[PF_Count] = {
		//0-----------------------------------------------------------------------
		{
			PF_Unknown,
			DString("PF_Unknown"),
			/* Bits per element */
			0,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//1-----------------------------------------------------------------------
		{
			PF_R32G32B32A32,
			DString("PF_R32G32B32A32"),
			/* Bits per element */
			128,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			32, 32, 32, 32,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//2----------------------------------------------------------------------
		{
			PF_R32G32B32A32_Float,
			DString("PF_R32G32B32A32_Float"),
			/* Bits per element */
			128,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_Float, 4,
			/* rbits, gbits, bbits, abits */
			32, 32, 32, 32,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//3-----------------------------------------------------------------------
		{
			PF_R32G32B32,
			DString("PF_R32G32B32"),
			/* Bits per element */
			96,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			32, 32, 32, 0,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//4-----------------------------------------------------------------------
		{
			PF_R32G32B32_Float,
			DString("PF_R32G32B32_Float"),
			/* Bits per element */
			96,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Float, 3,
			/* rbits, gbits, bbits, abits */
			32, 32, 32, 0,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//5-----------------------------------------------------------------------
		{
			PF_R16G16B16A16,
			DString("PF_R16G16B16A16"),
			/* Bits per element */
			64,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			16, 16, 16, 16,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//6-----------------------------------------------------------------------
		{
			PF_Q16W16V16U16,
			DString("PF_Q16W16V16U16"),
			/* Bits per element */
			64,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			16, 16, 16, 16,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//7----------------------------------------------------------------------
		{
			PF_R16G16B16A16_Float,
			DString("PF_R16G16B16A16_Float"),
			/* Bits per element */
			64,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_Float, 4,
			/* rbits, gbits, bbits, abits */
			16, 16, 16, 16,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//8-----------------------------------------------------------------------
		{
			PF_G32R32,
			DString("PF_G32R32"),
			/* Bits per element */
			64,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 2,
			/* rbits, gbits, bbits, abits */
			32, 32, 0, 0,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//9-----------------------------------------------------------------------
		{
			PF_G32R32_Float,
			DString("PF_G32R32_Float"),
			/* Bits per element */
			64,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Float, 2,
			/* rbits, gbits, bbits, abits */
			32, 32, 0, 0,
			// channel mask, 
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//10-----------------------------------------------------------------------
		{
			PF_A2B10G10R10,
			DString("PF_A2B10G10R10"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			10, 10, 10, 2,
			// channel mask, r, g, b, a
			0x000003ff, 0x000ffc00, 0x3ff00000, 0xc0000000,
			// channel shift
			0, 10, 20, 30
		},
		//11-----------------------------------------------------------------------
		{
			PF_A2R10G10B10,
			DString("PF_A2R10G10B10"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			10, 10, 10, 2,
			// channel mask, r, g, b, a
			0x3ff00000, 0x000ffc00, 0x000003ff, 0xc0000000,
			// channel shift
			0, 10, 20, 30
		},
	
		//12-----------------------------------------------------------------------
		{
			PF_G16R16,
			DString("PF_G16R16"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 2,
			/* rbits, gbits, bbits, abits */
			16, 16, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//13-----------------------------------------------------------------------
		{
			PF_G16R16_Float,
			DString("PF_G16R16_Float"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Float, 2,
			/* rbits, gbits, bbits, abits */
			16, 16, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//14-----------------------------------------------------------------------
		{
			PF_R32,
			DString("PF_R32"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			32, 0, 0, 0,
			// channel mask, r, g, b, a
			0xffffffff, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//15-----------------------------------------------------------------------
		{
			PF_D32_Float,
			DString("PF_D32_Float"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Depth,
			/* Component type and count */
			PCT_Float, 1,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//16-----------------------------------------------------------------------
		{
			PF_R32_Float,
			DString("PF_R32_Float"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Float, 1,
			/* rbits, gbits, bbits, abits */
			32, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//17-----------------------------------------------------------------------
		{
			PF_A8B8G8R8,
			DString("PF_A8B8G8R8"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 8,
			// channel mask, r, g, b, a
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
			// channel shift
			0, 8, 16, 24
		},
		//18-----------------------------------------------------------------------
		{
			PF_A8R8G8B8,
			DString("PF_A8R8G8B8"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 8,
			// channel mask, r, g, b, a
			0x00ff0000, 0x0000ff00, 0x0000ff00, 0xff000000,
			// channel shift
			16, 8, 0, 24
		},
		//19-----------------------------------------------------------------------
		{
			PF_B8G8R8A8,
			DString("PF_B8G8R8A8"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 8,
			// channel mask, r, g, b, a
			0x0000ff00, 0x00ff0000, 0xff000000, 0x000000ff,
			// channel shift
			8, 16, 24, 0 
		},
		//20-----------------------------------------------------------------------
		{
			PF_R8G8B8A8,
			DString("PF_R8G8B8A8"),
			/* Bits per element */
			32,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 8,
			// channel mask, r, g, b, a
			0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff,
			// channel shift
			24, 16, 8, 0 
		},
		//21-----------------------------------------------------------------------
		{
			PF_X8R8G8B8,
			DString("PF_X8R8G8B8"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 0,
			// channel mask, r, g, b, a
			0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000,
			// channel shift
			16, 8, 0, 24 
		},
		//22-----------------------------------------------------------------------
		{
			PF_X8B8G8R8,
			DString("PF_X8B8G8R8"),
			/* Bits per element */
			32,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 0,
			// channel mask, r, g, b, a
			0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000,
			// channel shift
			0, 8, 16, 24 
		},
		//23-----------------------------------------------------------------------
		{
			PF_R8G8B8,
			DString("PF_R8G8B8"),
			/* Bits per element */
			24,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 0,
			// channel mask, r, g, b, a
			0xff0000, 0x00ff00, 0x0000ff, 0,
			// channel shift
			16, 8, 0, 0
		},
		//24-----------------------------------------------------------------------
		{
			PF_B8G8R8,
			DString("PF_B8G8R8"),
			/* Bits per element */
			24,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			8, 8, 8, 0,
			// channel mask, r, g, b, a
			0x0000ff, 0x00ff00, 0xff0000, 0,
			// channel shift
			0, 8, 16, 0
		},
		//25-----------------------------------------------------------------------
		{
			PF_A1R5G5B5,
			DString("PF_A1R5G5B5"),
			/* Bits per element */
			16,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			5, 5, 5, 1,
			// channel mask, r, g, b, a
			0x7c00, 0x03e0, 0x001f, 0x8000,
			// channel shift
			10, 5, 0, 15
		},
		//26-----------------------------------------------------------------------
		{
			PF_X1R5G5B5,
			DString("PF_X1R5G5B5"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			5, 5, 5, 1,
			// channel mask, r, g, b, a
			0x7c00, 0x03e0, 0x001f, 0x8000,
			// channel shift
			10, 5, 0, 15
		},
		//27-----------------------------------------------------------------------
		{
			PF_R5G6B5,
			DString("PF_R5G6B5"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			5, 6, 5, 0,
			// channel mask, r, g, b, a
			0xf800, 0x07e0, 0x001f, 0,
			// channel shift
			11, 5, 0, 0
		},
		//28-----------------------------------------------------------------------
		{
			PF_B5G6R5,
			DString("PF_B5G6R5"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			5, 6, 5, 0,
			// channel mask, r, g, b, a
			0x001f, 0x07e0, 0xf800, 0,
			// channel shift
			0, 5, 11, 0
		},
		//29-----------------------------------------------------------------------
		{
			PF_A4R4G4B4,
			DString("PF_A4R4G4B4"),
			/* Bits per element */
			16,
			/* Flag */
			PFF_Alpha,
			/* Component type and count */
			PCT_UInt, 4,
			/* rbits, gbits, bbits, abits */
			4, 4, 4, 4,
			// channel mask, r, g, b, a
			0x0f00, 0x00f0, 0x000f, 0xf000,
			// channel shift
			8, 4, 0, 12
		},
		//30-----------------------------------------------------------------------
		{
			PF_X4R4G4B4,
			DString("PF_X4R4G4B4"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 3,
			/* rbits, gbits, bbits, abits */
			4, 4, 4, 0,
			// channel mask, r, g, b, a
			0x0f00, 0x00f0, 0x000f, 0xf000,
			// channel shift
			8, 4, 0, 12
		},
		//31-----------------------------------------------------------------------
		{
			PF_R16,
			DString("PF_R16"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			16, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//32-----------------------------------------------------------------------
		{
			PF_R16_Float,
			DString("PF_R16_Float"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_Float, 1,
			/* rbits, gbits, bbits, abits */
			16, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//33-----------------------------------------------------------------------
		{
			PF_D16,
			DString("PF_D16"),
			/* Bits per element */
			16,
			/* Flag */
			PFF_Depth,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//34-----------------------------------------------------------------------
		{
			PF_A8L8,
			DString("PF_A8L8"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 2,
			/* rbits, gbits, bbits, abits */
			8, 0, 0, 8,
			// channel mask, r, g, b, a
			0x00ff, 0, 0, 0xff00,
			// channel shift
			0, 0, 0, 8
		},
		//35-----------------------------------------------------------------------
		{
			PF_L16,
			DString("PF_L16"),
			/* Bits per element */
			16,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			16, 0, 0, 0,
			// channel mask, r, g, b, a
			0xffff, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//36-----------------------------------------------------------------------
		{
			PF_R8,
			DString("PF_R8"),
			/* Bits per element */
			8,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			8, 0, 0, 0,
			// channel mask, r, g, b, a
			0xff, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},	
		//37-----------------------------------------------------------------------
		{
			PF_A8,
			DString("PF_A8"),
			/* Bits per element */
			8,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 8,
			// channel mask, r, g, b, a
			0, 0, 0, 0xff,
			// channel shift
			0, 0, 0, 0
		},	
		//38-----------------------------------------------------------------------
		{
			PF_L8,
			DString("PF_L8"),
			/* Bits per element */
			8,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 1,
			/* rbits, gbits, bbits, abits */
			8, 0, 0, 0,
			// channel mask, r, g, b, a
			0xff, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//39-----------------------------------------------------------------------
		{
			PF_A4L4,
			DString("PF_A4L4"),
			/* Bits per element */
			8,
			/* Flag */
			0,
			/* Component type and count */
			PCT_UInt, 2,
			/* rbits, gbits, bbits, abits */
			4, 0, 0, 4,
			// channel mask, r, g, b, a
			0x0f, 0, 0, 0xf0,
			// channel shift
			0, 0, 0, 4
		},

		//40-----------------------------------------------------------------------
		{
			PF_BC1,
			DString("PF_BC1"),
			/* Bits per element */
			0,
			/* Flag */
			PFF_Compressed,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//41-----------------------------------------------------------------------
		{
			PF_BC2,
			DString("PF_BC2"),
			/* Bits per element */
			0,
			/* Flag */
			PFF_Compressed,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//42-----------------------------------------------------------------------
		{
			PF_BC3,
			DString("PF_BC3"),
			/* Bits per element */
			0,
			/* Flag */
			PFF_Compressed,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//43-----------------------------------------------------------------------
		{
			PF_BC4,
			DString("PF_BC4"),
			/* Bits per element */
			0,
			/* Flag */
			PFF_Compressed,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		},
		//44-----------------------------------------------------------------------
		{
			PF_BC5,
			DString("PF_BC5"),
			/* Bits per element */
			0,
			/* Flag */
			PFF_Compressed,
			/* Component type and count */
			PCT_Unknown, 0,
			/* rbits, gbits, bbits, abits */
			0, 0, 0, 0,
			// channel mask, r, g, b, a
			0, 0, 0, 0,
			// channel shift
			0, 0, 0, 0
		}
	};

	// convert a color channel's value with bit cut/extended,
	uint32 saturateColor(uint32 value, uint32 srcBits, uint32 destBits)
	{
		if (srcBits > destBits)
		{
			value >>= srcBits - destBits;
		}
		else if (srcBits < destBits)
		{
			if (value == 0)
			{
				// nothing to do, just return it
			}
			// the maximum value of an binary at width of srcBits
			else if (value == (static_cast<uint32>(1)<<srcBits) - 1) 
			{
				value = (1<<destBits) - 1; 
			}
			else
			{
				value = value * (1<<destBits)/((1<<srcBits)-1); 
			}
		}
		return value;
	}
	// convert a color channel's value to a float value
	float saturateColor(uint32 value, uint32 srcBits)
	{
		return (float)value/((1<<srcBits)-1);
	}

	bool DPixelFormatTool::isCompressed( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return (_pixelFormats[format].elemFlag & PFF_Compressed) > 0;
		}
		return false;
	}


	bool DPixelFormatTool::hasAlpha( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return (_pixelFormats[format].elemFlag & PFF_Alpha) > 0;
		}
		return false;
	}

	bool DPixelFormatTool::isDepth( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return (_pixelFormats[format].elemFlag & PFF_Depth) > 0;
		}
		return false;
	}



	bool DPixelFormatTool::hasComponentType( DPixelFormat format, PixelComponentType pct )
	{
		if(format < PF_Count)
		{
			return (_pixelFormats[format].componentType & pct) > 0;
		}
		return false;
	}

	uint16 DPixelFormatTool::getFormatBits( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			if (isCompressed(format))
			{
				switch(format)
				{
				case PF_BC1:
				case PF_BC4:
					return 16;
				case PF_BC2:
				case PF_BC3:
				case PF_BC5:
					return 32;

				// other compressed type are not supported yet.
				default:
					assert(false);
					return 0;
				}
			}
			return _pixelFormats[format].elemBits;
		}
		// out of boundary.
		return 0;
	}


	uint32 DPixelFormatTool::getFormatBytes( DPixelFormat format )
	{
		return	(getFormatBits(format)) / 8;
	}


	DString DPixelFormatTool::getFormatName( DPixelFormat format )
	{
		if (format < PF_Count)
		{
			return _pixelFormats[format].name;
		}
		return DStringTool::BLANK;
	}

	Duel::DPixelFormat DPixelFormatTool::getFormatFromName( const DString& name )
	{
		for (uint32 i = 0; i < PF_Count; i++)
		{
			if (_pixelFormats[i].name == name)
			{
				return _pixelFormats[i].format;
			}
		}
		return PF_Unknown;
	}

	uint8 DPixelFormatTool::getRedChannelBits( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return _pixelFormats[format].rbits;
		}
		return 0;
	}

	uint8 DPixelFormatTool::getGreenChannelBits( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return _pixelFormats[format].gbits;
		}
		return 0;
	}

	uint8 DPixelFormatTool::getBlueChannelBits( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return _pixelFormats[format].bbits;
		}
		return 0;
	}

	uint8 DPixelFormatTool::getAlphaChannelBits( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			if(hasAlpha(format))
			{
				return _pixelFormats[format].abits;
			}
		}
		return 0;
	}

	uint8 DPixelFormatTool::getComponentCount( DPixelFormat format )
	{
		if(format < PF_Count)
		{
			return _pixelFormats[format].componentCount;
		}
		return 0;
	}

	uint32 DPixelFormatTool::getSurfaceSize( uint32 width, uint32 height, uint32 depth, DPixelFormat format )
	{
		if(isCompressed(format))
		{
			switch(format)
			{
			// DXT formats work by dividing the image into 4x4 blocks, then encoding each
			// 4x4 block with a certain number of bytes. 
			case PF_BC1:
				return ((width+3)/4) * ((height +3)/4) * 8 * depth;
			case PF_BC2:
			case PF_BC3:
			case PF_BC4:
			case PF_BC5:
				return ((width+3)/4) * ((height+3)/4) * 16 * depth;
			// as to BC6/BC7 format, not support yet~
			default:
				return 0;
			}
		}
		else
		{
			return (width*height*depth*getFormatBytes(format));
		}
	}

	bool DPixelFormatTool::isDirectColor( DPixelFormat format )
	{
		if (format	== PF_Unknown)
		{
			return	false;
		}
		if (isDepth(format))
		{
			return false;
		}
		if (isCompressed(format))
		{
			return false;
		}
		return true;
	}

	void DPixelFormatTool::unpackColor( DColor* result, DPixelFormat format, void* src )
	{
		if (!isCompressed(format))
		{
			unpackColor(&result->a, &result->r, &result->g, &result->b, format, src);
		}
		else
		{
			*result = DColor::WHITE;
		}
	}

	void DPixelFormatTool::unpackColor( uint8* a, uint8* r, uint8* g, uint8* b, DPixelFormat format, void* src )
	{
		if (format < PF_Count)
		{
 			// handle large format firstly
 			switch(format)
 			{
 			case PF_R32G32B32A32:
			case PF_R32G32B32A32_Float:
			case PF_R32G32B32:
			case PF_R32G32B32_Float:
			case PF_R16G16B16A16:
			case PF_Q16W16V16U16:
			case PF_R16G16B16A16_Float:
			case PF_G32R32:
			case PF_G32R32_Float:
			case PF_G16R16:
			case PF_G16R16_Float:
			case PF_R32_Float:
			case PF_R16:
			case PF_R16_Float:
				float fr, fg, fb, fa;
				unpackColor(&fa, &fr, &fg, &fb, format, src);
				*r = (uint8)DBitTool::floatToFixedClamped(fr, 8);
				*g = (uint8)DBitTool::floatToFixedClamped(fg, 8);
				*b = (uint8)DBitTool::floatToFixedClamped(fb, 8);
				*a = (uint8)DBitTool::floatToFixedClamped(fa, 8);
				return;
				// do normal way:
 			}			
			
			const PixelFormatDescription& desc = _pixelFormats[format];
			// get int value, should never bigger than 32-bits.
			const uint32 color = DBitTool::readInt(src, getFormatBytes(format));

			
			if (format == PF_L16 || 
				format == PF_A8L8 ||
				format == PF_L8 || 
				format == PF_A4L4)
			{
				// handle luminance.
				*r = *g = *b = (uint8)saturateColor((color & desc.rMask)>>desc.rShift, desc.rbits, 8);
			}
			else
			{		
				*r = (uint8)saturateColor((color & desc.rMask)>>desc.rShift, desc.rbits, 8);
				*g = (uint8)saturateColor((color & desc.gMask)>>desc.gShift, desc.gbits, 8);
				*b = (uint8)saturateColor((color & desc.bMask)>>desc.bShift, desc.bbits, 8);
			}
			// if it has alpha channel, extract it, else make it opaque
			if (desc.elemFlag & PFF_Alpha)
			{
				*a = (uint8)saturateColor((color & desc.aMask)>>desc.aShift, desc.abits, 8);
			}
			else
			{
				*a = 255;
			}
		}
	}

	void DPixelFormatTool::unpackColor( float* a, float* r, float* g, float* b, DPixelFormat format, void* src )
	{
		if (format < PF_Count)
		{
			switch(format)
			{
			case PF_R32G32B32A32:
				*r = saturateColor(((uint32*)src)[0],32);
				*g = saturateColor(((uint32*)src)[1],32);
				*b = saturateColor(((uint32*)src)[2],32);
				*a = saturateColor(((uint32*)src)[3],32);
				return;
			case PF_R32G32B32A32_Float:
				*r = ((float*)src)[0];
				*g = ((float*)src)[1];
				*b = ((float*)src)[2];
				*a = ((float*)src)[3];
				return;
			case PF_R32G32B32:
				*r = saturateColor(((uint32*)src)[0],32);
				*g = saturateColor(((uint32*)src)[1],32);
				*b = saturateColor(((uint32*)src)[2],32);
				*a = 1.0f;
				return;
			case PF_R32G32B32_Float:
				*r = ((float*)src)[0];
				*g = ((float*)src)[1];
				*b = ((float*)src)[2];
				*a = 1.0f;
				return;
			case PF_G32R32:
				*g = saturateColor(((uint32*)src)[0],32);
				*r = *b = saturateColor(((uint32*)src)[1],32);
				*a = 1.0f;
				return;
			case PF_G32R32_Float:
				*g = ((float*)src)[0];
				*r = *b = ((float*)src)[1];
				*a = 1.0f;
				return;
			case PF_R32:
				*r = *g = *b = saturateColor(((uint32*)src)[0],32);
				*a = 1.0f;
				return;
			case PF_R32_Float:
				*r = *g = *b = ((float*)src)[0];
				*a = 1.0f;
				return;
			case PF_R16G16B16A16:
			case PF_Q16W16V16U16:
				*r = saturateColor(((uint16*)src)[0],16);
				*g = saturateColor(((uint16*)src)[1],16);
				*b = saturateColor(((uint16*)src)[2],16);
				*a = saturateColor(((uint16*)src)[3],16);
				return;
			case PF_R16G16B16A16_Float:
				*r = DBitTool::halfToFloat(((uint16*)src)[0]);
				*g = DBitTool::halfToFloat(((uint16*)src)[1]);
				*b = DBitTool::halfToFloat(((uint16*)src)[2]);
				*a = DBitTool::halfToFloat(((uint16*)src)[3]);
				return;
			case PF_G16R16:
				*g = saturateColor(((uint16*)src)[0],16);
				*r = *b = saturateColor(((uint16*)src)[1],16);
				*a = 1.0f;
				return;
			case PF_G16R16_Float:
				*g = DBitTool::halfToFloat(((uint16*)src)[0]);
				*r = *b = DBitTool::halfToFloat(((uint16*)src)[1]);
				*a = 1.0f;
				return;
			case PF_R16:
				*r = *g = *b = saturateColor(((uint16*)src)[0],16);
				*a = 1.0f;
				return;
			case PF_R16_Float:
				*r = *g = *b = DBitTool::halfToFloat(((uint16*)src)[0]);
				*a = 1.0f;
				return;
				// other format is handled individually
			}

			const PixelFormatDescription& desc = _pixelFormats[format];
			// get int value, should never bigger than 32-bits.
			const uint32 color = DBitTool::readInt(src, getFormatBytes(format));


			if (format == PF_L16 || 
				format == PF_A8L8 ||
				format == PF_L8 || 
				format == PF_A4L4)
			{
				// handle luminance.
				*r = *g = *b = saturateColor((color & desc.rMask)>>desc.rShift, desc.rbits);
			}
			else
			{		
				*r = saturateColor((color & desc.rMask)>>desc.rShift, desc.rbits);
				*g = saturateColor((color & desc.gMask)>>desc.gShift, desc.gbits);
				*b = saturateColor((color & desc.bMask)>>desc.bShift, desc.bbits);
			}
			// if it has alpha channel, extract it, else make it opaque
			if (desc.elemFlag & PFF_Alpha)
			{
				*a = saturateColor((color & desc.aMask)>>desc.aShift, desc.abits);
			}
			else
			{
				*a = 1.0f;;
			}
		}
	}

	void DPixelFormatTool::packColor( const DColor& color, DPixelFormat format, void* dest )
	{
		packColor(color.a, color.r, color.g, color.b, format, dest);
	}

	void DPixelFormatTool::packColor( uint8 a, uint8 r, uint8 g, uint8 b, DPixelFormat format, void* dest )
	{
		if (format < PF_Count)
		{
			// handle large format firstly
			switch(format)
			{
			case PF_R32G32B32A32:
			case PF_R32G32B32A32_Float:
			case PF_R32G32B32:
			case PF_R32G32B32_Float:
			case PF_R16G16B16A16:
			case PF_Q16W16V16U16:
			case PF_R16G16B16A16_Float:
			case PF_G32R32:
			case PF_G32R32_Float:
			case PF_G16R16:
			case PF_G16R16_Float:
			case PF_R32_Float:
			case PF_R16:
			case PF_R16_Float:
				packColor((float)a/255.0f, (float)r/255.0f, (float)g/255.0f, (float)b/255.0f, format, dest);
				return;
				// do normal way:
			}
			const PixelFormatDescription &desc = _pixelFormats[format];
			uint32 value = (
				(saturateColor(r, 8, desc.rbits)<<desc.rShift)&desc.rMask |
				(saturateColor(g, 8, desc.gbits)<<desc.gShift)&desc.gMask |
				(saturateColor(b, 8, desc.rbits)<<desc.bShift)&desc.bMask |
				(saturateColor(a, 8, desc.abits)<<desc.aShift)&desc.aMask);
			DBitTool::writeInt(dest, getFormatBytes(format), value);
		}
	}

	void DPixelFormatTool::packColor( float a, float r, float g, float b, DPixelFormat format, void* dest )
	{
		if (format < PF_Count)
		{
			switch(format)
			{
			case PF_R32G32B32A32:
				((uint32*)dest)[0] = DBitTool::floatToFixedClamped(r,32);
				((uint32*)dest)[1] = DBitTool::floatToFixedClamped(g,32);
				((uint32*)dest)[2] = DBitTool::floatToFixedClamped(b,32);
				((uint32*)dest)[3] = DBitTool::floatToFixedClamped(a,32);
				return;
			case PF_R32G32B32A32_Float:
				((float*)dest)[0] = r;
				((float*)dest)[1] = g;
				((float*)dest)[2] = b;
				((float*)dest)[3] = a;
				return;
			case PF_R32G32B32:
				((uint32*)dest)[0] = DBitTool::floatToFixedClamped(r,32);
				((uint32*)dest)[1] = DBitTool::floatToFixedClamped(g,32);
				((uint32*)dest)[2] = DBitTool::floatToFixedClamped(b,32);
				return;
			case PF_R32G32B32_Float:
				((float*)dest)[0] = r;
				((float*)dest)[1] = g;
				((float*)dest)[2] = b;
				return;
			case PF_G32R32:
				((uint32*)dest)[0] = DBitTool::floatToFixedClamped(g,32);
				((uint32*)dest)[1] = DBitTool::floatToFixedClamped(r,32);
				return;
			case PF_G32R32_Float:
				((float*)dest)[0] = g;
				((float*)dest)[1] = r;
				return;
			case PF_R32:
				((uint32*)dest)[0] = DBitTool::floatToFixedClamped(r,32);
				return;
			case PF_R32_Float:
				((float*)dest)[0] = r;
				return;
			case PF_R16G16B16A16:
			case PF_Q16W16V16U16:
				((uint16*)dest)[0] = DBitTool::floatToFixedClamped(r,16);
				((uint16*)dest)[1] = DBitTool::floatToFixedClamped(g,16);
				((uint16*)dest)[2] = DBitTool::floatToFixedClamped(b,16);
				((uint16*)dest)[3] = DBitTool::floatToFixedClamped(a,16);
				return;
			case PF_R16G16B16A16_Float:
				((uint16*)dest)[0] = DBitTool::floatToHalf(r);
				((uint16*)dest)[1] = DBitTool::floatToHalf(g);
				((uint16*)dest)[2] = DBitTool::floatToHalf(b);
				((uint16*)dest)[3] = DBitTool::floatToHalf(a);
				return;
			case PF_G16R16:
				((uint16*)dest)[0] = DBitTool::floatToFixedClamped(g,16);
				((uint16*)dest)[1] = DBitTool::floatToFixedClamped(r,16);
				return;
			case PF_G16R16_Float:
				((uint16*)dest)[0] = DBitTool::floatToHalf(g);
				((uint16*)dest)[1] = DBitTool::floatToHalf(r);
				return;
			case PF_R16:
				((uint16*)dest)[0] = DBitTool::floatToFixedClamped(r,16);
				return;
			case PF_R16_Float:
				((uint16*)dest)[0] = DBitTool::floatToHalf(r);
				return;
				// other format is handled individually
			}
			const PixelFormatDescription &desc = _pixelFormats[format];
			uint32 value = (
				(DBitTool::floatToFixedClamped(r, desc.rbits)<<desc.rShift)&desc.rMask |
				(DBitTool::floatToFixedClamped(g, desc.gbits)<<desc.gShift)&desc.gMask |
				(DBitTool::floatToFixedClamped(b, desc.bbits)<<desc.bShift)&desc.bMask |
				(DBitTool::floatToFixedClamped(a, desc.abits)<<desc.aShift)&desc.aMask);
			DBitTool::writeInt(dest, getFormatBytes(format), value);
		}
	}

}