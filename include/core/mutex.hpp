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

    namespace internal {
        template <arch::mutex_type type>
        class basic_mutex : public basic_lock {
        public:
            using this_type = basic_mutex;
            using base_type = basic_lock;
            using nativ_handle = void*;

            basic_mutex() : m_pMutex(nullptr), m_isLocked(false) {

                m_pMutex = arch::arch_create_mutex(type);

                if (m_pMutex) {
                    m_isLocked = true;
                    unlock();
                }
            }
            basic_mutex(const basic_mutex& other)
                : m_pMutex(other.m_pMutex), m_isLocked(other.m_isLocked) {  }
            basic_mutex(const basic_mutex&& other)
                : m_pMutex(squads::move(other.m_pMutex)), m_isLocked( squads::move(other.m_isLocked))  { }
            ~basic_mutex() { arch::arch_delete_mutex(m_pMutex); }

            /**
             *  lock (take) a LokObject
             *  @param timeout How long to wait to get the Lock until giving up.
             */
            virtual int lock(unsigned int timeout = 0) noexcept override {
                if(arch::arch_take_mutex(m_pMutex, timeout, type) == 0) {
                    m_isLocked = true;
                    return 0;
                } else {
                    return 1;
                }
            }

            virtual int time_lock(const struct timespec *timeout) noexcept override { return 0; }
            /**
             *  unlock (give) a semaphore.
             */
            virtual int unlock() noexcept override {
                if(arch::arch_give_mutex(m_pMutex, type) == 0) {
                    m_isLocked = true;
                    return 0;
                } else {
                    return 1;
                }
            }

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
            virtual bool is_initialized() const { return m_pMutex != nullptr; };

            /**
             * @brief Is locked?
             * @return True if locked and false when not.
             */
            virtual bool is_locked() const { return m_isLocked; };

        protected:
            nativ_handle m_pMutex;
            bool m_isLocked;
        };
    }
    using mutex = internal::basic_mutex<arch::mutex_type::simple>;
    using recursive_mutex = internal::basic_mutex<arch::mutex_type::recursive>;

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
            assert( (m_ref_lock.lock(SQUADS_PORTMAX_DELAY) != NO_ERROR) );
        }

        lock_guard(mutex_type &m, unsigned long xTicksToWait)
            : m_ref_lock(m) {
            assert( (m_ref_lock.lock(xTicksToWait) != NO_ERROR) );
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
            if (m_mut == nullptr) return;
            if (m_owns) return;

            m_owns = m_mut->lock(0) == 0;
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
            if (!__owns_) return;
            m_mut->unlock();
            m_owns = false;
        }

        void swap(unique_lock& u) noexcept {
            mn::swap(m_mut, u.m_mut);
            mn::swap(m_owns, u.m_owns);
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