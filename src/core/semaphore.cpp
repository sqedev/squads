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
#include "core/semaphore.hpp"


namespace squads {
    basic_binary_semaphore::basic_binary_semaphore() : m_bLock(false) { }
    
    int basic_binary_semaphore::lock(unsigned int timeout) noexcept {
        for (;;) {
            if (!m_bLock.exchange(true, atomic::memory_order::Acquire)) {
                break;
            }
            while (m_bLock.load(atomic::memory_order::Relaxed)) {
                squads::arch::arch_yield();
            }
        }
        return 0;
    }

    bool basic_binary_semaphore::try_lock() noexcept {
        return !m_bLock.load(atomic::memory_order::Relaxed) &&
            (!m_bLock.exchange(true, atomic::memory_order::Acquire));
    }

    int basic_binary_semaphore::unlock() noexcept {
        m_bLock.store(false, atomic::memory_order::Release);
        return 0;
    }

    
}