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
#ifndef __SQUADS_BASIC_STACK_ALLOCATOR_H__
#define __SQUADS_BASIC_STACK_ALLOCATOR_H__

#include "basic_storage.hpp"
#include "basic_lock_storage.hpp"
#include "allocator_typetraits.hpp"


#include <cstdlib>
#include <memory>

namespace squads {
    namespace memory {
        
        /**
         * @brief Stack based allocator.
         * @note - operates on buffer of TBUFFERSIZE bytes of stack memory
         * @note - never frees memory
         * @note - cannot be copied
         *
         * @author RoseLeBlood
         * @date 2021.02.21
         * @version 1.0
         */
        template <int TBUFFERSIZE>
		class basic_allocator_stack_impl {
		public:
			using allocator_category = std_allocator_tag();
			using is_thread_safe = ::squads::false_type  ;

			static void first() noexcept { }

			static void* allocate(size_t size, size_t alignment) noexcept {
				
				if(m_bufferTop < TBUFFERSIZE) {
					if( (m_bufferTop+size) <= TBUFFERSIZE) {
						char* ret = &m_aBuffer[0] + m_bufferTop;
						m_bufferTop += size;

						return (void*)ret;
					}
				}
				return nullptr;
			}

			static void deallocate(void* ptr, size_t size, size_t alignment) noexcept {

			}

			static size_t max_node_size()  {
				return size_t(-1);
			}
			static size_t get_max_alocator_size()  {
				return __SIZE_MAX__;
			}
		private:
           	static size_t          m_bufferTop;
            static char*           m_aBuffer[TBUFFERSIZE];
		};

		template <int TBUFFERSIZE>
		size_t basic_allocator_stack_impl<TBUFFERSIZE>::m_bufferTop = 0;
		template <int TBUFFERSIZE>
		char* basic_allocator_stack_impl<TBUFFERSIZE>::m_aBuffer[TBUFFERSIZE];

		template <int TBUFFERSIZE, class TFilter = basic_allocator_filter>
		using stack_allocator = basic_storage<basic_allocator_stack_impl<TBUFFERSIZE>, TFilter>;

		template <int TBUFFERSIZE, class TMutex, class TFilter = basic_allocator_filter>
		using stack_allocator_safe = basic_lock_storage<TMutex, basic_allocator_stack_impl<TBUFFERSIZE>, TFilter>;
    } 
} 



#endif