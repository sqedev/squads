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
#ifndef __SQUADS_INTERFACE_LOCK_H__
#define __SQUADS_INTERFACE_LOCK_H__

#include "config.hpp"
#include "defines.hpp"

namespace squads {
    /**
     * Interface for all lock types in this library
     * This is an abstract base class.
     * To use this, you need to subclass it. All of your LockObjetcs should
     * be derived from the basic_lock class. Then implement the virtual lock,
     * unlock and is_initialized functions.
     *
     * @ingroup Interface
     * @ingroup lock
     */
    class basic_lock {
    public:
        using this_type = basic_lock;

        basic_lock() { }

        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) noexcept = 0;

        virtual int time_lock(const struct timespec *timeout) noexcept = 0;
        /**
         *  unlock (give) a semaphore.
         */
        virtual int unlock() noexcept = 0;

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
        virtual bool is_initialized() const = 0;

        /**
         * @brief Is locked?
         * @return True if locked and false when not.
         */
        virtual bool is_locked() const = 0;
    };

}

#endif