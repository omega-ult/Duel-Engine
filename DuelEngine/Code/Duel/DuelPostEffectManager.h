//  [3/1/2014 OMEGA] created

#ifndef _DUELPOSTEFFECTSYSTEM_H_
#define _DUELPOSTEFFECTSYSTEM_H_

#include "DuelCommon.h"

namespace Duel
{

	class DUEL_API DPostEffectManager : public DObject, public DSingleton<DPostEffectManager>
	{
		DUEL_DECLARE_RTTI(DPostEffectManager)
	public:
		DPostEffectManager();

		typedef	std::map<DString, DPostEffectPtr>	PostEffectMap;
		typedef MapIterator<PostEffectMap>			PostEffectIterator;
		PostEffectIterator	getPostEffectIterator() { return PostEffectIterator(mPostEffectMap); }
		// register a new material use unique name.
		void				registerPostEffect(DPostEffectPtr pe);
		// unregister a material, this method will not affect created associated material instance.
		void				unregisterPostEffect(const DString& name);
		// create a material instance, may return NULL if no specified material resource registered.
		DPostEffectInstancePtr	createPostEffectInstance(const DString& name);

	protected:
		PostEffectMap	mPostEffectMap;
	};

}

#endif