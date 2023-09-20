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
#ifndef __SQUADS_QUEUE_H__
#define __SQUADS_QUEUE_H__

#include "config.hpp"
#include "defines.hpp"
#include "initializer_list.hpp"
#include "functional.hpp"

#include "type_traits.hpp"
#include "iterator.hpp"

#include "arch/arch_queue_impl.hpp"

namespace squads {
    
    template<typename TQUEUE>
    struct queue_traits {
        using queue_type = TQUEUE;

        using value_type = typename queue_type::value_type;
        using reference = typename queue_type::reference;
        using const_reference = typename queue_type::const_reference;
        using pointer = typename queue_type::pointer;

        using difference_type = typename queue_type::difference_type;
        using size_type = typename queue_type::size_type;
        using allocator = typename queue_type::allocator;
        using deleter = typename queue_type::deleter;
        using iterator = typename queue_type::iterator;
        using const_iterator = typename queue_type::const_iterator;

        static const size_type TypeSize = queue_type::TypeSize;
    };
    /**
     * @brief Helper class for array iterators.
     */
    template <class T, class TQUEUE> 
    class basic_queue_iterator {
    public:
        using iterator_category = mofw::forward_iterator_tag;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;
        using difference_type = ptrdiff_t;
        
        using queue_type = TQUEUE;
        using self_type = basic_queue_iterator<T, TQUEUE>;
        using pointer_queue = TQUEUE*;

        explicit basic_queue_iterator(pointer_queue _pQueue, bool isEnd = false) 
            : m_pValue(NULL), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

        explicit basic_queue_iterator(pointer value, pointer_queue _pQueue, bool isEnd = false) 
            : m_pValue(value), m_pQueue(_pQueue), m_bIsEnd(isEnd) { }

        template<class U, class UQUEUE> 
        basic_queue_iterator(const basic_queue_iterator<U, UQUEUE>& rhs)
            : m_pValue(rhs.m_pValue), m_pQueue(rhs._pQueue), m_bIsEnd(rhs.isEnd) { }

        pointer get() const { return m_pValue; }

        reference operator*() const { return *m_pValue; }
        pointer operator->() const  { return m_pValue; }

        self_type& operator++()     { 
            if(!m_bIsEnd) { m_pQueue.dequeue(m_pValue); } return *this; }

        self_type operator++(int)   { 
            self_type copy(*this); ++(*this); return copy; }

        bool operator == (const self_type& rhs) const {
            return rhs.m_pValue == m_pValue; }

        bool operator != (const self_type& rhs) const {
            return !(rhs == *this); }
    private:
        pointer*      m_pValue;
        pointer_queue m_pQueue;
        bool          m_bIsEnd;
    };

    template <typename T, unsigned int maxItems = 32>
    class basic_queue {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_reference = const T&;
        using self_type = basic_queue<T, maxItems>;
        using difference_type = squads::ptrdiff_t;
        using size_type = squads::size_t;
        using allocator_type = TAllocator;
        using deleter_type = TDeleter;
        using iterator = basic_queue_iterator<T, self_type>;
        using const_iterator = const iterator;
        using cointainer_type = internal::arch_queue_impl;

        static const size_type TypeSize = sizeof(value_type);

                    
		explicit basic_queue() 
        : m_archQueueType(maxItems, sizeof(value_type) ) {
            m_archQueueType.create();
        }    

		basic_queue(const self_type& x ) 
            : m_archQueueType(other.m_archQueueType), 
              m_pEnd(other.m_pEnd), 
              m_pFront(other.m_pFront) { }
		basic_queue(self_type&& x ) 
            : m_archQueueType(squads::move(x.m_archQueueType))
              m_pEnd(squads::move(other.m_pEnd)), 
              m_pFront(squads::move(other.m_pFront)) { }

		basic_queue(initializer_list<value_type> ilist) {
            if(m_archQueueType.create() == 0) {

                for(typename squads::initializer_list<value_type>::iterator it = ilist.begin(); it != ilist.end(); ++it) {
                    const value_type& value = *it;
                    push(&value, SQUADS_PORTMAX_DELAY);
                }

            }
        }
        ~basic_queue()  { m_archQueueType.destroy(); }

        iterator        begin() 		{ return iterator( intern_getfront(), m_archQueueType); }
        const_iterator  begin() const 	{ return const_iterator( intern_getfront(), m_archQueueType); }

        iterator        end() 			{ return iterator(m_pEnd, &m_archQueueType, true); }
        const_iterator  end() const 	{ return const_iterator(m_pEnd, &m_archQueueType, true); }

		reference       front()         { assert(!empty()); return *intern_getfront(); }
		const_reference front() const   { assert(!empty()); return *intern_getfront(); }

		reference       back()          { assert(!empty()); return *m_pEnd; }
		const_reference back() const    { assert(!empty()); return *m_pEnd; }

        bool            empty() const   { return m_archQueueType.is_empty(); }
		size_type       size() const    { return m_archQueueType.get_num_items(); }

		void            push(const value_type& value, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
             
            if( m_archQueueType.enqueue_back(&value) == 0) {
                    m_pEnd = &value;
            }
        }
		void            push(value_type&& x, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
            if( m_archQueueType.enqueue_back(squads::move(x)) == 0) {
                    m_pEnd = &squads::move(x);
            }
        }

		void            pop(value_type* value = NULL, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
            m_archQueueType.dequeue(value, timeout);
        }
        /**
         * @brief Clear the queue
         */
        void            clear() {
            m_archQueueType.clear();
        }

        /**
         *  How many empty spaves are currently left in the queue.
         *  @return the number of remaining spaces.
         */
        size_type       left()  {
            return m_archQueueType.get_left();
        }


		void            swap(self_type& x) {
            squads::swap(m_archQueueType, x.m_archQueueType);
        }

        bool            equel(const self_type& o) {
            return (m_archQueueType.get_handle() == o.m_archQueueType.get_handel());
        }

        self_type& operator = (const self_type& other) {
            m_archQueueType = other.m_archQueueType;
            m_pEnd = other.m_pEnd;
            m_pFront = other.m_pFront;
            return *this;
        }
        self_type& operator = (const self_type&& other) {
            m_archQueueType = squads::move(other.m_archQueueType);
            m_pEnd = squads::move(other.m_pEnd);
            m_pFront = squads::move(other.m_pFront);
            return *this;
        }
    private:
        T* intern_getfront() {
            m_archQueueType.peek(m_pFront, SQUADS_PORTMAX_DELAY);
            return m_pFront;
        }
    private:
        cointainer_type     m_archQueueType;
        pointer             m_pEnd;
        pointer             m_pFront;
    };

    template <typename T, unsigned int maxItems>
	inline bool operator==(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return a.equel(b);
	}

	template <typename T, unsigned int maxItems>
	inline bool operator!=(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return !a.equel(b);
	}

	template <typename T, unsigned int maxItems>
	inline bool operator<(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return (a.size() < b.size());
	}

	template <typename T, unsigned int maxItems>
	inline bool operator>(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return (a.size() > b.size());
	}

	template <typename T, unsigned int maxItems>
	inline bool operator<=(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return (a.size() <= b.size());
	}

	template <typename T, unsigned int maxItems>
	inline bool operator>=(const basic_queue<T, maxItems>& a, const basic_queue<T, maxItems>& b)
	{
		return (a.size() >= b.size());
	}


	template <typename T, unsigned int maxItems>
	inline void swap(basic_queue<T, maxItems>& a, basic_queue<T, maxItems>& b) 
	{
		a.swap(b);
	}

    template <class T, class TQUEUE> 
    using queue_iterator = basic_queue_iterator<T, TQUEUE>;

    template <typename T, unsigned int maxItems = 32>
    using queue = basic_queue<T, maxItems>;
}

#endif