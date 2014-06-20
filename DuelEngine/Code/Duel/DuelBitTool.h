//  [8/24/2012 OMEGA] created

#ifndef _DUELBITTOOL_H_
#define _DUELBITTOOL_H_

#include "DuelCommon.h"

namespace Duel
{
	class DUEL_API DBitTool 
	{
	public:
		// used for endian switch,
		static inline void	switchEndian(void* p, int size)
		{
			uint8*	head = static_cast<uint8*>(p);
			uint8*	tail = static_cast<uint8*>(p) + size - 1;
			uint8		temp;
			while(head < tail)
			{
				temp = *head;
				*head = *tail;
				*tail = temp;
				++head;
				--tail;
			}
		}

		// convert from native endian to little endian
		static inline void	nativeToLittle(void* p, int size)
		{
#if defined(DUEL_BIG_ENDIAN)
			switchEndian(p, size);
#endif
			// otherwise do nothing.
		}

		// convert from native endian to big endian
		static inline void	nativeToBig(void* p, int size)
		{
#if defined(DUEL_LITTLE_ENDIAN)
			switchEndian(p, size);
#endif
			// otherwise do nothing.
		}

		// convert from little endian to native endian
		static inline void	littleToNative(void* p, int size)
		{
#if defined(DUEL_BIG_ENDIAN)
			switchEndian(p, size);
#endif
			// otherwise do nothing.
		}

		// convert from big endian to native endian
		static inline void	bigToNative(void* p, int size)
		{
#if defined(DUEL_LITTLE_ENDIAN)
			switchEndian(p, size);
#endif
			// otherwise do nothing.
		}

		// from ogre-------------------------
		//  Read a n*8 bits integer value to memory in native endian.
		// bytes should be less or equal to 4
		static inline int	readInt(void* src, size_t bytes)
		{
			assert(bytes <= 4);
			switch(bytes)
			{
			case 1:
				return ((uint8*)src)[0];
			case 2:
				return ((uint16*)src)[0];
			case 3:
#if defined(DUEL_BIG_ENDIAN)
				return ((uint32)((uint8*)src)[0]<<16) |
					((uint32)((uint8*)src)[1]<<8) |
					((uint32)((uint8*)src)[2]);
#else
				return ((uint32)((uint8*)src)[0]) |
					((uint32)((uint8*)src)[1]<<8) |
					((uint32)((uint8*)src)[2]<<16);
#endif
			case 4:
				return ((uint32*)src)[0];
			}
			return 0;
		}

		/**
         * Write a n*8 bits integer value to memory in native endian.
         */
        static inline void writeInt(void *dest, const int bytes, const unsigned int value)
        {
            switch(bytes) {
                case 1:
                    ((uint8*)dest)[0] = (uint8)value;
                    break;
                case 2:
                    ((uint16*)dest)[0] = (uint16)value;
                    break;
                case 3:
#if defined(DUEL_BIG_ENDIAN)      
                    ((uint8*)dest)[0] = (uint8)((value >> 16) & 0xFF);
                    ((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
                    ((uint8*)dest)[2] = (uint8)(value & 0xFF);
#else
                    ((uint8*)dest)[2] = (uint8)((value >> 16) & 0xFF);
                    ((uint8*)dest)[1] = (uint8)((value >> 8) & 0xFF);
                    ((uint8*)dest)[0] = (uint8)(value & 0xFF);
#endif
                    break;
                case 4:
                    ((uint32*)dest)[0] = (uint32)value;                
                    break;                
            }        
        }

		/** Convert a float32 to a float16 (NV_half_float)
         	Courtesy of OpenEXR
        */
		static inline uint16 floatToHalf(float i)
		{
			union { float f; uint32 i; } v;
			v.f = i;
			return floatToHalfI(v.i);
		}

		/** Converts float in uint32 format to a a half in uint16 format
		*/
		static inline uint16 floatToHalfI(uint32 i)
		{
			register int s =  (i >> 16) & 0x00008000;
			register int e = ((i >> 23) & 0x000000ff) - (127 - 15);
			register int m =   i        & 0x007fffff;

			if (e <= 0)
			{
				if (e < -10)
				{
					return 0;
				}
				m = (m | 0x00800000) >> (1 - e);

				return static_cast<uint16>(s | (m >> 13));
			}
			else if (e == 0xff - (127 - 15))
			{
				if (m == 0) // Inf
				{
					return static_cast<uint16>(s | 0x7c00);
				} 
				else    // NAN
				{
					m >>= 13;
					return static_cast<uint16>(s | 0x7c00 | m | (m == 0));
				}
			}
			else
			{
				if (e > 30) // Overflow
				{
					return static_cast<uint16>(s | 0x7c00);
				}

				return static_cast<uint16>(s | (e << 10) | (m >> 13));
			}
		}
        
        /**
         * Convert a float16 (NV_half_float) to a float32
         * Courtesy of OpenEXR
         */
		static inline float	halfToFloat(uint16 y)
		{
			union { float f; uint32 i; } v;
			v.i = halfToFloatI(y);
			return v.f;
		}
		/** Converts a half in uint16 format to a float
		 	in uint32 format
		 */
		static inline uint32 halfToFloatI(uint16 y)
		{
			register int s = (y >> 15) & 0x00000001;
			register int e = (y >> 10) & 0x0000001f;
			register int m =  y        & 0x000003ff;

			if (e == 0)
			{
				if (m == 0) // Plus or minus zero
				{
					return s << 31;
				}
				else // Denormalized number -- renormalize it
				{
					while (!(m & 0x00000400))
					{
						m <<= 1;
						e -=  1;
					}

					e += 1;
					m &= ~0x00000400;
				}
			}
			else if (e == 31)
			{
				if (m == 0) // Inf
				{
					return (s << 31) | 0x7f800000;
				}
				else // NaN
				{
					return (s << 31) | 0x7f800000 | (m << 13);
				}
			}

			e = e + (127 - 15);
			m = m << 13;

			return (s << 31) | (e << 23) | m;
		}
		
		/**
         * Convert floating point colour channel value between 0.0 and 1.0 (otherwise clamped) 
		   works on 0 to 32 bits.
         */
        static inline uint32 floatToFixedClamped(const float value, const unsigned int bits)
		{
			if(value <= 0.0f) return 0;
			else if (value >= 1.0f) return (-1);
			else return (uint32)(value * (1ll<<bits));
		}
	};


#define NativeToLittle(X) \
	(DBitTool::nativeToLittle(X, sizeof(X)))

#define NativeToBig(X) \
	(DBitTool::nativeToBig(X, sizeof(X)))

#define LittleToNative(X) \
	(DBitTool::littleToNative(X, sizeof(X)))

#define BigToNative(X) \
	(DBitTool::bigToNative(X, sizeof(X)))
}


#endif
