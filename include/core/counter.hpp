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
#ifndef __SQUADS_BASIC_COUNTER_H__
#define __SQUADS_BASIC_COUNTER_H__

#include "config.h"
#include "atomic/atomic.hpp"
#include "copyable.hpp"

namespace squads {

    /**
     * @brief This class implements a simple counter , whit atomic operations
     * for use in a multithreaded environment.
     */
    template <typename T>
    class basic_atomic_counter  {
    public:
        using value_type = T;
        using this_type = basic_atomic_counter  <value_type>;
        using ref_type = value_type&;
        using pointer = value_type*;
        using atomic_type = atomic::_atomic<value_type>;

        /**
         * @brief Construct a new basic_atomic_counter  and initializes it to zero.
         */
        basic_atomic_counter () : m_atomicCount(0) { }
        /**
         * @brief Construct a new basic_atomic_counter  and initializes it with the given
         * value.
         * @param value The start value for this the basic_atomic_counter .
         */
        basic_atomic_counter (const ref_type value) : m_atomicCount(counter) { }
        /**
         * @brief Construct a new basic_atomic_counter  from a other basic_atomic_counter .
         * @param other The other basic_atomic_counter  from copyed it.
         */
        basic_atomic_counter (const this_type& other) : m_atomicCount(other.m_atomicCount.load() ) { }

        /**
         * @brief Converts the this_type  to value_type.
         */
        inline operator value_type () const 			{ return m_atomicCount.load(); }
        /**
         * @brief Get the value of the basic_atomic_counter .
         * @return The value of the basic_atomic_counter .
         */
        inline value_type value() const					{ return m_atomicCount.load(); }

        /**
         * @brief Increments the basic_atomic_counter .
         * @return The result of the increment.
         */
        inline value_type operator ++ ()				{ return ++m_atomicCount; }
        /**
         * @brief Decrements the basic_atomic_counter .
         * @return The result of the decrement.
         */
        inline value_type operator -- ()				{ return --m_atomicCount; }

        /**
         * @brief Increments the basic_atomic_counter .
         * @return The previous value.
         */
        inline value_type operator ++ (int) 			{ return m_atomicCount++; }
        /**
         * @brief Decrements the basic_atomic_counter .
         * @return The previous value.
         */
        inline value_type operator -- (int) 			{ return m_atomicCount--; }
        /**
         * @brief If the basic_atomic_counter  zero?
         * @return If true the basic_atomic_counter  is zero and if false otherwise.
         */
        inline bool operator ! () const					{ return m_atomicCount.load() == 0; }
        /**
         * @brief Assigns the value of another this_type .
         */
        this_type& operator = (const this_type & other) {
            m_atomicCount.store(counter.m_atomicCount.load()); return *this;
        }
        /**
         * @brief Assigns a value to the basic_atomic_counter .
         * @param value The value for assigns the basic_atomic_counter .
         */
        this_type & operator = (const value_type& value) {
            m_atomicCount.store(value); return *this;
        }
    private:
        atomic_type	m_atomicCount;
    };

    template <typename T>
    using counter = basic_atomic_counter<T>;

}

#endif