//  [7/16/2012 OMEGA] created


#include "DuelCommon.h"
#include "DuelViewport.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DViewport, DObject);
// 
// 	Viewport::Viewport( DCamera* cam, DRenderTarget* parent, DReal left, DReal top, DReal width, DReal height, int Zorder ) :
// 		mCamera(cam),
// 		mTarget(parent),
// 		mLeft(left),
// 		mTop(top),
// 		mWidth(width),
// 		mHeight(height),
// 		mZorder(Zorder)
// 	{
// 
// 	}
// 
// 	Viewport::Viewport( DRenderTarget* parent, DReal left, DReal top, DReal width, DReal height, int Zorder ) :
// 		mCamera(NULL),
// 		mTarget(parent),
// 		mLeft(left),
// 		mTop(top),
// 		mWidth(width),
// 		mHeight(height),
// 		mZorder(Zorder)
// 	{
// 
// 	}
// 
// 
// 
// 	Viewport::~Viewport()
// 	{
// 
// 	}
// 
// 	void Viewport::reset( DReal left, DReal top, DReal width, DReal height, int Zorder )
// 	{
// 		mLeft = left;
// 		mTop = top;
// 		mWidth = width;
// 		mHeight = height;
// 		mZorder = Zorder;
// 	}


	DViewport::DViewport() :
		mTop(0),
		mLeft(0),
		mHeight(1),
		mWidth(1)
	{

	}

	DViewport::DViewport( uint32 left, uint32 top, uint32 width, uint32 height ) :
		mTop(top),
		mLeft(left),
		mHeight(height),
		mWidth(width)
	{

	}

	void DViewport::reset( uint32 left, uint32 top, uint32 width, uint32 height )
	{
		mLeft = left;
		mTop = top;
		mWidth = width;
		mHeight = height;
	}

}