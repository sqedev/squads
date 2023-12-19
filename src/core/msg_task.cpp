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

#include "core/msg_task.hpp"

namespace squads {
    msg_task::msg_task(priority uiPriority, unsigned short  usStackDepth) noexcept 
        : base_type("msg_task", uiPriority, usStackDepth),
        m_ltMessageQueueLock(),
        m_cvMessage(),
        m_qeMessageQueue(),
        m_waitMaxPop (SQUADS_PORTMAX_DELAY) { }

    void msg_task::post_msg(message* msg, unsigned int timeout) {
        autolock<mutex> lock(m_ltMessageQueueLock);
        m_qeMessageQueue.push(msg, timeout);
        m_cvMessage.signal();
    }

    
    

}