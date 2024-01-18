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

#include "core/eventgroup.hpp"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"

#include "esp_log.h"

namespace squads  {
    //-----------------------------------
    //  construtor
    //-----------------------------------
    eventgroup::eventgroup(const char* strName)
    	: m_pHandle(nullptr) {
		sprintf(m_strName, "evg_%s", strName);
    }

    //-----------------------------------
    //  construtor
    //-----------------------------------
    eventgroup::eventgroup(native_handle_type handle)
        : m_pHandle(handle) {
        	if( !is_init() ) ESP_LOGE(m_strName, "the given handle is NULL this group will not work!!");
	}

    //-----------------------------------
    //  deconstrutor
    //-----------------------------------
    eventgroup::~eventgroup() {
        if( is_init() ) {
            vEventGroupDelete(  (EventGroupHandle_t )m_pHandle);
        }
    }

    //-----------------------------------
    //  sync
    //-----------------------------------
    eventgroup::event_bit_type eventgroup::sync( const event_bit_type uxBitsToSet, const event_bit_type uxBitsToWaitFor, unsigned int xTicksToWait){

		if( is_init() )
			return xEventGroupSync(  (EventGroupHandle_t )m_pHandle, uxBitsToSet, uxBitsToWaitFor, xTicksToWait);
		else {
			ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
		}
    }

    //-----------------------------------
    //  is_bit
    //-----------------------------------
    bool eventgroup::is_bit( const event_bit_type uxBitToWaitFor, uint32_t timeout) {

		event_bit_type _uxBits;

		_uxBits = wait(uxBitToWaitFor, false, true, timeout);
		return ( ( _uxBits & uxBitToWaitFor ) != 0 );
    }

    //-----------------------------------
    //  wait
    //-----------------------------------
    eventgroup::event_bit_type eventgroup::wait( const event_bit_type uxBitsToWaitFor,
                                        bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout) {

		event_bit_type _uxBits = 0;

		if( is_init() )
			_uxBits = xEventGroupWaitBits( (EventGroupHandle_t )m_pHandle,
										uxBitsToWaitFor,
										xClearOnExit ? pdTRUE : pdFALSE,
										xWaitForAllBits ? pdTRUE : pdFALSE,
										timeout);
		else {
			ESP_LOGE(m_strName, "the event group handle is not created, call create first");
		}



		return _uxBits;
    }

    //-----------------------------------
    //  clear
    //-----------------------------------
    eventgroup::event_bit_type eventgroup::clear(const event_bit_type uxBitsToClear) {
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

        if(xPortInIsrContext()) {
            return xEventGroupClearBitsFromISR((EventGroupHandle_t )m_pHandle, uxBitsToClear);
        } else {
            return xEventGroupClearBits((EventGroupHandle_t )m_pHandle, uxBitsToClear);
        }
    }

    //-----------------------------------
    //  get
    //-----------------------------------
    eventgroup::event_bit_type eventgroup::get() {
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

        if(xPortInIsrContext()) {
            return xEventGroupGetBitsFromISR((EventGroupHandle_t )m_pHandle);
        } else {
            return xEventGroupGetBits((EventGroupHandle_t )m_pHandle);
        }
    }

    //-----------------------------------
    //  set
    //-----------------------------------
    eventgroup::event_bit_type eventgroup::set(const event_bit_type uxBitsToSet) {
    	if(! is_init() ) {
    		ESP_LOGE(m_strName, "the event group handle is not created, call create first");
			return portMAX_DELAY;
    	}

        event_bit_type success;

        if(xPortInIsrContext()) {
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            success = xEventGroupSetBitsFromISR((EventGroupHandle_t )m_pHandle, uxBitsToSet, &xHigherPriorityTaskWoken);
            if(xHigherPriorityTaskWoken)
                _frxt_setup_switch();
        } else {
            success = xEventGroupSetBits((EventGroupHandle_t )m_pHandle, (event_bit_type)uxBitsToSet);
        }
        return success;
    }

    //-----------------------------------
    //  init_internal
    //-----------------------------------
    void eventgroup::init_internal() {

        m_pHandle = xEventGroupCreate();

		if( m_pHandle == NULL ) ESP_LOGE(m_strName, "out of mem xEventGroupCreate - failed");
	}

	//-----------------------------------
    //  create
    //-----------------------------------
	int eventgroup::create() {
		if( m_pHandle == nullptr )
			init_internal();

		return m_pHandle != nullptr ? 0 : 1;
	}

	//-----------------------------------
    //  set_name
    //-----------------------------------
	void eventgroup::set_name(const char* strName) {
		ESP_LOGI(m_strName, "rename the event group to %s", strName);
		sprintf(m_strName, "evg_%s", strName);
    }
}

#endif