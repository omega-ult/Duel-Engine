//  [8/7/2012 OMEGA] created


#include "DuelCommon.h"
#include "DuelColor.h"

namespace Duel
{
	const DColor DColor::ZERO	= DColor(0.0f, 0.0f, 0.0f, 0.0f);
	const DColor DColor::BLACK	= DColor(0.0f, 0.0f, 0.0f, 1.0f);
	const DColor DColor::WHITE	= DColor(1.0f, 1.0f, 1.0f, 1.0f);
	const DColor DColor::GRAY	= DColor(0.5f, 0.5f, 0.5f, 1.0f);
	const DColor DColor::RED		= DColor(1.0f, 0.0f, 0.0f, 1.0f);
	const DColor DColor::GREEN	= DColor(0.0f, 1.0f, 0.0f, 1.0f);
	const DColor DColor::BLUE	= DColor(0.0f, 0.0f, 1.0f, 1.0f);

	const DColor DColor::CYAN	= DColor(0.0f, 0.6274509803921569f, 0.9137254901960784f, 1.0f);
	const DColor DColor::MAGENTA	= DColor(0.8941176470588235f, 0.0f, 0.4980392156862745f, 1.0f);
	const DColor DColor::YELLOW	= DColor(1.0f, 0.9450980392156863f, 0.0f, 1.0f);

	DColor::DColor( DReal red /*= 1.0f*/, DReal green /*= 1.0f*/, DReal blue /*= 1.0f*/, DReal alpha /*= 1.0f */ ) :
		r(red), g(green), b(blue), a(alpha)
	{

	}


	DColor::~DColor()
	{

	}

	DColor& DColor::operator=( const DColor& c )
	{
		r = c.r;
		g = c.g;
		b = c.b;
		a = c.a;

		return *this;
	}

	DColor DColor::operator+( const DColor& c ) const
	{
		return DColor( r + c.r, g + c.g, b + c.b, a + c.a );
	}

	DColor& DColor::operator+=( const DColor& c )
	{
		r += c.r;
		g += c.g;
		b += c.b;
		a += c.a;

		return *this;
	}

	DColor DColor::operator-( const DColor& c ) const
	{
		return DColor( r - c.r, g - c.g, b - c.b, a - c.a );
	}

	DColor& DColor::operator-=( const DColor& c )
	{
		r -= c.r;
		g -= c.g;
		b -= c.b;
		a -= c.a;

		return *this;
	}

	DColor DColor::operator*( const DColor& c ) const
	{
		return DColor( r * c.r, g * c.g, b * c.b, a * c.a );
	}

	DColor DColor::operator*( const DReal scalar ) const
	{
		return DColor( r * scalar, g * scalar, b * scalar, a * scalar );
	}

	DColor& DColor::operator*=( const DReal scalar )
	{
		r *= scalar;
		g *= scalar;
		b *= scalar;
		a *= scalar;

		return *this;
	}

	DColor DColor::operator/( const DColor& c ) const
	{
		return DColor( r / c.r, g / c.g, b / c.b, a / c.a );
	}

	DColor DColor::operator/( const DReal scalar ) const
	{
		return DColor( r / scalar, g / scalar, b / scalar, a / scalar );
	}

	DColor& DColor::operator/=( const DReal scalar )
	{
		r /= scalar;
		g /= scalar;
		b /= scalar;
		a /= scalar;

		return *this;
	}


	DColor operator*( const DReal scalar, const DColor& c )
	{
		return DColor( c.r * scalar, c.g * scalar, c.b * scalar, c.a * scalar );
	}


	bool DColor::operator==( const DColor& c ) const
	{
		return (r == c.r && g == c.g && b == c.b && a == c.a);
	}

	bool DColor::operator!=( const DColor& c ) const
	{
		return !(operator ==(c));
	}
#if defined(DUEL_BIG_ENDIAN)
	ABGR Colro::getABGR( void ) const
#else
	RGBA DColor::getRGBA( void ) const
#endif
	{
		uint8		val8;
		uint32	val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		DColor	clamped(r,g,b,a);
		clamped.clamp();

		// R
		val8 = static_cast<uint8>(clamped.r * 255);
		val32 = val8 << 24;

		// G
		val8 = static_cast<uint8>(clamped.g * 255);
		val32 += val8 << 16;

		// B
		val8 = static_cast<uint8>(clamped.b * 255);
		val32 += val8 << 8;

		// A
		val8 = static_cast<uint8>(clamped.a * 255);
		val32 += val8;

		return val32;
	}
#if defined(DUEL_BIG_ENDIAN)
	BGRA DColor::getBGRA( void ) const
#else
	ARGB DColor::getARGB( void ) const
#endif
	{
		uint8		val8;
		uint32	val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		DColor	clamped(r,g,b,a);
		clamped.clamp();

		// A
		val8 = static_cast<uint8>(clamped.a * 255);
		val32 = val8 << 24;

		// R
		val8 = static_cast<uint8>(clamped.r * 255);
		val32 += val8 << 16;

		// G
		val8 = static_cast<uint8>(clamped.g * 255);
		val32 += val8 << 8;

		// B
		val8 = static_cast<uint8>(clamped.b * 255);
		val32 += val8;

		return val32;
	}
#if defined(DUEL_BIG_ENDIAN)
	ARGB DColor::getARGB( void ) const
#else
	BGRA DColor::getBGRA( void ) const
#endif
	
	{
		uint8		val8;
		uint32	val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		DColor	clamped(r,g,b,a);
		clamped.clamp();

		// B
		val8 = static_cast<uint8>(clamped.b * 255);
		val32 = val8 << 24;

		// G
		val8 = static_cast<uint8>(clamped.g * 255);
		val32 += val8 << 16;

		// R
		val8 = static_cast<uint8>(clamped.r * 255);
		val32 += val8 << 8;

		// A
		val8 = static_cast<uint8>(clamped.a * 255);
		val32 += val8;

		return val32;
	}
#if defined(DUEL_BIG_ENDIAN)
	RGBA DColor::getRGBA( void ) const
#else
	ABGR DColor::getABGR( void ) const
#endif
	
	{
		uint8		val8;
		uint32	val32 = 0;

		// Convert to 32bit pattern
		// (RGBA = 8888)

		DColor	clamped(r,g,b,a);
		clamped.clamp();

		// A
		val8 = static_cast<uint8>(clamped.a * 255);
		val32 = val8 << 24;

		// B
		val8 = static_cast<uint8>(clamped.b * 255);
		val32 += val8 << 16;

		// G
		val8 = static_cast<uint8>(clamped.g * 255);
		val32 += val8 << 8;

		// R
		val8 = static_cast<uint8>(clamped.r * 255);
		val32 += val8;

		return val32;
	}
#if defined(DUEL_BIG_ENDIAN)
	void DColor::setABGR( const ABGR val )
#else
	void DColor::setRGBA( const RGBA val )
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)
		
		// R
		r = ((val32 >> 24) & 0xff) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;

	}
#if defined(DUEL_BIG_ENDIAN)
	void DColor::setBGRA( const BGRA val )
#else
	void DColor::setARGB( const ARGB val )
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Blue
		b = (val32 & 0xFF) / 255.0f;
	}
#if defined(DUEL_BIG_ENDIAN)
	void DColor::setARGB( const ARGB val )
#else
	void DColor::setBGRA( const BGRA val )
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ARGB = 8888)

		// Blue
		b = ((val32 >> 24) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 16) & 0xFF) / 255.0f;

		// Red
		r = ((val32 >> 8) & 0xFF) / 255.0f;

		// Alpha
		a = (val32 & 0xFF) / 255.0f;
	}
#if defined(DUEL_BIG_ENDIAN)
	void DColor::setRGBA( const RGBA val )
#else
	void DColor::setABGR( const ABGR val )
#endif
	{
		uint32 val32 = val;

		// Convert from 32bit pattern
		// (ABGR = 8888)

		// Alpha
		a = ((val32 >> 24) & 0xFF) / 255.0f;

		// Blue
		b = ((val32 >> 16) & 0xFF) / 255.0f;

		// Green
		g = ((val32 >> 8) & 0xFF) / 255.0f;

		// Red
		r = (val32 & 0xFF) / 255.0f;
	}

	const DColor& DColor::clamp()
	{
		if(r < 0.0f)
			r = 0.0f;
		else if(r > 1.0f)
			r = 1.0f;
		
		if(g < 0.0f)
			g = 0.0f;
		else if(g > 1.0f)
			g = 1.0f;

		if(b < 0.0f)
			b = 0.0f;
		else if(b > 1.0f)
			b = 1.0f;
		
		if(a < 0.0f)
			a = 0.0f;
		else if(a > 1.0f)
			a = 1.0f;

		return *this;
	}

	const DColor& DColor::clampSigned()
	{
		if(r < -1.0f)
			r = -1.0f;
		else if(r > 1.0f)
			r = 1.0f;

		if(g < -1.0f)
			g = -1.0f;
		else if(g > 1.0f)
			g = 1.0f;

		if(b < -1.0f)
			b = -1.0f;
		else if(b > 1.0f)
			b = 1.0f;

		if(a < -1.0f)
			a = -1.0f;
		else if(a > 1.0f)
			a = 1.0f;

		return *this;
	}

	void DColor::setHSB( DReal hue, DReal saturation, DReal brightness )
	{
		// wrap hue
		if (hue > 1.0f)
		{
			hue -= (int)hue;
		}
		else if (hue < 0.0f)
		{
			hue += (int)hue + 1;
		}

		// clamp saturation / brightness
		
		saturation = saturation < 1.0f ? saturation : 1.0f;
		saturation = saturation > 0.0f ? saturation : 0.0f;
		brightness = brightness < 1.0f ? brightness : 1.0f;
		brightness = brightness > 0.0f ? brightness : 0.0f;

		if (brightness == 0.0f)
		{   
			// early exit, this has to be black
			r = g = b = 0.0f;
			return;
		}

		if (saturation == 0.0f)
		{   
			// early exit, this has to be grey

			r = g = b = brightness;
			return;
		}

		DReal hueDomain  = hue * 6.0f;
		if (hueDomain >= 6.0f)
		{
			// wrap around, and allow mathematical errors
			hueDomain = 0.0f;
		}
		uint16 domain = (uint16)hueDomain;
		DReal f1 = brightness * (1 - saturation);
		DReal f2 = brightness * (1 - saturation * (hueDomain - domain));
		DReal f3 = brightness * (1 - saturation * (1 - (hueDomain - domain)));

		switch (domain)
		{
		case 0:
			// red domain; green ascends
			r = brightness;
			g = f3;
			b = f1;
			break;
		case 1:
			// yellow domain; red descends
			r = f2;
			g = brightness;
			b = f1;
			break;
		case 2:
			// green domain; blue ascends
			r = f1;
			g = brightness;
			b = f3;
			break;
		case 3:
			// cyan domain; green descends
			r = f1;
			g = f2;
			b = brightness;
			break;
		case 4:
			// blue domain; red ascends
			r = f3;
			g = f1;
			b = brightness;
			break;
		case 5:
			// magenta domain; blue descends
			r = brightness;
			g = f1;
			b = f2;
			break;
		}
	}

	void DColor::getHSB( DReal* hue, DReal* saturation, DReal* brightness ) const
	{
		DReal vMin = r < g ? r : g;
			 vMin = vMin < b ? vMin : b;
		DReal vMax = r > g ? r : g;
			 vMax = vMax > b ? vMax : b;
		DReal delta = vMax - vMin;

		*brightness = vMax;

		if ( DMath::realEqual(delta, 0.0f))
		{
			// grey
			*hue = 0;
			*saturation = 0;
		}
		else
		{
			// a color
			*saturation = delta / vMax;

			DReal deltaR = (((vMax - r) / 6.0f) + (delta / 2.0f)) / delta;
			DReal deltaG = (((vMax - g) / 6.0f) + (delta / 2.0f)) / delta;
			DReal deltaB = (((vMax - b) / 6.0f) + (delta / 2.0f)) / delta;

			if (DMath::realEqual(r, vMax))
				*hue = deltaB - deltaG;
			else if (DMath::realEqual(g, vMax))
				*hue = 0.3333333f + deltaR - deltaB;
			else if (DMath::realEqual(b, vMax)) 
				*hue = 0.6666667f + deltaG - deltaR;

			if (*hue < 0.0f) 
				*hue += 1.0f;
			if (*hue > 1.0f)
				*hue -= 1.0f;
		}
	}

}