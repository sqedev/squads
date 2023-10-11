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
#ifndef __SQUADS_LATCH_H__
#define __SQUADS_LATCH_H__


#include "algorithm.hpp"
#include "atomic/atomic.hpp"

namespace squads {

    class latch {
    public:
        static constexpr ptrdiff_t max() { return UINT64_MAX; }

        constexpr explicit latch(ptrdiff_t expected) : m_val(expected) { }

        ~latch() = default;
        latch(const latch&) = delete;
        latch& operator=(const latch&) = delete;

        inline void  count_down(ptrdiff_t i = 1, unsigned int timeout) {
            auto const temp = m_val.fetch_sub(i, atomic::memory_order::Release);
            if (temp == i)
                m_val.notify_all(timeout);
        }

        bool try_wait() const {
            return m_val.load(atomic::memory_order::Acquire) == 0;
        }
        void wait(unsigned int timeout) const noexcept  {
            auto const pred = [this] { return this->try_wait(); };
            auto& s = atomic::atomic_long::waitstate_type::for_address((void*)&m_val);
            
            s.wait(pred, timeout);
        }
        void arrive_and_wait(ptrdiff_t up = 1, unsigned int timeout) {
            count_down(up, timeout);
            wait(timeout);
        }
    private:
         atomic::atomic_long m_val;

    };

}

#endif