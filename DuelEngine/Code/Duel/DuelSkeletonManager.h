//  [12/30/2012 OMEGA] created

#ifndef _DUELSKELETONMANAGER_H_
#define _DUELSKELETONMANAGER_H_

#include "DuelCommon.h"
#include "DuelResourceManager.h"
namespace Duel
{
	class DUEL_API DSkeletonDescription : public DResourceDescription
	{
	DUEL_DECLARE_RTTI(DSkeletonDescription)
	public:
		DSkeletonDescription(const DString& name, const DString& groupName);

		// format that used to identify the codec for loading. 
		// now only "ds" supported.
		DString	fileFormat;
	};

#define DEFAULT_SKELETON_CODEC	DSCodec_v1_0

	class DUEL_API DSkeletonManager : public DResourceManager, public DSingleton<DSkeletonManager>
	{
	DUEL_DECLARE_RTTI(DSkeletonManager)
	public:
		DSkeletonManager();
		// override:DResourceManager--------------------------------
		// data content should be an XML, format like:
		// <SkeletonResourceParameter>
		//		<Name>xxxx</Name>
		//		<Group>xxxx</Group>
		//		<FileFormat>xxxx</FileFormat> 
		// </SkeletonResourceParameter>
		// <SkeletonResourceParameter>
		//		............
		//		............
		// </SkeletonResourceParameter>
		ResourceDescriptionList parseParameter(DDataStream* data);
	protected:
		// override : DResourceManager------------------------
		DResource*				createImpl(DResourceDescription* createParam);
		DResourceCodec*			mCommonCodec;
	};
}


#endif