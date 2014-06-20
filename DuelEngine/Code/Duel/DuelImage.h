//  [8/28/2012 OMEGA] created


#ifndef _DUELIMAGE_H_
#define _DUELIMAGE_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelImageDesc.h"

namespace Duel
{
	typedef	UBox	ImageBox;
	typedef	URect	ImageRect;

	// filter type used in scale.
	enum ImageFilter
	{
		IF_Nearest,
		IF_Bilinear/*,
		IF_BICUBIC_SHARP,
		IF_BICUBIC_SOFT,
		IF_BICUBIC_AUTO*/  //TODO: not support yet.
	};

	enum ImageCubeFace
	{
		ICF_PositiveX	= 0,
		ICF_NegativeX	= 1,
		ICF_PositiveY	= 2,
		ICF_NegativeY	= 3,
		ICF_PositiveZ	= 4,
		ICF_NegativeZ	= 5,
	};

	// lock of the specified area in an image, used to indicate 
	// the access to operate on specified area.
	enum ImageLock
	{
		IL_Read,
		IL_Write
	};

	// base class,
	class DUEL_API DImage : public DObject
	{
	DUEL_DECLARE_RTTI(DImage)
	public:
		DImage();
		DImage(const DString& name);
		// copy constructor, copies all data from the src image.
		DImage(const DImage& img);
		// do free the data.
		virtual ~DImage();

		// use specified DImageDesc to create a new image, make sure the
		// image is well decoded.
		// notice: the pData will be copied to the image with clamped
		// length of idesc, be sure the length is match the pData.
		// the pData will not be managed by this image.
		virtual void	loadFromData(const char* pData, const DImageDesc& iDesc);
		// use specified DImageDesc to create a new image, make sure the
		// image is well decoded.
		// notice: the ds will be copied to the image with clamped
		// length of idesc, be sure the length is match the ds's length.
		virtual void	loadFromData(DDataStream& ds, const  DImageDesc& iDesc);
		
		
		// this method will delegate the task to loadImage();
// 		DImage& operator = (const DImage& img);

		//@note
		//	Whilst typically your image is likely to be a simple 2D image,
		//	you can define complex images including cube maps, volume maps,
		//	and images including custom mip levels. The layout of the 
		//	internal memory should be:
		//		<ul><li>face 0, mip 0 (top), width x height (x depth)</li>
		//			<li>face 0, mip 1, width/2 x height/2 (x depth/2)</li>
		//			<li>face 0, mip 2, width/4 x height/4 (x depth/4)</li>
		//			<li>.. remaining mips for face 0 .. </li>
		//			<li>face 1, mip 0 (top), width x height (x depth)</li
		//			<li>.. and so on. </li>
		//			</ul>
		//			Of course, you will never have multiple faces (cube map) and
		//			depth too.

		// get/set the name of the image
		void			setName(const DString& name) { mName = name; }
		const DString&	getName() const { return mName; }

		uint32			getWidth() const { return mDesc.width; }
		uint32			getHeight() const { return mDesc.height; }
		uint32			getDepth() const { return mDesc.depth; }
		uint32			getArraySize() const { return mDesc.arraySize; }
		uint32			getMipMapCount() const { return mDesc.mipMapCount; }
		DPixelFormat		getFormat() const { return mDesc.pixelFormat; }
		ImageType 		getImageType() const { return mDesc.imageType; }
		
		// width in byte of a row(scan line)
		const size_t	getRowPitch() const 
		{
			return mRowPitch;
		}
		// bytes of a image face
		const size_t	getSlicePitch() const 
		{ 
			return mSlicePitch;
		}	
		
		// scale an image to a given image with different size.
		virtual void	scale(DImage& destImg, ImageFilter filter = IF_Bilinear) = 0;

		// query the property of the image.
		bool			hasProperty(ImageProperty ip) const;
		void			enableProperty( ImageProperty ip );
		void			disableProperty( ImageProperty ip );

		// get access to the work box
		ImageLock		getAccess() const ;
		void			setAccess(ImageLock il);


		// save the image to a file, using specified codec
		void			saveToFile(const DString& fileName, DMediaCodec* codec);
		// encode the DImage to an DDataStream.
		DDataStreamPtr	encode(DMediaCodec* codec);

		// get the pointer to the data.
		char*			getChunkPtr() { return mData; }
		const char*		getChunkPtr() const { return mData; }
		// used to calculate the memory cost the image may use, in byte.
		size_t			getImageSize() const ;


	protected:
		// the name of the DImage, because it is not handled as a resource,
		// so the name can be duplicated
		DString		mName;
		// make the work box valid(clamp it using image's desc)
		//void		validateWorkBox();

		// all information about the image is contained in mDesc
		DImageDesc	mDesc;

		// data pointer to the data.
		char*		mData;

		// indicate the size of the image.
		size_t		mSize;
		// restore a size of row elements of an image in bytes, for quick access;
		size_t		mRowPitch;
		// restore a size of the main surface size in bytes, for quick access.
		size_t		mSlicePitch;
		// box lock, indicate the access to the box
		ImageLock	mLock;
	};
}

#endif