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
#ifndef __SQUADS_ATOMIC_ATOMIC_H__
#define __SQUADS_ATOMIC_ATOMIC_H__

#include <stdint.h>

#include "defines.hpp"
#include "basic_atomic_impl.hpp"
#include "flags.hpp"


#ifndef __IMPL_ATOMIC_TASK_TYPE
#define __IMPL_ATOMIC_TASK_TYPE void
#endif


namespace squads {
    namespace atomic {
        /**
         * @brief Only for declar special version for types
         */
        template <typename T, class TTASKTYPE = __IMPL_ATOMIC_TASK_TYPE>
        struct _atomic : basic_atomic_impl<T, TTASKTYPE> {
            using base_type = basic_atomic_impl<T, TTASKTYPE>;
            using self_type = _atomic<T, TTASKTYPE>;
            using value_type = typename base_type::value_type;
            using difference_type =  typename base_type::value_type;
        };

        /**
         * @brief Special version for bool
         */
        template <>
        struct _atomic<bool, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<bool, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_BOOL_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<bool, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for char
         */
        template<>
        struct _atomic<char, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<char, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<char,__IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for signed char
         */
        template<>
        struct _atomic<signed char, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<signed char, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<signed char, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for short
         */
        template<>
        struct _atomic<short, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<short, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_SHORT_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<short, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for int
         */
        template<>
        struct _atomic<int, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<int, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_INT_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<int, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for long
         */
        template<>
        struct _atomic<long, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<long, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<long, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for long long
         */
        template<>
        struct _atomic<long long, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<long long, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<long long, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        //----------------
        /**
         * @brief Special version for unsigned char
         */
        template<>
        struct _atomic<unsigned char, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<unsigned char, __IMPL_ATOMIC_TASK_TYPE> {
            static constexpr bool is_always_lock_free  = ATOMIC_CHAR_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<unsigned char, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value) : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for short
         */
        template<>
        struct _atomic<unsigned short, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<unsigned short, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_SHORT_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<unsigned short, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for int
         */
        template<>
        struct _atomic<unsigned int, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<unsigned int, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_INT_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<unsigned int, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for long
         */
        template<>
        struct _atomic<unsigned long, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<unsigned long, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_LONG_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<unsigned long, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for char16_t
         */
        template<>
        struct _atomic<char16_t, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<char16_t, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_CHAR16_T_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<char16_t, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for char32_t
         */
        template<>
        struct _atomic<char32_t, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<char32_t, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_CHAR32_T_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<char32_t, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for wchar_t
         */
        template<>
        struct _atomic<wchar_t, __IMPL_ATOMIC_TASK_TYPE> : basic_atomic_impl<wchar_t, __IMPL_ATOMIC_TASK_TYPE>  {
            static constexpr bool is_always_lock_free  = ATOMIC_WCHAR_T_LOCK_FREE == 2;

            using base_type = basic_atomic_impl<wchar_t, __IMPL_ATOMIC_TASK_TYPE> ;
            using self_type = typename base_type::self_type;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;

            _atomic(const self_type&) = delete;
            _atomic& operator=(const self_type&) = delete;
            _atomic& operator=(const self_type&) volatile = delete;

            constexpr _atomic(value_type value)  : base_type(value) { }

            using base_type::operator value_type;
            using base_type::operator=;
        };

        /**
         * @brief Special version for pointer
         */
        template<typename T, class TTASK = __IMPL_ATOMIC_TASK_TYPE>
        struct _atomic_ptr : basic_atomic_impl<T*, TTASK>  {
            static constexpr bool is_always_lock_free  = ATOMIC_POINTER_LOCK_FREE == 2;

            using pointer = T*;
            using base_type = basic_atomic_impl<pointer, TTASK> ;
            using self_type = _atomic_ptr<pointer, TTASK>;
            using difference_type = squads::ptrdiff_t;

            _atomic_ptr() = default;
            ~_atomic_ptr() = default;

            constexpr _atomic_ptr(pointer value)  : base_type(value) { }

            _atomic_ptr(const self_type&) = delete;
            _atomic_ptr& operator=(const self_type&) = delete;
            _atomic_ptr& operator=(const self_type&) volatile = delete;

            using base_type::operator pointer;
            using base_type::operator=;

        };

        template<typename T>
        using atomic_ptr            = _atomic_ptr<T*>;

        // Signad basic types
        using atomic_bool           = _atomic<bool>;
        using atomic_char	        = _atomic<char>;
        using atomic_schar	        = _atomic<signed char>;
        using atomic_short	        = _atomic<short>;
        using atomic_int	        = _atomic<int>;
        using atomic_long	        = _atomic<long>;
        using atomic_int_least8_t	= _atomic<int_least8_t>;
        using atomic_int_least16_t	= _atomic<int_least16_t>;
        using atomic_int_least32_t	= _atomic<int_least32_t>;
        using atomic_int_least64_t	= _atomic<int_least64_t>;
        using atomic_int_fast8_t	= _atomic<int_fast8_t>;
        using atomic_int_fast16_t	= _atomic<int_fast16_t>;
        using atomic_int_fast32_t	= _atomic<int_fast32_t>;
        using atomic_int_fast64_t	= _atomic<int_fast64_t>;
        using atomic_int8_t         = _atomic<int8_t>;
        using atomic_int16_t        = _atomic<int16_t>;
        using atomic_int32_t        = _atomic<int32_t>;
        using atomic_int64_t        = _atomic<int64_t>;

        // unsigned types
        using atomic_uchar	        = _atomic<unsigned char>;
        using atomic_ushort	        = _atomic<unsigned short>;
        using atomic_uint	        = _atomic<unsigned int>;
        using atomic_ulong	        = _atomic<unsigned long>;
        using atomic_uint_least8_t	= _atomic<uint_least8_t>;
        using atomic_uint_least16_t	= _atomic<uint_least16_t>;
        using atomic_uint_least32_t	= _atomic<uint_least32_t>;
        using atomic_uint_least64_t	= _atomic<uint_least64_t>;
        using atomic_uint_fast8_t	= _atomic<uint_fast8_t>;
        using atomic_uint_fast16_t	= _atomic<uint_fast16_t>;
        using atomic_uint_fast32_t	= _atomic<uint_fast32_t>;
        using atomic_uint_fast64_t	= _atomic<uint_fast64_t>;
        using atomic_uint8_t        = _atomic<uint8_t>;
        using atomic_uint16_t       = _atomic<uint16_t>;
        using atomic_uint32_t       = _atomic<uint32_t>;
        using atomic_uint64_t       = _atomic<uint64_t>;


        // Spzial types
        using atomic_intptr_t	    = _atomic<intptr_t>;
        using atomic_uintptr_t	    = _atomic<uintptr_t>;
        using atomic_size_t	        = _atomic<size_t>;
        using atomic_ptrdiff_t	    = _atomic<ptrdiff_t>;
        using atomic_intmax_t	    = _atomic<intmax_t>;
        using atomic_uintmax_t	    = _atomic<uintmax_t>;
        using atomic_char16_t   	= _atomic<char16_t>;
        using atomic_char32_t	    = _atomic<char32_t>;
        using atomic_wchar_t	    = _atomic<wchar_t>;
        using atomic_sig_t          = _atomic<int>;

        template <typename TAtomicType, TAtomicType TTrue = true, TAtomicType TFalse = false>
        class basic_atomic_flag  {
        public:
            using flag_type = TAtomicType;
            using reference = TAtomicType&;

            constexpr basic_atomic_flag() noexcept
                : m_bFlag(0) { }

            constexpr basic_atomic_flag(const flag_type& flag) noexcept
                : m_bFlag(flag) { }

            basic_atomic_flag(const basic_atomic_flag&) noexcept = delete;

            reference operator=(const reference)          noexcept = delete;
            reference operator=(const reference) volatile noexcept = delete;

            void clear(memory_order order = memory_order::SeqCst) noexcept {
                m_bFlag.store(TFalse, order);
            }
            flag_type test_and_set(memory_order order = memory_order::SeqCst) noexcept {
                return m_bFlag.exchange(TTrue, order);
            }
            flag_type test(memory_order order = memory_order::SeqCst) const {
                return m_bFlag.load(order);
            }
        private:
            _atomic<flag_type> m_bFlag;
        };

        using atomic_flag = basic_atomic_flag<bool>;
    }
}
#endif