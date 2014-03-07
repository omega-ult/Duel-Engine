//  [12/12/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelParticleSystem.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DParticleSystem, DMovable);


	DParticleSystem::DParticleSystem( const DString& name ) :
		DMovable(name)
	{

	}

	DString DParticleSystem::getTypeName() const
	{
		return "ParticleSystem";
	}

}