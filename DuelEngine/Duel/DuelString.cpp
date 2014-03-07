//  [7/15/2012 OMEGA] created

#include "DuelCommon.h"
#include "DuelString.h"
#include "DuelType.h"
#include "DuelMathLib.h"
#include "DuelException.h"


namespace Duel {

	DUEL_IMPLEMENT_RTTI_1(DNameGenerator, DObject);

	const DString DStringTool::BLANK = DString("BLANK");
	
	
	void DStringTool::setLocale( const DString& locale )
	{
		setlocale(LC_ALL, locale.c_str());
	}


	/////////////////////////////////////////////
	void DStringTool::trim( DString& str, bool left, bool right )
	{
			
		static const DString delimiter = " \t\r\n";
			
		if(right)
			str.erase(str.find_last_not_of(delimiter)+1);
		if(left)
			str.erase(0, str.find_first_not_of(delimiter));
	}
	/////////////////////////////////////////////
	DStringVector DStringTool::split( const DString& str, const DString& delimiter, uint32 maxSplits )
	{
		DStringVector ret;
		// Ogre: 10 is guessed capacity for most case
		ret.reserve(maxSplits ? maxSplits+1 : 10);

		uint32 numSplits = 0;

		// STL will do as:
		size_t start, pos;
		start = 0;
		do
		{
			pos = str.find_first_of(delimiter, start);
			if( pos == start )
			{
				// not find, then do nothing
				start = pos + 1;
			}
			else if ( pos == DString::npos || (maxSplits && numSplits == maxSplits))
			{
				// copy the rest of the string
				ret.push_back( str.substr(start) );
				break;
			}
			else
			{
				// copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos +1;
			}

			// move start to next section
			start = str.find_first_not_of( delimiter, start );
			++numSplits;
		} while ( pos != DString::npos );

		return ret;
	}
	/////////////////////////////////////////////
	DStringVector DStringTool::tokenize(  const DString& str, const DString& singleDelimiter, 
			const DString& doubleDelimiter, uint32 maxSplits)
	{
		DStringVector ret;
		// Ogre: 10 is guessed capacity for most case
		ret.reserve(maxSplits ? maxSplits+1 : 10);

		uint32 numSplits = 0;
		DString delimiter = singleDelimiter + doubleDelimiter;

		// STL will do as:
		size_t start, pos;
		char curDoubleDelimiter = 0;
		start = 0;
		do
		{
			if ( curDoubleDelimiter != 0 )
			{
				pos = str.find( curDoubleDelimiter, start );
			}
			else
			{
				pos = str.find_first_of( delimiter, start );
			}

			if ( pos == start )
			{
				char curDelimiter = str.at( pos );
				if ( doubleDelimiter.find_first_of(curDelimiter) !=  DString::npos )
				{
					curDoubleDelimiter = curDelimiter;
				}
				start = pos + 1;
			}
			else if ( pos == DString::npos || (maxSplits && numSplits == maxSplits) )
			{
				if ( curDoubleDelimiter != 0 )
				{
					DUEL_EXCEPT(Duel::DException::ET_InvalidParameters,
						"miss paired symbols",
						"one or more bracket(s) included in the input string is(are) not completed",
						"Duel::DStringTool::tokenize"
						);
					
				}

				ret.push_back( str.substr(start) );
				break;
			}
			else
            {
				if (curDoubleDelimiter != 0)
				{
					curDoubleDelimiter = 0;
				}

				// Copy up to delimiter
				ret.push_back( str.substr(start, pos - start) );
				start = pos + 1;
            }
			if (curDoubleDelimiter == 0)
			{
				// parse up to next real data
				start = str.find_first_not_of(singleDelimiter, start);
			}
            
            ++numSplits;

        } while (pos != DString::npos);

        return ret;
	}
	/////////////////////////////////////////////
	void DStringTool::toLowerCase(DString& str)
    {
		std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			tolower);
    }
	/////////////////////////////////////////////
	void DStringTool::toUpperCase(DString& str) 
    {
        std::transform(
            str.begin(),
            str.end(),
            str.begin(),
			toupper);
    }
	/////////////////////////////////////////////
	bool DStringTool::isStartWith(const DString& str, const DString& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        DString startOfThis = str.substr(0, patternLen);
        if (lowerCase)
            DStringTool::toLowerCase(startOfThis);

        return (startOfThis == pattern);
    }
	/////////////////////////////////////////////
	bool DStringTool::isEndWith(const DString& str, const DString& pattern, bool lowerCase)
    {
        size_t thisLen = str.length();
        size_t patternLen = pattern.length();
        if (thisLen < patternLen || patternLen == 0)
            return false;

        DString endOfThis = str.substr(thisLen - patternLen, patternLen);
        if (lowerCase)
            DStringTool::toLowerCase(endOfThis);

        return (endOfThis == pattern);
    }
	/////////////////////////////////////////////
	DString DStringTool::standardizePath(const DString& init)
    {
        DString path = init;

        std::replace( path.begin(), path.end(), '\\', '/' );
        if( path[path.length() - 1] != '/' )
            path += '/';

        return path;
    }
	/////////////////////////////////////////////
	void DStringTool::splitFileName(const DString& qualifiedName, 
        DString& outFullName, DString& outPath)
    {
        DString path = qualifiedName;
        // Replace \ with / first
        std::replace( path.begin(), path.end(), '\\', '/' );
        // split based on final /
        size_t i = path.find_last_of('/');

        if (i == DString::npos)
        {
            outPath.clear();
			outFullName = qualifiedName;
        }
        else
        {
            outFullName = path.substr(i+1, path.size() - i - 1);
            outPath = path.substr(0, i+1);
        }

    }
	/////////////////////////////////////////////
	void DStringTool::splitFileName(const DString& qualifiedName, 
		DString& outBasename, DString& outExtention, DString& outFullName,  DString& outPath)
	{
		splitFileName(qualifiedName,outFullName,outPath);
		size_t i = outFullName.find_last_of(".");
		if (i == DString::npos)
		{
			outExtention.clear();
			outBasename = outFullName;
		}
		else
		{
			outExtention = outFullName.substr(i+1);
			outBasename = outFullName.substr(0, i);
		}
	}
	/////////////////////////////////////////////
	bool DStringTool::isMatch(const DString& str, const DString& pattern, bool caseSensitive)
    {
        DString tmpStr = str;
		DString tmpPattern = pattern;
        if (!caseSensitive)
        {
            toLowerCase(tmpStr);
            toLowerCase(tmpPattern);
        }

        DString::const_iterator strIt = tmpStr.begin();
        DString::const_iterator patIt = tmpPattern.begin();
		DString::const_iterator lastWildCardIt = tmpPattern.end();
        while (strIt != tmpStr.end() && patIt != tmpPattern.end())
        {
            if (*patIt == '*')
            {
				lastWildCardIt = patIt;
                // Skip over looking for next character
                ++patIt;
                if (patIt == tmpPattern.end())
				{
					// Skip right to the end since * matches the entire rest of the string
					strIt = tmpStr.end();
				}
				else
                {
					// scan until we find next pattern character
                    while(strIt != tmpStr.end() && *strIt != *patIt)
                        ++strIt;
                }
            }
            else
            {
                if (*patIt != *strIt)
                {
					if (lastWildCardIt != tmpPattern.end())
					{
						// The last wildcard can match this incorrect sequence
						// rewind pattern to wildcard and keep searching
						patIt = lastWildCardIt;
						lastWildCardIt = tmpPattern.end();
					}
					else
					{
						// no wildwards left
						return false;
					}
                }
                else
                {
                    ++patIt;
                    ++strIt;
                }
            }

        }
		// If we reached the end of both the pattern and the string, we succeeded
		if (patIt == tmpPattern.end() && strIt == tmpStr.end())
		{
        	return true;
		}
		else
		{
			return false;
		}

    }
	/////////////////////////////////////////////
	const DString DStringTool::replaceAll(const DString& str, const DString& src, const DString& dest)
	{
		DString result = str;
        DString::size_type pos = 0;
		while(1)
		{
			pos = result.find(src,pos);
			if (pos == DString::npos) break;
			result.replace(pos,src.size(),dest);
            pos += dest.size();
		}
		return result;
	}
	/////////////////////////////////////////////

	DString DStringTool::toString( DReal val, uint16 precision, uint16 width, char fill, std::ios::fmtflags flags )
	{
		std::stringstream outS;
		outS.precision(precision);
		outS.width(width);
		outS.fill(fill);
		if (flags)
		{
			outS.setf(flags);
		}
		outS << val;
		return outS.str();
	}

	DString DStringTool::toString( double val, uint16 precision, uint16 width, char fill, std::ios::fmtflags flags )
	{
		std::stringstream outS;
		outS.precision(precision);
		outS.width(width);
		outS.fill(fill);
		if (flags)
		{
			outS.setf(flags);
		}
		outS << val;
		return outS.str();
	}

	DString DStringTool::toString( bool val )
	{
		if (val == true)
		{
			return DString("true");
		}
		else
		{
			return DString("false");
		}
	}

	DString DStringTool::toString( const DDegree& d )
	{
		std::stringstream outS;
		outS << d.getDegree();
		return outS.str();
	}

	DString DStringTool::toString( const DRadian& r )
	{
		std::stringstream outS;
		outS << r.getRadian();
		return outS.str();
	}

	DString DStringTool::toString( const DVector2& v2, const DString& delim/* = " " */)
	{
		std::stringstream outS;
		outS << v2.x << delim << v2.y;
		return outS.str();
	}

	DString DStringTool::toString( const DVector3& v3, const DString& delim/* = " " */ )
	{
		std::stringstream outS;
		outS << v3.x << delim << v3.y << delim << v3.z;
		return outS.str();
	}

	DString DStringTool::toString( const DVector4& v4, const DString& delim/* = " " */ )
	{
		std::stringstream outS;
		outS << v4.x << delim << v4.y << delim << v4.z << delim << v4.w;
		return outS.str();
	}

	DString DStringTool::toString( const DMatrix3& m3, const DString& delim/* = " " */ )
	{
		std::stringstream outS;
		outS << m3[0][0] << delim
			 << m3[0][1] << delim
			 << m3[0][2] << delim
			 << m3[1][0] << delim
			 << m3[1][1] << delim
			 << m3[1][2] << delim
			 << m3[2][0] << delim
			 << m3[2][1] << delim
			 << m3[2][2];
		return outS.str();
	}

	DString DStringTool::toString( const DMatrix4& m4, const DString& delim/* = " " */  )
	{
		std::stringstream outS;
		outS << m4[0][0] << delim
			 << m4[0][1] << delim
			 << m4[0][2] << delim
			 << m4[0][3] << delim
			 << m4[1][0] << delim
			 << m4[1][1] << delim
			 << m4[1][2] << delim
			 << m4[1][3] << delim
			 << m4[2][0] << delim
			 << m4[2][1] << delim
			 << m4[2][2] << delim
			 << m4[2][3] << delim
			 << m4[3][0] << delim
			 << m4[3][1] << delim
			 << m4[3][2] << delim
			 << m4[3][3];
			return outS.str();
	}

	DString DStringTool::toString( const DQuaternion& q, const DString& delim/* = " " */ )
	{
		std::stringstream outS;
		outS << q.w << delim << q.x << delim << q.y << delim << q.z;
		return outS.str();
	}

	DString DStringTool::toString( const DRotator& r, const DString& delim/* = " " */ )
	{
		std::stringstream outS;
		outS << r.yaw.getRadian() << delim << r.pitch.getRadian() << delim << r.roll.getRadian();
		return outS.str();
	}

	bool DStringTool::parseDegree( const DString& val, DDegree& out )
	{
		DReal d;
		if (parseString(val, d) == true)
		{
			out = d;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseRadian( const DString& val, DRadian& out )
	{
		DReal r;
		if (parseString(val, r) == true)
		{
			out = r;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseVector2( const DString& val, DVector2& out, const DString& delim/* = " " */ )
	{
		// only split first two element.
		DStringVector vec = split(val, delim, 2);
		DReal x, y;
		if (vec.size() == 2 &&
			parseString(vec[0], x) && parseString(vec[1], y))
		{
			out.x = x;
			out.y = y;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseVector3( const DString& val, DVector3& out, const DString& delim/* = " " */ )
	{
		// only split first three element.
		DStringVector vec = split(val, delim, 3);
		DReal x, y, z;
		if (vec.size() == 3 &&
			parseString(vec[0], x) && parseString(vec[1], y) && parseString(vec[2], z))
		{
			out.x = x;
			out.y = y;
			out.z = z;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseVector4( const DString& val, DVector4& out, const DString& delim/* = " " */ )
	{
		// only split first four element.
		DStringVector vec = split(val, delim, 4);
		DReal x, y, z, w;
		if (vec.size() == 4 &&
			parseString(vec[0], x) && parseString(vec[1], y) && parseString(vec[2], z) && parseString(vec[3], w))
		{
			out.x = x;
			out.y = y;
			out.z = z;
			out.w = w;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseMatrix3( const DString& val, DMatrix3& out, const DString& delim/* = " " */ )
	{
		// only split first 9 element.
		DStringVector vec = split(val, delim, 9);
		DReal temp[9];
		if (vec.size() == 9 &&
			parseString(vec[0], temp[0]) && parseString(vec[1], temp[1]) && parseString(vec[2], temp[2]) && 
			parseString(vec[3], temp[3]) && parseString(vec[4], temp[4]) && parseString(vec[5], temp[5]) && 
			parseString(vec[6], temp[6]) && parseString(vec[7], temp[7]) && parseString(vec[8], temp[8]))
		{
			out[0][0] = temp[0];
			out[0][1] = temp[1];
			out[0][2] = temp[2];

			out[1][0] = temp[3];
			out[1][1] = temp[4];
			out[1][2] = temp[5];

			out[2][0] = temp[6];
			out[2][1] = temp[7];
			out[2][2] = temp[8];
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseMatrix4( const DString& val, DMatrix4& out, const DString& delim/* = " " */ )
	{
		// only split first 16 element.
		DStringVector vec = split(val, delim, 16);
		DReal temp[16];
		if (vec.size() == 16 &&
			parseString(vec[0], temp[0]) && parseString(vec[1], temp[1]) && parseString(vec[2], temp[2]) && parseString(vec[3], temp[3]) && 
			parseString(vec[4], temp[4]) && parseString(vec[5], temp[5]) && parseString(vec[6], temp[6]) && parseString(vec[7], temp[7]) && 
			parseString(vec[8], temp[8]) && parseString(vec[9], temp[9]) && parseString(vec[10], temp[10]) && parseString(vec[11], temp[11]) && 
			parseString(vec[12], temp[12]) && parseString(vec[13], temp[13]) && parseString(vec[14], temp[14]) && parseString(vec[15], temp[15]))
		{
			out[0][0] = temp[0];
			out[0][1] = temp[1];
			out[0][2] = temp[2];
			out[0][3] = temp[3];

			out[1][0] = temp[4];
			out[1][1] = temp[5];
			out[1][2] = temp[6];
			out[1][3] = temp[7];

			out[2][0] = temp[8];
			out[2][1] = temp[9];
			out[2][2] = temp[10];
			out[2][3] = temp[11];

			out[3][0] = temp[12];
			out[3][1] = temp[13];
			out[3][2] = temp[14];
			out[3][3] = temp[15];
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseQuaternion( const DString& val, DQuaternion& out, const DString& delim/* = " " */ )
	{
		// only split first four element.
		DStringVector vec = split(val, delim, 4);
		DReal w, x, y, z;
		if (vec.size() == 4 &&
			 parseString(vec[0], w) && parseString(vec[1], x) && parseString(vec[2], y) && parseString(vec[3], z))
		{
			out.w = w;
			out.x = x;
			out.y = y;
			out.z = z;
			return true;
		}
		else
		{
			return false;
		}
	}

	bool DStringTool::parseRotator( const DString& val, DRotator& out, const DString& delim/* = " " */ )
	{
		// only split first three element.
		DStringVector vec = split(val, delim, 3);
		DReal yaw, pitch, roll;
		if (vec.size() == 4 &&
			parseString(vec[0], yaw) && parseString(vec[1], pitch) && parseString(vec[2], roll))
		{
			out.yaw = yaw;
			out.pitch = pitch;
			out.roll = roll;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	// 把一个const wstring转化为string	 
	/////////////////////////////////////////////////////////////////////////////////
	DString DStringTool::convert( const DWString& src)
	{

		// 使用标准库.
		DString ret;
		size_t s = wcstombs(NULL, src.c_str(), 0);
		if(s == -1)
		{
			return ret;
		}
		std::vector<char> vec(s);
		wcstombs(&vec[0], src.c_str(), s);
		ret.assign(vec.begin(), vec.end());
		return ret;
		
		// windows version.
// 		int const mbs_len = WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0, NULL, NULL);
// 		std::vector<char> tmp(mbs_len + 1);
// 		WideCharToMultiByte(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], mbs_len, NULL, NULL);
// 
// 		DString ret;
// 		ret.assign(tmp.begin(), tmp.end() - 1);

//		return ret;
	}

	// 把一个const string转化为wstring
	/////////////////////////////////////////////////////////////////////////////////
	DWString DStringTool::convert( const DString& src)
	{
		// 使用标准库.
		DWString ret;
		size_t s = mbstowcs(NULL, src.c_str(), 0);
		if(s == -1)
		{
			return ret;
		}
		std::vector<wchar_t> wVec(s);
		mbstowcs(&wVec[0], src.c_str(), s);
		ret.assign(wVec.begin(), wVec.end());
		
		return ret;
		// windows version.
// 		int const wcs_len = MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), NULL, 0);
// 		std::vector<wchar_t> tmp(wcs_len + 1);
// 		MultiByteToWideChar(CP_ACP, 0, src.c_str(), static_cast<int>(src.size()), &tmp[0], wcs_len);
// 
// 		DWString ret;
// 		ret.assign(tmp.begin(), tmp.end() - 1);

//		return ret;
	}

}	