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
#ifndef __SQUADS_SCOPED_PTR_H__
#define __SQUADS_SCOPED_PTR_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

namespace squads {

    template <typename T>
    class basic_scoped_ptr  {
    public:
        using value_type = T;
        using element_type = T;
        using pointer = value_type*;
        using const_value_type = const value_type;
        using self_type = basic_scoped_ptr<value_type>;

        explicit basic_scoped_ptr( pointer pValue = 0 )  : m_pPointer( pValue ) { }
        ~basic_scoped_ptr() { }

        void swap(self_type& b)             { squads::swap<value_type*>(m_pPointer, b.m_pPointer); }
        void reset( pointer pValue = 0)     { self_type(pValue).swap(*this); }

        pointer get()                       { return m_pPointer; }

        pointer operator -> ()        { assert(m_pPointer != 0); return m_pPointer; }
        value_type& operator *()      { assert(m_pPointer != 0); return *m_pPointer; }

        operator bool()                     { return m_pPointer != 0; }
    private:
        pointer m_pPointer;
    };

    /**
     * @brief Make a scoped pointer
     * @tparam T Value type of the pointer.
     * @tparam Args Argument for the object.
     */
    template<typename T, typename... Args >
    inline basic_scoped_ptr<T> make_scoped(Args&&... args) {
        return basic_scoped_ptr<T>(new T (squads::forward<Args>(args)...) );
    }

    template <typename T>
    using scoped_ptr = basic_scoped_ptr<T>;


} // namespace squads

#endif
