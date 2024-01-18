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
#ifndef __SQUADS_ARCH_QUEUE_H__
#define __SQUADS_ARCH_QUEUE_H__

#include "config.hpp"
#include "defines.hpp"
#include "core/algorithm.hpp"
#include "core/functional.hpp"

namespace squads {
    namespace arch {
        class arch_queue_impl {
        public:
            /**
             *  ctor
             * 
             *  @param maxItems Maximum number of items this basic_queue can hold.
             *  @param itemSize Size of an item in a basic_queue.
             */
            arch_queue_impl(unsigned int maxItems, unsigned int itemSize) 
                : m_pHandle(0), m_imaxItems(maxItems), 
                  m_iitemSize(itemSize) { }

            arch_queue_impl(const arch_queue_impl& other) 
                : m_pHandle(other.m_pHandle), m_imaxItems(other.m_imaxItems), 
                  m_iitemSize(other.m_iitemSize) {  }

            arch_queue_impl(const arch_queue_impl&& other) 
                : m_pHandle(squads::move(other.m_pHandle)), m_imaxItems(squads::move(other.m_imaxItems)), 
                  m_iitemSize(squads::move(other.m_iitemSize)) {  }
            

            /**
             *  dtor
             */
            ~arch_queue_impl() {  }

            /**
             * Create the basic_queue
             * 
             *  @return '0': the basic_queue was created
             *          '1': the basic_queue is allready created
             *          '99': basic_queue can not created
             * 
             */ 
            int create();

            /**
             * Destroy the Queue
             * 
             *  @return '0' the basic_queue was destroyed 
             *          '2' the basic_queue is not created
             */
            int destroy();
            /**
             *  Add an item to the back of the basic_queue.
             *
             *  @param item The item you are adding.
             *  @param timeout How long to wait to add the item to the basic_queue 
             *  @return '0' the item was added, '1' on an error
             *          and '2' when the basic_queue not created
             */
            int enqueue_back(const void *item, unsigned int timeout = SQUADS_PORTMAX_DELAY);
            /**
             *  Add an item to the front of the basic_queue.
             *
             *  @param item The item you are adding.
             *  @param timeout How long to wait to add the item to the basic_queue 
             *  @return '0' the item was added, '1' on an error
             *          and '2' when the basic_queue not created
             */
            int enqueue_front(const void *item, unsigned int timeout = SQUADS_PORTMAX_DELAY);


            /**
             *  Make a copy of an item from the front of the basic_queue. 
             *  This will not remove it
             *
             *  @param item Where the item you are getting will be returned to.
             *  @param timeout How long to wait 
             *  @return '0' if an item was copied, '1' on error 
             *  and '2' when the basic_queue not created
             */
            int peek(void *item, unsigned int timeout = SQUADS_PORTMAX_DELAY);

            /**
             *  Remove an item from the front of the basic_queue.
             *
             *  @param item Where the item you are removing will be returned to.
             *  @param timeout How long to wait to remove an item to the basic_queue.
             *  @return '0' the item was removed, '1' on an error
             *          and '2' when the basic_queue not created
             */
            int dequeue(void *item,  unsigned int timeout = SQUADS_PORTMAX_DELAY);

            /**
             *  Overwritte an item of the basic_queue.
             *
             *  @param item The item you are overwritte.
             *  @param timeout How long to wait to add the item to the basic_queue 
             *  @return '0' the item was overwritte
             *          and '2' when the basic_queue not created
             */ 
            int overwrite(void *item,  unsigned int timeout = SQUADS_PORTMAX_DELAY);
            /**
             *  Remove all objects from the basic_queue.
             */
            int clear();

            /**
             *  How many items are currently in the basic_queue.
             *  @return the number of items in the basic_queue.
             */
            unsigned int get_num_items() const;

            /**
             *  How many empty spaves are currently left in the basic_queue.
             *  @return the number of remaining spaces.
             */
            unsigned int get_left() const;


            bool   is_created() const { return m_pHandle != NULL; }

            /**
             *  Is the basic_queue empty?
             *  @return true the basic_queue is empty and false when not
             */
            bool is_empty() const;
            /**
             *  Is the basic_queue full?
             */
            bool is_full() const;

            template <typename TRET = void*> 
            TRET get_handle() const noexcept { return (TRET*)m_pHandle; }

            void swap(arch_queue_impl& other) noexcept {
                squads::swap(m_pHandle, other.m_pHandle);
                squads::swap(m_imaxItems, other.m_imaxItems);
                squads::swap(m_iitemSize, other.m_iitemSize);
            }

            arch_queue_impl* operator = (arch_queue_impl& other) {
                m_pHandle = other.m_pHandle;
                m_imaxItems = other.m_imaxItems;
                m_iitemSize = other.m_iitemSize;
                return this;
            }

            arch_queue_impl* operator = (arch_queue_impl&& other) {
                m_pHandle = squads::move(other.m_pHandle);
                m_imaxItems = squads::move(other.m_imaxItems);
                m_iitemSize = squads::move(other.m_iitemSize);
                return this;
            }

            bool operator == (arch_queue_impl& other) {
                return (m_pHandle == other.m_pHandle);
            }
            bool operator != (arch_queue_impl& other) {
                return (m_pHandle != other.m_pHandle);
            }

        private:
            /**
             *  Arch basic_queue handle.
             */
            void*  m_pHandle;

            unsigned int m_imaxItems;
            unsigned int m_iitemSize;

        };

        inline void swap(arch_queue_impl& a, arch_queue_impl& b) noexcept {
            a.swap(b);
        }
    }
}

#endif