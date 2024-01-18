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
#ifndef __SQUADS_MSG_TASK_H__
#define __SQUADS_MSG_TASK_H__

#include "task.hpp"

namespace squads {

    class msg_task : public task {
    public:
        /**
        * The specific task message
        */
        struct message {
            using message_id = int;

            message_id id;              /*!< The message id */
            void* _message;              /*!< The message*/


            message(message_id _id, void* msg = nullptr)
                : id(_id), _message(msg) { }
        };

        using this_type = msg_task;
        using base_type = task;
        using message_id = typename message::message_id;
        using native_handle_type = typename base_type::native_handle_type;
        using convar_type = typename base_type::convar_type;

        /**
         * Basic Constructor for this task.
         * The priority is PriorityNormal and use SQUADS_CONFIG_MINIMAL_STACK_SIZE for the stack size
         */
        msg_task() noexcept : base_type("msg_task", priority::Normal, SQUADS_CONFIG_MINIMAL_STACK_SIZE) { }
        /**
         * Constructor for this task.
         *
         * @param strName Name of the Task. Only useful for debugging.
         * @param uiPriority FreeRTOS priority of this Task.
         * @param usStackDepth Number of "words" allocated for the Task stack. default SQUADS_CONFIG_MINIMAL_STACK_SIZE
         */
        explicit msg_task(priority uiPriority = priority::Normal,
            unsigned short  usStackDepth = SQUADS_CONFIG_MINIMAL_STACK_SIZE) noexcept;

        /**
         * @brief Add a pre-created task message to the task queue
         *
         * @param[in] msg The specific message you are adding to the task queue
         * @param timeout How long to wait to add the item to the queue
         */
        void post_msg(message*  msg, unsigned int timeout = SQUADS_PORTMAX_DELAY);


        bool have_message() {
            autolock<mutex> lock(m_ltMessageQueueLock);
            return !(m_qeMessageQueue.is_empty());
        }
        bool get_message(message *msg, unsigned int popTimeOut = SQUADS_PORTMAX_DELAY, unsigned int waitTimeOut = SQUADS_PORTMAX_DELAY) {
            autolock<mutex> lock(m_ltMessageQueueLock);

            while (m_qeMessageQueue.is_empty())
                wait(m_cvMessage, m_ltMessageQueueLock, waitTimeOut);

            if(!m_qeMessageQueue.is_empty()) {
                return m_qeMessageQueue.pop(&msg, popTimeOut);
            }
            return false;
        }
    private:
        mutex          m_ltMessageQueueLock;
        convar_type    m_cvMessage;
        queue<message*> m_qeMessageQueue;
        uint32_t       m_waitMaxPop;
    };
}

#endif