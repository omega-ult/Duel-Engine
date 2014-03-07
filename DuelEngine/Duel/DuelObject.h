//  [7/1/2013 OMEGA] created

#ifndef _DUELOBJECT_H_
#define _DUELOBJECT_H_

#include "DuelCommon.h"
#include "DuelException.h"

namespace Duel
{

	class DUEL_API DObject
	{
	DUEL_DECLARE_RTTI(DObject);
	public:
		DObject();
		virtual	~DObject();

		const DString&	getClassName() const;
		const DString&	getParentClassName() const;

		// a safe-cast for casting pointer to spcified class type, if it 
		// can not be cast, a NULL pointer will be returned.
		// param throwException : if true, throw exception when this function
		// fails. example obj->getAs<DObject>(true);
		template<typename T>
		T*		getAs(bool throwException = true) 
		{
			if (RTTI::isKind(T::getClassRTTI(), this->getObjectRTTI()))
			{
				T*	ret = static_cast<T*>(this);
				return ret;
			}
			if (throwException)
			{
				DUEL_EXCEPT(DException::ET_InvalidParameters,
					"static cast failed",
					"unable to execute static cast with type: "
					+ getClassName()
					+ "to"
					+ T::getClassRTTI()->getName(),
					"Duel::Object::getAs");
			}
			return NULL;
		}
		template <typename T>
		const T*		getAs(bool throwException = true) const 
		{
			if (RTTI::isKind(T::getClassRTTI(), this->getObjectRTTI()))
			{
				const T*	ret = static_cast<const T*>(this);
				return ret;
			}
			if (throwException)
			{
				DUEL_EXCEPT(DException::ET_InvalidParameters,
					"static cast failed",
					"unable to execute static cast with type: "
					+ getClassName()
					+ "to"
					+ T::getClassRTTI()->getName(),
					"Duel::Object::getAs");
			}
			return NULL;
		}

	};

}

#endif