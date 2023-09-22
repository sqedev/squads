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
#ifndef __SQUADS_CONFIG_H__
#define __SQUADS_CONFIG_H__

#define SQUADS_CONFIG_ARCH_FREERTOS 1

#if SQUADS_CONFIG_ARCH_FREERTOS == 1
#include "arch/freertos/config.hpp"
#endif

#define SQUADS_PORTMAX_DELAY                     SQUADS_ARCH_CONFIG_MAX_DELAY
#define SQUADS_SUPPORT_DYNAMIC_ALLOCATION         SQUADS_ARCH_SUPPORT_DYNAMIC_ALLOCATION

/// @brief Pre defined helper values for config items - Use a mutex
#define SQUADS_CONFIG_MUTEX                1
/// @brief Pre defined helper value for config items - Use a counting semaphore
#define SQUADS_CONFIG_COUNTING_SEMAPHORE   2
/// @brief Pre defined values for config items - Use a binary semaphore
#define SQUADS_CONFIG_BINARY_SEMAPHORE     3
/// @brief Pre defined helper values for config items - Use for aktivating
#define SQUADS_CONFIG_YES        1
/// @brief Pre defined helper values for config items - Use for deaktivating
#define SQUADS_CONFIG_NO         2


/**
 * @brief Pre defined values for config items -
 * Use for run the config task on core 0
 */
#define SQUADS_CONFIG_CORE_ONE   0
/**
 * @brief Pre defined values for config items -
 * Use for run the config task on core 1
 */
#define SQUADS_CONFIG_CORE_TWO   1
/**
 * @brief Pre defined values for config items -
 * Use for run the config task on core 2
 */
#define SQUADS_CONFIG_CORE_THREE 2
/**
 * @brief Pre defined values for config items -
 * Use for run the config task on core 3
 */
#define SQUADS_CONFIG_CORE_FOUR  3


//Start Config section
//==================================



#define SQUADS_CONFIG_DEFAULT_CORE   SQUADS_ARCH_CONFIG_BASE_CORE
#define SQUADS_CONFIG_STACK_DEPTH    SQUADS_ARCH_CONFIG_STACK_DEPTH

/**
 * @brief Pre defined on which core must run the work queue task,
 * can override in the create function
 */
#define SQUADS_CONFIG_DEFAULT_WORKQUEUE_CORE     SQUADS_ARCH_CONFIG_WORKQUEUE_CORE




// start basic config
//==================================
#ifndef SQUADS_CONFIG_SIZE_TYPE
    /**
     * The type for squads::size_t
     */
    #define SQUADS_CONFIG_SIZE_TYPE          long unsigned int
#endif

#ifndef SQUADS_CONFIG_STACK_TYPE
    ///The stack using type
    #define SQUADS_CONFIG_STACK_TYPE     unsigned long
#endif


#ifndef SQUADS_CONFIG_BASIC_HASHMUL_VAL
	/// Basic value for struct::hash as basic hash calculate @see squads::hash
	#define SQUADS_CONFIG_BASIC_HASHMUL_VAL 2149645487U
#endif // SQUADS_CONFIG_BASIC_HASHMUL_VAL

//==================================
// end basic config


// start task config
//==================================
#ifndef SQUADS_CONFIG_CORE_PRIORITY_IDLE
    /**
     * @brief Task priority for basic_task::PriorityIdle
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_IDLE     SQUADS_ARCH_CONFIG_TASK_IDLE
#endif

#ifndef SQUADS_CONFIG_CORE_PRIORITY_LOW
    /**
     * @brief  Task priority for basic_task::PriorityLow
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_LOW      1
#endif

#ifndef SQUADS_CONFIG_CORE_PRIORITY_NORM
    /**
     * @brief  Task priority for basic_task::PriorityNormal
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_NORM     2
#endif

#ifndef SQUADS_CONFIG_CORE_PRIORITY_HALFCRT
    /**
     * @brief Task priority for basic_task::PriorityHalfCritical
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_HALFCRT  (SQUADS_ARCH_CONFIG_TASK_MAXPRO-1)/2
#endif

#ifndef SQUADS_CONFIG_CORE_PRIORITY_URGENT
    /**
     * @brief Task priority for basic_task::PriorityUrgent
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_URGENT   (SQUADS_ARCH_CONFIG_TASK_MAXPRO-2)
#endif

#ifndef SQUADS_CONFIG_CORE_PRIORITY_CRITICAL
    /**
     * @brief Task priority for basic_task::PriorityCritical
     */
    #define SQUADS_CONFIG_CORE_PRIORITY_CRITICAL (SQUADS_ARCH_CONFIG_TASK_MAXPRO-1)
#endif


#ifndef SQUADS_CONFIG_MINIMAL_STACK_SIZE
    /**
     * The minimal stack size for a  task
     * @note default: 2048
     */
    #define SQUADS_CONFIG_MINIMAL_STACK_SIZE		SQUADS_ARCH_CONFIG_MIN_STACK_DEPTH
#endif

//==================================
// end task config


// start workqueue config
//==================================
#ifndef SQUADS_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT
    /**
     * Very Importend !!!
     * Handled the timeout for get_next_job timeout in the work queue
     * Only in this config medificated
     */
    #define SQUADS_CONFIG_WORKQUEUE_GETNEXTITEM_TIMEOUT  512
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_SINGLE_MAXITEMS
    /**
     * How many work items to queue in the workqueue single-threaded default: 8
     */
    #define SQUADS_CONFIG_WORKQUEUE_SINGLE_MAXITEMS      8
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_SINGLE_STACKSIZE
    /**
     * Stak size for the workqueue single-threaded thread
     * @note default: SQUADS_CONFIG_MINIMAL_STACK_SIZE
     */
    #define SQUADS_CONFIG_WORKQUEUE_SINGLE_STACKSIZE     SQUADS_CONFIG_MINIMAL_STACK_SIZE
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_SINGLE_PRIORITY
    /**
     * @note default: Priority for the workqueue single-threaded thread
     * @note default: basic_thread::PriorityLow
     */
    #define SQUADS_CONFIG_WORKQUEUE_SINGLE_PRIORITY      1
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_MULTI_WORKER
    /**
     * How many worker threads run in the workqueue multi-threaded
     * @note default: 4
     */
    #define SQUADS_CONFIG_WORKQUEUE_MULTI_WORKER         4
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_MULTI_MAXITEMS
    /**
     * How many work items to queue in the workqueue multi-threaded
     * @note default: 16
     */
    #define SQUADS_CONFIG_WORKQUEUE_MULTI_MAXITEMS      8
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_MULTI_STACKSIZE
    /**
     * Stak size for the workqueue multi-threaded for all worked thread
     * @note default: SQUADS_CONFIG_MINIMAL_STACK_SIZE
     */
    #define SQUADS_CONFIG_WORKQUEUE_MULTI_STACKSIZE     SQUADS_CONFIG_MINIMAL_STACK_SIZE
#endif

#ifndef SQUADS_CONFIG_WORKQUEUE_MULTI_PRIORITY
    /**
     * @note default: Priority for the workqueue multi-threaded for all worked thread
     * @note default: basic_thread::PriorityLow
     */
    #define SQUADS_CONFIG_WORKQUEUE_MULTI_PRIORITY     1
#endif
//==================================
// end workqueue config



#ifndef SQUADS_CONFIG_CSEMAPHORE_MIN_COUNT
    /**
     * @note default: min initial count for counting semaphore, can override in constuctor default  1
     */
    #define SQUADS_CONFIG_CSEMAPHORE_MIN_COUNT       1
#endif

#ifndef SQUADS_CONFIG_CSEMAPHORE_MAX_COUNT
    /**
     * @note default: max initial count for counting semaphore, can override in constuctor
     * @note default:  0x7fffffff
     */
    #define SQUADS_CONFIG_CSEMAPHORE_MAX_COUNT       0x7fffffff
#endif
// end SEMAPHORE config


#ifndef SQUADS_CONFIG_RECURSIVE_MUTEX_CHEAKING
    /**
     * When SQUADS_CONFIG_YES then cheaking for recursive mutex
     * and when FreeRTOS recursive mutex supported then can you use thes
     */
    #define SQUADS_CONFIG_RECURSIVE_MUTEX_CHEAKING     SQUADS_CONFIG_YES
#endif
// end mutex config


// start sharedobject config
//==================================
#ifndef SQUADS_CONFIG_SHAREDOBJECT_PREUSING
    /// Using pre defines (usings) for basic types for the shared objects
    #define SQUADS_CONFIG_SHAREDOBJECT_PREUSING SQUADS_CONFIG_YES
#endif //SQUADS_CONFIG_SHAREDOBJECT_PREUSING
//==================================
// end sharedobject config

// start mempool config
//==================================
#ifndef SQUADS_CONFIG_MEMPOOL_USE_MAGIC
    /// Use the magic guard for detect heap memory corruption - default: SQUADS_CONFIG_NO
    #define SQUADS_CONFIG_MEMPOOL_USE_MAGIC SQUADS_CONFIG_NO
#endif //SQUADS_CONFIG_SHAREDOBJECT_PREUSING

#ifndef SQUADS_CONFIG_MEMPOOL_MAGIC_START
    ///The start magic guard byte for detect heap memory corruption, not use in simple version
    #define SQUADS_CONFIG_MEMPOOL_MAGIC_START   0x6d //109
#endif

#ifndef SQUADS_CONFIG_MEMPOOL_MAGIC_END
    ///The end magic guard byte for detect heap memory corruption, not use in simple version
    #define SQUADS_CONFIG_MEMPOOL_MAGIC_END     0xa8 //168
#endif

//==================================
// end mempool config



// start tickhook config
//==================================
#ifndef SQUADS_CONFIG_TICKHOOK_MAXENTRYS
    ///The max entrys are hold the tickhook queue
    #define SQUADS_CONFIG_TICKHOOK_MAXENTRYS     10
#endif
//==================================
// end tickhook config



// start net / socket config
//==================================
#ifndef SQUADS_CONFIG_NET_IPADDRESS6_ENABLE
	#define SQUADS_CONFIG_NET_IPADDRESS6_ENABLE SQUADS_CONFIG_YES
#endif

#ifndef SQUADS_CONFIG_NET_IPADDRESS6_USE_SCOPEID
	#define SQUADS_CONFIG_NET_IPADDRESS6_USE_SCOPEID SQUADS_CONFIG_NO
#endif

#ifndef SQUADS_CONFIG_NET_IPADDRESS6_SCOPEID_VAL
	#define SQUADS_CONFIG_NET_IPADDRESS6_SCOPEID_VAL 0
#endif
//==================================
// end net / socket config



#ifndef SQUADS_CONFIG_FREELIST_MEMPOOL_FREE
    ///The mempool object is not used
    #define SQUADS_CONFIG_FREELIST_MEMPOOL_FREE 1
#endif

#ifndef SQUADS_CONFIG_FREELIST_MEMPOOL_USED
    ///The mempool object is used, not free
    #define SQUADS_CONFIG_FREELIST_MEMPOOL_USED 0
#endif



#if defined(__has_builtin)
	#define SQUADS_CONFIG_HAS_BUILTIN 	SQUADS_CONFIG_YES
#else
	#define SQUADS_CONFIG_HAS_BUILTIN 	SQUADS_CONFIG_NO
#endif // defined







#endif