//  [8/6/2012 OMEGA] created

#ifndef _DUELPIXELFORMAT_H_
#define _DUELPIXELFORMAT_H_

#include "DuelCommon.h"
#include "DuelColor.h"

namespace Duel
{

	enum DPixelFormat
	{
		PF_Unknown                      = 0,
		
		// 32 bits unsigned int for each channel.
		PF_R32G32B32A32					= 1,
		// 32 bits float for each channel.
		PF_R32G32B32A32_Float           = 2,

		// 32 bits unsigned int for each channel.
		PF_R32G32B32					= 3,
		// 32 bits float for each channel.
		PF_R32G32B32_Float              = 4,
		
		// 16 bits unsigned int for each channel.
		PF_R16G16B16A16					= 5,
		// 16 bits signed int for each channel.
		PF_Q16W16V16U16					= 6,
		// 16 bits float for each channel.
		PF_R16G16B16A16_Float           = 7,
		
		// 32 bits unsigned int for each channel.
		PF_G32R32					    = 8,
		// 32 bits float for each channel.
		PF_G32R32_Float                 = 9,
		
		// 10 bits unsigned int for red, green, blue, and 2 unsigned int for alpha
		PF_A2B10G10R10					= 10,
		PF_A2R10G10B10					= 11,

		// 16 bits unsigned int for red, green.
		PF_G16R16					    = 12,
		// 16 bits float for red, green.
 		PF_G16R16_Float                 = 13,

		// 32 bits unsigned int for red.
		PF_R32							= 14,
		// 32 bits float for depth.
		PF_D32_Float                    = 15,
		// 32 bits float for red.
		PF_R32_Float                    = 16,
		
		// 8 bits unsigned int for each channel.
		PF_A8B8G8R8						= 17,
		// 8 bits unsigned int for each channel.
		PF_A8R8G8B8						= 18,
		// 8 bits unsigned int for each channel.
		PF_B8G8R8A8						= 19,
		// 8 bits unsigned int for each channel.
		PF_R8G8B8A8						= 20,
		// 8 bits unsigned int for red, green, blue, 8 bits unused.
		PF_X8R8G8B8						= 21,
		// 8 bits unsigned int for red, green, blue, 8 bits unused.
		PF_X8B8G8R8						= 22,

		// 8 bits unsigned int for each channel.
		PF_R8G8B8						= 23,
		// 8 bits unsigned int for each channel.
		PF_B8G8R8						= 24,

		// 1 bits for alpha, 5 bits unsigned int for red, green, blue.
		PF_A1R5G5B5						= 25,
		// 1 bits unused, 5 bits unsigned int for red, green, blue.
		PF_X1R5G5B5						= 26,
		// 5 bits unsigned int for red, 6 bits unsigned int for green, 5 bits unsigned int for blue.
		PF_R5G6B5						= 27,
		// 5 bits unsigned int for red, 6 bits unsigned int for green, 5 bits unsigned int for blue.
		PF_B5G6R5						= 28,

		// 4 bits unsigned int for each channel.
		PF_A4R4G4B4						= 29,
		// 4 bits for red, green, blue, 4 bits unused.
		PF_X4R4G4B4						= 30,
		
		// 16 bits unsigned int for red.
		PF_R16			                = 31,
		// 16 bits float for red.
		PF_R16_Float                    = 32,
		// 16 bits unsigned int for depth.
		PF_D16							= 33,
		// 8 bits unsigned int for alpha, 8 bits luminance.
		PF_A8L8							= 34,
		// L: luminance
		PF_L16							= 35,

		// 8 bits unsigned int for red.
		PF_R8			                = 36,
		// 8 bits unsigned int for alpha.
		PF_A8							= 37,
		// L: luminance
		PF_L8							= 38,
		// 4 bits unsigned int for alpha, 4 bits luminance.
		PF_A4L4							= 39,

		// block compressed format, 
		PF_BC1							= 40,
		PF_BC2							= 41,
		PF_BC3							= 42,
		PF_BC4							= 43,
		PF_BC5							= 44,
	
		PF_Count						= 45,
		PF_FORCE_UINT					= 0xffffffffUL // never used
	};

	/** Pixel component format */
	enum PixelComponentType
	{
	
		// A floating-point value; 32-bit floating-point formats use IEEE 754 single-precision (s23e8 format):
		// sign bit, 8-bit biased (127) exponent, and 23-bit mantissa. 
		// 16-bit floating-point formats use half-precision (s10e5 format): sign bit, 5-bit biased (15) exponent, and 10-bit mantissa.
		PCT_Float = 0x01,	
		
		// Unsigned normalized integer; which is interpreted in a resource as an unsigned integer,
		// and is interpreted in a shader as an unsigned normalized floating-point value in the range [0, 1].  
		PCT_UInt = 0x02,	 

		PCT_Unknown	= 0x80
	};

	enum PixelFormatFlag
	{
		// a format contains depth component
		PFF_Depth	= 0x01,

		// a block-compressed format
		PFF_Compressed	= 0x02,

		// a format contains alpha channel
		PFF_Alpha	= 0x04
	};

	// utility class for query format's information
	class DUEL_API DPixelFormatTool
	{
	private:
		// utiliti class has no need for instanciation
		DPixelFormatTool(){}
		~DPixelFormatTool(){}

	public:
		static bool		isCompressed(DPixelFormat format);
		static bool		hasAlpha(DPixelFormat format);
		static bool		isDepth(DPixelFormat format);

		static bool		hasComponentType(DPixelFormat format, PixelComponentType pct);

		static uint16	getFormatBits(DPixelFormat format);
		static uint32	getFormatBytes(DPixelFormat format);
		static DString	getFormatName(DPixelFormat format);
		// notice the name must have "PF_" prefix, and all be upper case, according to the enum name.
		static DPixelFormat	getFormatFromName(const DString& name); 

		static uint8	getRedChannelBits(DPixelFormat format);
		static uint8	getGreenChannelBits(DPixelFormat format);
		static uint8	getBlueChannelBits(DPixelFormat format);
		static uint8	getAlphaChannelBits(DPixelFormat format);

		static uint8	getComponentCount(DPixelFormat format);

		// query the suface(i.e a main image or a mipmap)'s size, in bytes. 
		static uint32	getSurfaceSize(uint32 width, uint32 height, uint32 depth, DPixelFormat format);

		// query the value is used to describe color directly, i.e uncompressed or not a depth
		static bool		isDirectColor(DPixelFormat format);

		// main algorithm taken from Ogre-------------
		// get a int color value from specified format and source, source must be DUEL_LITTLE_ENDIAN
		static void		unpackColor(DColor* result, DPixelFormat format, void* src);
		// get a int color value from specified format and source, source must be DUEL_LITTLE_ENDIAN
		static void		unpackColor(uint8* a, uint8* r, uint8* g, uint8* b, DPixelFormat format, void* src);
		// get a float color value from specified format and source, source must be DUEL_LITTLE_ENDIAN
		static void		unpackColor(float* a, float* r, float* g, float* b, DPixelFormat format, void* src);

		// write a color value in specified format to the dest.
		static void		packColor(const DColor& color, DPixelFormat format, void* dest);
		// write a color value in specified format to the dest.
		static void		packColor(uint8 a, uint8 r, uint8 g, uint8 b, DPixelFormat format, void* dest);
		// write a color value in specified format to the dest.
		static void		packColor(float a, float r, float g, float b, DPixelFormat format, void* dest);

	};

}





#endif