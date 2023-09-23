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
#ifndef __SQUADS_NULL_LOCK_H__
#define __SQUADS_NULL_LOCK_H__

#include "basic_lock.hpp"

namespace squads {
    /**
     * @ingroup Interface
     * @ingroup lock
     */
    class null_lock {
    public:
        using this_type = null_lock;

        null_lock() { }

        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) noexcept  { m_bLocked = true;  return 0; }

        virtual int time_lock(const struct timespec *timeout) noexcept { m_bLocked = true; return 0; }
        /**
         *  unlock (give) a semaphore.
         */
        virtual int unlock() noexcept { m_bLocked = false; return 0; }

        /**
         * Try to lock the ILockObject
         *
         * @note call lock with timeout from 0
         *
         * @return true if the Lock was acquired, false when not
         */
        virtual bool try_lock() noexcept {
            return (lock(0) == 0);
        }

        /**
         * Is the ILockObject created (initialized) ?
         *
         * @return true if the ILockObject created (initialized) and false when not
         */
        virtual bool is_initialized() const { return true; }

        /**
         * @brief Is locked?
         * @return True if locked and false when not.
         */
        virtual bool is_locked() const { return m_bLocked; }
    private:
        bool m_bLocked;
    };

}

#endif