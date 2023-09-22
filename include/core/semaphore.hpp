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
#ifndef __SQUADS_SEMAPHORE_H__
#define __SQUADS_SEMAPHORE_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

#include "autolock.hpp"
#include "atomic/atomic.hpp"

namespace squads {

    class basic_binary_semaphore : public basic_lock {
    public:
        using self_type = basic_binary_semaphore;
        using base_type = basic_lock;

        basic_binary_semaphore();
        basic_binary_semaphore (const self_type&) = delete;
        basic_binary_semaphore (const self_type&&) = delete;

        int lock(unsigned int timeout = 0) noexcept override;
        bool try_lock() noexcept override;

        int time_lock(const struct timespec *timeout) noexcept { return 1; }
        int unlock() noexcept override;

        bool is_initialized() const override { return true; }
        bool is_locked() const override { return m_bLock.load(); }

        void operator = (const self_type&) = delete;
        void operator = (const self_type&&) = delete;

    private:
        atomic::atomic_bool m_bLock;
    };    

    template <size_t TMAXCOUNT, typename TLOCK = basic_binary_semaphore>
    class basic_counting_semaphore : public basic_lock {
    public:
        using self_type = basic_counting_semaphore<TMAXCOUNT, TLOCK>;
        using base_type = basic_lock;

        constexpr size_t max_count() { return TMAXCOUNT; }

        basic_counting_semaphore()  : m_bLock(), m_iCount(0) { }
        basic_counting_semaphore (const self_type&) = delete;
        basic_counting_semaphore (const self_type&&) = delete;

        int lock(unsigned int timeout = 0) noexcept override {
             if(m_iCount >= TMAXCOUNT) return 1;

            if(m_iLocks > 0) {
                m_iLocks++; 
            } else if(m_refLockRefObject.lock(timeout) == 0) {
                m_iLocks++;  }

            return 0;

        }
        bool try_lock() noexcept override { 
            bool _ret = m_bLock.try_lock();
            if(_ret ) m_iCount++; return _ret; 
        }

        int time_lock(const struct timespec *timeout) noexcept { return 1; }
        int unlock() noexcept override {
            int lock_ret = 0;

            if(m_iLocks > 0) {
                m_iLocks--;
                if(m_iLocks == 0) {
                    lock_ret = m_refLockRefObject.unlock();
                }
            }
            return lock_ret;
        }

        bool is_initialized() const override { return true; }
        bool is_locked() const override { return m_iLocks > 0;  }

        size_t get_count () { return m_iCount; }
        size_t get_left() { return TMAXCOUNT - m_iCount; }

        void operator = (const self_type&) = delete;
        void operator = (const self_type&&) = delete;
    private:
        basic_binary_semaphore m_refLockRefObject;
        atomic::atomic_size_t m_iLocks;
    };

    using binary_semaphore = basic_binary_semaphore;

    template <size_t TMAXCOUNT, typename TLOCK = basic_binary_semaphore>
    using counting_semaphore = basic_counting_semaphore<TMAXCOUNT, TLOCK>;
}

#endif