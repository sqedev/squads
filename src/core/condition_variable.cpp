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

#include "core/task.hpp"
#include "core/condition_variable.hpp"

namespace squads {
    condition_variable::condition_variable()
            : m_mutex(), m_waitList() { }
    
    void condition_variable::add_list(task_type *thread) {
        autolock<mutex> autolock(m_mutex);

        m_waitList.push_back(thread);
    }
    void condition_variable::signal() {
        autolock<mutex> autolock(m_mutex);

        if ( !m_waitList.empty() ) {
            task_type *thr = m_waitList.front();
            m_waitList.pop_front();

            thr->signal();
        }
    }
    void condition_variable::broadcast() {
        autolock<mutex> autolock(m_mutex);

        while ( !m_waitList.empty() ) {
            task_type *thr = m_waitList.front();
            m_waitList.pop_front();

            thr->signal();
        }
    }

    int condition_variable::wait(mutex& mx, unsigned int timeOut) {
        task_type *thr =  task_type::get_self();
        return thr->wait(*this, mx, timeOut);
    }
}
