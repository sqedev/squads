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
#ifndef __SQUADS_ALLOCTYPE_TRAITS_H__
#define __SQUADS_ALLOCTYPE_TRAITS_H__

#include "config.hpp"
#include "defines.hpp"

#include "core/type_traits.hpp"
#include "core/alignment.hpp"

namespace squads {
    namespace memory {
        struct std_allocator_tag { };
        struct nodeleter_allocator_tag { };


		/**
		 * @brief The default specialization of the allocator_traits for a allocator.
		 * Any specialization must provide the same interface.
         */
        template <class TAllocator>
        class allocator_traits {
		public:
			using allocator_type = TAllocator ;
            using value_type = typename allocator_type::value_type;
            using pointer = typename allocator_type::pointer;
            using const_pointer = typename allocator_type::const_pointer;
            using difference_type = typename allocator_type::difference_type;
            using size_type = typename allocator_type::size_type;


            using allocator_category = typename TAllocator::allocator_category ;
            using is_thread_safe = typename TAllocator::is_thread_safe ;

			static pointer allocate_node(allocator_type& state, size_type size, size_type alignment) {
				return state.allocate(size, alignment);
			}

			static pointer allocate_array(allocator_type& state, size_type count, size_type size,
                                          size_type alignment) {
				return state.allocate(count, size, alignment);
			}

			static void deallocate_node(allocator_type& state, pointer node, size_type size,
                                        size_type alignment) noexcept {
            	state.deallocate(node, size, alignment);
			}
			static void deallocate_array(allocator_type& state, pointer node, size_type count,
											size_type size, size_type alignment) noexcept {
            	state.deallocate(node, count, size, alignment);
			}

			static size_type max_node_size(const allocator_type& state) {
				return size_t(-1);
			}
			static size_type max_array_size(const allocator_type& state) {
				return size_t(-1);
			}
			static size_type max_alignment(const allocator_type& state) {
				return alignof(uintptr_t);
			}
        };


		template <class TAllocator>
		inline void* allocate(const TAllocator& alloc, size_t size, size_t alignment,
							  squads::memory::std_allocator_tag) {
			return alloc.allocate(size, alignment);
		}

		template <class TAllocator>
		inline void* allocate(const TAllocator& alloc, size_t size, size_t alignment,
							  squads::memory::nodeleter_allocator_tag) {
			return alloc.allocate(size, alignment);
		}
		template <class TAllocator>
		inline void* deallocate(const TAllocator& alloc, void* address, size_t size, size_t alignment,
							   squads::memory::std_allocator_tag)  {
			alloc.deallocate(address, size, alignment); return address;
		}

		template <class TAllocator>
		inline void* deallocate(const TAllocator& alloc, void* address, size_t size, size_t alignment,
							   squads::memory::nodeleter_allocator_tag)  {
			return address;
		}
    }
}


#endif // __SQUADS_ADDRESSOF_H__
