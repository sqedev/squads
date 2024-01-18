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
#ifndef __SQUADS_ARCH_MUTEX_IMPL_H__
#define __SQUADS_ARCH_MUTEX_IMPL_H__

#include "config.hpp"
#include "defines.hpp"
#include "core/algorithm.hpp"
#include "core/functional.hpp"
#include "core/basic_lock.hpp"

namespace squads {
    namespace arch {
        
        class arch_mutex_simple_impl : public basic_lock{
        public:

            arch_mutex_simple_impl()
                : m_pHandle(NULL),  
                  m_bLocked(false) { }

            arch_mutex_simple_impl(arch_mutex_simple_impl& other) 
                : m_pHandle(other.m_pHandle), 
                  m_bLocked(other.m_bLocked) { }

            arch_mutex_simple_impl(arch_mutex_simple_impl&& other) 
                : m_pHandle(squads::move(other.m_pHandle)), 
                  m_bLocked(squads::move(other.m_bLocked)) { }

            
            /**
             * Create the arch mutex
             */ 
            virtual bool create();

            /**
             * Destroy the mutex
             */
            bool destroy();

            virtual bool take(unsigned int xt) noexcept;
            virtual bool give() noexcept;

            /**
             * Try to lock the ILockObject
             *
             * @note call lock with timeout from 0
             *
             * @return true if the Lock was acquired, false when not
             */
            bool try_lock() noexcept {
                return (take(0) == 0);
            }

            /**
             * Is the ILockObject created (initialized) ?
             *
             * @return true if the ILockObject created (initialized) and false when not
             */
            bool is_initialized() const { return m_pHandle != NULL; };

            /**
             * @brief Is locked?
             * @return True if locked and false when not.
             */
            bool is_locked() const { return m_bLocked; };

            template <typename TRET = void*>
            TRET get_handle() const { return (TRET*)m_pHandle; }

            arch_mutex_simple_impl* operator = (arch_mutex_simple_impl& other) {
                m_pHandle = other.m_pHandle;
                m_bLocked = other.m_bLocked;
                return this;
            }

            arch_mutex_simple_impl* operator = (arch_mutex_simple_impl&& other) {
                m_pHandle = squads::move(other.m_pHandle);
                m_bLocked = squads::move(other.m_bLocked);
                return this;
            }

            bool operator == (arch_mutex_simple_impl& other) {
                return (m_pHandle == other.m_pHandle);
            }
            bool operator != (arch_mutex_simple_impl& other) {
                return (m_pHandle != other.m_pHandle);
            }

            void swap(arch_mutex_simple_impl& other) noexcept {
                squads::swap(m_pHandle, other.m_pHandle);
                squads::swap(m_bLocked, other.m_bLocked);
            }

        public:
            virtual int lock(unsigned int timeout = 0) noexcept override { 
                return take(timeout) ? 0 : 1;
            }
            virtual int unlock() noexcept override {
                return give() ? 0 : 1;
            }
            virtual int time_lock(const struct timespec *timeout) noexcept override {
                return 1;
            }
        protected:
            void* m_pHandle;
            bool m_bLocked;
        };

        class arch_mutex_recursive_impl : public arch_mutex_simple_impl {
        public:
            arch_mutex_recursive_impl() : arch_mutex_simple_impl() { }

            arch_mutex_recursive_impl(arch_mutex_recursive_impl& other) 
                : arch_mutex_simple_impl(other) { }

            arch_mutex_recursive_impl(arch_mutex_recursive_impl&& other) 
                : arch_mutex_simple_impl(other) { }

            
            /**
             * Create the arch mutex
             */ 
            bool create();
            bool create_static(void* pStaticBuffer);

            bool take(unsigned int xt) noexcept;
            bool give() noexcept;
        };

        inline void swap(arch_mutex_simple_impl& a, arch_mutex_simple_impl& b) noexcept {
            a.swap(b);
        }
    };
}


#endif