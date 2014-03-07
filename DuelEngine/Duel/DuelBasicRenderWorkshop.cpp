//  [6/23/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelBasicRenderWorkshop.h"
#include "DuelRenderWorkshop.h"

namespace Duel
{

	DUEL_IMPLEMENT_RTTI(DABasicRenderWorkshop, DRenderWorkshop);


	void DABasicRenderWorkshop::renderDeferStage()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void DABasicRenderWorkshop::renderForwardStage()
	{
		throw std::exception("The method or operation is not implemented.");
	}

	void DABasicRenderWorkshop::setOptions( const RenderOption& option )
	{
		throw std::exception("The method or operation is not implemented.");
	}

	DABasicRenderWorkshop::DABasicRenderWorkshop()
	{
		// 创建4个ds用的texture.
	}

	void DABasicRenderWorkshop::setRenderTarget( DRenderTarget* target )
	{
		// 这里只是把最后的渲染目标暂存起来. 真正渲染的时候根据是否渲染ds步骤确定.
	}



}