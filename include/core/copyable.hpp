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

#ifndef __SQUADS_COPYABLE_H__
#define __SQUADS_COPYABLE_H__

#include "config.hpp"

#define	SQUADS_ONCOPYABLE_CLASS		public squads::only_copyable_tag<void>
#define SQUADS_ONMOVABLE_CLASS		public squads::only_move_tag<void>
#define SQUADS_ONSIGLETN_CLASS		public squads::only_oneable_tag<void>

#define SQUADS_DEFAULT_CLASS		public squads::copy_move_tag<void>

namespace squads {
	/**
	 * @brief Not movable
	 */
	template <typename t>
	class only_copyable_tag {
 	public:
		using this_type = only_copyable_tag;

		only_copyable_tag() = default;

	  	only_copyable_tag(const this_type& other) = default;
	  	this_type& operator=(const this_type& other) = default;
	};

	/**
	 * @brief Not copyable
	 */
	template <typename t>
	class only_move_tag {
	public:
		using this_type = only_move_tag;

		only_move_tag() = default;

		only_move_tag(this_type&& other) = default;
		only_move_tag(const this_type&) = delete;

		this_type& operator=(const this_type&) = delete;
		this_type& operator=(this_type&& other) = default;
	};

	/**
	 * @brief Not copyable or movable
	 */
	template <typename t>
	class only_oneable_tag {
	public:
		using this_type = only_oneable_tag;

		only_oneable_tag() = default;

		only_oneable_tag(this_type&& other) = delete;
		only_oneable_tag(const this_type&) = delete;

		this_type& operator=(const this_type&) = delete;
		this_type& operator=(this_type&& other) = delete;
	};

	/**
	 * @brief copyable and movable
	 */
	template <typename t>
	class copy_move_tag {
	public:
		using this_type = copy_move_tag;

		copy_move_tag() = default;
	};
}

#endif 
