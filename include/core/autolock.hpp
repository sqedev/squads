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
#ifndef __SQUADS_AUTOLOCK_LOCK_H__
#define __SQUADS_AUTOLOCK_LOCK_H__

#include "config.hpp"
#include "defines.hpp"
#include "basic_lock.hpp"

/**
 * Macro for locked sections
 *
 * @param LOCK The typ of lock class
 * @param OBJECT The lock object
 *
 * @example @code
 * mutex_t mutex;
 * LOCKED_SECTION(mutex_t, mutex) {
 *  locked
 * }
 * unlocked
 * @endcode
 *
 * @ingroup lock
 */
#define LOCKED_SECTION(LOCK, OBJECT) if( (squads::basic_autolock<LOCK> lock(OBJECT)) )
#define UNLOCKED_SECTION(LOCK, OBJECT) if( (squads::basic_autounlock<LOCK> ulock(OBJECT)) )

namespace squads {

    /**
     *  Synchronization helper class that leverages the C++ language to help
     *  prevent deadlocks.
     *  This is a C++11 feature that allows basic_lock  
     *  Locking and Unlocking to behave following an RAII style.
     *  The constructor of this helper object locks the basic_lock  .
     *  The destructor unlocks the basic_lock  .
     *
     * @ingroup semaphore
     * \ingroup lock
     */
    template <class LOCK>
    class  basic_autolock   {
    public:
        /**
         *  Create a basic_autolock with a specific LockType, without timeout
         *
         *  @post The basic_lock  will be locked.
         */
        basic_autolock(LOCK &m)
        : m_ref_lock(m) {
        assert( (m_ref_lock.lock(portMAX_DELAY) != 0) );
        }
        /**
         * Create a basic_autolock with a specific LockType, with timeout
         *
         * @param xTicksToWait How long to wait to get the lock until giving up.
         *
         * @post The basic_lock  will be locked.
         */
        basic_autolock(LOCK &m, unsigned long xTicksToWait)
        : m_ref_lock(m) {
        assert( (m_ref_lock.lock(xTicksToWait) != 0) );
        }
        /**
         *  Destroy a basic_autolock.
         *
         *  @post The basic_lock  will be unlocked, when the lock Object locked
         */
        ~basic_autolock() {
            m_ref_lock.unlock();
        }

        operator bool () {
            return m_ref_lock.is_locked();
        }
    private:
        /**
         *  Reference to the basic_lock  we locked, so it can be unlocked
         *  in the destructor.
         */
        LOCK &m_ref_lock;
    };

    template <class LOCK>
    using autolock = basic_autolock<LOCK>;

    /**
     *  This is a C++11 feature that allows basic_lock  
     *  Locking and unlocking to behave following an RAII style.
     *  The constructor of this helper object unlocks the basic_lock  .
     *  The destructor locks the basic_lock  .
     *
     * @ingroup semaphore
     * \ingroup lock
     */
    template <class LOCK>
    class  basic_autounlock   {
    public:
        /**
         *  Create a basic_autounlock with a specific LockType.
         *  @post The basic_lock  will be unlocked.
         */
        basic_autounlock(LOCK &m)
        : m_ref_lock(m) {
            m_ref_lock.unlock();
            m_xTicksToWait = SQUADS_PORTMAX_DELAY;
        }

        basic_autounlock(LOCK &m, unsigned long xTicksToWait)
        : m_ref_lock(m) {
            m_ref_lock.unlock();
            m_xTicksToWait = xTicksToWait;
        }
        /**
         *  Destroy a basic_autounlock.
         *
         *  @post The basic_lock  will be locked.
         */
        ~basic_autounlock() {
            assert( (m_ref_lock.lock(m_xTicksToWait) != 0) );
        }

        void set_timeout(unsigned long xTicksToWait = SQUADS_PORTMAX_DELAY) {
            m_xTicksToWait = xTicksToWait;
        }

        operator bool () {
            return !(m_ref_lock.is_locked());
        }
    private:
        /**
         *  Reference to the basic_lock  we unlocked, so it can be locked
         *  in the destructor.
         */
        LOCK &m_ref_lock;

        /**
         * @brief A saved / coped lock timeout for lock in the destructor.
         */
        unsigned long m_xTicksToWait;
    };

    template <class LOCK>
    using autounlock = basic_autounlock<LOCK>;
}

#endif