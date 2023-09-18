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
#ifndef __SQUADS_ATOMIC_FLAGS_H__
#define __SQUADS_ATOMIC_FLAGS_H__


#ifndef __ATOMIC_RELAXED
#define __ATOMIC_RELAXED		0
#endif
#ifndef __ATOMIC_CONSUME
#define __ATOMIC_CONSUME		1
#endif
#ifndef __ATOMIC_ACQUIRE
#define __ATOMIC_ACQUIRE		2
#endif
#ifndef __ATOMIC_RELEASE
#define __ATOMIC_RELEASE		3
#endif
#ifndef __ATOMIC_ACQ_REL
#define __ATOMIC_ACQ_REL		4
#endif
#ifndef __ATOMIC_SEQ_CST
#define __ATOMIC_SEQ_CST		5
#endif

namespace squads {
    namespace atomic {
         enum class memory_order : int {
            Relaxed = __ATOMIC_RELAXED,
            Consume = __ATOMIC_CONSUME,
            Acquire = __ATOMIC_ACQUIRE,
            Release = __ATOMIC_RELEASE,
            AcqRel = __ATOMIC_ACQ_REL,
            SeqCst = __ATOMIC_SEQ_CST
        };
        namespace modifier {

            enum  basic_atomic_gcc_memory_order_modifier  {
                Mask          = 0x0ffff,
                ModifierMask  = 0xffff0000,
                HleAcquire    = 0x10000,
                HleRelease    = 0x20000
            };
            using memory_order = basic_atomic_gcc_memory_order_modifier;
        }
        inline  memory_order operator | (memory_order mMemOrder, modifier::memory_order mod) {
            return memory_order(int(mMemOrder) | int(mod));
        }

        inline  memory_order operator & (memory_order mMemOrder, modifier::memory_order mod) {
            return memory_order(int(mMemOrder) & int(mod));
        }

        namespace internal {
            template <typename TFlag>
            struct atomic_flag {
                using flag_type = TFlag;
                flag_type __flag;

                constexpr atomic_flag(const flag_type& flag) : __flag(flag) { }
            };
        }
    }
}


#endif