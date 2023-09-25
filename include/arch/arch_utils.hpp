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
        

        void yield();

        /**
        * Get the current micros
        * @return Current micros
        */
        unsigned long micros();
        /**
         * Get the current millis or from ISR context - automatic switch
         * @return Current milliseconds
         */
        unsigned long millis();

        /**
         *  Get the current tick count or from ISR context - automatic switch
         *  @return Current tick count.
         */
        unsigned int get_ticks();


        void disable_interrupts();
        void enable_interrupts();
    
        void schedular_suspend();
        void schedular_resume();

        typedef struct critical_lock critical_lock_t;

        int critical_start(critical_lock_t* lock) ;
        int critical_lock(critical_lock_t* lock, unsigned int timeout);
        void critical_unlock(critical_lock_t* lock);

        typedef struct spin_lock spin_lock_t; 
        int spinlock_start(spin_lock_t* lock);
        int spinlock_aacquire(spin_lock_t* lock, unsigned int timeout);
        void spinlock_release(spin_lock_t* lock);

        void task_panic();

        void delay(const unsigned long& ts);



       

    }
}

SQUADS_EXTERNC_END

#endif