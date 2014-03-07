//  [8/7/2012 OMEGA] created

#ifndef _DUELCOLOR_H_
#define _DUELCOLOR_H_

#include "DuelCommon.h"

namespace Duel
{
	// each color element has a value range from 0 to 255
	typedef uint32 RGBA;
	typedef uint32 ARGB;
	typedef uint32 ABGR;
	typedef uint32 BGRA;

	class DUEL_API DColor
	{
	public:
		DColor( DReal red = 1.0f, DReal green = 1.0f, DReal blue = 1.0f, DReal alpha = 1.0f );
		virtual ~DColor();

		DColor&	operator =	(const DColor& c);
		DColor	operator +	(const DColor& c) const;
		DColor&	operator +=	(const DColor& c);
		DColor	operator -	(const DColor& c) const;
		DColor&	operator -=	(const DColor& c);
		DColor	operator *	(const DColor& c) const;
		DColor	operator *	(const DReal scalar) const;
		DColor&	operator *=	(const DReal scalar);
		DColor	operator /	(const DColor& c) const;
		DColor	operator /	(const DReal scalar) const;
		DColor&	operator /=	(const DReal scalar);
		friend	DColor	operator *	(const DReal scalar, const DColor& c);

		bool	operator == (const DColor& c) const;
		bool	operator != (const DColor& c) const;

		/** Retrieves colour as RGBA.
	    */
		RGBA	getRGBA(void) const;

		/** Retrieves colour as ARGB.
	    */
		ARGB	getARGB(void) const;

		/** Retrieves colour as BGRA.
		*/
		BGRA	getBGRA(void) const;

		/** Retrieves colours as ABGR */
		ABGR	getABGR(void) const;

		/** Sets colour as RGBA.
	    */
        void	setRGBA(const RGBA val);

	    /** Sets colour as ARGB.
	    */
        void	setARGB(const ARGB val);

		/** Sets colour as BGRA.
		*/
		void	setBGRA(const BGRA val);

	    /** Sets colour as ABGR.
	    */
        void	setABGR(const ABGR val);

		/** Clamps colour value to the range [0, 1].
        */
		const DColor&	clamp();

		/** Clamps colour value to the range [-1, 1].
        */
		const DColor&	clampSigned();

		/// Array accessor operator
		DReal operator [] ( const size_t i ) const
		{
			assert( i < 4 );

			return *(&r+i);
		}

		/// Array accessor operator
		DReal& operator [] ( const size_t i )
		{
			assert( i < 4 );

			return *(&r+i);
		}

		/// Pointer accessor for direct copying
		DReal* ptr()
		{
			return &r;
		}
		/// Pointer accessor for direct copying
		const DReal* ptr() const
		{
			return &r;
		}

		/** Set a colour value from Hue, Saturation and Brightness.
		@param hue Hue value, scaled to the [0,1] range as opposed to the 0-360
		@param saturation Saturation level, [0,1]
		@param brightness Brightness level, [0,1]
		*/
		void setHSB(DReal hue, DReal saturation, DReal brightness);

		/** Convert the current colour to Hue, Saturation and Brightness values. 
		@param hue Output hue value, scaled to the [0,1] range as opposed to the 0-360
		@param saturation Output saturation level, [0,1]
		@param brightness Output brightness level, [0,1]
		*/
		void getHSB(DReal* hue, DReal* saturation, DReal* brightness) const;


		static const DColor ZERO;
		static const DColor BLACK;
		static const DColor WHITE;
		static const DColor GRAY;
		static const DColor RED;
		static const DColor GREEN;
		static const DColor BLUE;

		static const DColor CYAN;
		static const DColor MAGENTA;
		static const DColor YELLOW;

		DReal r, g, b, a;
	};

}

#endif