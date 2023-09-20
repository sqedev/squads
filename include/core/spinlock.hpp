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
#ifndef __SQUADS_BASIC_SPINLOCK_H__
#define __SQUADS_BASIC_SPINLOCK_H__

#include "config.h"
#include "atomic/atomic.hpp"
#include "copyable.hpp"
#include "basic_lock.hpp"
#include "autolock.hpp"

namespace squads {
    /**
	 * @brief This class implements a simple spinlock, whit atomic operations.
	 * @tparam T The type of the object to lock
	 */
    template<typename T>
    class basic_spinlock : public basic_lock {
    public:
        using self_type = basic_spinlock <T>;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;
        using lock_guard = basic_autolock<self_type> ;

        /**
         * @brief Construct a new basic_spinlock 
         */
        basic_spinlock() : m_locked(false), m_value() { }

        /**
         * @brief Construct a new basic_spinlock  and initializes it with the given value.
         * @param value The locked value
         */
        explicit basic_spinlock (const reference value) : m_locked(false), m_value(value) { }

        /**
         *  lock (take) a basic_spinlock 
         *  @param timeout Not use
         */
        virtual int lock(unsigned int not_use = 0) {
            while(! m_locked.compare_exchange_t(false, true, atomic::memory_order::Acquire) ) { }
            return 0;
        }

        virtual int time_lock(const struct timespec *timeout) {
            return lock();
        }
        /**
         *  unlock (give) a basic_spinlock .
         */
        virtual int unlock() {
            m_locked.store(false, atomic::memory_order::Release);
            return 0;
        }

        /**
         * Try to lock the basic_spinlock 
         *
         * @note call lock with timeout from 0
         *
         * @return true if the Lock was acquired, false when not
         */
        virtual bool try_lock() {
            if( m_locked.compare_exchange_t(false, true, atomic::memory_order::Acquire) )
                return false;
            else
                lock();
            return true;
        }
        /**
         * Is the basic_spinlock  created (initialized) ?
         *
         * @return true if the basic_spinlock  created (initialized)
         */
        virtual bool is_initialized() const {
            return true;
        }

        /**
		 * @brief Converts the basic_spinlock  to value_type.
		 * @return The convertet value
		 */
        operator value_type() { return m_value; }

        self_type& operator = (const value_type& oValue) {
            lock_guard lock(*this);
            m_value = oValue;
            return *this;
        }

        bool operator == (const value_type& oValue) {
            lock_guard lock(*this);
            return m_value == oValue;
        }
        bool operator != (const value_type& oValue) {
            lock_guard lock(*this);
            return m_value != oValue;
        }

        basic_spinlock (const self_type&) = delete;
        self_type& operator=(const self_type&) = delete;
    protected:
        atomic::atomic_bool m_locked;
        value_type m_value;
    };

    template<typename T>
    using spinlock = basic_spinlock<T>;
}

#endif