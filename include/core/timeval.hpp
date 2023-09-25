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

#ifndef __SQUADS_TIMEVAL_H__
#define __SQUADS_TIMEVAL_H__

#include "config.hpp"
#include <sys/time.h>
#include <time.h>

namespace squads {
     /**
     * Operator to sub two timevals
     *
     * @param lhs The left operater timeval
     * @param rhs The right operater timeval
     */
    static inline struct timeval operator - (const struct timeval &lhs, const struct timeval &rhs) {
        int s = lhs.tv_sec - rhs.tv_sec;
        int ns = lhs.tv_usec - rhs.tv_usec;
        if (ns < 0) {
            s--;
            ns += 1000000;
        }

        return timeval{s, ns};
    }

    /**
     * Operator to add two timevals
     *
     * @param lhs The left operater timeval
     * @param rhs The right operater timeval
     */
    static inline struct timeval operator + (const struct timeval &lhs, const struct timeval &rhs) {
        int s = lhs.tv_sec + rhs.tv_sec;
        int ns = lhs.tv_usec + rhs.tv_usec;
        if (ns >= 1000000) {
            s++;
            ns -= 1000000;
        }

        return timeval{s, ns};
    }
}

#endif