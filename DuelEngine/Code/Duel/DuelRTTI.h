//  [9/1/2012 OMEGA] created
//  [8/10/2013 OMEGA] 增加多重继承的能力. 目前最多3重, 如有需要请自行扩增.

#ifndef _DUELRTTI_H_
#define _DUELRTTI_H_

#include "DuelType.h"
#include <vector>

namespace Duel
{

	// run-time type information, I'm sure you need it.
	// please using macro DUEL_DECLARE_RTTI in the class
	// and use DUEL_IMPLEMENT_RTTI in the .cpp file,
	// like this:
	// class A { DUEL_DECLARE_RTTI(A) }
	// in cpp:
	// DUEL_IMPLEMENT_RTTI_BASE(A);
	// for subclass subA, do this:
	// DUEL_IMPLEMENT_DUEL(A, subA),
	// then you can use macro DUEL_IS_KIND(A,objA) to get a 
	// type check.
	class DUEL_API RTTI
	{
	public:
		// for base class only.
		RTTI(const DString& name) :
			mName(name), mParentCount(0) {}
		// for most case, one or two parents are engough, we provide inheritance capacity
		// with three parent, of course you can make it more powerful with few codes.
		enum 
		{
			InheritanceCapacity	= 3
		};
		// 1-parent template:
		RTTI(const DString& name, const RTTI* fatherClass)
			: mName(name), mParentCount(1)
		{
			mParentClasses[0] = fatherClass;
			mParentName = fatherClass->getName();
		}
		// 2-parent template:
		RTTI(const DString& name, const RTTI* fatherClass1, const RTTI* fatherClass2)
			: mName(name), mParentCount(2)
		{
			mParentClasses[0] = fatherClass1;
			mParentClasses[1] = fatherClass2;
			mParentName = fatherClass1->getName() + fatherClass2->getName();
		}
		// 3-parent template:
		RTTI(const DString& name, const RTTI* fatherClass1, const RTTI* fatherClass2, const RTTI* fatherClass3)
			: mName(name), mParentCount(3)
		{
			mParentClasses[0] = fatherClass1;
			mParentClasses[1] = fatherClass2;
			mParentClasses[2] = fatherClass3;
			mParentName = fatherClass1->getName() + fatherClass2->getName() + fatherClass3->getName();
		}

		// get the class's name
		inline	const DString&	getName() const { return mName; }
		inline	const DString&	getParentName() const { return mParentName; }
		inline	size_t	getParentCount() const { return mParentCount ;}
		// get the pointer to the class's base class
		// no bounds checking, because we do not allow clients use this function directly.
		inline	const RTTI*		getParentRTTI(uint32 i)	const { return mParentClasses[i]; }

		static bool		isKind(const Duel::RTTI* dst, const Duel::RTTI* src);
		static bool		isExactKind(const Duel::RTTI* dst, const Duel::RTTI* src);

	protected:
		DString							mName;
		// if this rtti has more than 1 parent, its name will be "Parent1,Parent2,..."
		DString							mParentName;
		size_t							mParentCount;
		const RTTI*						mParentClasses[InheritanceCapacity];
	};

	// declare a class as a RTTI enabled.
#define	DUEL_DECLARE_RTTI(classname)												\
public:																				\
	static const Duel::RTTI		msRTTI_##classname;									\
	static const Duel::RTTI*	getClassRTTI() { return &msRTTI_##classname; }		\
	virtual const  Duel::RTTI*	getObjectRTTI() const { return &msRTTI_##classname; }\
private:																			\


	
// note: DUEL_IMPLEMENT_RTTI_BASE/DUEL_IMPLEMENT_DUEL !MUST! be written in .cpp file.
#define DUEL_IMPLEMENT_RTTI_BASE(classname)											\
	const Duel::RTTI classname::msRTTI_##classname(#classname);

#define DUEL_IMPLEMENT_RTTI_1(classname, fatherclassname)							\
	const Duel::RTTI classname::msRTTI_##classname(#classname, fatherclassname::getClassRTTI());

#define DUEL_IMPLEMENT_RTTI_2(classname, fatherclassname1, fatherclassname2 )		\
	const Duel::RTTI classname::msRTTI_##classname(#classname, fatherclassname1::getClassRTTI(), fatherclassname2::getClassRTTI());

#define DUEL_IMPLEMENT_RTTI_3(classname, fatherclassname1, fatherclassname2, fatherclassname3 )		\
	const Duel::RTTI classname::msRTTI_##classname(#classname, fatherclassname1::getClassRTTI(), fatherclassname2::getClassRTTI(), fatherclassname3::getClassRTTI());

#define DUEL_SAME_KIND(obja, objb)												\
	((obj)->getObjectRTTI() == (obj)->getObjectRTTI())										

#define DUEL_IS_EXACT_KIND(classname, obj)										\
	Duel::RTTI::isExactKind(classname::getClassRTTI(), (obj)->getObjectRTTI())

#define DUEL_IS_KIND(classname, obj)											\
	Duel::RTTI::isKind(classname::getClassRTTI(), (obj)->getObjectRTTI())

#define DUEL_CLASS_NAME(obj)													\
	((obj)->getObjectRTTI()->getName())

#define DUEL_PARENT_CLASS_NAME(obj)												\
	((obj)->getObjectRTTI()->getParentName())
}

#endif