//  [8/20/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelDataStream.h"
#include "DuelMemoryStream.h"
#include "DuelResource.h"
#include "DuelFont.h"
#include "DuelResourceGroupManager.h"
#include "DuelTextureManager.h"
#include "DuelTexture.h"
#include "DuelPixelBuffer.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_BITMAP_H

namespace Duel
{

	DUEL_IMPLEMENT_RTTI_1(DFont, DResource);

#define _DFONT_CHECK_LOAD_STATE 	if ( mLoadState.get() == LS_Loading ||	\
										mLoadState.get() == LS_Loaded)		\
									{	return; }								
		


	DFont::DFont( DResourceManager* fathermanager, const DString& name, const DString& groupName) :
		DResource(fathermanager, name, groupName, "Font"),
		mGlyphSize(32),
		mTexureSize(1024), 
		mTtfMaxBearingY(0), 
		mAntialiasColour(false),
		mCurLocX(0),
		mCurLocY(0),
		mCurRowMaxHeight(0)
	{

	}

	void DFont::loadImpl() 
	{
		if (mPreparedData == NULL)
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Prepared data is empty",
				"Duel::DFont::loadImpl()");
		}
		// Init freetype
		if( FT_Init_FreeType( &mFtLib ) )
		{
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to call FT_Init_FreeType()",
				"Duel::DFont::loadImpl()");
// 			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
// 			"Could not init FreeType library!",
// 			"Duel::DFont::loadImpl");
		}
		mCurLocX = 0;
		mCurLocY = 0;
		mCurRowMaxHeight = 0;
		// create a new texture.
		appendNewTexture();
		const FT_Byte* data = (FT_Byte*)mPreparedData->getChunkPtr();
		
		// load font
		if(FT_New_Memory_Face( mFtLib, data, (FT_Long)mPreparedData->getSize() , 0, &mFtFace ))
		{
			FT_Done_FreeType(mFtLib);
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to call FT_New_Memory_Face()",
				"Duel::DFont::loadImpl()");
// 			DUEL_EXCEPT_BRIEF(DException::ET_InternalError, 
// 				"Could not open font face!",
// 				"Duel::DFont::loadImpl");
		}

		if( FT_Set_Pixel_Sizes(mFtFace, mGlyphSize, mGlyphSize))
			/*FT_Set_Char_Size( mFtFace, ftSize, 0, mGlyphResolution, mGlyphResolution ) )*/
		{		
			FT_Done_Face(mFtFace);
			FT_Done_FreeType(mFtLib);
			DUEL_EXCEPT_BRIEF(DException::ET_InvalidParameters,
				"Failed to call FT_Set_Pixel_Sizes()",
				"Duel::DFont::loadImpl()");
// 			DUEL_EXCEPT_BRIEF(DException::ET_InternalError,
// 			"Could not set char size!", 
// 			"Duel::DFont::loadImpl" );
		}
	}

	void DFont::unloadImpl()
	{
		mCurLocX = 0;
		mCurLocY = 0;
		mCurRowMaxHeight = 0;
		FT_Done_Face(mFtFace);
		FT_Done_FreeType(mFtLib);
		mCodePointMap.clear();
		mGlyphTextures.clear();
	}

	size_t DFont::calculateSize()
	{
		// 待处理.
		size_t sumSize = 0;
		if (mExternSrc != NULL)
		{
			sumSize += mExternSrc->getSize();
		}
		if (mPreparedData != NULL)
		{
			sumSize += mPreparedData->getSize();
		}
		// size of the texture is calculated in TextureManager.
		return sumSize;
	}

	void DFont::setGlyphSize( uint32 glyphSize )
	{
		_DFONT_CHECK_LOAD_STATE
		// 这里确保size是2的倍数以及小于等于128.
		uint32 s = 1;
		uint32 tmpSize = glyphSize >> 1;
		while (tmpSize != 0 && s < 128)
		{
			tmpSize >>= 1;
			s <<= 1;
		}
		mGlyphSize = s;
	}

	uint32 DFont::getGlyphSize( void ) const
	{
		return mGlyphSize;
	}

	int32 DFont::getTrueTypeMaxBearingY() const
	{
		return mTtfMaxBearingY;
	}

	void DFont::setAntialiasColour( bool enabled )
	{
		_DFONT_CHECK_LOAD_STATE
		mAntialiasColour = enabled;
	}

	bool DFont::getAntialiasColour( void ) const
	{
		return mAntialiasColour;
	}

	DResourcePtr DFont::getGlyphTexture(uint32 idx)
	{
		assert(idx < mGlyphTextures.size());
		return mGlyphTextures[idx];
	}

	void DFont::setTextureSize( uint32 size )
	{
		_DFONT_CHECK_LOAD_STATE
		uint32 co = 1;
		uint32 tmpSize = (size >> 1)/256; // 右移1为了凑足256的因子.
		while (tmpSize != 0 && co < 4)
		{
			tmpSize >>= 1;
			co <<= 1;
		}
		mTexureSize = co * 256;
	}

	uint32 DFont::getTextureSize()
	{
		return mTexureSize;
	}

	uint32 DFont::getNextGlyphSpace( uint32 w, uint32 h, uint32& outX, uint32& outY )
	{
#define	GLYPH_SPACING	(1)
		// 先处理横向的位置.
		if (w + mCurLocX  > mTexureSize)
		{
			// 移入下一格. 重置最高高度.
			mCurLocY = mCurLocY + GLYPH_SPACING + mCurRowMaxHeight;
			mCurRowMaxHeight = 0;
			mCurLocX = 0;
		}
		// 再处理纵向的位置.
		if (h + mCurLocY > mTexureSize)
		{
			// 超出本纹理可容纳大小. 另起一新纹理
			appendNewTexture();
			mCurLocY = 0;
			mCurLocX = 0;
		}
		// 撑开高度.
		mCurRowMaxHeight = DMath::Max<uint32>(h, mCurRowMaxHeight);
		outX = mCurLocX;
		outY = mCurLocY;
		mCurLocX += ( w + GLYPH_SPACING);
		// 返回最新的textureIndex.
 		return mGlyphTextures.size() - 1;
	}

	void DFont::appendNewTexture()
	{
		// 增加一个随机数确保重新载入后不冲突.
		srand((uint32)time(0));
		uint32 r = rand();
		DResourceManager* texMgr = DResourceGroupManager::getSingleton().getResouceManager("Texture");
		DString uniqueName = "_" + mName + "_" + DStringTool::toString(mGlyphTextures.size()) + "_" + DStringTool::toString(r) + "_";
		if (texMgr->getResource("", uniqueName) != NULL)
		{
			uniqueName = uniqueName + "_dup";
		}
		DTextureDescription texParam(	uniqueName,mGroupName);
		texParam.codecFormat = CF_DDS;
		texParam.memoryStrategy = HBU_Dynamic;

		DResourcePtr res = texMgr->create(&texParam);
		DTexture* tex = res->getAs<DTexture>();
		tex->setWidth(mTexureSize);
		tex->setHeight(mTexureSize);
		tex->setMipMapCount(1);
		tex->setTextureType(TT_2D);
		tex->setFormat(PF_A8);
		tex->load();
		DPixelBufferPtr pxbuf = tex->getBuffer();
		LockedRect lkRect = pxbuf->lockRect(URect(0,0, pxbuf->getWidth(), pxbuf->getHeight()), HBL_Normal);
		uint32 surfaceSize = DPixelFormatTool::getSurfaceSize(pxbuf->getWidth(), pxbuf->getHeight(), pxbuf->getDepth(), PF_A8);
		memset(lkRect.dataPtr, 0, sizeof(uint8));
		pxbuf->unlock();
		mGlyphTextures.push_back(res);
	}

	DFont::GlyphInfo DFont::getGlyphInfo( uint32 codePoint )
	{
		if (mLoadState.get() != LS_Loaded)
		{
			return GlyphInfo();
		}
		CodePointMap::iterator i = mCodePointMap.find(codePoint);
		if (i == mCodePointMap.end())
		{
			FT_Bitmap tmpBitmap;
			FT_Bitmap_New(&tmpBitmap);

			uint32 locX = 0;
			uint32 locY = 0;
			uint32 texId;
			FT_Load_Glyph( mFtFace, FT_Get_Char_Index(mFtFace, codePoint), FT_LOAD_RENDER );
			GlyphInfo info;
			info.codePoint = codePoint;
			/*
				字段face->glyph->format描述了字形槽中存储的字形图像的格式。
				如果它的值不是FT_GLYPH_FORMAT_BITMAP，你可以通过FT_Render_Glyph把它直接转换为一个位图。如下：
				error = FT_Render_Glyph( face->glyph, render_mode );  
			*/

			info.hAdvance = mFtFace->glyph->metrics.horiAdvance >> 6;	// FT_Fixed to uint32
			info.hBearingX = mFtFace->glyph->metrics.horiBearingX >> 6;		// FT_Fixed to uint32
			info.hBearingY = mFtFace->glyph->metrics.horiBearingY >> 6;  	// FT_Fixed to uint32
			info.width = mFtFace->glyph->bitmap.width;
			info.height = mFtFace->glyph->bitmap.rows;
			texId = getNextGlyphSpace(info.width, info.height, locX, locY);
			// calculate uv.
			info.uvRect.xBegin = (DReal)locX / (DReal)mTexureSize;
			info.uvRect.yBegin = (DReal)locY / (DReal)mTexureSize;
			info.uvRect.xExtend = (DReal)info.width / (DReal)mTexureSize;
			info.uvRect.yExtend = (DReal)info.height / (DReal)mTexureSize;
			
			// 写入纹理:
			if (mFtFace->glyph->bitmap.buffer != NULL)
			{
				uint8* glyphBuffer = NULL;
				FT_Bitmap* finalBitMap = &mFtFace->glyph->bitmap;
				switch (mFtFace->glyph->bitmap.pixel_mode)
				{
				case FT_PIXEL_MODE_GRAY:
					glyphBuffer = mFtFace->glyph->bitmap.buffer;
					break;

				case FT_PIXEL_MODE_MONO:
					// Convert the monochrome bitmap to 8-bit before rendering it.
					if (FT_Bitmap_Convert(mFtLib, &mFtFace->glyph->bitmap, &tmpBitmap, 1) == 0)
					{
						// Go through the bitmap and convert all of the nonzero values to 0xFF (white).
						for (uint8* p = tmpBitmap.buffer, *endP = p + tmpBitmap.width * tmpBitmap.rows; p != endP; ++p)
						{
							*p ^= -*p ^ *p;
						}
						glyphBuffer = tmpBitmap.buffer;
					}
					finalBitMap = &tmpBitmap;
					break;
				}
				if (glyphBuffer != NULL)
				{
					// lock the texture to write glyph.
					DTexture* tex = mGlyphTextures[texId]->getAs<DTexture>();
					DPixelBufferPtr pBuffer = tex->getBuffer();
					LockedRect rec = pBuffer->lockRect(URect(0,0, pBuffer->getWidth(), pBuffer->getHeight()), HBL_Normal);
					uint32 bitWidth = finalBitMap->width;
					uint32 bitHeight = finalBitMap->rows;
					uint32 dstStart = rec.rowPitch * locY + locX;
					for (uint32 y = 0; y < bitHeight; y++ )
					{
						uint32 offset = y * rec.rowPitch;
						uint32 glyBitmapOffset = y * bitWidth;
						for (uint32 x = 0; x < bitWidth; x++)
						{
							*((uint8*)(rec.dataPtr) + (dstStart + offset + x)) = glyphBuffer[glyBitmapOffset + x];
						}
					}
					pBuffer->unlock();
				}
			}
			mCodePointMap[codePoint] = info;
			FT_Bitmap_Done(mFtLib, &tmpBitmap);
			return info;
		}
		else
		{
			return i->second;
		}
		return GlyphInfo();
	}

}
