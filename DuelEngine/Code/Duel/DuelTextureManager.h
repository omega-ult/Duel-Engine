//  [10/10/2012 OMEGA] created

#ifndef _DUELTEXTUREMANAGER_H_
#define _DUELTEXTUREMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"
#include "DuelCodec.h"
#include "DuelHardwareBuffer.h"

namespace Duel
{

	class DUEL_API DTextureDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DTextureDescription)
	public:
		DTextureDescription(const DString& name, const DString& groupName);
		// refered image's file name, if it is specified, the codec type should be
		// provided too. 
		CodecFormat				codecFormat;
		HardwareBufferUsage		memoryStrategy;
	};

	// this class is designed for sub-class, it will act differently in different render system
	// such as DX, OpenGL, so it will not implement creatImpl()
	class DUEL_API DTextureManager : public DResourceManager
	{
	DUEL_DECLARE_RTTI(DTextureManager)
	public:
		DTextureManager();
		~DTextureManager();

		// 这里使用xml文件记录纹理文件信息,
		// 在这个项目里就写:
		/*
			<TextureResourceParameter>
				<Name>T_debug_default.dds</Name>
				<Group>_BasicMediaPack</Group>
				<Empty>false</Empty>
				<CodecFormat>dds</CodecFormat>
				<MemoryStrategy>DYNAMIC</MemoryStrategy>
			</TextureResourceParameter>
			<TextureResourceParameter>
				///
			</TextureResourceParameter>
		*/
		// 如需知道详细释义请大声呼叫.

		ResourceDescriptionList	parseParameter(DDataStream* data);

	protected:
		// leave to sub-class
		// Resource* createImpl(...);
	};
}

#endif