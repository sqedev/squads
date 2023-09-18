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
#ifndef __SQUADS_AUTO_PTR_H__
#define __SQUADS_AUTO_PTR_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "atomic/atomic.hpp"

namespace squads {
        template <typename T, typename TRefType = atomic::atomic_size_t >
        class basic_weak_ptr {
        public:
            using value_type = T;
            using element_type = T;
            using reference = T&;
            using const_value_type = const value_type;
            using pointer = value_type*;
            using count_type = TRefType;


            using self_type = basic_weak_ptr<value_type, count_type>;
            using shared_type = basic_shared_ptr<value_type, count_type>;


            explicit basic_weak_ptr(value_type pValue = 0)
                : m_ptr(pValue), m_ref(1)  { }

            basic_weak_ptr( const self_type& r )
                : m_ptr(r.m_ptr), m_ref(r.m_ref)  { }

            basic_weak_ptr( shared_type& pShrd)
                : m_ptr(pShrd.get()), m_ref( pShrd.ref() ) {}

            template<class U, typename YRefType = count_type>
            basic_weak_ptr( const basic_weak_ptr<U, YRefType>& r )
                : m_ptr(r.m_ptr), m_ref(r.m_ref)  { }

            template<class U, typename YRefType = count_type>
            basic_weak_ptr( const basic_shared_ptr<U, YRefType>& pShrd)
                : m_ptr(pShrd.get()), m_ref(pShrd.ref() ) {}

            shared_type lock()                          { return shared_type(m_ptr); }
            bool expired()                              { return m_ref.get() == 0; }
            void reset()                                { self_type(0).swap(*this); }

            count_type use_count()                  	{ return m_ref.get(); }

            void swap(self_type& other) {
                squads::swap<pointer>(m_ptr, other.m_ptr);
                squads::swap<count_type >(m_ref, other.m_ref);
            }
            template<class Y, typename YRefType = atomic_size_t>
            bool owner_before( const basic_weak_ptr<Y, YRefType> & rhs ) {
                return m_ref < rhs.m_ref;
            }
            template<class Y, typename YRefType = atomic_size_t>
            bool owner_before( const basic_shared_ptr<Y, YRefType> & rhs ) {
                return m_ref < rhs.ref();
            }

            pointer get() const {
		        return static_cast<T*>(m_ptr);
	        }

            pointer operator->() const {
                assert(get() != 0);
                return this->get();
            }
            const_value_type& operator*() {
                assert(get() != 0);
                return *this->get();
            }

            operator bool() {
                return m_ptr != 0;
            }

            self_type& operator=( const self_type& r ) {
                m_ptr = r.m_ptr;
                m_ref = r.m_ref;
                return *this;
            }
        private:
        	pointer m_ptr;
            count_type m_ref;
        };

        template < typename T >
		using weak_ptr = basic_weak_ptr<T, size_t>;

		template < typename T >
		using weak_atomic_ptr = basic_weak_ptr<T, atomic_size_t>;

		/**
		 * @brief Make a weak pointer
		 * @tparam T Value type of the pointer.
		 * @tparam Args Argument for the object.
		 */
		template<typename T, typename... Args >
		inline weak_ptr<T>  make_weak(Args&&... args) {
			return weak_ptr<T>(new T (squads::forward<Args>(args)...) );
		}

		/**
		 * @brief Make a weak atomic pointer
		 * @tparam T Value type of the pointer.
		 * @tparam Args Argument for the object.
		 */
		template<typename T, typename... Args >
		inline weak_atomic_ptr<T> make_atomic_weak(Args&&... args) {
			return weak_atomic_ptr<T>(new T (squads::forward<Args>(args)...) );
		}
    }


#endif
