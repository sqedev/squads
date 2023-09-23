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
#ifndef __SQUADS_DEBUG_LOCK_H__
#define __SQUADS_DEBUG_LOCK_H__

#include "basic_lock.hpp"

namespace squads {
    /**
     * @ingroup Interface
     * @ingroup lock
     */
    template <class TLOCK, TLOCK reflock, const char* name>
    class debug_lock {
    public:
        using this_type = debug_lock<TLOCK, reflock, name>;

        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) noexcept  { 
            int _ret = reflock.lock(timeout);
            if(_ret == 0) { printf("%s lock %d", name, _ret);  }

            return _ret;
        }

        virtual int time_lock(const struct timespec *timeout) noexcept  { 
            int _ret = reflock.time_lock(timeout);
            if(_ret == 0) { printf("%s time_lock %d", name, _ret); }

            return _ret;
        }
        /**
         *  unlock (give) a semaphore.
         */
        virtual int unlock() noexcept   { 
            int _ret = reflock.unlock();
            if(_ret == 0) { printf("%s unlock %d", name, _ret); }

            return _ret;
        }

        /**
         * Try to lock the ILockObject
         *
         * @note call lock with timeout from 0
         *
         * @return true if the Lock was acquired, false when not
         */
        virtual bool try_lock() noexcept {
            int _ret = reflock.try_lock();
            if(_ret == 0) { printf("%s try_lock %d", name, _ret); }

            return _ret;
        }

        /**
         * Is the ILockObject created (initialized) ?
         *
         * @return true if the ILockObject created (initialized) and false when not
         */
        virtual bool is_initialized() const { return reflock.is_initialized(); }

        /**
         * @brief Is locked?
         * @return True if locked and false when not.
         */
        virtual bool is_locked() const { return reflock.is_locked(); }

    };

}

#endif