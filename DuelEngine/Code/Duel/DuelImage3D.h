//  [10/9/2012 OMEGA] created

#ifndef _DUELIMAGE3D_H_
#define _DUELIMAGE3D_H_

#include "DuelCommon.h"
#include "DuelImage.h"

namespace Duel
{
	// a 3D image, although there is probablity to process image that has 
	// more than one surface, including mipmaps(because the DImageDesc
	// is designed to represent every type of image for commonness) 
	// but it is recommanded to use this class to handle only one surface
	// of an image, because every operations will only happen on the main surface.
	class DUEL_API DImage3D : public DImage
	{
	DUEL_DECLARE_RTTI(DImage3D)
	public:
		DImage3D();
		DImage3D(const DString& name);
		// copy constructor, copies all data from the src image,
		// the specified imageType MUST be an IT_IMAGE2D, or IT_IMAGE1D, 
		// which will have height of 1.
		DImage3D(const DImage& img);
		// do nothing because father class will free the data.
		virtual ~DImage3D();

		//override:DImage---------------------------------
		void	loadImage(const char* pData, const DImageDesc& iDesc);
		//override:DImage---------------------------------
		void	loadImage(DDataStream& ds, const  DImageDesc& iDesc);

		// scale current DImage to the destImage, notice that destImg should be writable
		// and only pixels in WorkRect can be processed, so does the destImg.
		// notice that the format of the two DImage should be uncompressed, otherwise the
		// scaled result will definitely be wrong.
		// override:DImage--------------------------------------------
		void	scale(DImage& destImg, ImageFilter filter = IF_Bilinear);

		// resize the image
		void	resize(uint32 width, uint32 height, uint32 depth, ImageFilter filter = IF_Bilinear);

		// from the top-left corner of the 3d DImage.
		//    w  (depth)
		//   /
		//  +----u  (width)
		//  |
		//  |
		//  v (height)
		// get the color at specified u, v, w coordinate
		// if query failed, a DColor::WHITE will be returned.
		DColor	getColorAt(size_t u, size_t v, size_t w);
		// set color at specified point.
		// if this image is writable. mipmap will not be affected, 
		void	setColorAt(const DColor& color, size_t u, size_t v, size_t w);


		// set the work box to a new one, if  the dstBox is bigger than the image,
		// its exceeded part will be cut.
		void			setWorkBox(const ImageBox& dstBox);
		// get current WorkBox
		const ImageBox&	getWorkBox();
		// set the work box to the max.
		void			maximizeWorkBox();
		// get the pointer to the beginning of the WorkBox
		char*			getWorkChunkPtr();
		const char*		getWorkChunkPtr() const;
		// get the byte size of work area
		size_t			getWorkChunkSize();
		// validate current working box, cutting the exceeded part
		void			validateWorkBox();
	protected:
		// sampler methods, used in scale function, not provided for public.
		void	nearestSample(DImage3D& destImg);
		void	bilinearSample(DImage3D& destImg);
		// not supported in 3d image.
		//void	bicubicSample(DImage3D& destImg, ImageFilter filter);
		// current working box, modifications will happen within it.
		// notice the right is the direction the image width increases
		// the down is the direction the image height increases
		// the direction your face toward to is the direction image
		// depth increases.
		ImageBox	mWorkBox;
	};

}

#endif