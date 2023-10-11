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
#ifndef __SQUADS_TIMEDLOCK_H__
#define __SQUADS_TIMEDLOCK_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"
#include "autolock.hpp"

namespace squads {

    template<class TLOCK, class TTASK, class TCONVAR>
    class timed_lock {
    public:
        using task_type = TTASK;
        using convar_type = TCONVAR;
        using this_type = timed_lock;
        using lock_type = TLOCK;

        timed_lock()
            : m_cv(), m_lockObject(), m_bLocked(false)  { }
        
        ~timed_lock() {
            autolock<TLOCK> lock(m_lockObject);
        }

         /**
         *  Lock the timed mutex.
         *
         * @param task The current canvar Task
         * @param Timeout How long to wait to get the Lock until giving up. (default = 0xffffffffUL)
         */
        void lock(task_type& task, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
            autolock<TLOCK> lock(m_lockObject);
            while (m_bLocked) {
                task.wait(m_cv, m_mutex, timeout);
            }
            m_bLocked = true;
        }

         /**
         * Unlock the timed Mutex.
         *
         * @param signal_childs On notify the task, send a notify to the child task?
         */
        void unlock(bool signal_childs = true) {
            autolock<TLOCK> lock(m_lockObject);
            m_bLocked = false;
            m_cv.signal(signal_childs);
        }
    protected:
            convar_type m_cv;
            lock_type   m_lockObject;
            bool        m_bLocked
    };
}

#endif