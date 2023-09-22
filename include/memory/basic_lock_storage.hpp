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
#ifndef __SQUADS_BASIC_LOCK_STORAGE_H__
#define __SQUADS_BASIC_LOCK_STORAGE_H__


#include "core/autolock.hpp"
#include "core/algorithm.hpp"

#include "basic_storage.hpp"

namespace squads {
    namespace memory {
        /**
		 * The basic allocater for all allocator impl in this library, with thread-safed version.
		 */
		template <class TMutex, class TAllocator, class TFilter = basic_allocator_filter>
		class basic_lock_storage {
		public:
			using allocator_category = typename TAllocator::allocator_category ;

			using value_type = void;
			using pointer = void*;
			using const_pointer = const void*;
			using difference_type = squads::ptrdiff_t;
			using size_type = size_t;

			using is_thread_safe = squads::true_type;

			using lock_type = TMutex;
			using allocator_impl = TAllocator;
			using filter_type = TFilter;
			using self_type = basic_lock_storage<TMutex, TAllocator, TFilter>;
			using lock_guard = squads::basic_autolock<lock_type>;


			basic_lock_storage() noexcept
				: m_lockObjct(), m_fFilter(), m_xTicksToWait(SQUADS_PORTMAX_DELAY) { allocator_impl::first();  }

			basic_lock_storage(const lock_type& lckObject) noexcept
				: m_lockObjct(lckObject), m_fFilter(), m_xTicksToWait(SQUADS_PORTMAX_DELAY)  { allocator_impl::first();  }

			basic_lock_storage(const lock_type& lckObject, const filter_type& _fFilter ) noexcept
				: m_lockObjct(lckObject), m_fFilter(_fFilter), m_xTicksToWait(SQUADS_PORTMAX_DELAY)  { allocator_impl::first();  }

			/**
			 * @brief malloc() a buffer in a given TAllocator and cheak with the given TFilter
			 * is this okay to alloc
			 * @param size		Size of desired buffer.
			 * @param alignment
			 * @return Pointer to new memory, or NULL if allocation fails.
			 */
			pointer allocate(size_t size, size_t alignment) {
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				pointer _mem = nullptr;

				if(m_fFilter.on_pre_alloc(size)) {
					_mem = allocator_impl::allocate(size, alignment);
					m_fFilter.on_alloc(size);
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
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				if(m_fFilter.on_pre_dealloc(size)) {
					allocator_impl::deallocate(address, size, alignment);
					m_fFilter.on_dealloc(size);
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
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				size = size * count;
				if(m_fFilter.on_pre_dealloc(size)) {
					allocator_impl::deallocate(address, size, (alignment == 0) ? squads::alignment_for(size) : alignment);
					m_fFilter.on_dealloc(size);
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
			template <class Type>
			void destroy(Type* address) noexcept {
				if(address == nullptr) return;

				auto _size = sizeof(Type);

				squads::destruct<Type>(address);
				deallocate(address, _size, squads::alignment_for(_size));
			}

			/**
			 * @brief Get the maximal size to allocate.
			 * @return The maximal size to allocate.
			 */
			size_t get_max_alocator_size() const noexcept {
				lock_guard lock(m_lockObjct, m_xTicksToWait);

				return allocator_impl::get_max_alocator_size();
			}

			void lock(unsigned long xTicksToWait = 0) const {

				m_lockObjct.lock(xTicksToWait == 0  ? m_xTicksToWait : xTicksToWait);
			}

            void unlock() const noexcept {
				m_lockObjct.unlock();
			}

			void set_ticksToWait(unsigned long xTicksToWait) {
				m_lockObjct.lock(m_xTicksToWait);
					m_xTicksToWait = xTicksToWait;
				m_lockObjct.unlock();
			}
			bool is_locked() {
				return m_lockObjct.is_locked();
			}

			basic_lock_storage(const self_type& other) noexcept = delete;
			self_type& operator = (const basic_lock_storage& other) noexcept  = delete;
		private:
			lock_type	m_lockObjct;
			filter_type m_fFilter;
			unsigned long m_xTicksToWait;
    	};
	}
}


#endif