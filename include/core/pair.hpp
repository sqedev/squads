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
*<https://www.gnu.org/licenses/>
*/

#ifndef __SQUADS_COMPRESSED_PAIR_H__
#define __SQUADS_COMPRESSED_PAIR_H__

#include "config.hpp"

#include "type_traits.hpp"
#include "algorithm.hpp"
#include "ebo_storage.hpp"
#include "void.hpp"

namespace squads {

	/**
	 * A simple compressed pair
	 *
	 * @tparam T type for first value.
	 * @tparam U type for second value.
	 */
	template <typename TFIRST, typename TSECOND>
	class basic_pair  {
	public:
		using self_type = basic_pair<TFIRST, TSECOND>;

		using first_value_type 		 = TFIRST;
		using first_reference 		 = TFIRST&;
		using first_const_reference  = const TFIRST&;
		using first_pointer			 = TFIRST*;
		using second_value_type 	 = TSECOND;
		using second_reference 		 = TSECOND&;
		using second_const_reference = const TSECOND&;
		using second_pointer		 = TSECOND*;

		constexpr basic_pair() = default;
		explicit  basic_pair(first_const_reference f) noexcept
			: m_first(f) { }

		explicit basic_pair(second_const_reference s) noexcept
			: m_second(s) { }

		constexpr basic_pair(first_const_reference f, second_const_reference s) noexcept
			: m_first(f), m_second(s) { }

		constexpr basic_pair(const self_type& other) noexcept
				: basic_pair(other.first(), other.second() ) { }

				  first_reference 		 first()  noexcept 		 { return m_first.get(); }
		constexpr first_const_reference  first()  const noexcept { return m_first.get(); }

				  second_reference 		 second() noexcept 		 { return m_second.get(); }
		constexpr second_const_reference second() const noexcept { return m_second.get(); }

		void swap(self_type& o) {
			squads::swap(m_first,  o.m_first);
			squads::swap(m_second, o.m_second);
		}

		self_type& operator = (const self_type& rhs) noexcept {
			squads::swap(m_first,  rhs.m_first);
			squads::swap(m_second, rhs.m_second);
		}

		bool operator == (const self_type& rhs) noexcept {
			if(m_first != rhs.m_first) return false;
			return m_second == rhs.m_second;
		}
		bool operator != (const self_type& rhs) noexcept {
			if(m_first == rhs.m_first) return false;
			return m_second != rhs.m_second;
		}
	private:
		ebo_storage<T, 0>  m_first;
		ebo_storage<U, 1>  m_second;
	};

	template<typename TFIRST, typename TSECOND>
	basic_pair<TFIRST, TSECOND> make_pair(const TFIRST& a, const TSECOND& b) {
		return basic_pair<TFIRST, TSECOND>(a, b);
	}

	template <typename ForwardIterator, typename TComp>
	inline constexpr basic_pair<ForwardIterator,ForwardIterator>  equal_range (ForwardIterator first,
																				ForwardIterator last,
																				const TComp& value) {
		basic_pair<ForwardIterator,ForwardIterator> rv;

		rv.second = rv.first = squads::lower_bound (first, last, value);
		while (rv.second != last && !(value < *(rv.second)))
			++ rv.second;
		return rv;
	}

	template<typename TFIRST, typename TSECOND>
    using pair = basic_pair<TFIRST, TSECOND>;

	template <typename TFIRST, typename TSECOND>
	struct is_pod< basic_pair<TFIRST, TSECOND> > {
		enum {
			value = ( (is_pod<TFIRST>::value || is_fundamental<TFIRST>::value) &&
					  (is_pod<TSECOND>::value || is_fundamental<TSECOND>::value) )
		};
	};
}

#endif // __SQUADS_COMPRESSED_PAIR_H__
