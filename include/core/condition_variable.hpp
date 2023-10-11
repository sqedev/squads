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
#ifndef __SQUADS_CONVAR_H__
#define __SQUADS_CONVAR_H__


#include "config.hpp"
#include "defines.hpp"
#include "mutex.hpp"
#include "autolock.hpp"
#include "list"

//TODO: Add own list 

namespace squads {
    class task;

    /**
     *  Class implementation of condition variable
     *
     *  A condition variable isn't really a variable. It's a list
     *  of threads.
     *
     *  The design here is that a basic_convar_task "waits", and a condition_variable
     *  "signals". This affects where the public interfaces reside.
     */
    class condition_variable {
        /**
         *  The basic_task class and the condition_variable class are interdependent.
         *  If we allow the task class to access the internals of the
         *  condition_variable, we can reduce the public interface which is a
         *  good thing.
         */
        friend class task;
    public:
        using this_type = condition_variable;
        using task_type = task;

        /**
         *  Constructor to create a condition variable.
         */
        condition_variable();

        /**
         *  Signal a thread waiting on this condition_variable (FIFO list).
         */
        void signal();

        /**
         * Signal all threads waiting on this condition_variable.
         */
        void broadcast();

        inline void notify_one() noexcept { // wake up one waiter
            signal();
        }

        inline void notify_all() noexcept { // wake up all waiters
            broadcast();
        }

        int wait(squads::mutex& mx, unsigned int timeOut);
    private:
        /**
         *  Internal helper function to queue a task to
         *  this condition_variable's wait list.
         *
         * @param task The task to add to the waiting list
         */
        void add_list(task_type *task);
    protected:
         /**
         *  Protect the internal condition_variable state.
         */
        mutex                           m_mutex;
        /**
         *  Implementation of a wait list of Threads.
         */
        std::list<task_type*>    m_waitList;
    };
}

#endif