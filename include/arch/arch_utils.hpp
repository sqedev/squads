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
#ifndef __SQUADS_ARCH_UTILS_H__
#define __SQUADS_ARCH_UTILS_H__

#include "config.hpp"
#include "defines.hpp"



SQUADS_EXTERNC_BEGINN

namespace squads {
    namespace arch {
        
        /**
        * Get the current micros
        * @return Current micros
        */
        unsigned long arch_micros();
        /**
         * Get the current millis or from ISR context - automatic switch
         * @return Current milliseconds
         */
        unsigned long arch_millis();

        /**
         *  Get the current tick count or from ISR context - automatic switch
         *  @return Current tick count.
         */
        unsigned int arch_get_ticks();
    
        

        typedef struct critical_lock critical_lock_t;

        int arch_critical_start(critical_lock_t* lock) ;
        int arch_critical_lock(critical_lock_t* lock, unsigned int timeout);
        void arch_critical_unlock(critical_lock_t* lock);

        typedef struct spin_lock spin_lock_t; 
        int arch_spinlock_start(spin_lock_t* lock);
        int arch_spinlock_aacquire(spin_lock_t* lock, unsigned int timeout);
        void arch_spinlock_release(spin_lock_t* lock);

        
        void arch_yield() ;
 
        int arch_disable_interrupts_isr() ;
        void arch_enable_interrupts_isr(int state) ;

        void arch_disable_interrupts() ;
        void arch_enable_interrupts();

        void arch_schedular_suspend();
        void arch_schedular_resume();

        void arch_task_panic();

        void arch_delay(const unsigned long& ts);

    }
}

SQUADS_EXTERNC_END

#endif