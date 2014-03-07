/*
	[7/15/2012 OMEGA] created
	Provide string class to enable more string operations.
*/

#ifndef _DUELSTRING_H_
#define _DUELSTRING_H_


#include <string>
#include "DuelConfig.h"
#include "DuelThread.h"
#include "DuelType.h"
#include "DuelMathLib.h"
#include "DuelRTTI.h"
#include "DuelObject.h"
#include <boost/lexical_cast.hpp>

// TODO: 筛选使用到的boost文件:
// 
/*

#include <boost/config.hpp>

#include <boost/limits.hpp>
#include <boost/mpl/if.hpp>

	#include <boost/mpl/aux_/value_wknd.hpp>
	#include <boost/mpl/aux_/static_cast.hpp>
		#include <boost/mpl/aux_/config/eti.hpp>
	#include <boost/mpl/aux_/na_spec.hpp>
	#include <boost/mpl/aux_/lambda_support.hpp>
	#include <boost/mpl/aux_/config/integral.hpp>
	#include <boost/mpl/aux_/config/ctps.hpp>
	#include <boost/mpl/aux_/config/workaround.hpp>
	// 把boost/mpl 里的东西都拿下

#include <boost/throw_exception.hpp>
#include <boost/type_traits/ice.hpp>
#include <boost/type_traits/is_pointer.hpp>
#include <boost/static_assert.hpp>
#include <boost/detail/lcast_precision.hpp>
#include <boost/detail/workaround.hpp>

*/
namespace Duel {
	// TODO: this section should be platform-independent.
#if defined(DUEL_COMPILER_MSVC)
	typedef	std::tr1::hash<std::string>	_StringHash;
#endif

	class DUEL_API DNameGenerator : public DObject
	{
	DUEL_DECLARE_RTTI(DNameGenerator)
	public:
		DNameGenerator(const DString& prefix) : mPrefix(prefix), mNext(1) {}
		DNameGenerator(const DNameGenerator& rhs) : mPrefix(rhs.mPrefix), mNext(rhs.mNext) {}
		virtual ~DNameGenerator() {}
		// generate a new name.
		DString	generate()
		{
			DUEL_LOCK_AUTO_MUTEX
			std::ostringstream s;
			s << mPrefix << mNext++;
			return s.str();
		}
		// reset the internal counter
		void	reset()
		{
			DUEL_LOCK_AUTO_MUTEX
			mNext = 1ULL;
		}
		// manually set the internal counter
		void	setNext(uint64 val)
		{
			DUEL_LOCK_AUTO_MUTEX
			mNext = val;
		}
		// get the counter.
		uint64	getNext() const
		{
			DUEL_LOCK_AUTO_MUTEX
			return mNext;
		}

	protected:
		DUEL_AUTO_MUTEX
		DString	mPrefix;
		uint64	mNext;
	};
	/*
		Referred Ogre.
	*/

	// provide useful functions to deal with DString, 
	class DUEL_API DStringTool
	{
	public:
		// set the global string environment, if locale is blank. the system environment will be used.
		static void			setLocale(const DString& locale);
		// use left and right flag to indicate how to trim, resulting in string without
		// spaces tabs or returns(/r).
		static void			trim( DString& str, bool left = true, bool right = false );

		// returns a DStringVector that contains all the substrings delimited by the given delimiter or the doubleDelimiter
		// which is used to include (normal) 
		// delimeters in the tokenized string. For example, "strings like this".
		static DStringVector tokenize( const DString& str, const DString& singleDelimiter = "\t\n ", 
			const DString& doubleDelimiter = "\"", uint32 maxSplits = 0);

		// split string with given delimiter, if maxSplits is 0, by default it will be 10. 
		static DStringVector split( const DString& str, const DString& delimiter, uint32 maxSplits = 10 );

		// act on all characters in the string
		static void		 toLowerCase( DString& str );
		static void		 toUpperCase( DString& str );

		// returns whether the string is started/ended with the given pattern.
		// use flag lowerCase to indicate whether the comparison takes place after
		// the str and pattern is both converted into lower case.
		static bool		 isStartWith( const DString& str, const DString& pattern, bool lowerCase = true );
		static bool		 isEndWith( const DString& str, const DString& pattern, bool lowerCase = true );

		// standardizing paths - use forward slashes only, end with slash.
		static DString	 standardizePath( const DString &init );

		// split fully qualified filename into base name and path, path must be standardised 
		// using standardisePath.
		static void		 splitFileName( const DString& qualifiedName, DString& outBasename, DString& outPath );

		// split fully qualified filename into base name and path and extention, path must be standardised 
		// using standardisePath.
		static void		 splitFileName( const DString& qualifiedName, DString& outBasename, DString& outExtention, DString& outFullName, DString& outPath );

		// compare two strings, can include simple '*' wildcards.
		static bool		 isMatch( const DString& str, const DString& pattern, bool caseSensitive = true );

		// replace all instances of a sub-string with another sub-string
		static const DString replaceAll( const DString& str, const DString& src, const DString& dest );

		// Constant blank string, useful for returning by ref where local does not exist
		static const DString BLANK;

		// convert function between string and naive data type.
		// fit for int8/uint8 int16/uint16 int32/uint32 int64/uint64 
		template<typename T>
		static DString	toString(T val, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0))
		{
			std::stringstream outS;
			outS.width(width);
			outS.fill(fill);
			if (flags)
			{
				outS.setf(flags);
			}
			outS << val;
			return outS.str();
		}
		static DString	toString(DReal	val, uint16 precision = 6, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		static DString	toString(double	val, uint16 precision = 6, uint16 width = 0, char fill = ' ', std::ios::fmtflags flags = std::ios::fmtflags(0));
		static DString	toString(bool	val);


		static DString	toString(const DDegree& d);
		static DString	toString(const DRadian& r);
		// output form: "x y"
		static DString	toString(const DVector2& v2, const DString& delim = " ");
		// output form: "x y z"
		static DString	toString(const DVector3& v3, const DString& delim = " ");
		// output form: "x y z w"
		static DString	toString(const DVector4& v4, const DString& delim = " ");
		// output form: "m00 m01 m02 m10 m11 m12 m20 m21 m22"
		static DString	toString(const DMatrix3& m3, const DString& delim = " ");
		// output form: "m00 m01 m02 m03 m10 m11 m12 m13 m20 m21 m22 m23 m30 m31 m32 m33"
		static DString	toString(const DMatrix4& m4, const DString& delim = " ");
		// output form: "w x y z"
		static DString	toString(const DQuaternion& q, const DString& delim = " ");
		// output form: "yaw pitch roll"
		static DString	toString(const DRotator& r, const DString& delim = " ");

		// return true if parse successed, result will be set to param out
		// fit for int8/uint8 int16/uint16 int32/uint32 int64/uint64 DReal/double
		template<typename T>
		static bool	parseString(const DString& val, T& out)
		{
			T temp;
			try
			{
				temp = boost::lexical_cast<T>(val);
			}
			catch (boost::bad_lexical_cast& e)
			{
				(void)e;
				return false;
			}
			out = temp;
			return true;
		}

		static bool	parseDegree(const DString& val, DDegree& out);
		static bool	parseRadian(const DString& val, DRadian& out);
		// input form: "x y"
		static bool	parseVector2(const DString& val, DVector2& out, const DString& delim = " ");
		// input form: "x y z"
		static bool	parseVector3(const DString& val, DVector3& out, const DString& delim = " ");
		// input form: "x y z w"
		static bool	parseVector4(const DString& val, DVector4& out, const DString& delim = " ");
		// input form: "m00 m01 m02 m10 m11 m12 m20 m21 m22"
		static bool	parseMatrix3(const DString& val, DMatrix3& out, const DString& delim = " ");
		// input form: "m00 m01 m02 m03 m10 m11 m12 m13 m20 m21 m22 m23 m30 m31 m32 m33"
		static bool	parseMatrix4(const DString& val, DMatrix4& out, const DString& delim = " ");
		// input form: "w x y z"
		static bool	parseQuaternion(const DString& val, DQuaternion& out, const DString& delim = " ");
		// input form: "yaw pitch roll"
		static bool	parseRotator(const DString& val, DRotator& out, const DString& delim = " ");


		// referd : KlayGE
		static DWString		convert( const DString& src);
		static DString		convert( const DWString& src);


	};
}


#endif