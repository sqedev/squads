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


#ifndef __SQUADS_BACIC_RANDOM_LFSR_H__
#define __SQUADS_BACIC_RANDOM_LFSR_H__

#include "basic_random_impl.hpp"

namespace squads {
    /**
     * @brief Pseudro-Random Number Generator based on a 32-bit linear-feedback
     * shift register.  
     * 
     * @note For reference, the implementation is based on the
     * Maxim IC application note "AN4000"
     * http://www.maximintegrated.com/app-notes/index.mvp/id/4400
     * 
     * @author Amber-Sophia Schröck
     */ 
    class basic_random_lfsr : public basic_random_impl<unsigned int> {
    public:
        using base_type = basic_random_impl<unsigned int> ;
        using this_type = basic_random_lfsr;
        
        using seed_t = typename base_type::seed_t;
        using result_type = typename base_type::result_type;
        using rand8_type = typename base_type::rand8_type;
        using rand16_type = typename base_type::rand16_type;
        using rand32_type = typename base_type::rand32_type;

        basic_random_lfsr(seed_t startSeed = 0xFEABDC72, seed_t polyMask = 0xA1B3C6DF) 
            : basic_random_impl<seed_t>(startSeed), m_uiPolyMask(0xA1B3C6DF) { }

        /**
         * @brief Get a random rand8_type number 
         * 
         * @return A random rand8_type number
         */
        virtual rand8_type rand8() override;
        /**
         * @brief Get a random unsigned short number
         * 
         * @return A random unsigned short number 
         */
        virtual rand16_type rand16() override;
        /**
         * @brief Get a random unsigned int number
         * 
         * @return A random unsigned int number 
         */
        virtual rand32_type rand32() override;

        /**
         * @brief Get the polymask 
         * 
         * @return The polymask
         */
        seed_t get_polymask() { return m_uiPolyMask; }
        /**
         * @brief Set the a new polymask
         * 
         * @param mask The new polymask
         */
        void set_polymask(seed_t mask) { m_uiPolyMask = mask; }
        /**
         * @brief Set a new seed, with new polymask
         * 
         * @param seed The new seed
         * @param mask The new polymask
         */
        void set_seed(seed_t seed, seed_t mask) {
            m_startSeed = seed; m_uiPolyMask = mask; 
        }
    private:
        seed_t m_uiPolyMask;
    };

}

#endif
    