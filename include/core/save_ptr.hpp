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
#ifndef __SQUADS_SAVE_PTR_H__
#define __SQUADS_SAVE_PTR_H__

#include "config.hpp"
#include "defines.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

namespace squads {


    template <typename T>
    class basic_save_ptr   {
    public:
        using value_type = T;
        using element_type = T;
        using pointer = T*;
        using reference = T&;
        using self_type = basic_save_ptr<T>;

        basic_save_ptr() : m_ptr(0)                 { }
        explicit basic_save_ptr(T *v) : m_ptr(v)    { }

        ~basic_save_ptr()                           { if(m_ptr) delete m_ptr; }

        value_type const *get() const               { return m_ptr;  }
        value_type *get()                           { return m_ptr; }

        value_type const &operator *() const        { return *m_ptr; }
        value_type &operator *()                    { return *m_ptr;  }
        value_type const *operator->() const        {  return m_ptr; }
        value_type *operator->()                    {  return m_ptr; }

        value_type *release() {
            value_type *tmp = m_ptr;
            m_ptr = NULL;
            return tmp;
        }
        void reset(value_type *p = NULL) {
            if(m_ptr != NULL)  delete m_ptr;
            m_ptr = p;
        }
        void swap(self_type &other) {
            value_type *tmp = other.m_ptr;
            other.m_ptr = m_ptr;
            m_ptr = tmp;
        }

        basic_save_ptr(const basic_save_ptr &other) = delete;
        basic_save_ptr const &operator=(const basic_save_ptr &other) = delete;

    private:
        pointer m_ptr;
    };

    template <typename T>
    using save_ptr = basic_save_ptr<T>;


    /**
    * @brief Make a save pointer
    * @tparam T Value type of the pointer.
    * @tparam Args Argument for the object.
    */
    template<typename T, typename... Args >
    inline save_ptr<T> make_save(Args&&... args) {
        return save_ptr<T>(new T (squads::forward<Args>(args)...) );
    }

    /**
     * @brief swap a save pointer.
     */
    template <typename T>
    inline void swap(save_ptr<T> & a, save_ptr<T> & b) {
        a.swap(b);
    }

}



#endif
