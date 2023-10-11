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
#ifndef __SQUADS_ARCH_MUTEX_IMPL_H__
#define __SQUADS_ARCH_MUTEX_IMPL_H__

#include "config.hpp"
#include "defines.hpp"




SQUADS_EXTERNC_BEGINN

namespace squads {
    namespace arch {


        enum class mutex_type {
            simple = 0,
            recursive = 1
        };
       void* arch_create_mutex(mutex_type type);
       void arch_delete_mutex(void* m, mutex_type type);

       int arch_take_mutex(void* m, unsigned int xt, mutex_type type);
       int arch_give_mutex(void* m, mutex_type type);

    }
}

SQUADS_EXTERNC_END

#endif