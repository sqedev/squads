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

#include "core/basic_random_lfsr.hpp"


#define __LFSR_PM32 0xB4BCD35C
#define __LFSR_PM31 0x7A5BC2E3

#define __LFSR_SHIFT(LFSR, POLYMASK) \
( ((LFSR & 1) != 0U) ? ( (LFSR >>= 1) ^ POLYMASK) : (LFSR >>= 1) )


namespace squads {
    unsigned char basic_random_lfsr::rand8() {
    basic_random_impl::convers tp;
    tp.g16[0] = rand16();
    return tp.g[0] & tp.g[1];
}
unsigned short basic_random_lfsr::rand16() {
    unsigned int _randomNumber =  0;

    __LFSR_SHIFT(m_uiPolyMask, __LFSR_PM32);

    _randomNumber  = __LFSR_SHIFT(m_uiPolyMask, __LFSR_PM32);
    _randomNumber ^= __LFSR_SHIFT(m_startSeed, __LFSR_PM31);

    return static_cast<unsigned short>(_randomNumber);
}
unsigned int basic_random_lfsr::rand32() {
    basic_random_impl::convers tp;
    tp.g16[0] = rand16();
    tp.g16[1] = rand16();

    return tp.g32;
}

}