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
#ifndef __SQUADS_MUTEX_H__
#define __SQUADS_MUTEX_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"
#include "basic_lock.hpp"
#include "arch/arch_mutex_impl.hpp"
#include "timed_lock.hpp"

namespace squads {
    class task;
    
    using mutex = arch::arch_mutex_simple_impl;
    using recursive_mutex = arch::arch_mutex_recursive_impl;

    template<class TTASK = task, class TCONVAR = TTASK::convar_type>
    using timed_mutex = timed_lock<mutex, TTASK, TCONVAR>;


    template<class TTASK = task, class TCONVAR = TTASK::convar_type>
    using recursive_timed_mutex = timed_lock<recursive_mutex, TTASK, TCONVAR>;

    struct defer_lock_t {};
    struct try_to_lock_t {};
    struct adopt_lock_t {};

    inline constexpr defer_lock_t  defer_lock{};
    inline constexpr try_to_lock_t try_to_lock{};
    inline constexpr adopt_lock_t  adopt_lock{};

    template <class Mutex>
    class lock_guard {
    public:
        using mutex_type = Mutex;

        explicit lock_guard(mutex_type& m) : m_ref_lock(m) {
            assert( (m_ref_lock.lock(SQUADS_PORTMAX_DELAY) != 0) );
        }

        lock_guard(mutex_type &m, unsigned long xTicksToWait)
            : m_ref_lock(m) {
            assert( (m_ref_lock.lock(xTicksToWait) != 0) );
        }

        lock_guard(mutex_type& m, adopt_lock_t) : lock_guard(m) { }

        ~lock_guard() {
            m_ref_lock.unlock();
        }

        lock_guard(lock_guard const&) = delete;
        lock_guard& operator=(lock_guard const&) = delete;
    private:
        mutex_type m_ref_lock;
    };

    template <class Mutex>
    class unique_lock {
    public:
        using mutex_type = Mutex;

        unique_lock() noexcept : m_mut(nullptr), m_owns(false) { }
        explicit unique_lock(mutex_type& m, unsigned int timeout = SQUADS_PORTMAX_DELAY) 
            : m_mut(m), m_owns(true) {m_mut->lock(timeout);}

        unique_lock(mutex_type& m, defer_lock_t) noexcept
            : m_mut(m), m_owns(false) { } 

        unique_lock(mutex_type& m, try_to_lock_t)
            : m_mut(m), m_owns(m.try_lock()) { } 
        unique_lock(mutex_type& m, adopt_lock_t) 
            : m_mut(m), m_owns(false) { }  

        /*template <class Clock, class Duration>
        unique_lock(mutex_type& m, const chrono::time_point<Clock, Duration>& abs_time)
            : m_mut(m), m_owns(m.try_lock_until(abs_time)) {}

        template <class Rep, class Period>
        unique_lock(mutex_type& m, const chrono::duration<Rep, Period>& rel_time)
             :  m_mut(m), m_owns(m.try_lock_for(rel_time)) {}*/

        ~unique_lock() { if (m_owns) m_mut->unlock(); }

        unique_lock(unique_lock const&) = delete;
        unique_lock& operator=(unique_lock const&) = delete;

        unique_lock(unique_lock&& u) noexcept;
        unique_lock& operator=(unique_lock&& u) noexcept;

        void lock(unsigned int timeout = SQUADS_PORTMAX_DELAY) {
            if (m_mut == nullptr) return;
            if (m_owns) return;

            m_mut->lock(timeout);
            m_owns = true;
        }
        bool try_lock() {
            if (m_mut == nullptr) return false;
            if (m_owns) return false;

            m_owns = m_mut->lock(0) == 0;

            return m_owns;
        }

        /*template <class Rep, class Period>
        bool try_lock_for(const chrono::duration<Rep, Period>& rel_time) {
            if (m_mut == nullptr) return;
            if (m_owns) return;

            m_owns = m_mut->try_lock_for(rel_time);
        }
        template <class Clock, class Duration>
        bool try_lock_until(const chrono::time_point<Clock, Duration>& abs_time) {
            if (m_mut == nullptr) return;
            if (m_owns) return;

            m_owns = m_mut->try_lock_until(abs_time);
        }*/

        void unlock() {
            if (!m_owns) return;
            m_mut->unlock();
            m_owns = false;
        }

        void swap(unique_lock& u) noexcept {
            squads::swap(m_mut, u.m_mut);
            squads::swap(m_owns, u.m_owns);
        }
        mutex_type* release() noexcept {
            mutex_type* _mut = m_mut;
            m_mut = nullptr;
            m_owns = false;
            return _mut;
        }

        bool owns_lock() const noexcept             { return m_owns; }
        explicit operator bool () const noexcept    { return m_owns; }
        mutex_type* mutex() const noexcept          { return m_mut; }
    private:
        mutex_type* m_mut;
        bool        m_owns;
    };
    template <class _Mutex> 
    inline void swap(unique_lock<_Mutex>& a, unique_lock<_Mutex>& b) noexcept
        { a.swap(b); }
}

#endif