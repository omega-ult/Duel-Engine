//  [12/12/2013 OMEGA] created

#ifndef _DUELPARTICLESYSTEM_H_
#define _DUELPARTICLESYSTEM_H_

#include "DuelCommon.h"
#include "DuelMovable.h"

namespace Duel
{
	// base manager.
	class DUEL_API DParticleSystem : public DMovable
	{
		DUEL_DECLARE_RTTI(DParticleSystem)
	public:
		DParticleSystem(const DString& name);

		virtual DString		getTypeName() const;

	};
	
}

#endif // !_DUELPARTICLESYSTEM_H_
