#include "config.hpp"

#if SQUADS_CONFIG_ARCH_ESP32 == 1
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

int arch_queue_impl::enqueue_back(void *item, unsigned int timeout) {
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
int arch_queue_impl::enqueue_front(void *item, unsigned int timeout) {
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

bool arch_queue_impl::is_empty() {
    unsigned int cnt = get_num_items();

    return cnt == 0 ? true : false;
}
bool arch_queue_impl::is_full() {
    unsigned int cnt = get_left();

    return cnt == 0 ? true : false;
}

int arch_queue_impl::clear() {
    xQueueReset((QueueHandle_t)m_pHandle);
    return 0;
}


unsigned int arch_queue_impl::get_num_items() {
    return uxQueueMessagesWaiting((QueueHandle_t)m_pHandle);
}


unsigned int arch_queue_impl::get_left() {
    return uxQueueSpacesAvailable((QueueHandle_t)m_pHandle);
}

#endif