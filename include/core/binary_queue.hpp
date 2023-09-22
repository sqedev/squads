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
#ifndef __SQUADS_BINARYQUEUE_H__
#define __SQUADS_BINARYQUEUE_H__

#include "queue.hpp"

namespace squads {
    /**
     * @brief This queue can only hold one item.
     * If sucessive Enqueue operations are called, that item is overwritten
     * with whatever the last item was.
     * 
     * @ingroup queue
     */
    template <typename T>
    class basic_binary_queue : public basic_queue<T, 1U> {
        using base_type = basic_queue<T, 1U>;
    public:
        using self_type = basic_binary_queue<T>;
        using value_type = typename base_type::value_type;
        using pointer = typename base_type::pointer;
        using reference = typename base_type::reference;
        using const_reference = typename base_type::const_reference;
        using difference_type = typename base_type::difference_type;
        using size_type = typename base_type::size_type;
        using iterator = basic_queue_iterator<T, self_type>;
        using const_iterator = const basic_queue_iterator<T, self_type>;
        using cointainer_type = typename base_type::cointainer_type;

        static const size_type TypeSize = sizeof(value_type);

        explicit basic_binary_queue() : base_type() { }    

		basic_binary_queue(const self_type& x )  : base_type(x) { } 
		basic_binary_queue(self_type&& x )  : base_type(x) { } 
        basic_binary_queue(initializer_list<value_type> ilist) : base_type(ilist) { }

        virtual bool    push(value_type&& x, unsigned int timeout = SQUADS_PORTMAX_DELAY) override {
            return m_archQueueType.overwrite(squads::move(x), timeout) == 0;
        }

        virtual bool    push(value_type& x, unsigned int timeout = SQUADS_PORTMAX_DELAY) override {
            return m_archQueueType.overwrite(x, timeout) == 0;
        }

    };

    template <typename T>
    using binary_queue = basic_binary_queue<T>;
}

#endif