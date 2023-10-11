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
#ifndef __SQUADS_ATOMIC_WAITSTATE_H__
#define __SQUADS_ATOMIC_WAITSTATE_H__

#include "arch/arch_utils.hpp"


#include "defines.hpp"
#include "flags.hpp"
#include "core/task.hpp"


namespace squads {
    namespace atomic {
         template <class TTYPE, class TaskType>
        struct basic_wait_state {
            TTYPE m_waiters{0};
            mutex m_locked;
            TaskType m_convar;
            uint64_t m_version;

            // Get the wait state for a given address.
            static basic_wait_state &for_address( void *__address) noexcept {
                constexpr uintptr_t count = 16;
                static basic_wait_state w[count];
                return w[(reinterpret_cast<uintptr_t>(__address) >> 2) % count];
            }

            void notify(unsigned int timeout) noexcept {
                if (m_waiters.load() != 0) {
                    m_locked.lock(timeout);
                    m_version++;
                    m_locked.unlock();
                }
                m_convar.notify_all();
            }

            template <typename TPred>
            void wait(TPred pred, unsigned int timeout) {
                for (int i = 0; i < 10; i++) {
                    if (pred())
                        return;
                    arch::arch_yield();
                }

                m_waiters.fetch_add(1, memory_order::SeqCst);

                uint64_t prev = [&] {
                    m_locked.lock(timeout);
                    uint64_t ver = m_version;
                    m_locked.unlock();

                    return ver;
                }();

                while (!pred()) {
                    m_locked.lock(timeout); 
                    if (m_version == prev) {
                        m_convar.wait(m_locked, timeout );
                    }
                    prev = m_version;
                }
                m_waiters.fetch_sub(1, memory_order::Release);
            }
        };
    }
}

#endif
