//  [8/26/2012 OMEGA] created

#ifndef _DUELIMAGEDESC_H_
#define _DUELIMAGEDESC_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelCodec.h"

namespace Duel
{
	// indicate the property of the image
	enum ImageProperty
	{
		// indicate the image using sRGB format
		IP_SRGB			= 0x00000040,
		// indicate the image is compressed
		IP_Compressed	= 0x00000080,
		// indicate the image contains alpha channel
		IP_Alpha		= 0x00000100
	};

	// indicate the image type when regard this image as a texture.
	enum ImageType
	{
		IT_Image1D,
		IT_Image2D,
		IT_Image3D,
		IT_ImageCube
	};

	// used to descripe a image's attributes, such as width, height, depth(volume) format
	// or image count, usually cooperate with code/decode processing.
	class DUEL_API DImageDesc : public DMediaDesc
	{
	DUEL_DECLARE_RTTI(DImageDesc)
	public:
		DImageDesc() : width(0), height(0), depth(0), arraySize(0), imageType(IT_Image2D),
			imageProperty(0), mipMapCount(0), pixelFormat(PF_Unknown){}
		~DImageDesc(){}
		// specified the width of the image, if the image contais
		// more than one suface(such as mipmaps), this value specified
		// the main one's.
		uint32	width;
		// specified the height of the image, if the image contais
		// more than one suface(such as mipmaps), this value specified
		// the main one's.
		uint32	height;
		// specified the depth of the image, if the image contais
		// more than one suface(such as mipmaps), this value specified
		// the main one's.
		uint32	depth;

		// indicate the Image array length
		// if it is a 2D/CubeMap, then this value indicate
		// the count of the cubes, if it is a 3D/CubeMap,
		// this value should always be 1.
		uint32	arraySize;
		// image texture type
		ImageType	imageType;
		// indicate the image property
		uint32	imageProperty;
		// specified the mipmap count of the image.
		// NOTICE: main surface is also regard as a mipmap.
		uint32	mipMapCount;
		// specified of the format of the image.
		DPixelFormat	pixelFormat;

		bool	hasProperty(ImageProperty ip)
		{
			return ((imageProperty & ip) != 0);
		}

		void	enableProperty( ImageProperty ip )
		{
			imageProperty &= ip;
		}

		void	disableProperty( ImageProperty ip )
		{
			imageProperty &= ~ip;
		}

		// used to calculate the memory cost the image may use, in byte.
		size_t	getImageSize() const
		{
			// start calculating from main surface
			uint32 mipWidth	= width;
			uint32 mipHeight	= height;
			uint32 mipDepth	= (imageType == IT_Image3D)? depth : 1;
			size_t	totalSize	= 0;

			if(imageType == IT_ImageCube)
			{
				for(uint32 i = 0; i < arraySize; ++i)
				{
					// here, main surface is also a mip map.
					for(uint32 mip = 0; mip < mipMapCount; ++mip)
					{
						totalSize += 6 * DPixelFormatTool::getSurfaceSize(mipWidth, mipHeight, 1, pixelFormat);
						if(mipWidth != 1)
						{
							mipWidth /= 2;
						}
						if(mipHeight != 1)
						{
							mipHeight /= 2;
						}
					}
				}
			} // IT_TEXTURECUBE
			else
			{
				for(uint32 i = 0; i < arraySize; ++i)
				{
					// here, main surface is also a mip map.
					for(uint32 mip = 0; mip < mipMapCount; ++mip)
					{
						totalSize += DPixelFormatTool::getSurfaceSize(mipWidth, mipHeight, mipDepth, pixelFormat);
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
					}
				}
			} 
			return totalSize;
		}
	};
}

#endif