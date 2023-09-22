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
#ifndef __SQUADS_DEFAULT_ALLOCATOR_H__
#define __SQUADS_DEFAULT_ALLOCATOR_H__

#include "memory/basic_malloc_allocator.hpp"
#include "memory/basic_stack_allocator.hpp"

namespace squads {

    template <class TFilter = memory::basic_allocator_filter>
	using default_allocator = memory::malloc_allocator<TFilter>;

}

#endif