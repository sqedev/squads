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
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <stdio.h>
#include <esp_attr.h>

#include "arch/arch_mutex_impl.hpp"
using namespace squads::arch;


bool arch_mutex_impl::create() {

    if(m_tType == mutex_type::simple) {
        m_pHandle = xSemaphoreCreateMutex();
    } else if (m_tType == mutex_type::recursive) {
        m_pHandle = xSemaphoreCreateRecursiveMutex();
           
    }
    if (m_pHandle) {  give(); return true; }
    return false; 
}
bool arch_mutex_impl::create_static(void* pStaticBuffer) {
    m_pHandle = xSemaphoreCreateRecursiveMutexStatic((StaticQueue_t *)pStaticBuffer);

    if (m_pHandle) {  give(); return true; }
    return false; 
}

bool arch_mutex_impl::take(unsigned int timeout) {

    if(m_tType == mutex_type::simple) {
        return (xSemaphoreTake((QueueHandle_t)m_pHandle, timeout) == pdTRUE);
    } else if (m_tType == mutex_type::recursive) {
        return (xSemaphoreTakeRecursive((QueueHandle_t)m_pHandle, timeout) == pdTRUE ); 
    }
    return false;
}

bool arch_mutex_impl::give() {

    if(m_tType == mutex_type::simple) {
        return (xSemaphoreGive((QueueHandle_t)m_pHandle) == pdTRUE);  
    } else if (m_tType == mutex_type::recursive) {
        return (xSemaphoreGiveRecursive((QueueHandle_t)m_pHandle) == pdTRUE);  
    }
    return false;
}


#endif