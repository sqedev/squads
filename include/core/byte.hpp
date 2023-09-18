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

#ifndef __SQUADS_BYTE_H__
#define __SQUADS_BYTE_H__

#include "functional.hpp"

namespace squads {
    template<typename IntegerType, class = typename squads::enable_if<squads::is_integral<IntegerType>::value>::type>
    inline IntegerType to_integer(byte b) {
        return IntegerType(b);
    }

	template < class IntegerType, class = typename squads::enable_if<squads::is_integral<IntegerType>::value>::type>
	inline constexpr byte to_byte( IntegerType v ) noexcept {
    	return static_cast<byte>( v );
	}

	inline constexpr unsigned char to_uchar( byte b ) noexcept {
    	return to_integer<unsigned char>( b );
	}

	inline constexpr unsigned char to_uchar( int i ) noexcept {
    	return static_cast<unsigned char>( i );
	}
}


#endif