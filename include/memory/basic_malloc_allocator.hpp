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
#ifndef __SQUADS_BASIC_MALLOC_ALLOCATOR_H__
#define __SQUADS_BASIC_MALLOC_ALLOCATOR_H__

#include <cstdlib>
#include <memory>

#include "basic_storage.hpp"
#include "basic_lock_storage.hpp"
#include "allocator_typetraits.hpp"
#include "core/type_traits.hpp"


namespace squads {
    namespace memory {
        
        class basic_malloc_allocaor_impl {
		public:
			using allocator_category = squads::memory::std_allocator_tag();
			using is_thread_safe = false_type  ;

			static void first() noexcept { }

			static void* allocate(size_t size, size_t alignment) noexcept {
				return malloc(size);
			}

			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {
				free(ptr);
			}

			static size_t max_node_size()  {
				return size_t(-1);
			}
			static size_t get_max_alocator_size()  {
				return __SIZE_MAX__;
			}
		};

		template <class TFilter = basic_allocator_filter>
		using malloc_allocator = basic_storage<basic_malloc_allocaor_impl, TFilter>;

		template <class TMutex, class TFilter = basic_allocator_filter>
		using malloc_allocator_safe = basic_lock_storage<TMutex, basic_malloc_allocaor_impl, TFilter>;
    } 
} 



#endif