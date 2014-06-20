//  [7/6/2013 OMEGA] created

#include "DuelCommon.h"
#include "DuelObject.h"

// temporary variable's address returned, at RTTI
#pragma warning( disable : 4172 )

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_BASE(DObject);


	DObject::DObject()
	{

	}

	DObject::~DObject()
	{

	}

	const DString& DObject::getClassName() const
	{
		return DUEL_CLASS_NAME(this);
	}

	const DString& DObject::getParentClassName() const
	{
		return DUEL_PARENT_CLASS_NAME(this);
	}

}
#pragma warning( default : 4172 )