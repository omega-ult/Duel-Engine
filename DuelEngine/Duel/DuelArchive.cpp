//  [9/16/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelArchive.h"

namespace Duel
{
	DUEL_IMPLEMENT_RTTI_1(DArchive, DObject);

	DArchive::DArchive( const DString& name, const DString& type ) :
		mName(name),
		mType(type),
		mbReadOnly(true)
	{

	}

	void DArchive::remove( const DString& fileName )
	{
		DUEL_EXCEPT(DException::ET_NotImplemented,
			"Base class do not support remove method",
			"Base class do not support remove method, make sure you are using subclass",
			"Duel::DArchive::remove")
	}

	Duel::DDataStreamPtr DArchive::create( const DString fileName )
	{
		DUEL_EXCEPT(DException::ET_NotImplemented,
			"Base class do not support create method",
			"Base class do not support create method, make sure you are using subclass",
			"Duel::DArchive::create")
	}

	bool DArchive::isReadOnly()
	{
		return mbReadOnly;
	}

	const DString& DArchive::getType() const
	{
		return mType;
	}

	const DString& DArchive::getName( void ) const
	{
		return mName;
	}

	DArchive::~DArchive()
	{

	}

}