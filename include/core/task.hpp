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
#ifndef __SQUADS_TASK_H__
#define __SQUADS_TASK_H__

#include "config.h"
#include "defines.hpp"
#include "timespan.hpp"
#include "queue.hpp"
#include "mutex.hpp"
#include "condition_variable.hpp"
#include "eventgroup.hpp"

#define EVENTGROUP_BIT_STARTED  1
#define EVENTGROUP_BIT_JOINABLE	2

namespace squads {
    /**
     * @brief Wrapper class around  implementation of a task.
     *
     * @note To use this, you need to subclass it. All of your task should
     * be derived from the task class. Then implement the virtual on_task
     * function.
     *
     * @code
     *
     * #include <task/task.hpp>
     * // The number of test tasks
     * #define NUMBER_OF_TEST_TASK      2
     *
     * // The tost task class
     * class hello_world_task : public task {
     * public:
     *     hello_world_task() : task("HelloWorld", 1) { }
     *
     *     virtual void*  on_task() override {
     *         int id = get_id(); // get the task id from this task
     *         int core = get_on_core(); // get the core on run this task
     *
     *         while(true) { // infinity run
     *             // print the hello world message to the std output
     *             // out on core: 0 and with id: 0: "[0 @ 0] Hello World!"
     *             printf("[%d @ %d] Hello World!!\n", id, core );
     *         }
     *
     *         return NULL;
     *     }
     * };
     * extern "C" void app_main() {
     *     hello_world_task tasks[NUMBER_OF_TEST_TASK];
     *
     *     for(int i = 0; i < NUMBER_OF_TEST_TASK; i++) {
     *         tasks[i].start( i % 2 );
     *     }
     *     arch_panic();
     * }
     * // Examplo output:
     * // [0 @ 0] Hello World!
     * // [1 @ 1] Hello World!
     * // [0 @ 0] Hello World!
     * // ...
     * @endcode
     *
     * @author Amber-Sophia Schröck
     * @ingroup task
     */
    class task {
    public:
        /**
         *  The Thread class and the basic_condition_variable class are interdependent.
         *  If we allow the basic_condition_variable class to access the internals of
         *  the task class, we can reduce the public interface, which is a
         *  good thing.
         */
        friend class condition_variable;

        enum  class priority {
            Idle = SQUADS_ARCH_CONFIG_TASK_IDLE,           /*!< Priority for no real time operations - idle task */
            Low = SQUADS_CONFIG_CORE_PRIORITY_LOW,             /*!< Priority for low operation  */
            Normal = SQUADS_CONFIG_CORE_PRIORITY_NORM,         /*!< Priority for tasks for normal operatins - user interfaces for example */
            HalfCritical = SQUADS_CONFIG_CORE_PRIORITY_HALFCRT,/*!< Priority for tasks with normal deadlines and not a lot of processings*/
            Urgent = SQUADS_CONFIG_CORE_PRIORITY_URGENT,       /*!< priority for tasks with short deadlines and a lot of processings */
            Critical = SQUADS_CONFIG_CORE_PRIORITY_CRITICAL    /*!< Priority for critical tasks - the highest priority  */
        };
            enum class state {
            Running = 0,	  /*!< A task is querying the state of itself, so must be running. */
            Ready,			    /*!< The task being queried is in a read or pending ready list. */
            Blocked,		    /*!< The task being queried is in the Blocked state. */
            Suspended,		  /*!< The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
            Deleted		    /*!< The task being queried has been deleted, but its TCB has not yet been freed. */
        };

        
        using this_type = task;
        using native_handle_type = SQUADS_THREAD_NATIVE_HANDLE;
        using convar_type = condition_variable;

        /**
         * Basic Constructor for this task.
         * The priority is PriorityNormal and use MN_THREAD_CONFIG_MINIMAL_STACK_SIZE for the stack size
         */
        task() noexcept : task(" ", priority::Normal, SQUADS_CONFIG_MINIMAL_STACK_SIZE) { }

        /**
         * Constructor for this task.
         *
         * @param strName Name of the Task. Only useful for debugging.
         * @param uiPriority FreeRTOS priority of this Task.
         * @param usStackDepth Number of "words" allocated for the Task stack. default SQUADS_CONFIG_MINIMAL_STACK_SIZE
         */
        explicit task(const char* name, priority uiPriority = priority::Normal,
            unsigned short  usStackDepth = SQUADS_CONFIG_MINIMAL_STACK_SIZE) noexcept;

        /**
         * @brief Our destructor. Delete the task
         */
        virtual ~task();

        int start(int iCore);

        /**
         * @brief Destroy and delete the task and call the function 'on_kill'
         *
         * @return
         *  - 0 The task are destroyed
         *  - 1 The task was not running
         */
        int                   kill();

        /**
         * @brief Is the Task  running?
         *
         * @return true If the task  running, false If not
         */
        bool                  is_running();

        /**
         * @brief Get the debug name of this task
         *
         * @return The name of this task
         */
        const char*          get_name();
        /**
         * @brief Get the priority of this task
         *
         * @return The priority
         */
        priority get_priority();
        /**
         * @brief Get the stack depth of this task
         *
         * @return The stack depth
         */
        unsigned short        get_stackdepth();
        /**
         * @brief Accessor to get the task's backing task handle.
         * There is no setter, on purpose.
         *
         * @return FreeRTOS task handle.
         */
        native_handle_type           get_handle();

        /**
         * @brief Get the return value of this task  - after run
         *
         * @return The return value
         */
        int                  get_return_value();
        /**
         * @brief Get the time since start of this task
         *
         * @return The time since start of this task
         */
        timespan_t           get_time_since_start() const;
        /**
         * @brief Get the FreeRTOS task Numberid of this task
         *
         * @return The FreeRTOS task   Number
         */
        int32_t              get_id();
        /**
         * @brief Get the core number of this task  run
         *
         * @return The core number
         */
        int32_t              get_on_core();

        /**
         * @brief Set the priority of this task.
         *
         * @param uiPriority The task's new priority.
         */
        void                  set_priority(priority uiPriority);

        /**
         * @brief Suspend this task.
         *
         * @note While a task can suspend() itself, it cannot resume()
         * itself, becauseit's suspended.
         */
        void                  suspend();
        /**
         * @brief Resume a specific task.
         */
        void                  resume();

        bool 				  joinable() const noexcept;

        /**
         * @brief join the task, Wait in other task to end this task.
         * @param xTickTimeout The maximum amount of ticks to wait.
         * @note call never in the this task, then wait this task to end this task.
         * @return
         *		- 2 Call start first.
        *		- 1 Don't do this ... see the notes
        *		- 0 No error
        */
        int 				  join(unsigned int xTickTimeout = SQUADS_PORTMAX_DELAY);

        /**
         * @brief join the task, Wait in other task to end this task.
         * @param time The maximum amount of time to wait.
         * @note call never in the this task, then wait this task to end this task.
         * @return
         *		- ERR_TASK_NOTRUNNING Call start first.
        *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
        *		- NO_ERROR No error
        */
        int				  	join(timespan_t time);

        /**
         * @brief Wait for start the task.
         * @param xTickTimeout The maximum amount of ticks to wait.
         * @note call never in the this task, then wait this task to start this task.
         * @return
         *		- ERR_TASK_NOTRUNNING Call start first.
        *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
        *		- NO_ERROR No error
        */
        int				  	wait(unsigned int xTimeOut = SQUADS_PORTMAX_DELAY);

        /**
         * @brief Wait for start the task.
         * @param xTickTimeout The maximum amount of time to wait.
         * @note call never in the this task, then wait this task to start this task.
         * @return
         *		- ERR_TASK_NOTRUNNING Call start first.
        *		- ERR_TASK_CALLFROMSELFTASK Don't do this ... see the notes
        *		- NO_ERROR No error
        */
        int				  	wait(timespan_t time);

        

        /**
         * @brief Get the state of the task
         *
         * @return The state of the task at the time the function was called.
         */
        state               get_state();
        /**
         *  helper function to signal this thread.
         */
        virtual void          signal();

        /**
         *  Have this thread wait on a condition variable.
         *
         *  @note Threads wait, while condition_variables signal.
         *
         *  @param cv The condition variable associated with the Wait.
         *  @param cvl The required condition variable lock. The
         *  Lock must be held before calling Wait.
         *  @param timeOut Allows you to specify a timeout on the Wait,
         *  if desired.
         *
         *  @return
         */
        virtual int           wait(condition_variable& cv, mutex& cvl, unsigned int timeOut = SQUADS_PORTMAX_DELAY);

        /**
         * @brief Get the current task
         *
         * @return The current task
         */
        static this_type* get_self();
    
        

        /**
         *  @brief Operator to get the task's backing task handle.
         * @return FreeRTOS task handle.
         */
        operator native_handle_type () { return (native_handle_type)get_handle(); }
        /**
         * @brief Operator to get the ID assigned to the task.
         * @return The ID assigned to the task being queried.
         */
        operator int () { return get_id(); }
    public:
        static bool notify(task* task, uint32_t ulValue, int action);
        static bool notify_give(task* task);
        static uint32_t notify_take(bool bClearCountOnExit, unsigned int xTicksToWait = SQUADS_PORTMAX_DELAY);
        static bool notify_wait(uint32_t ulBitsToClearOnEntry, uint32_t ulBitsToClearOnExit, uint32_t *pulNotificationValue, 
            unsigned int xTicksToWait = SQUADS_PORTMAX_DELAY);
        static void set_storage_pointer(task* task, unsigned short index, void* value);
        static void* get_storage_pointer(task* task, unsigned short index);

    protected:
        /**
         * @brief This virtual function call on creating, use for user code
         * It is optional whether you implement this or not.
         */
        virtual void          on_start() {  }
        /**
         * @brief This virtual function call on kill, use for user code
         * It is optional whether you implement this or not.
         */
        virtual void          on_kill()   {  }

        /**
         * @brief Implementation of your actual task code.
         * You must override this function.
         *
         * @return Your return your task function, get with get_return_value()
         */
        virtual int         on_task() { return 0; }

        /**
         * @brief Called on exit from your on_task() routine.
         *
         * @note It is optional whether you implement this or not.
         * If you allow your task to exit its on_task method,
         */
        virtual void          on_cleanup() { }

        virtual void          on_signal() { }

    public:
        bool operator == (const this_type &r) const {
            return m_pHandle == r.m_pHandle;
        }

        bool operator != (const this_type &r) const {
            return !operator==(r);
        }

        bool operator < (const this_type &r) const {
            return m_pHandle < r.m_pHandle;
        }

        bool operator > (const this_type &r) const {
            return m_pHandle > r.m_pHandle;
        }
    
    private:
        static void runtaskstub(void* parm);
    protected:
        mutable mutex m_runningMutex, m_contextMutext, m_continuemutex;

        /**
         * @brief The name of this task.
         */
        const char* m_strName;
        /**
         * @brief A saved / cached copy of what the task's priority is.
         */
        priority m_uiPriority;
        /**
         *@brief  Stack depth of this task, in words.
        */
        unsigned short m_usStackDepth;
        /**
         * @brief The return value from user task routine
         */
        int m_retval;
        /**
         *  @brief Flag whether or not the task was started.
         */
        bool m_bRunning;
        /**
         * @brief The FreeRTOS task Number
         */
        int32_t m_iID;
        /**
         * @brief A saved / cached copy of which core this task is running on
         */
        int32_t m_iCore;
        /**
         * @brief Reference to the underlying task handle for this task.
         * @note Can be obtained from get_handle().
         */
        native_handle_type m_pHandle;

        eventgroup m_eventGroup;

        /**
         *  How we wait and signal the thread when using condition variables.
         *  Because a semaphore maintains state, this solves the race
         *  condition between dropping the CvLock and waiting.
         */
        mutex m_waitSem;
    
    };

    
    
    
}

#endif