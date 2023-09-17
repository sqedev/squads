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


#ifndef __SQUADS_BACIC_RANDOM_IMPL_H__
#define __SQUADS_BACIC_RANDOM_IMPL_H__

#include "numeric_limits.hpp"

namespace squads {
    /**
     * @brief An Intarface for all randoms classes in this library
     * @author Amber-Sophia Schröck
     * @tparam TSEEDTYPE Seed Type
     */
    template <typename TSEEDTYPE>
    class basic_random_impl {
    protected:
        /**
         * @brief Union for type convertion
         */
        union convers {
            unsigned int g32;
            unsigned char g[4];
            unsigned short g16[2];

        };
    public:
        using seed_t = TSEEDTYPE;
        using result_type = TSEEDTYPE;
        using rand8_type = unsigned char;
        using rand16_type = unsigned short;
        using rand32_type = unsigned int;

        /**
         * @brief Construct a new basic_random_impl object
         *
         * @param startSeed The start seed
         */
        basic_random_impl(seed_t startSeed)
            : m_startSeed(startSeed) { }

        /**
         * @brief Get a random unsigned char number
         *
         * @return A random unsigned char number
         */
        virtual rand8_type rand8() = 0;
        /**
         * @brief Get a random unsigned short number
         *
         * @return A random unsigned short number
         */
        virtual rand16_type rand16() = 0;
        /**
         * @brief Get a random unsigned int number
         *
         * @return A random unsigned int number
         */
        virtual rand32_type rand32() = 0;

        /**
         * @brief Get the current used Seed
         *
         * @return The current used Seed
         */
        virtual seed_t get_seed() { return m_startSeed; }
        /**
         * @brief Set the a new seed
         *
         * @param seed Tde new used seed
         */
        virtual void set_seed(seed_t seed) { m_startSeed = seed; }

        result_type operator () (bool b = true) const noexcept {
			return rand32();
        }

        result_type min() { return squads::numeric_limits<result_type>::min(); }
        result_type max() { return squads::numeric_limits<result_type>::max(); }
    protected:
        seed_t m_startSeed;
    };
}


#endif
