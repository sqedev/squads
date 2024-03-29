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

#ifndef __SQUADS_EVENTGROUP_H__
#define __SQUADS_EVENTGROUP_H__

#include "config.hpp"

#include "algorithm.hpp"
#include "defines.hpp"
#include "uint128.hpp"


#include <stdint.h>

namespace squads {
     /**
     * @brief Wrapper class around FreeRTOS's implementation of a event_group.
     * @ingroup base
     */
    class eventgroup  {
    public:
        using this_type = eventgroup;
        using native_handle_type = void*;
        using event_bit_type = SQUADS_THREAD_CONFIG_TICK_TYPE;
        /**
         * @brief Consruct a new event group.
         *
         * @param strName The name for this event group, only for debugging.
         *
         * @note Although event groups are not related to ticks, for internal implementation
         * reasons the number of bits available for use in an event group is dependent
         * on the configUSE_16_BIT_TICKS setting in FreeRTOSConfig.h.  If
         * configUSE_16_BIT_TICKS is 1 then each event group contains 8 usable bits (bit
         * 0 to bit 7).  If configUSE_16_BIT_TICKS is set to 0 then each event group has
         * 24 usable bits (bit 0 to bit 23).  The event_bit_type type is used to store
         * event bits within an event group.
         *
         * @note Version >2.30 call create after this!
         */
        eventgroup(const char* strName = "unamed");
        /**
         * @brief Consruct from a FreeRTOS created event group
         */
        eventgroup(native_handle_type handle);

        /**
         * @brief  Delete an event group
         * @note Tasks that are blocked on the event group will be
         * unblocked and obtain 0 as the event group's value.
         */
        virtual ~eventgroup();

        /**
         * @brief Create the event group.
         * @return
         *		- ERR_MNTHREAD_NULL: Error on create the event group.
         *		- NO_ERROR: The event group are sucessfull created
         */
         int create();

        /**
         * @brief  Allow two or more tasks to use an event group to sync each other.
         *
         * @param uxBitsToSet The bits to set in the event group before determining
         * if, and possibly waiting for, all the bits specified by the uxBitsToWait
         * parameter are set.
         *
         * @param uxBitsToWaitFor A bitwise value that indicates the bit or bits to test
         * inside the event group.  For example, to wait for bit 0 and bit 2 set
         * uxBitsToWaitFor to 0x05.  To wait for bits 0 and bit 1 and bit 2 set
         * uxBitsToWaitFor to 0x07.  Etc.
         *
         * @param xTicksToWait The maximum amount of time (specified in 'ticks') to wait
         * for all of the bits specified by uxBitsToWaitFor to become set.
         *
         * @return The value of the event group at the time either the bits being waited
         * for became set, or the block time expired.  Test the return value to know
         * which bits were set.  If xEventGroupSync() returned because its timeout
         * expired then not all the bits being waited for will be set.  If
         * xEventGroupSync() returned because all the bits it was waiting for were
         * set then the returned value is the event group value before any bits were
         * automatically cleared.
         */
        event_bit_type sync( const event_bit_type uxBitsToSet, const event_bit_type uxBitsToWaitFor,
                    unsigned int xTicksToWait = SQUADS_PORTMAX_DELAY);

        /**
         * @brief Block to wait for one bit to be set within a previously created event group.
         *
         * This function cannot be called from an interrupt.
         *
         * @param uxBitToWaitFor A bitwise value that indicates the bit
         * or bits to test inside the event group.
         *
         * @param xTicksToWait The maximum amount of time (specified in
         * 'ticks') to wait for one/all (depending on the xWaitForAllBits
         * value) of the bits specified by uxBitToWaitFor to become set.
         *
         * @return If true then was set the bit and false if not.
         */
        bool is_bit( const event_bit_type uxBitToWaitFor, uint32_t timeout);

		/**
         * @brief Block to wait for one or more bits to be set within a
         * previously created event group.
         *
         * This function cannot be called from an interrupt.
         *
         * @param uxBitsToWaitFor A bitwise value that indicates the bit
         * or bits to test inside the event group.
         *
         * @param xClearOnExit If xClearOnExit is set to true then any bits
         * set in the value passed as the uxBitsToWaitFor parameter will be
         * cleared in the event group before eventgroup::wait returns
         * if eventgroup::wait returns for any reason other than a
         * timeout. The timeout value is set by the xTicksToWait parameter.
         * If xClearOnExit is set to false then the bits set in the event
         * group are not altered when the call to eventgroup::wait
         * returns.
         *
         * @param xWaitForAllBits is used to create either a logical AND
         * test (where all bits must be set) or a logical OR test (where
         * one or more bits must be set) as follows:
         *
         *     @par    If xWaitForAllBits is set to true then
         *             eventgroup::wait will return when either all the
         *             bits set in the value passed as the uxBitsToWaitFor
         *             parameter are set in the event group or the specified
         *             block time expires.
         *
         *     @par   If xWaitForAllBits is set to false then
         *             eventgroup::wait will return when any of the bits
         *             set in the value passed as the uxBitsToWaitFor
         *             parameter are set in the event group or the specified
         *             block time expires.
         *
         * @param xTicksToWait The maximum amount of time (specified in
         * 'ticks') to wait for one/all (depending on the xWaitForAllBits
         * value) of the bits specified by uxBitsToWaitFor to become set.
         *
         * @return The value of the event group at the time either the event
         * bits being waited for became set, or the block time expired. The
         * current value of the event bits in an event group will be
         * different to the returned value if a higher priority task or
         * interrupt changed the value of an event bit between the calling
         * task leaving the Blocked state and exiting the
         * eventgroup::wait function. Test the return value to know
         * which bits were set. If eventgroup::wait returned because its
         * timeout expired then not all the bits being waited for will be
         * set. If eventgroup::wait returned because the bits it was
         * waiting for were set then the returned value is the event group
         * value before any bits were automatically cleared because the
         * xClearOnExit parameter was set to true.
         */
        event_bit_type wait( const event_bit_type uxBitsToWaitFor,
                    bool xClearOnExit, bool xWaitForAllBits, uint32_t timeout = SQUADS_PORTMAX_DELAY);

        /**
         * @brief  Clear bits (flags) within an event group.
         *
         *  @param uxBitsToClear A bitwise value that indicates the bit or
         *  bits to clear in the event group.
         *
         *  @return The value of the event group before the specified bits
         *  were cleared.
         */
        event_bit_type clear(const event_bit_type uxBitsToClear);

        /**
         * @brief Returns the current value of the event bits (event flags) in an
         * event group.
         *
         * @return The value of the event bits in the event group at the time
         * eventgroup::get was called.
         */
        event_bit_type get();

        /**
         * @brief Set bits (flags) within an event group.
         *
         * @param uxBitsToSet A bitwise value that indicates the bit or bits
         * to set in the event group.
         *
         * @return The value of the event group at the time the call to
         * eventgroup::set returns
         */
        event_bit_type set(const event_bit_type uxBitsToSet);

        /**
         * @brief Get the FreeRTOS Event Group handle.
         * @return The FreeRTOS Event Group handle.
         */
        native_handle_type get_handle() { return m_pHandle; }

        /**
         * @brief Is the eventgroup initilisiert?
         * @return True The eventgroup is initilisiert and false if not.
         */
        bool is_init() { return m_pHandle != NULL; }

        /**
         * @brief Get the debug name.
         * @return The debug name of this event group.
         */
        const char* get_name() { return m_strName; }

        /**
         * @brief Set a new debug name for this event group.
         * @param strName The name of this class.
         */
        void set_name(const char* strName);
	private:
		/**
		 * @brief Initialisert the eventgroup
		 */
		void init_internal();
    private:
        /**
         *  FreeRTOS Event Group handle.
         */
        native_handle_type m_pHandle;
		/**
		 * The name of this event group, for debuging.
		 */
        char m_strName[16];
    };
}
#endif