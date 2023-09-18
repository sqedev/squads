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

#ifndef __SQUADS_ARCH_ESP32_H__
#define __SQUADS_ARCH_ESP32_H__

/**
 * All cunfig properties can override in your sdkconfig.h
 */
#ifdef ESP_BUILD_WITH_SDKCONFIG
#include "sdkconfig.h"
#endif

#define CHAR_BIT 8
#define SQUADS_THREAD_CONFIG_SIZE_TYPE          long unsigned int
#define SQUADS_THREAD_CONFIG_STACK_TYPE         unsigned long
#define SQUADS_THREAD_CONFIG_BASIC_ALIGNMENT     sizeof(unsigned char*)

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

#endif