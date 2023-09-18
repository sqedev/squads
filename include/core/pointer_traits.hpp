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
#ifndef __SQUADS_POITER_TRAITS_H__
#define __SQUADS_POITER_TRAITS_H__

#include "config.hpp"
#include "defines.hpp"
#include "type_traits.hpp"
#include "addressof.hpp"

namespace squads {

        template <class TPR>
        struct pointer_traits {
            using value_type = typename TPR::value_type;
            using element_type = typename TPR::element_type;
            using reference = typename TPR::reference;
            using const_value_type = typename TPR::const_value_type;
            using pointer = typename TPR::pointer;

            static inline element_type* to_address(const TPR& _pPtr) noexcept {
        		return squads::addressof<element_type>(_pPtr.get()); }
        };

        template <class TPR>
        inline typename pointer_traits<TPR>::pointer get_pointer(const TPR& _pPtr) {
            return _pPtr.get(); }

		template <class TPR>
        inline bool is_null(const TPR& _pPtr) {
            return _pPtr.get() == nullptr; }



    }
}


#endif
