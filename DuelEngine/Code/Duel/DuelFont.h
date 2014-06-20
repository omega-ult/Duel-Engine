//  [8/20/2013 OMEGA] created

#ifndef _DUELFONT_H_
#define _DUELFONT_H_

#include "DuelCommon.h"
#include "DuelResource.h"
#include "DuelResourceManager.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H


namespace Duel
{
	/*
		这里使用的技术详解:
		我们使用的字体文件(假设我们只使用ttf格式的)包含的字形含有下面几种属性:
		字形大小(glyphSize), 分辨率(glyphResolution),width/height, hBearingX, hBeartingY,
		vBeartingX(竖向), vBeartingY(竖向), hAdvance, vAdvance(竖向), 
		详情参照:
		http://www.unixresources.net/linux/clf/kylix/archive/00/00/59/21/592188.html
		(暂时我先排除竖向的属性, 去掉竖排的支持). 这些属性完成了字体的排版, 字体类中只能取得这些文字的信息,
		以及文字在纹理图中的uv坐标(以及所在纹理序号),排版工作交给上层应用程序. 为了实现像中文这样
		大型字库的支持, 我们不可能一次性把所有字符写到纹理上, 只能一次次做缓存, 因此字形大小和分辨率
		一旦load以后就不可改变, 字体类也会在每次取到新字的时候将字形写入相应的纹理中.
		用户不需要知道写如纹理时发生了什么, 但是这里为了日后方便, 我还是稍微记录一下.
		我们使用一般的从左到右, 从上到下的记录文字缓存的方式, 每个字形占一个小框, 但是由于字形框大小不一,
		而且我们不知道用户下次取用的是哪个字,因此我们没法预判文字的占用空间, 使用统一空间又略显浪费,
		因而我采用的方式是适配最佳行高的做法, 记录每一行最高的字形的高, 如果新增字形没有高出这个行高,
		则继续往后增加, 如果字形高出, 则将最高行高记录为当前字形的高, 转入下一行时根据这个行高重新定位,
		直到超出当前纹理高度(一旦超出纹理高度立即写入新纹理, 不再利用剩余位置, 依次保证新加入的字形永远处于
		最后一块纹理上).
	*/
	// font resource, 
	class DUEL_API DFont : public DResource
	{
	DUEL_DECLARE_RTTI(DFont)
	public:
		DFont(DResourceManager* fathermanager, const DString& name, const DString& groupName);
		typedef	uint32	CodePoint;
		typedef	FRect	UVRect;
		/// Information about the position and size of a glyph in a texture
		struct GlyphInfo 
		{
			CodePoint	codePoint;
			UVRect		uvRect;
			uint32		textureIndex;	
			// the index of this glyph located in the font file.
			uint32		hAdvance;
			uint32		hBearingX;
			uint32		hBearingY;
			uint32		width;
			uint32		height;
			GlyphInfo() : codePoint(0), textureIndex(0), hAdvance(0), hBearingY(0), hBearingX(0), width(0), height(0),
				uvRect(0.0f, 0.0f, 0.0f, 0.0f) {}
		};
		/// A range of code points, inclusive on both ends
		/// Map from unicode code point to texture coordinates
		typedef std::map<CodePoint, GlyphInfo>	CodePointMap;
		typedef	std::vector<DResourcePtr>		GlyphTextures;
		typedef	MapIterator<CodePointMap>		CodePointIterator;
		typedef	VectorIterator<GlyphTextures>	GlyphTextureIterator;
		CodePointIterator	getCodePointIterator() { return CodePointIterator(mCodePointMap); }
		GlyphTextureIterator	getGlyphTextureIterator() { return GlyphTextureIterator(mGlyphTextures); }
       
		/** Sets the size of a truetype font (only required for FT_TRUETYPE). 
        @param ttfSize The size of the font in points. Note that the
            size of the font does not affect how big it is on the screen, just how large it is in
            the texture and thus how detailed it is.
			resolution of single glyph, no more than 128px, and it must be 2^n.
			it will not be modified if the font is already loaded.
        */
        void			setGlyphSize(uint32 size);

        /** Gets the point size of the font used to generate the texture.
        @remarks
            Only applicable for FT_TRUETYPE Font objects.
            Note that the size of the font does not affect how big it is on the screen, 
            just how large it is in the texture and thus how detailed it is.            
        */
        uint32			getGlyphSize(void) const;

		// set the size of the texture holding the glyph, ranges from 256 to 1024, and must be 2^n
		// this function will force the size to fit the requirement.
		// it will not be modified if the font is already loaded.
		void			setTextureSize(uint32 size);
		uint32			getTextureSize();

		/** Gets the maximum baseline distance of all glyphs used in the texture.
		@remarks
			Only applicable for FT_TRUETYPE Font objects.
			The baseline is the vertical origin of horizontal based glyphs.  The bearingY
			attribute is the distance from the baseline (origin) to the top of the glyph's 
			bounding box.
		@note
			This value is only available after the font has been loaded.
		*/
		int32			getTrueTypeMaxBearingY() const;

		/** Sets whether or not the colour of this font is antialiased as it is generated
            from a true type font.
        @remarks
        	This is valid only for a FT_TRUETYPE font. If you are planning on using 
            alpha blending to draw your font, then it is a good idea to set this to
            false (which is the default), otherwise the darkening of the font will combine
            with the fading out of the alpha around the edges and make your font look thinner
            than it should. However, if you intend to blend your font using a colour blending
            mode (add or modulate for example) then it's a good idea to set this to true, in
            order to soften your font edges.
        */
		void			setAntialiasColour(bool enabled);

		/** Gets whether or not the colour of this font is antialiased as it is generated
		from a true type font.
		*/
        bool			getAntialiasColour(void) const;

		// use this method to get glyph for rendering.
		GlyphInfo		getGlyphInfo(uint32 codePoint);
		
		// get the texture storing the glyphs.
		DResourcePtr		getGlyphTexture(uint32 idx);

	protected:
		// override : DResource-------------------------------
		virtual void		loadImpl() ;
		virtual void		unloadImpl();
		virtual size_t		calculateSize();
		//////////////////////////////////////////////////////////////////////////
		// internal method, return a new pace for the new glyph, returned value is texture index.
		uint32				getNextGlyphSpace(uint32 w, uint32 h, uint32& outX, uint32& outY);
		void				appendNewTexture();

		CodePointMap	mCodePointMap;

		/// Size of the truetype font, in points
		uint32			mGlyphSize;
		// width(height) of the texture.
		uint32			mTexureSize;
		/// Max distance to baseline of this (truetype) font
		int32			mTtfMaxBearingY;

		/// for TRUE_TYPE font only
		bool			mAntialiasColour;

		GlyphTextures	mGlyphTextures;
		
		// recording the position the new glyph should be placed.
		uint32			mCurRowMaxHeight;
		uint32			mCurLocX;
		uint32			mCurLocY;
		// internal object used for reading glyphs.
		FT_Library		mFtLib;
		FT_Face			mFtFace;
	};

}

#endif