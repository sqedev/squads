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
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#include "arch/arch_queue_impl.hpp"


using namespace squads::arch;


int arch_queue_impl::create() {
    if(m_pHandle != NULL) return 1;

    m_pHandle = xQueueCreate(m_imaxItems, m_iitemSize );

    return (m_pHandle != NULL) ? 0 : 99;
}

int arch_queue_impl::destroy() {
    if(m_pHandle == NULL) return 99;

    vQueueDelete((QueueHandle_t)m_pHandle);
    m_pHandle = NULL;

    return 0;
}

int arch_queue_impl::enqueue_back(const void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return 99;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success =xQueueSendFromISR((QueueHandle_t)m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueSend((QueueHandle_t)m_pHandle, item, timeout );
    }

    return success == pdTRUE ? 0 : 1;
}
int arch_queue_impl::enqueue_front(const void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return 99;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xQueueSendToFrontFromISR((QueueHandle_t)m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueSendToFront((QueueHandle_t)m_pHandle, item, timeout );
    }

    return success == pdTRUE ? 0 : 1;
}
int arch_queue_impl::overwrite(void *item,  unsigned int timeout) {
    if (m_pHandle == NULL)
            return 2;

    if (xPortInIsrContext())
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        (void)xQueueOverwriteFromISR((QueueHandle_t)m_pHandle, item, &xHigherPriorityTaskWoken);

        if (xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    }
    else
    {
        (void)xQueueOverwrite((QueueHandle_t)m_pHandle, item);
    }
    return 0;
}
int arch_queue_impl::dequeue(void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return 99;

    if (xPortInIsrContext()) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;

        success = xQueueReceiveFromISR((QueueHandle_t)m_pHandle, item, &xHigherPriorityTaskWoken);

        if(xHigherPriorityTaskWoken)
            _frxt_setup_switch();
    } else {
        success = xQueueReceive((QueueHandle_t)m_pHandle, item, timeout);
    }

    return success == pdTRUE ? 0 : 1;
}
int arch_queue_impl::peek(void *item, unsigned int timeout) {
    BaseType_t success;

    if(m_pHandle == NULL) return 99;

    if (xPortInIsrContext()) {
        success = xQueuePeekFromISR((QueueHandle_t)m_pHandle, item);
    } else {
        success = xQueuePeek((QueueHandle_t)m_pHandle, item, timeout);
    }

    return success == pdTRUE ? 0 : 1;
}

bool arch_queue_impl::is_empty() const {
    unsigned int cnt = get_num_items();

    return cnt == 0 ? true : false;
}
bool arch_queue_impl::is_full() const {
    unsigned int cnt = get_left();

    return cnt == 0 ? true : false;
}

int arch_queue_impl::clear() {
    xQueueReset((QueueHandle_t)m_pHandle);
    return 0;
}


unsigned int arch_queue_impl::get_num_items() const {
    return uxQueueMessagesWaiting((QueueHandle_t)m_pHandle);
}


unsigned int arch_queue_impl::get_left() const {
    return uxQueueSpacesAvailable((QueueHandle_t)m_pHandle);
}

#endif