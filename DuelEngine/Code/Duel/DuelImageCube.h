//  [10/9/2012 OMEGA] created

#ifndef _DUELIMAGECUBE_H_
#define _DUELIMAGECUBE_H_

#include "DuelCommon.h"
#include "DuelImage.h"

namespace Duel
{
	// a Cube image, although there is probablity to process image that has 
	// more than one surface, including mipmaps(because the DImageDesc
	// is designed to represent every type of image for commonness) 
	// but it is recommanded to use this class to handle only one surface
	// of an image, because every operations will only happen on the main surface.
	class DUEL_API DImageCube : public DImage
	{
	DUEL_DECLARE_RTTI(DImageCube)
	public:
		DImageCube();
		DImageCube(const DString& name);
		// copy constructor, copies all data from the src image,
		// the specified imageType MUST be an IT_IMAGE2D, or IT_IMAGE1D, 
		// which will have height of 1.
		DImageCube(const DImage& img);
		// do nothing because father class will free the data.
		virtual ~DImageCube();

		//override:DImage---------------------------------
		void	loadImage(const char*	pData, const DImageDesc& iDesc);
		//override:DImage---------------------------------
		void	loadImage(DDataStream& ds, const  DImageDesc& iDesc);

		// scale current DImage to the destImage, notice that destImg should be writable
		// and only pixels in WorkRect can be processed, so does the destImg.
		// notice that the format of the two DImage should be uncompressed, otherwise the
		// scaled result will definitely be wrong.
		// override:DImage-----------------------------------------
		void	scale(DImage& destImg, ImageFilter filter = IF_Bilinear);

		// resize the image
		void	resize(uint32 width, ImageFilter filter = IF_Bilinear);


		// get the color at specified x, y and face
		// if query failed, a DColor::WHITE will be returned.
		DColor	getColorAt(size_t x, size_t y, ImageCubeFace face);
		// set color at specified point.
		// if this image is writable. mipmap will not be affected, 
		void	setColorAt(const DColor& color, size_t x, size_t y, ImageCubeFace face);


		// set work area for the image, the rect will be cut if it exceed the image size.
		// the rect will not affect mipmaps,
		void				setWorkRect(const ImageRect& rect);
		// set the current WorkFace.
		void				setWorkFace(ImageCubeFace face);
		// set WorkRect to the biggest.
		void				maxmizeWorkRect();
		// get current working rect.
		const ImageRect&	getWorkRect();
		// get current WorkFace
		ImageCubeFace		getWorkFace();
		// get the pointer to the beginning of the WorkBox
		char*				getWorkChunkPtr();
		const char*			getWorkChunkPtr() const;
		// get the byte size of work area
		size_t				getWorkChunkSize();
		// validate current rect, cut the exceeded part.
		void				validateWorkRect();

	protected:
		// sampler methods, used in scale function, not provided for public.
		void	nearestSample(DImageCube& destImg);
		void	bilinearSample(DImageCube& destImg);
		void	bicubicSample(DImageCube& destImg, ImageFilter filter);
		// current working rect, modifications will happen in the rect.
		ImageRect	mWorkRect;
		// the current working face, indicate on which face the modifications will
		// happen
		ImageCubeFace	mWorkFace;
	};
	
}

#endif