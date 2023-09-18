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
#ifndef __SQUADS_SHARED_PTR_H__
#define __SQUADS_SHARED_PTR_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

namespace squads {

    template < typename T, typename TRefType >
    class basic_shared_ptr   {
    public:
        using value_type = T;
        using element_type = T;
        using reference = T&;
        using const_value_type = const value_type;
        using pointer = value_type*;
        using ref_type = TRefType;

        using self_type = basic_shared_ptr<value_type, ref_type>;


        explicit basic_shared_ptr(pointer ptr )
            : m_ref(1), m_ptr(ptr)  { }

        basic_shared_ptr(const self_type& sp) {
            assert( (++m_ref != 0) );
            m_ptr = sp.m_ptr;
        }

        ~basic_shared_ptr() {
            if (--m_ref == 0) delete m_ptr;
        }

        pointer release() {
            pointer __px = this->get();
            if (--m_ref == 0) delete m_ptr;

            return __px;
        }
        void reset( pointer pValue = 0)
            { self_type(pValue).swap(*this); }

        ref_type ref() {
            return m_ref;
        }
        void swap(self_type& b) {
            squads::swap<pointer>(m_ptr, b.m_ptr);
            squads::swap<ref_type >(m_ref, b.m_ref);
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

        self_type& operator = (self_type& sp) {
            release();
            m_ptr = sp.m_ptr;
            m_ref = sp.m_ref;
            return *this;
        }
    private:
        ref_type m_ref;
        pointer m_ptr;
    };

    template < typename T, typename TRefType >
    void swap(basic_shared_ptr<T, TRefType>& a, basic_shared_ptr<T, TRefType>& b) {
        a.swap(b);
    }

    template < typename T >
    using shared_ptr = basic_shared_ptr<T, size_t>;

    template < typename T >
    using shared_atomic_ptr = basic_shared_ptr<T, atomic_size_t>;

    /**
     * @brief Make a shared pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline shared_ptr<T> make_shared(Args&&... args) {
        return shared_ptr<T>(new T (squads::forward<Args>(args)...) );
    }

    /**
     * @brief Make a shared atomic pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline shared_atomic_ptr<T> make_atomic_shared(Args&&... args) {
        return shared_atomic_ptr<T>(new T (squads::forward<Args>(args)...) );
    }
}


#endif
