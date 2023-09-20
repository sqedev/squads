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

#ifndef __SQUADS_TIMESPAN_H__
#define __SQUADS_TIMESPAN_H__

#include <stdint.h>
#include <time.h>

#include "config.hpp"
#include "algorithm.hpp"


namespace squads  {

	/**
	 * @brief This class represents a time span (using microseconds)
	 */
	class basic_timespan {
	public:
		using time_type = int64_t;
		using int_type= uint64_t;
		using self_type = basic_timespan;

		/**
		 * @brief Construct a new time span and initializes it to zero.
		 */
		basic_timespan();

		/**
		 * @brief Construct a new time span and initializes it with the given value.
		 */
		basic_timespan(time_type ms);

		/**
		 * @brief Construct a new time span and initializes it with the given timeval.
		 */
		basic_timespan(struct timeval& val);

		/**
		 * @brief Construct a new time span and initializes it the given values.
		 */
		basic_timespan(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t microSeconds = 0);

		/**
		 * @brief Construct a new time span from another time span.
		 */
		basic_timespan(const self_type& other);


		/**
		 * @brief Get the number of days.
		 */
		uint16_t get_days() const;

		/**
		 * @brief Get the number of (0 to 23) hours.
		 */
		uint8_t get_hours() const;

		/**
		 * @brief Get the number of (0 to 59) minutes.
		 */
		uint8_t get_minutes() const;

		/**
		 * @brief Get the number of  (0 to 59) seconds.
		 */
		uint8_t get_seconds() const;

		/**
		 * @brief Get the number of milliseconds.
		 */
		uint16_t get_milliseconds() const;

		/**
		 * @brief Get the number of microseconds.
		 */
		int_type get_microseconds() const;


		/**
		 * @brief Get the total number of hours.
		 */
		int_type get_total_hours() const;

		/**
		 * @brief Get the total number of minutes.
		 */
		int_type get_total_minutes() const;

		/**
		 * @brief Get the total number of seconds.
		 */
		int_type get_total_seconds() const;

		/**
		 * @brief Get the total number of milliseconds.
		 */
		int_type get_total_milliseconds() const;

		/**
		 * @brief Get the total number of microseconds.
		 */
		int_type get_total_microseconds() const;


		self_type& operator = (const self_type& timespan);

		self_type& operator = (time_type microseconds);

		self_type& operator = (struct timeval val);


		bool operator == (const self_type& ts) const
			{ return m_timeSpan == ts.m_timeSpan; }

		bool operator == (time_type microSeconds) const
			{ return m_timeSpan == microSeconds; }

		bool operator != (const self_type& ts) const
			{ return m_timeSpan != ts.m_timeSpan; }

		bool operator != (time_type microSeconds) const
			{ return m_timeSpan != microSeconds; }


		bool operator >  (const self_type& ts) const
			{ return m_timeSpan > ts.m_timeSpan; }

		bool operator >  (time_type microSeconds) const
			{ return m_timeSpan > microSeconds; }

		bool operator >= (const self_type& ts) const
			{ return m_timeSpan >= ts.m_timeSpan; }

		bool operator >= (time_type microSeconds) const
			{ return m_timeSpan >= microSeconds; }

		bool operator <  (const self_type& ts) const
			{ return m_timeSpan < ts.m_timeSpan; }

		bool operator <  (time_type microSeconds) const
			{ return m_timeSpan < microSeconds; }

		bool operator <= (const self_type& ts) const
			{ return m_timeSpan <= ts.m_timeSpan; }

		bool operator <= (time_type microSeconds) const
			{ return m_timeSpan <= microSeconds; }


		self_type& operator += (const self_type& other)
			{ m_timeSpan += other.m_timeSpan; return *this; }

		self_type& operator += (time_type ms)
			{ m_timeSpan += ms; return *this; }

		self_type& operator -= (const self_type& other)
			{ m_timeSpan -= other.m_timeSpan; return *this; }

		self_type& operator -= (time_type ms)
			{ m_timeSpan -= ms; return *this; }

		self_type operator + (const self_type& other) const
			{ return self_type(m_timeSpan + other.m_timeSpan); }

		self_type operator - (const self_type& other) const
			{ return self_type(m_timeSpan - other.m_timeSpan); }

		self_type operator + (time_type ms) const
			{ return self_type(m_timeSpan + ms); }

		self_type operator - (time_type ms) const
			{ return self_type(m_timeSpan - ms); }

		/**
		 * @brief Get the current time.
		 * @return The current time.
		 */
		static basic_timespan now();
	public:
		/**
		 * @brief Swap this time span with a other time span
		 */
		void swap(self_type& other) {
			squads ::swap(m_timeSpan, other.m_timeSpan);
		}

		/**
		 * @brief Assigns a new time span.
		 */
		self_type& assign(uint16_t days, uint8_t hours, uint8_t minutes, uint8_t seconds, uint16_t microSeconds);

		/**
		 * @brief Assigns a new time span.
		 */
		self_type& assign(struct timeval val);
	public:
		static basic_timespan from_ticks(const unsigned int& ticks);
		time_type to_ticks() const;
	private:
		/**
		 * @brief
		 */
		time_type m_timeSpan;
	};

	inline void swap(basic_timespan& s1, basic_timespan& s2) {
		s1.swap(s2);
	}

	using timespan_t = basic_timespan;
}

#endif 
