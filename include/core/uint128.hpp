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
*<https://www.gnu.org/licenses/>.*/

#ifndef __SQUADS_UINT128_H__
#define __SQUADS_UINT128_H__

#include "config.hpp"

#include <stddef.h>
#include <stdint.h>



namespace squads {
    struct basic_uint128_t {
        union {
            struct {
                uint64_t high;
                uint64_t low;
            };
            uint64_t value[2];
        };
    };

    using uint128_t       = basic_uint128_t;
    using uint_least128_t = basic_uint128_t;
}

#endif
