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

#include "config.hpp"

#if SQUADS_CONFIG_ARCH_FREERTOS == 1

#include "core/task.hpp"
#include "core/autolock.hpp"
#include "freertos/FreeRTOS.h"
#include "esp_task.h"
#include "esp_log.h"

namespace squads {
    namespace internal {
            static int get_new_uniqid() {
                static uint32_t taskId = 0;

                return ++taskId;
            }
        }
        task::task(const char* name, priority uiPriority,
                unsigned short  usStackDepth) noexcept
        : m_runningMutex(),
          m_contextMutext(),
          m_continuemutex(),
          m_strName(name),
          m_uiPriority(uiPriority),
          m_usStackDepth(usStackDepth),
          m_retval(0),
          m_bRunning(false),
          m_iID(0),
          m_iCore(-1),
          m_pHandle(NULL),
          m_eventGroup(name),
          m_waitSem()
          { m_waitSem.lock(); } 

        task::~task() {
            if(m_pHandle != NULL)
                vTaskDelete( (TaskHandle_t) m_pHandle);
        }

        int task::start(int iCore) {
            m_iCore = iCore;

            m_continuemutex.lock();
            m_runningMutex.lock();
            if (m_bRunning)
            {
                m_runningMutex.unlock();
                m_continuemutex.unlock();

                ESP_LOGI(m_strName, "task allready running");
                return 3;
            }
            m_runningMutex.unlock();

            if(m_eventGroup.create() != 0) {
                m_continuemutex.unlock();
                ESP_LOGE(m_strName, "can create the event group for this task");
                return 4;
            }

        
            xTaskCreatePinnedToCore(
                    &runtaskstub, 
                    m_strName,
                    m_usStackDepth,
                    this, 
                    (UBaseType_t)m_uiPriority, 
                    (TaskHandle_t*)(&m_pHandle), 
                    m_iCore);
         

                if (m_pHandle == 0) {
                    m_continuemutex.unlock();
                    ESP_LOGE(m_strName, "the freertos task can not created");
                    return 1;
                }

                m_runningMutex.lock();

                m_iID = internal::get_new_uniqid();
                on_start();
                m_continuemutex.unlock();
                m_runningMutex.unlock();
                m_iCore = xTaskGetAffinity((TaskHandle_t)m_pHandle) ;

                xTaskNotify(m_pHandle, 0, eNoAction);


                return 0;
        }
        int task::join(timespan_t time) {
            timespan_t _time = time - timespan_t::now();
            return join(_time.to_ticks());
        }
        int task::wait(timespan_t time) {
            timespan_t _time = time - timespan_t::now();
            return wait(_time.to_ticks());
        }

        bool task::joinable() const noexcept {
            return (m_pHandle != nullptr);
        }
        
        int task::join(unsigned int xTimeOut) {
            if(!joinable()) return 1;

            if(!m_bRunning) return 1;
            if(m_pHandle == xTaskGetCurrentTaskHandle())  {
                return 2;
            }

            while ( !m_eventGroup.is_bit(EVENTGROUP_BIT_JOINABLE, xTimeOut) ) { }

            return 0;
        }

        //-----------------------------------
        //  wait
        //-----------------------------------
        int task::wait(unsigned int xTimeOut) {
            if(!m_bRunning) return 1;
            if(m_pHandle == xTaskGetCurrentTaskHandle())  {
                return 2;
            }

            while ( !m_eventGroup.is_bit(EVENTGROUP_BIT_STARTED, xTimeOut) ) { }

            return 0;
        }

        //-----------------------------------
        //  kill
        //-----------------------------------
        int task::kill() {
            m_continuemutex.lock();
            m_runningMutex.lock();

            if (!m_bRunning) {
                m_runningMutex.unlock();
                m_continuemutex.unlock();

                return 2;
            }
            vTaskDelete((TaskHandle_t)m_pHandle); m_pHandle = 0;
            m_bRunning = false;
            on_kill();

            m_runningMutex.unlock();
            m_continuemutex.unlock();

            return 0;
        }

        bool task::is_running() {
            autolock<mutex> autolock(m_runningMutex);
            return m_bRunning;
        }

        //-----------------------------------
        //  get_id
        //-----------------------------------
        int32_t task::get_id() {
            autolock<mutex> autolock(m_runningMutex);
            return m_iID;
        }

        //-----------------------------------
        //  get_on_core
        //-----------------------------------
        int32_t task::get_on_core() {
            autolock<mutex> autolock(m_runningMutex);
            return m_iCore;
        }

        //-----------------------------------
        //  get_name
        //-----------------------------------
        const char* task::get_name() {
            autolock<mutex> autolock(m_runningMutex);

            return m_strName;
        }

        //-----------------------------------
        //  get_priority
        //-----------------------------------
        task::priority task::get_priority() {
            autolock<mutex> autolock(m_runningMutex);

            if(m_pHandle == NULL) return m_uiPriority;

            if (xPortInIsrContext()) {
                return (task::priority)uxTaskPriorityGetFromISR(m_pHandle);
            } else {
                return (task::priority)uxTaskPriorityGet(m_pHandle);
            }
        }

        //-----------------------------------
        //  get_stackdepth
        //-----------------------------------
        unsigned short task::get_stackdepth() {
            autolock<mutex> autolock(m_runningMutex);
            return m_usStackDepth;
        }

        //-----------------------------------
        //  get_handle
        //-----------------------------------
        task::native_handle_type task::get_handle() {
            autolock<mutex> autolock(m_runningMutex);
            return m_pHandle;
        }

        //-----------------------------------
        //  get_tasks
        //-----------------------------------
        //uint32_t task::get_tasks() {
        //    return uxTaskGetNumberOfTasks();
        //}

        //-----------------------------------
        //  get_state
        //-----------------------------------
        task::state task::get_state() {
            autolock<mutex> autolock(m_runningMutex);
            return (task::state) eTaskGetState(m_pHandle);
        }

        //-----------------------------------
        //  get_return_value
        //-----------------------------------
        int task::get_return_value() {
            autolock<mutex> autolock(m_runningMutex);
            return (m_bRunning) ? -999 : m_retval;
        }
        //-----------------------------------
        //  get_time_since_start
        //-----------------------------------
        timespan_t task::get_time_since_start() const {
            autolock<mutex> autolock(m_runningMutex);
            auto ms = (xTaskGetTickCount()*portTICK_PERIOD_MS);

            return timespan_t(ms);
        }

        task* task::get_self() {
            auto _pHandle = xTaskGetCurrentTaskHandle();

            if (_pHandle == 0) return NULL;

            task* _task = new task();

            _task->m_runningMutex.lock();
            _task->m_iID = -1;
            _task->m_pHandle = _pHandle;
            _task->m_eventGroup.create();
            _task->m_runningMutex.unlock();

            return _task;
        }

        //-----------------------------------
        //  set_priority
        //-----------------------------------
        void  task::set_priority(task::priority uiPriority) {
            autolock<mutex> autolock(m_runningMutex);
            m_uiPriority = uiPriority;
            if(m_pHandle != NULL)
            vTaskPrioritySet(m_pHandle, (UBaseType_t)uiPriority);
        }

        //-----------------------------------
        //  suspend
        //-----------------------------------
        void task::suspend() {
            autolock<mutex> autolock(m_runningMutex);
            vTaskSuspend( m_pHandle );
        }

        //-----------------------------------
        //  resume
        //-----------------------------------
        void task::resume() {
            autolock<mutex> autolock(m_runningMutex);
            vTaskResume(m_pHandle);
        }
        //-----------------------------------
        //  runtaskstub
        //-----------------------------------
        void task::runtaskstub(void* parm) {
            task *esp_task;
            int ret; // the return value of the user task functions

            // cast the user data to this object
            esp_task = (static_cast<task*>(parm));

            // error?
            if(esp_task == nullptr) {
                // Log the error and delete the task
                ESP_LOGE("task", "unknown error on minilib task stub, task will delete");
                vTaskDelete(xTaskGetCurrentTaskHandle());
            } else { // on no error run normal minilib task system
                // set the started bit TODO:
                esp_task->m_eventGroup.set(EVENTGROUP_BIT_STARTED);

                // set running
                esp_task->m_runningMutex.lock();
                esp_task->m_continuemutex.lock();
                esp_task->m_bRunning = true;
                esp_task->m_runningMutex.unlock();

                // call the user task functions
                ret = esp_task->on_task();

                // clean up
                esp_task->on_cleanup();

                // set the return value and delete the task
                esp_task->m_runningMutex.lock();
                esp_task->m_bRunning = false;
                esp_task->m_retval = ret;
                vTaskDelete(esp_task->m_pHandle);
                esp_task->m_pHandle = 0;
                esp_task->m_runningMutex.unlock();
                esp_task->m_continuemutex.unlock();

                // TODO: set the join bit
                esp_task->m_eventGroup.set(EVENTGROUP_BIT_JOINABLE);
            }
        }

        //-----------------------------------
        //  signal
        //-----------------------------------
        void task::signal() {
            autolock<mutex> autolock(m_runningMutex);
            m_waitSem.unlock();

            xTaskNotifyGive( (TaskHandle_t) m_pHandle);

            on_signal();
        }

        //-----------------------------------
        //  wait
        //-----------------------------------
        int task::wait(condition_variable& cv, mutex& cvl, unsigned int timeOut)  {
            autolock<mutex> autolock(m_runningMutex);

            cv.add_list(this);

            cvl.unlock();
            int ret = m_waitSem.lock(timeOut);
            cvl.lock();

           ulTaskNotifyTake( pdTRUE, timeOut);

            return ret;
        }

}


#endif