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
#ifndef __SQUADS_CRITICAL_LOCK_H__
#define __SQUADS_CRITICAL_LOCK_H__

#include "basic_lock.hpp"
#include "arch/arch_utils.hpp"

/**
 * @code{c}
 * void function() {
 *  auto_critical_lock_t critical_section;
 *  CRITICAL_SECTION(critical_section) {
 *      // locked section code goes hier
 *  }
 *  // unlocked section  code goes hier
 * }
 * @endcode
 */
#define CRITICAL_SECTION(OBJECT) if( (bool)(squads::auto_critical_lock_t lock(OBJECT)) )

/**
 * @code{c}
 * void function() {
 *  interrupts_lock_isr critical_section;
 *  CRITICAL_SECTION_NESTED(critical_section) {
 *      // locked section code goes hier
 *  }
 *  // unlocked section  code goes hier
 * }
 * @endcode
 */
#define CRITICAL_SECTION_NESTED(OBJECT) if( (bool)(squads::auto_interrupts_lock_isr_t lock(OBJECT)) )


/**
 * @code{c}
 * void function() {
 *  interrupts_lock interrupts_lock;
 *  INTERRUPTS_LOCK(interrupts_lock) {
 *      // locked section code goes hier
 *  }
 *  // unlocked section  code goes hier
 * }
 * @endcode
 */
#define INTERRUPTS_LOCK(OBJECT) if( (bool)(squads::auto_interrupts_lock_t lock(OBJECT)) )

/**
 * @code{c}
 * void function() {
 *  shed_lock schedlock;
 *  SCHEDULAR_LOCK(schedlock) {
 *      // locked section code goes hier
 *  }
 *  // unlocked section  code goes hier
 * }
 * @endcode
 */
#define SCHEDULAR_LOCK(OBJECT) if( (bool)(squads::auto_schedular_lock_t lock(OBJECT)) )

namespace squads {

    template<typename T>
    class basic_critical_lock : public basic_lock {
    public:
        basic_critical_lock() : m_bIsLocked(false) { }
        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) noexcept = 0;

        int time_lock(const struct timespec *timeout) noexcept override { return 1; }
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
        bool try_lock() noexcept  {
            return (lock(0) == 0);
        }

        /**
         * Is the ILockObject created (initialized) ?
         *
         * @return true if the ILockObject created (initialized) and false when not
         */
        bool is_initialized() const override { return true; }

        /**
         * @brief Is locked?
         * @return True if locked and false when not.
         */
        bool is_locked() const override { return m_bIsLocked; }
    protected:
        bool m_bIsLocked;
        T m_lock;
    };


    /**
     * @ingroup Interface
     * @ingroup lock
     */
    class critical_lock : public basic_critical_lock<arch::critical_lock_t> {
    public:
        using self_type = critical_lock;
        using base_type = basic_critical_lock<arch::critical_lock_t>;

        critical_lock() : base_type() { 
            arch::arch_critical_start(&m_lock); }

        virtual int lock(unsigned int timeout = 0) noexcept {
            int i = arch::arch_critical_lock(m_lock, timeout);
            m_bIsLocked = i == 0; 
            return i;
        }
        virtual int lock(unsigned int timeout = 0) noexcept {
            arch::arch_critical_unlock(m_lock);
            m_bIsLocked = false; 
            return 0;
        }
    };

    class spin_lock : public basic_critical_lock<arch::spin_lock_t> {
    public:
        using self_type = spin_lock;
        using base_type = basic_critical_lock<arch::spin_lock_t>;

        spin_lock() : base_type() { 
            arch::arch_spinlock_start(&m_lock); }

        virtual int lock(unsigned int timeout = 0) noexcept {
            int i = arch::arch_spinlock_aacquire(m_lock, timeout);
            m_bIsLocked = i == 0; 
            return i;
        }
        virtual int lock(unsigned int timeout = 0) noexcept {
            arch::arch_spinlock_release(m_lock);
            m_bIsLocked = false; 
            return 0;
        }
    };

    class interrupts_lock_isr : public basic_critical_lock<int> { 
    public:
        using self_type = interrupts_lock_isr;
        using base_type = basic_critical_lock<int>;

        
        virtual int lock(unsigned int timeout = 0) noexcept {
            m_lock = arch::arch_disable_interrupts_isr();
            m_bIsLocked = true;
            return 0;
        }  
        virtual int unlock() noexcept {
            arch::arch_enable_interrupts_isr(m_lock);
            m_bIsLocked = false;
            return 0;
        } 
    };

    class interrupts_lock : public basic_critical_lock<void> { 
    public:
        using self_type = interrupts_lock;
        using base_type = basic_critical_lock<void>;

   
        virtual int lock(unsigned int timeout = 0) noexcept {
            arch::arch_disable_interrupts();
            return 0;
        }  
        virtual int unlock() noexcept {
            arch::arch_enable_interrupts();
            return 0;
        } 
    };

    class shed_lock  : public basic_critical_lock<void> { 
    public:
        using self_type = shed_lock;
        using base_type = basic_critical_lock<void>;

        /**
         *  lock (take) a LokObject
         *  @param timeout How long to wait to get the Lock until giving up.
         */
        virtual int lock(unsigned int timeout = 0) noexcept {
            arch::arch_schedular_suspend();
            return 0;
        }  
        virtual int unlock() noexcept {
            arch::arch_schedular_resume();
            return 0;
        } 
    }; 

    #include "autolock.hpp"

    /** A autolock guard type for basic_critical_lock objects */
    using auto_critical_lock_t = autolock<critical_lock>;

    /** A autolock guard type for critical_section_nested_t objects */
    using auto_interrupts_lock_isr_t = autolock<interrupts_lock_isr>;

    /** A autolock guard type for interrupts_lock_t objects */
    using auto_interrupts_lock_t = autolock<interrupts_lock>;
    /** A autolock guard type for schedular_lock_t objects */
    using auto_shed_lock_t = autolock<shed_lock>;

    
}

#endif