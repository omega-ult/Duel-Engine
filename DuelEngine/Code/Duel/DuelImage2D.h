//  [10/9/2012 OMEGA] created

#ifndef _DUELIMAGE2D_H_
#define _DUELIMAGE2D_H_

#include "DuelCommon.h"
#include "DuelImage.h"

namespace Duel
{
	// a 2D image, although there is probablity to process image that has 
	// more than one surface, including mipmaps(because the DImageDesc
	// is designed to represent every type of image for commonness) 
	// but it is recommanded to use this class to handle only one surface
	// of an image, because every operations will only happen on the main surface.
	class DUEL_API DImage2D : public DImage
	{
	DUEL_DECLARE_RTTI(DImage2D)
	public:
		DImage2D();
		DImage2D(const DString& name);
		// copy constructor, copies all data from the src image,
		// the specified imageType MUST be an IT_IMAGE2D, or IT_IMAGE1D, 
		// which will have height of 1.
		DImage2D(const DImage& img);
		// do nothing because father class will free the data.
		virtual ~DImage2D();

		//override:DImage---------------------------------
		void	loadImage(const char* pData, const DImageDesc& iDesc);
		//override:DImage---------------------------------
		void	loadImage(DDataStream& ds, const  DImageDesc& iDesc);

		// get the color at specified x, y.
		// if query failed, a DColor::WHITE will be returned.
		DColor	getColorAt(size_t x, size_t y);
		// set color at specified point.
		// if this image is writable. mipmap will not be affected, 
		void	setColorAt(const DColor& color, size_t x, size_t y);

		// scale current DImage to the destImage, notice that destImg should be writable
		// and only pixels in WorkRect can be processed, so does the destImg.
		// notice that the format of the two DImage should be uncompressed, otherwise the
		// scaled result will definitely be wrong.
		// override:DImage-------------------------------------
		void	scale(DImage& destImg, ImageFilter filter = IF_Bilinear);

		// resize the image
		void	resize(uint32 width, uint32 height, ImageFilter filter = IF_Bilinear);

		// set work area for the image, the rect will be cut if it exceed the image size.
		// the rect will not affect mipmaps,
		void				setWorkRect(const ImageRect& rect);
		// set WorkRect to the biggest.
		void				maxmizeWorkRect();
		// get current working rect.
		const ImageRect&	getWorkRect();
		// get the pointer to the beginning of the WorkBox
		char*				getWorkChunkPtr();
		const char*			getWorkChunkPtr() const;
		// get the byte size of work area
		size_t				getWorkChunkSize();
		// validate current rect, cut the exceeded part.
		void				validateWorkRect();

	protected:
		// sampler methods, used in scale function, not provided for public.
		void	nearestSample(DImage2D& destImg);
		void	bilinearSample(DImage2D& destImg);
		void	bicubicSample(DImage2D& destImg, ImageFilter filter);

		// current working rect, modifications will happen in the rect.
		ImageRect	mWorkRect;
	};

}

#endif