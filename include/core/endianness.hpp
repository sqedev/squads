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

#ifndef __SQUADS_ENDIAN_H__
#define __SQUADS_ENDIAN_H__

#include "config.hpp"
#include "functional.hpp"

namespace squads {

	namespace endian  {
		enum class order {
			little,
			big,
			native
		};
		enum class align {
			no,
			yes
		};
	}
	/**
	 * @brief Get the underlying endian architecture?
	 * @return If true then is the underlying architecture is big endian and false if not.
	 */
    struct system_endian {
    	/**
		 * @brief Get the underlying endian architecture?
		 * @return The underlying endian architecture.
		 */
    	operator endian::order () const noexcept  {
			short tester = 0x0001;
			return  (*(char*)&tester != 0) ? endian::order::little : endian::order::big;
    	}
    };
}



#endif 
