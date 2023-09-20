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

#ifndef __SQUADS_CHRONO_H__
#define __SQUADS_CHRONO_H__

#include "config.hpp"
#include "functional.hpp"
#include "type_traits.hpp"
#include "limits.hpp"
#include "ratio.hpp"
#include "timestamp.hpp"
#include "timespan.hpp"

namespace squads  {
    namespace chrono {

        template <typename Rep, typename Period = squads ::ratio<1>>
	    class basic_duration;

        namespace internal {
            template <typename> struct is_ratio                                               : squads ::false_type {};
		    template <intmax_t N, intmax_t D> struct is_ratio<squads ::ratio<N, D>>                : squads ::true_type {};
		    template <intmax_t N, intmax_t D> struct is_ratio<const squads ::ratio<N, D>>          : squads ::true_type {};
		    template <intmax_t N, intmax_t D> struct is_ratio<volatile squads ::ratio<N, D>>       : squads ::true_type {};
		    template <intmax_t N, intmax_t D> struct is_ratio<const volatile squads ::ratio<N, D>> : squads ::true_type {};

            template<typename> struct is_duration                                                            : squads ::false_type{};
		    template<typename Rep, typename Period> struct is_duration<basic_duration<Rep, Period>>                : squads ::true_type{};
		    template<typename Rep, typename Period> struct is_duration<const basic_duration<Rep, Period>>          : squads ::true_type{};
		    template<typename Rep, typename Period> struct is_duration<volatile basic_duration<Rep, Period>>       : squads ::true_type{};
		    template<typename Rep, typename Period> struct is_duration<const volatile basic_duration<Rep, Period>> : squads ::true_type{};
        
            template <typename TFDUR, typename TTDUR, typename TCOMP =
                typename ratio_divide<typename TFDUR::period_t, typename TTDUR::period_t>::type,
                typename TCOMR = typename squads ::decay<typename squads ::common_type<typename TTDUR::rep_t,
                                                                                 typename TFDUR::rep_t,
                                                                                  intmax_t>::type>::type,
            bool = TCOMP::num == 1,
            bool = TCOMP::den == 1>
		    struct duration_cast_impl;

            template <typename TFDUR, typename TTDUR, typename TCOMP, typename TCOMR>
            struct duration_cast_impl<TFDUR, TTDUR, TCOMP, TCOMR, true, true>
            {
                inline static TTDUR do_cast(const TFDUR& fd) {
                    return TTDUR(static_cast<typename TTDUR::rep_t>(fd.count()));
                }
            };

            template <typename TFDUR, typename TTDUR, typename TCOMP, typename TCOMR>
            struct duration_cast_impl<TFDUR, TTDUR, TCOMP, TCOMR, false, true>
            {
                inline static TTDUR do_cast(const TFDUR& d) {
                    return TTDUR(static_cast<typename TTDUR::rep_t>(static_cast<TCOMR>(d.count()) *
                                                                            static_cast<TCOMR>(TCOMP::num)));
                }
            };

            template <typename TFDUR, typename TTDUR, typename TCOMP, typename TCOMR>
            struct duration_cast_impl<TFDUR, TTDUR, TCOMP, TCOMR, true, false>
            {
                inline static TTDUR do_cast(const TFDUR& d) {
                    return TTDUR(static_cast<typename TTDUR::rep_t>(static_cast<TCOMR>(d.count()) /
                                                                            static_cast<TCOMR>(TCOMP::den)));
                }
            };

            template <typename TFDUR, typename TTDUR, typename TCOMP, typename TCOMR>
            struct duration_cast_impl<TFDUR, TTDUR, TCOMP, TCOMR, false, false>
            {
                inline static TTDUR do_cast(const TFDUR& d) {
                    return TTDUR(static_cast<typename TTDUR::rep_t>(static_cast<TCOMR>(d.count()) *
                                                                            static_cast<TCOMR>(TCOMP::num) /
                                                                            static_cast<TCOMR>(TCOMP::den)));
                }
            };
        }

        template <typename TTDUR, typename Rep, typename Period>
        inline typename squads ::enable_if<internal::is_duration<TTDUR>::value, TTDUR>::type
        duration_cast(const basic_duration<Rep, Period>& d) {
            return squads ::duration_cast_impl< typename basic_duration<Rep, Period>::this_type, TTDUR>::do_cast(d);
        }

        template <class TRep, class TPeriod = squads ::ratio<1>>
        class basic_duration {
        public:
            using this_type = basic_duration<TRep, TPeriod>;
            using rep_t = TRep;
            using period_t = TPeriod;

            template <class TTR2>
		    constexpr explicit basic_duration( const TTR2& rep )
		        : m_rep(static_cast<TRep>(rep)) {}

            template <class TTR2, class TTP2>
		    constexpr basic_duration(const basic_duration<TTR2, TTP2>& d2)
		        : m_rep(duration_cast<duration>(d2).count()) {}

            constexpr basic_duration() = default;
            basic_duration(const basic_duration&) = default;
            basic_duration& operator=(const basic_duration&) = default;

            constexpr rep_t count() const { return m_rep; }

            constexpr static this_type zero() { return duration(rep_t(0)); }
		    constexpr static this_type min()  { return duration(squads ::numeric_limits<rep_t>::min()); }
		    constexpr static this_type max()  { return duration(squads ::numeric_limits<rep_t>::max()); }

            constexpr inline this_type operator+() const { return *this; }
		    constexpr inline this_type operator-() const { return duration(0-m_rep); }

            inline this_type operator++(int)                  { return duration(m_rep++); }
            inline this_type operator--(int)                  { return duration(m_rep--); }
            inline this_type& operator++()                    { ++m_rep; return *this; }
            inline this_type& operator--()                    { --m_rep; return *this; }
            inline this_type& operator+=(const this_type& d)   { m_rep += d.count(); return *this; }
            inline this_type& operator-=(const this_type& d)   { m_rep -= d.count(); return *this; }
            inline this_type& operator*=(const rep_t& rhs)      { m_rep *= rhs; return *this; }
            inline this_type& operator/=(const rep_t& rhs)      { m_rep /= rhs; return *this; }
            inline this_type& operator%=(const rep_t& rhs)      { m_rep %= rhs; return *this; }
            inline this_type& operator%=(const this_type& d)   { m_rep %= d.count(); return *this; }

        private:
            rep_t m_rep;
        };

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type 
        operator+(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type;
            return _dur_t_(_dur_t_(lhs).count() + _dur_t_(rhs).count());
        }
        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type 
        operator-(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type;
            return _dur_t_(_dur_t_(lhs).count() - _dur_t_(rhs).count());
        }
        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type 
        operator%(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type;
            return _dur_t_(_dur_t_(lhs).count() % _dur_t_(rhs).count());
        }
        template <typename TR1, typename TP1, typename TR2>
        inline basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1> 
        operator*(const basic_duration<TR1, TP1>& lhs, const TR2& rhs) {
            using _dur_t_ = typename basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1>;

            return _dur_t_(_dur_t_(lhs).count() * rhs);
        }
        template <typename TR1, typename TR2, typename TP2>
        inline basic_duration<typename squads ::common_type<TR1, TR2>::type, TP2> 
        operator*(const TR1& lhs, const duration<TR2, TP2>& rhs) {
            using _dur_t_ = basic_duration<typename squads ::common_type<TR1, TR2>::type, TP2>;
            return _dur_t_(lhs * _dur_t_(rhs).count());
        }
        template <typename TR1, typename TP1, typename TR2>
        inline basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1> 
        operator/(const duration<TR1, TP1>& lhs, const TR2& rhs) {
            using _dur_t_ = basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1>;
            return _dur_t_(_dur_t_(lhs).count() / rhs);
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type 
        operator/(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs)
        {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type ;
            return _dur_t_(_dur_t_(lhs).count() / _dur_t_(rhs).count());
        }

        template <typename TR1, typename TP1, typename TR2>
        inline basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1> 
        operator%(const basic_duration<TR1, TP1>& lhs, const TR2& rhs) {
            using _dur_t_ = typename basic_duration<typename squads ::common_type<TR1, TR2>::type, TP1>;
            return _dur_t_(_dur_t_(lhs).count() % rhs);
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator==(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type ;
            return _dur_t_(lhs).count() == _dur_t_(rhs).count();
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator<(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _dur_t_ = typename squads ::common_type<basic_duration<TR1, TP1>, basic_duration<TR2, TP2>>::type ;
            return _dur_t_(lhs).count() < _dur_t_(rhs).count();
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator!=(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            return !(lhs == rhs);
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator<=(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            return !(rhs < lhs);
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator>(const basic_duration<TR1, TP1>& lhs,  const basic_duration<TR2, TP2>& rhs) {
            return rhs < lhs;
        }

        template <typename TR1, typename TP1, typename TR2, typename TP2>
        inline bool operator>=(const basic_duration<TR1, TP1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            return !(lhs < rhs);
        }

        template <typename TCLK, typename TDUR = typename TCLK::duration>
        class basic_time_point {
        public:
            using TCLK_t = TCLK; 
            using duration_t = TDUR;
            using this_type = basic_time_point<TCLK_t, duration_t>;

            using rep_t = typename duration_t::rep_t;
            using period_t = typename duration_t::period_t;
            

            inline constexpr basic_time_point() : m_dur(duration_t::zero()) {}
            constexpr explicit basic_time_point(const duration_t& other) : m_dur(other) {}

            template <typename TD2>
            inline constexpr basic_time_point( const basic_time_point<TCLK_t, TD2>& t)
                : m_dur(t.time_since_epoch()) {}

            constexpr duration_t time_since_epoch() const { return m_dur; }

            this_type& operator+=(const duration_t& d) { m_dur += d; return *this; }
            this_type& operator-=(const duration_t& d) { m_dur -= d; return *this; }

            static constexpr this_type min() { return this_type(duration_t::min()); }
            static constexpr this_type max() { return this_type(duration_t::max()); }
        private:
            duration_t m_dur;

        };

        template <typename ToTDUR, typename TCLK, typename TDUR>
        constexpr basic_time_point<TCLK, ToTDUR> time_point_cast(const basic_time_point<TCLK, TDUR>& t,
            typename squads ::enable_if<internal::is_duration<ToTDUR>::value>::type** = 0) {
            return basic_time_point<TCLK, ToTDUR>(duration_cast<ToTDUR>(t.time_since_epoch()));
        }

        template <class TCLK, class TD1, class TR2, class TP2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>
        operator+(const basic_time_point<TCLK, TD1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type> ;
            return _ctp_t_(lhs.time_since_epoch() + rhs);
        }

        template <class Rep1, class Period1, class TCLK, class TD2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD2, basic_duration<Rep1, Period1>>::type>
        operator+(const basic_duration<Rep1, Period1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD2, duration<Rep1, Period1>>::type> ;
            return _ctp_t_(lhs + rhs.time_since_epoch());
        }

        template <class TCLK, class TD1, class TR2, class TP2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>
        operator-(const basic_time_point<TCLK, TD1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type> ;
            return _ctp_t_(lhs.time_since_epoch() - rhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr typename squads ::common_type<TD1, TD2>::type operator-( const basic_time_point<TCLK, TD1>& lhs,
            const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() - rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator==(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() == rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator!=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(lhs == rhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator<(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() < rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator<=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(rhs < lhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator>(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return rhs < lhs;
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator>=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(lhs < rhs);
        }

        template <class TCLK, class TD1, class TR2, class TP2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>
        operator+(const basic_time_point<TCLK, TD1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>;
            return _ctp_t_(lhs.time_since_epoch() + rhs);
        }

        template <class Rep1, class Period1, class TCLK, class TD2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD2, basic_duration<Rep1, Period1>>::type>
        operator+(const basic_duration<Rep1, Period1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD2, basic_duration<Rep1, Period1>>::type> ;
            return _ctp_t_(lhs + rhs.time_since_epoch());
        }

        template <class TCLK, class TD1, class TR2, class TP2>
        inline constexpr basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>
        operator-(const basic_time_point<TCLK, TD1>& lhs, const basic_duration<TR2, TP2>& rhs) {
            using _ctp_t_ = basic_time_point<TCLK, typename squads ::common_type<TD1, basic_duration<TR2, TP2>>::type>;
            return _ctp_t_(lhs.time_since_epoch() - rhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr typename squads ::common_type<TD1, TD2>::type 
        operator-( const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() - rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator==(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() == rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator!=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(lhs == rhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator<(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return lhs.time_since_epoch() < rhs.time_since_epoch();
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator<=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(rhs < lhs);
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator>(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return rhs < lhs;
        }

        template <class TCLK, class TD1, class TD2>
        inline constexpr bool operator>=(const basic_time_point<TCLK, TD1>& lhs, const basic_time_point<TCLK, TD2>& rhs) {
            return !(lhs < rhs);
        }
        

        
        
        class system_clock {
        public:
            using rep = long long; 
            using period = squads ::ratio_multiply<squads ::ratio<SQUADS_ARCH_NSPER_TICK, 1>, nano>::type;
            using duration = chrono::basic_duration<rep, period>; 
            using time_point = chrono::basic_time_point<system_clock> ;

            // true if the time between ticks is always increases monotonically
            constexpr static bool is_steady = false;
#if 0
            // returns a time point representing the current point in time.
            static time_point now() {
                squads ::timespan_t ticks = squads ::task_t::get_self()->get_time_since_start();
                return time_point(duration(ticks.to_ticks()));
            }
#endif
        };


        

        template <class TRep, class TPeriod = squads ::ratio<1>>
        using duration = basic_duration<TRep, TPeriod>;

        template <typename TCLK, typename TDUR = typename TCLK::duration>
        using time_point = basic_time_point<TCLK, TDUR>;

        using nanoseconds = duration<long long, nano>;
        using microseconds = duration<long long, micro> ;
        using milliseconds = duration<long long, milli> ;
        using seconds = duration<long long> ;
        using minutes = duration<int, ratio<60>> ;
        using hours = duration<int, ratio<3600>> ;



    }
}

#endif