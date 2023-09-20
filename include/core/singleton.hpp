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
#ifndef __SQUADS_SINGLETON_H__
#define __SQUADS_SINGLETON_H__

#include "config.h"
#include "atomic/atomic.hpp"
#include "copyable.hpp"
#include "autolock.hpp"

namespace squads {

    /**
	 * @brief Singleton with double-checked locking pattern with atomic and mutex lock
	 * @tparam T The type of the class to be make as singleton.
	 */
	template < typename T, class TLOCK>
    class basic_singleton {
		friend class T;
	public:
		using value_type = T;
        using reference = value_type&;
        using pointer = value_type*;
        using self_type = basic_singleton<T, TLOCK>;
        using lock_type = TLOCK;

		basic_singleton(self_type&& other) = delete;
		basic_singleton(const self_type&) = delete;

		static reference instance() {
			pointer _tmp = m_pInstance.load (atomic::memory_order::Consume);
			if(!_tmp) {

                LOCKED_SECTION(lock_type, m_tLock) {

                    _tmp = m_pInstance.load (atomic::memory_order::Consume);
                    if( !_tmp ) {
                        _tmp = new T();
                        m_pInstance.store (_tmp, atomic::memory_order::Release);
                    } 
                }

			}
			return tmp;
		}
	private:
		static atomic::_atomic<pointer> m_pInstance;
        static lock_type  m_tLock;
	};

    template < typename T, class TLOCK>
    atomic::_atomic<T*> basic_singleton<T, TLOCK>::m_pInstance;

    template < typename T, class TLOCK>
    TLOCK basic_singleton<T, TLOCK>::m_tLock;

    template < typename T, class TLOCK>
    using singleton = basic_singleton<T, TLOCK>; 
}

#endif