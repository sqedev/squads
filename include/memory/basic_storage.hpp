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
#ifndef __SQUADS_BASIC_STORAGE_H__
#define __SQUADS_BASIC_STORAGE_H__

#include "config.hpp"
#include "core/type_traits.hpp"
#include "core/functional.hpp"
#include "core/alignment.hpp"
#include "core/utils.hpp"

#include "basic_allocator_sized_filter.hpp"

namespace squads {
    namespace memory {

		/**
		 * A a basic filter for a allocater
		 */
		class basic_allocator_filter {
		public:
			bool on_pre_alloc(size_t size, size_t alignment) { return true; }
			void on_alloc(size_t size, size_t alignment) { }

			bool on_pre_dealloc(size_t size, size_t alignment) { return true; }
			void on_dealloc(size_t size, size_t alignment) { }
		};

        /**
		 * The basic allocater for all allocator impl in this library.
		 */
		template <class TAllocator, class TFilter = basic_allocator_filter>
		class basic_storage {
		public:
			using allocator_category = typename TAllocator::allocator_category ;
			using is_thread_safe = typename TAllocator::is_thread_safe ;
			using filter_type = TFilter;

			using value_type = void;
			using pointer = void*;
			using const_pointer = const void*;
			using difference_type = squads::ptrdiff_t;
			using size_type = size_t;

			basic_storage() noexcept  { TAllocator::first();  }

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size, size_t alignment) {
				pointer _mem = nullptr;

				if(m_fFilter.on_pre_alloc(size, alignment )) {
					_mem = TAllocator::allocate(size, alignment);
					m_fFilter.on_alloc(size, alignment);
				}
				return _mem;
			}

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size) {
				return allocate(size, squads::alignment_for(size));
			}

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size The size of the Type
			 * @param count The count of the array
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t count, size_t size, size_t alignment = 0) {
				return allocate(count * size, (alignment == 0) ? squads::alignment_for(size) : alignment);
			}


			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free.
			 * @param alignment
			 * @param size The size of the Type
			 */
			void deallocate(pointer address, size_t size, size_t alignment) noexcept {
				if(m_fFilter.on_pre_dealloc(size, alignment)) {
					TAllocator::deallocate(address, size, alignment);
					m_fFilter.on_dealloc(size, alignment);
				}
			}

			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free.
			 * @param size The size of the Type
			 */
			void deallocate(pointer address, size_t size) noexcept {
				deallocate(address, size, squads::alignment_for(size));
			}

			/**
			 * @brief free() a buffer in a given heap.
			 * @param address The address to free
			 * @param count The count of the array
			 * @param size The size of the Type
			 * @param alignment
			 */
			void deallocate(pointer address, size_t count, size_t size, size_t alignment) noexcept {
				size = size * count;
				if(m_fFilter.on_pre_dealloc(size, alignment)) {
					TAllocator::deallocate(address, size, (alignment == 0) ? squads::alignment_for(size) : alignment);
					m_fFilter.on_dealloc(size, alignment);
				}
			}

			/**
			 * @brief Construct a object from allocated impl.
			 * @tparam Type The type of the object.
			 * @param Args The arguments for the constructer of the object.
			 */
			template <class Type, typename... Args>
			Type* construct(Args&&... args) {
				auto _size = sizeof(Type);

				void* _mem = allocate(_size, squads::alignment_for(_size) );

				return ::new (_mem) Type(squads::forward<Args>(args)...);
			}

			/**
			 * @brief Deconstruct a object (call deconstructor) and free the memory
			 * @tparam Type The type of the object.
			 * @param address The pointer of the object to be deconstruct.
			 */
			template <typename TT>
			void destroy(TT* address) noexcept {
				if(address == nullptr) return;

				auto _size = sizeof(TT);

				destruct<TT>(address);
				deallocate(address, _size, squads::alignment_for(_size));
			}

			/**
			 * @brief Get the maximal size to allocate.
			 * @return The maximal size to allocate.
			 */
			size_t get_max_alocator_size() const noexcept {
				return TAllocator::get_max_alocator_size();
			}

		private:
			filter_type m_fFilter;
		};

    }
}

#endif