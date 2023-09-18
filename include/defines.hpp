/*
*This file is part of the SQUADS Library (https://github.com/eotpcomic/squads ).
*Copyright (c) 2023 Amber-Sophia Schroeck
*
*The SQUADS Library is free software; you can redistribute it and/or modify
*it under the terms of the GNU Lesser General Public License as published by
*the Free Software Foundation, version 2.1, or (at your option) any later version.

*The SQUADS Library is distributed in the hope that it will be useful, but
*WITHOUT ANY WARRANTY; without even the implied warranty of
*MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
*General Public License for more details.
*
*You should have received a copy of the GNU Lesser General Public
*License along with the SQUADS  Library; if not, see
*<https://www.gnu.org/licenses/>.
*/

#ifndef __SQUADS_DEFINES_H__
#define __SQUADS_DEFINES_H__


#include "config.hpp"

#include <stddef.h>
#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <time.h>

#include "core/copyable.hpp"
#include "core/void.hpp"



#define SQUADS_TEMPLATE_FULL_DECL()                                     template< >
#define SQUADS_TEMPLATE_FULL_DECL_ONE(d1,t1)		                    template <d1 t1>
#define SQUADS_TEMPLATE_FULL_DECL_TWO(d1,t1,d2,t2)	                    template <d1 t1, d2 t2>
#define SQUADS_TEMPLATE_FULL_DECL_THREE(d1,t1,d2,t2,d3,t3)	            template <d1 t1, d2 t2, d3 t3>
#define SQUADS_TEMPLATE_DECL_ONE(t1)		                            SQUADS_TEMPLATE_FULL_DECL_ONE(typename,t1)
#define SQUADS_TEMPLATE_DECL_TWO(t1,t2)		                            SQUADS_TEMPLATE_FULL_DECL_TWO(typename,t1,typename,t2)
#define SQUADS_TEMPLATE_DECL_THREE(t1,t2,t3)	                        SQUADS_TEMPLATE_FULL_DECL_THREE(typename,t1,typename,t2,typename,t3)
#define SQUADS_TEMPLATE_TYPE_ONE(type,a1)		                        type<a1>
#define SQUADS_TEMPLATE_TYPE_TWO(type,a1,a2)	                        type<a1,a2>
#define SQUADS_TEMPLATE_TYPE_THREE(type,a1,a2,a3)	                    type<a1,a2,a3>

#define SQUADS_TEMPLATE_USING_ONE(name, basic_name,d1,t1)               template <d1 t1> using name = basic_name<t1>;
#define SQUADS_TEMPLATE_USING_TWO(name,basic_name, d1,t1,d2,t2)         template <d1 t1, d2 t2> using name = basic_name<t1, t2>;
#define SQUADS_TEMPLATE_USING_THREE(name,basic_name,d1,t1,d2,t2,d3,t3)  template <d1 t1, d2 t2, d3 t3> using name = basic_name<t1, t2, t3>;
#define SQUADS_TEMPLATE_USING(name, basic_name)                         using name = basic_name;

#define SQUADS_UNUSED_VARIABLE(var)		do { (void)(var); } while (false)
#define SQUADS_ZERO_SET(var, size)	    memset(var, 0, size)
#define SQUADS_SAFE_DELETE(var)			if((var)) { delete var; var = NULL; }
#define SQUADS_SAFE_FREE(var)			if((var)) { free(var); var = NULL; }

#ifdef __cplusplus
#define SQUADS_EXTERNC_BEGINN			extern "C" {
#define SQUADS_EXTERNC_END				}
#else
#define SQUADS_EXTERNC_BEGINN
#define SQUADS_EXTERNC_END
#endif


#define SQUADS_DEPRECATED 				        __attribute__ ((deprecated))
#define __SQUADS_STATIC_CAST_BYTE(BYTE)			static_cast<unsigned char>(BYTE)

#define offsetof(TYPE, MEMBER) 					__builtin_offsetof (TYPE, MEMBER)

/// Returns the number of bits in the given type
#define BitsInType(t)	(sizeof(t) * CHAR_BIT)

/// Returns the mask of type \p t with the lowest \p n bits set.
#define BitMask(t,n)	(t(~t(0)) >> (BitsInType(t) - (n)))

namespace squads {
    using max_align_t 	= uintptr_t;
    using nullptr_t 	= decltype(nullptr);
    using addrof_null_t = nullptr_t ;

    using size_t 	 	= SQUADS_THREAD_CONFIG_SIZE_TYPE;
    using ptrdiff_t  	= long;
    using uptrdiff_t 	= unsigned long;
    using intmax_t 	 	= signed long;
    using uintmax_t 	= unsigned long;

    const size_t default_alignment = alignof(void*);
	constexpr size_t max_alignment = alignof(max_align_t);

	using ::clock_t;
	using ::time_t;
	using ::tm;

	using ::clock;
	using ::difftime;
	using ::mktime;
	using ::time;
	using ::asctime;
	using ::ctime;
	using ::gmtime;
	using ::localtime;
	using ::strftime;

	using ::gettimeofday;

	class null_type { };

	namespace math {
		constexpr double pi               = 3.14159265358979;
		constexpr double pi_reciprocal    = 0.31830988618379;
		constexpr double pi_squared       = 9.86960440108936;
		constexpr double e                = 2.71828182845905;
		constexpr double e_reciprocal     = 0.36787944117144;
		constexpr double e_squared        = 7.38905609893065;
		constexpr double root2            = 1.41421356237310;
		constexpr double root2_reciprocal = 0.70710678118655;
		constexpr double euler            = 0.57721566490153;
		constexpr double golden_ratio     = 1.61803398874989;
	}

    enum class byte : unsigned char { };

   // __CHAR_MAX__

    template<typename IntegerType>
    inline byte& operator <<= (byte& b, IntegerType shift) noexcept {
        return b = byte(__SQUADS_STATIC_CAST_BYTE(b) << shift);
    }

    template<typename IntegerType>
    inline byte& operator >>= (byte& b, IntegerType shift) noexcept {
        return b = byte(__SQUADS_STATIC_CAST_BYTE(b) >> shift);
    }

    template<typename IntegerType>
    inline constexpr byte operator << (byte& b, IntegerType shift) noexcept {
        return b = byte(__SQUADS_STATIC_CAST_BYTE(b) << shift);
    }

    template<typename IntegerType>
    inline constexpr byte operator >> (byte& b, IntegerType shift) noexcept {
        return b = byte(__SQUADS_STATIC_CAST_BYTE(b) >> shift);
    }

    inline constexpr bool operator==( const byte l, const byte r ) noexcept {
		return l == r;
	}

	inline constexpr bool operator!=( const byte l, const byte r ) noexcept {
		return !( l == r );
	}

	inline constexpr bool operator< ( const byte l, const byte r ) noexcept {
		return l < r;
	}

	inline constexpr bool operator<=( const byte l, const byte r ) noexcept {
		return !( r < l );
	}

	inline constexpr bool operator> ( const byte l, const byte r ) noexcept {
		return ( r < l );
	}

	inline constexpr bool operator>=( const byte l, const byte r ) noexcept {
		return !( l < r );
	}

    inline byte operator |= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(__SQUADS_STATIC_CAST_BYTE(lhr) | __SQUADS_STATIC_CAST_BYTE(rhr));
        return lhr;
    }

    inline byte operator &= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(__SQUADS_STATIC_CAST_BYTE(lhr) & __SQUADS_STATIC_CAST_BYTE(rhr));
        return lhr;
    }
    inline byte operator ^= (byte& lhr, const byte rhr) noexcept {
        lhr = byte(__SQUADS_STATIC_CAST_BYTE(lhr) ^ __SQUADS_STATIC_CAST_BYTE(rhr));
        return lhr;
    }

    inline constexpr byte operator | (const byte& lhr, const byte& rhr) noexcept {
        return byte(__SQUADS_STATIC_CAST_BYTE(lhr) | __SQUADS_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator ^ (const byte& lhr, const byte& rhr) noexcept {
        return byte(__SQUADS_STATIC_CAST_BYTE(lhr) ^ __SQUADS_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator & (const byte& lhr, const byte& rhr) noexcept {
        return byte(__SQUADS_STATIC_CAST_BYTE(lhr) & __SQUADS_STATIC_CAST_BYTE(rhr));
    }

    inline constexpr byte operator ~ (byte b) noexcept {
        return byte(~__SQUADS_STATIC_CAST_BYTE(b));
    }
}

#endif