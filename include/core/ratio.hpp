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


#ifndef __SQUADS_RATIO_H__
#define __SQUADS_RATIO_H__

#include "limits.hpp"

#define ABS(x)	        		((x) < 0 ? -(x) : (x))

namespace squads {
     template <intmax_t X, intmax_t Y>
	struct addition_overflow {
		static const bool c1 = (X <= 0 && 0 <= Y) || (Y < 0 && 0 < X);   
		static const bool c2 = ABS(Y) <= INTMAX_MAX - ABS(X);
		static const bool value = c1 || c2;
	};

    template <intmax_t X, intmax_t Y>
    struct multiply_overflow {
	    static const bool value = (ABS(X) <= (INTMAX_MAX / ABS(Y)));
    };

    template <intmax_t N = 0, intmax_t D = 1>
	class ratio  {
	public:
		static constexpr intmax_t num = N;
		static constexpr intmax_t den = D;
		typedef ratio<num, den> type;
	};

        namespace Internal {
            template <intmax_t X, intmax_t Y> struct gcd { static const intmax_t value = gcd<Y, X % Y>::value; };
            template <intmax_t X> struct gcd<X, 0>       { static const intmax_t value = X; };
            template <> struct gcd<0, 0>                 { static const intmax_t value = 1; };

            template<intmax_t X, intmax_t Y>
            struct lcm { static const intmax_t value = (X * (Y / gcd<X,Y>::value)); };


            template <intmax_t X, intmax_t Y>
            struct ct_add   { 
                static_assert(addition_overflow<X,Y>::value, "compile-time addition overflow"); 
                static const intmax_t value = X + Y; 
            };
            template <intmax_t X, intmax_t Y>
            struct ct_sub {
                static_assert(addition_overflow<X,-Y>::value, "compile-time addition overflow"); 
                static const intmax_t value = X - Y;
            };
            template <intmax_t X, intmax_t Y>
            struct ct_multi {
                static_assert(multiply_overflow<X,Y>::value, "compile-time multiply overflow"); 
                static const intmax_t value = X * Y;
            };

            // ct_simplify
            template <class R1>
            struct ct_simplify { 
                static const intmax_t divisor = Internal::gcd<R1::num, R1::den>::value; 
                static const intmax_t num = R1::num / divisor;
                static const intmax_t den = R1::den / divisor; 

                using ratio_type = ratio<num, den> ;
                using this_type = ct_simplify<R1> ;
            };


            template <intmax_t N1, intmax_t N2> intmax_t ct_add_v   = ct_add<N1, N2>::value;
            template <intmax_t N1, intmax_t N2> intmax_t ct_multi_v = ct_multi<N1, N2>::value;
            template <class R1, class R2> R2 ct_simplify_t          = ct_simplify<R1>::ratio_type;



            template <class R1, class R2>
            struct ratio_add {
                using type = typename ct_simplify < typename ratio <
                            ct_add < ct_multi<R1::num, R2::den>::value, ct_multi<R2::num, R1::den>::value >::value,							
                            ct_multi<R1::den, R2::den>::value >::type >::ratio_type ;
            };
            template <class R1, class R2>
            struct ratio_subtract {
                using type = typename ct_simplify < typename ratio <
                                    ct_sub < ct_multi<R1::num, R2::den>::value, ct_multi<R2::num, R1::den>::value >::value,							
                                    ct_multi<R1::den, R2::den>::value >::type >::ratio_type ;
            };

            template <class R1, class R2>
            struct ratio_multiply {
                using type = typename ct_simplify<typename ratio<ct_multi<R1::num, R2::num>::value, 
                                 ct_multi<R1::den, R2::den>::value>::type>::ratio_type ;
            };
            template <class R1, class R2>
            struct ratio_divide {
                using type =  typename ct_simplify<typename ratio<ct_multi<R1::num, R2::den>::value, 
                        ct_multi<R1::den, R2::num>::value>::type>::ratio_type;
            };

            template <class R1, class R2>
            struct ratio_equal  {
                static const bool value = (ct_simplify<R1>::num == ct_simplify<R2>::num) && (ct_simplify<R1>::den == ct_simplify<R2>::den);
            };
	} // namespace Internal

    template <class R1, class R2> using ratio_add      = typename Internal::ratio_add<R1, R2>::type;
	template <class R1, class R2> using ratio_subtract = typename Internal::ratio_subtract<R1, R2>::type;
	template <class R1, class R2> using ratio_multiply = typename Internal::ratio_multiply<R1, R2>::type;
	template <class R1, class R2> using ratio_divide   = typename Internal::ratio_divide<R1, R2>::type;

    template <class R1, class R2> struct ratio_equal         : public integral_constant<bool, Internal::ratio_equal<R1, R2>::value> {};
	template <class R1, class R2> struct ratio_not_equal     : public integral_constant<bool, !ratio_equal<R1, R2>::value> {};
	template <class R1, class R2> struct ratio_less          : public integral_constant<bool, Internal::ratio_less<R1, R2>::value> {};
	template <class R1, class R2> struct ratio_less_equal    : public integral_constant<bool, !ratio_less<R2, R1>::value> {};
	template <class R1, class R2> struct ratio_greater       : public integral_constant<bool, ratio_less<R2, R1>::value> {};
	template <class R1, class R2> struct ratio_greater_equal : public integral_constant<bool, !ratio_less<R1, R2>::value> {};


	typedef ratio<1, 1000000000               > nano;
	typedef ratio<1, 1000000                  > micro;
	typedef ratio<1, 1000                     > milli;
	typedef ratio<1, 100                      > centi;
	typedef ratio<1, 10                       > deci;
	typedef ratio<10, 1                       > deca;
	typedef ratio<100, 1                      > hecto;
	typedef ratio<1000, 1                     > kilo;
	typedef ratio<1000000, 1                  > mega;
	typedef ratio<1000000000, 1               > giga;

}


#endif