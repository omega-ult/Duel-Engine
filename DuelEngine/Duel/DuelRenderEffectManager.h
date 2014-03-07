//  [4/2/2013 OMEGA] 停止使用 DRECodec_v1_0, 将默认codec换成 DRECodec_v2_0
//  [12/2/2012 OMEGA] created

#ifndef _DUELRENDEREFFECTMANAGER_H_
#define _DUELRENDEREFFECTMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"

namespace Duel
{

	class DUEL_API DRenderEffectDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DRenderEffectDescription)
	public:
		// type name: RenderEffect
		DRenderEffectDescription(const DString& name, const DString& groupName);
		
		// format that used to identify the codec for loading. 
		// now only dre supported.
		DString	fileFormat;
		// dadada...
	};

#define DEFAULT_RENDEREFFECT_CODEC	DRECodec_v2_0

	// RenderEffect relies on GpuProgramManager.
	class DUEL_API DRenderEffectManager : public DResourceManager
	{
	DUEL_DECLARE_RTTI(DRenderEffectManager)
	public:
		DRenderEffectManager();
		virtual ~DRenderEffectManager();
		//RenderEffectManager()
		// TODO:
		// 增加default的设置(setDefaultXXX...), 将新建的effect中未显式声明的部分用这些值填充
		// TODO:解析脚本, 你懂的.
		// <RenderEffectResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat>
		// </RenderEffectResourceParameter>
		ResourceDescriptionList	parseParameter(DDataStream* data);


	protected:
		DResource*				createImpl(DResourceDescription* createParam);

		// default codec for RenderEffects.
		DResourceCodec*		mCommonCodec;

	};

}

#endif