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

#ifndef __SQUADS_ARCH_FREERTOS_H__
#define __SQUADS_ARCH_FREERTOS_H__

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
/**
 * All cunfig properties can override in your sdkconfig.h
 */
#ifdef ESP_BUILD_WITH_SDKCONFIG
#include "sdkconfig.h"
#endif

#ifndef CHAR_BIT
#define CHAR_BIT 8
#endif

#define SQUADS_THREAD_CONFIG_SIZE_TYPE                  long unsigned int
#define SQUADS_THREAD_CONFIG_STACK_TYPE                 unsigned long
#define SQUADS_THREAD_CONFIG_TICK_TYPE                  TickType_t
#define SQUADS_THREAD_CONFIG_BASIC_ALIGNMENT            sizeof(unsigned char*)
#define SQUADS_THREAD_CONFIG_NATIVE_HANDLE              TaskHandle_t
#define SQUADS_THREAD_CONFIG_EVENTGROUTP_NATIVE_HANDLE  EventGroupHandle_t

/// @brief The max number of usable cores
#define SQUADS_THREAD_CONFIG_CORE_MAX   (portNUM_PROCESSORS - 1)

/**
 * @brief Pre defined values for config items -
 * Use for indicating the task has no affinity core
 */
#define SQUADS_THREAD_CONFIG_CORE_IFNO  tskNO_AFFINITY

#define SQUADS_ARCH_CONFIG_BASE_CORE            0
#define SQUADS_ARCH_CONFIG_WORKQUEUE_CORE       1
#define SQUADS_ARCH_CONFIG_STACK_DEPTH          8192
#define SQUADS_ARCH_CONFIG_MIN_STACK_DEPTH      4096
#define SQUADS_ARCH_CONFIG_TASK_IDLE            tskIDLE_PRIORITY     
#define SQUADS_ARCH_CONFIG_TASK_MAXPRO          configMAX_PRIORITIES   

#define SQUADS_ARCH_CONFIG_MAX_DELAY            portMAX_DELAY
#define SQUADS_ARCH_NSPER_TICK                  (  1000000000LL / configTICK_RATE_HZ )
#define SQUADS_ARCH_CLOCKS_PER_SEC              ( ( clock_t ) configTICK_RATE_HZ )
#define SQUADS_ARCH_TIMESTAMP_RESELUTION        1000000LL
#define SQUADS_ARCH_SUPPORT_DYNAMIC_ALLOCATION  configSUPPORT_DYNAMIC_ALLOCATION
#define SQUADS_ARCH_QUEUE_REGISTRY_SIZE         configQUEUE_REGISTRY_SIZE
#endif