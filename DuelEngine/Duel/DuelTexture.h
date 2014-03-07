//  [10/10/2012 OMEGA] created

#ifndef _DUELTEXTURE_H_
#define _DUELTEXTURE_H_

#include "DuelCommon.h"
#include "DuelPixelFormat.h"
#include "DuelCodec.h"
#include "DuelResource.h"
#include "DuelRenderState.h"

namespace Duel
{
	enum TextureType
	{
		// texture type is unknown.
		TT_Unknown	= 0,
		// texture have a height of 1, which means the texture
		// have only 1 dimension of color value.
		TT_1D		= 1,
		// texture have a 2 dimension of color value, most
		// pictures can be defined as this type.
		TT_2D		= 2,
		// texture have a 3 dimension of color value: width, height
		// and depth, this type can also be regard as a volume texture.
		TT_3D		= 3,
		// texture have 6 cubic 2D textures, every texture have the same
		// width and height, 
		TT_Cube		= 4
	};


	enum CubeFace
	{
		CF_PositiveX	= 0,
		CF_NegativeX	= 1,
		CF_PositiveY	= 2,
		CF_NegativeY	= 3,
		CF_PositiveZ	= 4,
		CF_NegativeZ	= 5
	};

#define	MIPMAP_MAXCOUNT	0xffffffff

	// base class, design the common interface that will be implemented in 
	// different render system, such as DX, OpenGL,
	class DUEL_API DTexture : public DResource
	{
	DUEL_DECLARE_RTTI(DTexture)
	public:
		DTexture(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		virtual ~DTexture() {}
		// get the texture type, if it is loaded, its type is fixed
		virtual	TextureType	getTextureType() { return mType; }

		// query the properties of the texture
		virtual size_t		getWidth() { return mWidth; }
		virtual size_t		getHeight() { return mHeight; }
		virtual size_t		getDepth() { return mDepth; }
		virtual size_t		getFaceCount() { return (mType == TT_Cube)? 6 : 1; }
		virtual size_t		getMipMapCount() { return mMipMapCount; }
		virtual DPixelFormat	getFormat() { return mFormat; }
		virtual bool		hasAlpha() { return DPixelFormatTool::hasAlpha(mFormat); }

		// set the properties of the texture, these method will do some
		// hardware-related operation depending on different rendersystem,
		// after loadFromImage() called, these value may be changed.
		// NOTICE: resize a texture may lead to content losing.
		virtual	void		resize(uint32 w, uint32 h) { mWidth = w; mHeight = h; }
		virtual void		setWidth(size_t width) { resize(width, mHeight);  }
		virtual	void		setHeight(size_t height) {  resize(mWidth, height); }
		virtual void		setDepth(size_t depth);
		// notice that the default mipmap count MIPMAP_MAXCOUNT means complete mipmap chain.
		virtual	void		setMipMapCount(size_t count = MIPMAP_MAXCOUNT) { mMipMapCount = count; }
		virtual void		setFormat(DPixelFormat format) { mFormat = format; }
		virtual void		setTextureType(TextureType type) { mType = type; }

		// called before loading, if the hardware support gamma, then it will 
		// enalbe gamma correction in real-time. if don't use gamma correction, 
		// set the value to 1.0f.
		virtual void		setGamma(float gamma) { mGamma = gamma; }
		virtual	float		getGamma() { return mGamma; }
		virtual bool		isHardwareGammaSupported() = 0;
		
		// enable/diable the hardware mipmap generating, if the system support.
		// use query function to acquire result.
		virtual void		setHardwareMipMap(bool flag);
		virtual bool		isHardwareMipMapSupported() = 0;

		// convert the texture to an image, according textureType.  notice that the
		// type of the given image MUST be identical to the texture, otherwise it 
		// will quit method, if withMipMap is true, then the mipmaps will be stored too. 
		// notice that the mipmap may lose in some operations of DImage,
		virtual void		saveToImage(DImage& dstImg, bool withMipMap);

		// use the 1st DImage if the img has arraySize bigger than 1.
		// distinct this method with the load() method, this method will
		// transfer data to the PixelBuffer that sub-class provided, and
		// will provide thread-safe as load(), so don't call it from outside the load();
		virtual void		loadFromImage(DImage* img);

		// copy the content to dest texture. should be implemented in sub-class, notice
		// that the two texture should have the same type
		virtual	void		copyToTexture(DTexture* dstTex) = 0;

		// this class provide access to specified texture buffer, must be implemented in sub-class
		virtual	DPixelBufferPtr	getBuffer(CubeFace face = CF_PositiveX, size_t mipmap = 0) = 0;

		// used for loading texture.
		void			setCodecFormat(CodecFormat fmt) { mCodecFormat = fmt; }
		CodecFormat		getCodecFormat() { return mCodecFormat; }

		// this method is implemented by sub class, used in bingding texture 
		// resource to the gpu pipeline.
		virtual DGpuTextureConstant*	getGpuTexutureConstant() = 0;

	protected:
		// override:DResource------------------------
		void			prepareImpl() ;
		// override:DResource------------------------
		void			loadImpl() ;
		// override:DResource------------------------
		void			unloadImpl();
		// override:DResource------------------------
		size_t			calculateSize();

		// this method is inner method used to process image.
		virtual void	loadFromImageImpl(DImage* img) ;
		// these two method is left to subclass to create hardware-related
		// resources, which is truely used in realtime rendering.
		virtual void	createHardwareResource() = 0;
		virtual void	releaseHardwareResource() = 0;

		// check whether we can create an empty texture, in those case the 
		// program wish to do such things.
		bool			validateParameters();

		size_t		mWidth;
		size_t		mHeight;
		size_t		mDepth;
		uint32		mMipMapCount;

		// the type of the texture.
		TextureType	mType;

		// indicate the format this texture used.
		CodecFormat		mCodecFormat;

		// indicate whether the current rendersystem support hardware
		// gamma correction, should be modified in sub-class.
		bool		mbHWGammaSupport;
		// indicate whether we attempt to use hardware mipmap generating
		// if the rendersystem support.
		bool		mbUseHWMipmap;
		// indicate whether the current rendersystem support hardware mipmap
		// generating.
		bool		mbHWMipMapSupport;
		// the format this texture is using.
		DPixelFormat	mFormat;
		// value of gamma, 1.0f means no gamma correction.
		float		mGamma;

	};
}

#endif