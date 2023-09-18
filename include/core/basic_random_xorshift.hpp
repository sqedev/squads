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


#ifndef __SQUADS_BACIC_RANDOM_XOR_H__
#define __SQUADS_BACIC_RANDOM_XOR_H__


#include "basic_random_impl.hpp"

namespace squads {
	/**
     * @brief Pseudro-Random Number Generator based on xor_shift128++ generator
     *
     * @note Based on https://codingforspeed.com/using-faster-psudo-random-generator-xorshift/
     *
     * @author Amber-Sophia Schröck
     */
	class basic_ramdom_xorshift : public squads::basic_random_impl<unsigned int>  {
     public:
		using base_type = basic_random_impl<unsigned int> ;
          using this_type = basic_ramdom_xorshift;

          using seed_t = typename base_type::seed_t;
          using result_type = typename base_type::result_type;
          using rand8_type = typename base_type::rand8_type;
          using rand16_type = typename base_type::rand16_type;
          using rand32_type = typename base_type::rand32_type;
	
		basic_ramdom_xorshift(seed_t startSeed)
            : base_type(startSeed) { init(startSeed); }

		 /**
         * @brief Get a random unsigned char number
         *
         * @return A random unsigned char number
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

        virtual seed_t get_seed() override { return m_xorandArray[0]; }
        virtual void set_seed(seed_t seed) override { init(seed); }
	private:
		void init(seed_t seed);
	private:
		unsigned long m_xorandArray[4];
	};
}
#endif // __MINILIB_BASIC_RAMDOM_XORSHIFT_H__
