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
#ifndef __SQUADS_ATOMIC_INTERNAL_IMPL_H__
#define __SQUADS_ATOMIC_INTERNAL_IMPL_H__

#if defined(__GNUC__) || defined(__clang__) || defined(__xlc__)
    #include "__internal_atomic_gcc_impl.hpp"
    #define BASE_ATOMIC_TYPE squads::atomic::basic_atomic_gcc
    #define BASE_ATOMIC_SMART_POINTER squads::atomic::basic_smart_pointer_atomic_gcc

    #undef ATOMIC_VAR_INIT
    #define	ATOMIC_VAR_INIT(value)		{ .__tValue = (value) }
    #define	atomic_init(obj, value)		((void)((obj)->__tValue = (value)))
    #define ATOMIC_FLAG_INIT            { 0 }
#else
#error Unsupported compiler / system.
#endif

#include "waitstate.hpp"

namespace squads {
    namespace atomic {
        namespace internal {
            template<typename TATOMIC, class TSTATE>
            class notify_token {
            public:
                void notify_one(unsigned int timeout) noexcept { m_state->notify(timeout); }
                void notify_all(unsigned int timeout) noexcept { m_state->notify(timeout); }

                explicit notify_token(TSTATE& state) 
                    : m_state(&state) {}
            private:
                TSTATE* m_state;
            };
        }

        template <typename T, class TTASKTYPE>
        class basic_atomic_impl : public BASE_ATOMIC_TYPE<T> {
        public:
            using base_type = BASE_ATOMIC_TYPE<T>;
            using value_type = typename base_type::value_type;
            using difference_type = typename base_type::difference_type;
            using self_type = basic_atomic_impl<value_type, TTASKTYPE>;
            
            //using waitstate_type = basic_wait_state<basic_atomic_impl<uint64_t>, TTASKTYPE >;
            //using notify_type = internal::notify_token<self_type, waitstate_type>;
            //using vnotify_type = internal::notify_token<volatile self_type, waitstate_type>;

            basic_atomic_impl()  = default;
            ~basic_atomic_impl()  = default;

            basic_atomic_impl(const self_type&) = delete;
            basic_atomic_impl& operator=(const self_type&) = delete;
            basic_atomic_impl& operator=(const self_type&) volatile = delete;

            constexpr basic_atomic_impl(value_type value)  : base_type(value) { }

#if 0
            void notify_one(unsigned int timeout) 
                { get_notify_token().notify_one(timeout); }

            void notify_one(unsigned int timeout) volatile  
                { get_notify_token().notify_one(timeout); }
    
            void notify_all(unsigned int timeout)  
                { get_notify_token().notify_all(timeout); }
                
            void notify_all(unsigned int timeout) volatile 
                { get_notify_token().notify_all(timeout); }

            notify_type get_notify_token()  {
                waitstate_type& temp = waitstate_type::for_address(this);
                return notify_type{temp};
            }

            vnotify_type get_notify_token() volatile {
                waitstate_type& temp = waitstate_type::for_address(this);
                return notify_type{temp};
            }

            void wait(T old, memory_order mo = memory_order::SeqCst, unsigned int timeout = SQUADS_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT) const {
                
                auto pred = [mo, old, this]() { return this->load(mo) != old; };
                auto& s = waitstate_type::for_address(this);
                s.wait(pred, timeout);
            }

            void wait(T old, memory_order mo = memory_order::SeqCst, unsigned int timeout = SQUADS_THREAD_CONFIG_TIMEOUT_MUTEX_DEFAULT) const volatile {
                
                auto pred = [mo, old, this]() { return this->load(mo) != old; };
                auto& s = waitstate_type::for_address(this);
                s.wait(pred, timeout);
            }
#endif
        };
    }
}

#endif