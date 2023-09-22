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
#ifndef __SQUADS_DEQUEUE_H__
#define __SQUADS_DEQUEUE_H__

#include "queue.hpp"

namespace squads {

    template <typename T, unsigned int maxItems = 32>
    class basic_dequeue : public basic_queue<T, maxItems> { 
        using base_type = basic_queue<T, maxItems> ;
    public:
        using self_type = basic_dequeue<T>;
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

        explicit basic_dequeue() : base_type() { }    

		basic_dequeue(const self_type& x )  : base_type(x) { } 
		basic_dequeue(self_type&& x )  : base_type(x) { } 
        basic_dequeue(initializer_list<value_type> ilist) : base_type(ilist) { }

        virtual iterator        begin() 		{ return iterator( m_pFront, m_archQueueType); }
        virtual const_iterator  begin() const 	{ return const_iterator( m_pFront, m_archQueueType); }

        virtual iterator        end() 			{ assert( 0 ); throw 0; }
        virtual const_iterator  end() const 	{ assert( 0 ); throw 0; }

        virtual reference       front()         { assert(!empty()); return *m_pFront; }
		virtual const_reference front() const   { assert(!empty()); return *m_pFront; }

        virtual reference       back()          { assert( 0 ); throw 0; }
		virtual const_reference back() const    { assert( 0 ); throw 0; }


        virtual bool    push(const value_type& value, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
             
            if( m_archQueueType.enqueue_front(&value) == 0) {
                    m_pFront = &value;
                    return true;
            }
            return false;
        }
		virtual bool    push(value_type&& x, unsigned int timeout = SQUADS_PORTMAX_DELAY) {
            if( m_archQueueType.enqueue_front(squads::move(x)) == 0) {
                    m_pFront = &squads::move(x);
                    return true;
            }
            return false;
        }
    };

    template <typename T, unsigned int maxItems = 32>
    using dequeue = basic_dequeue<T, maxItems>;
}

#endif