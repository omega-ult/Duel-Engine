//  [4/7/2013 OMEGA] created

#ifndef _DUELGLTEXTUREMANAGER_H_
#define _DUELGLTEXTUREMANAGER_H_

#include "Duel.h"
#include "DuelGLCommon.h"

namespace Duel
{

	class DUELGL_API GLTextureManager : public DTextureManager
	{
	DUEL_DECLARE_RTTI(GLTextureManager)
	public:
		virtual DResource* createImpl( DResourceDescription* createParam );
	protected:
	};

}

#endif