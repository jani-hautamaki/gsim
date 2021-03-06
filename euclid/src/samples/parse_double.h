//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

/**
 * parse_double() - a helper function for libeuclid samples.
 */

#ifndef PARSE_DOUBLE_H
#define PARSE_DOUBLE_H

// errno
#include <errno.h>

// isspace
#include <ctype.h>

// strtod
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static int parse_double(const char *s, double *val) {
        char *end;

        errno = 0;
        *val = strtod(s, &end);

        if ((end == s) || (errno != 0)) {
                // Either empty string or syscall error.
                return 0;
        }

        // Skip trailing ws
        while (isspace(*end)) end++;

        if (*end != '\0') {
                // Unexpected char
                return 0;
        }

        return 1;
}



#ifdef __cplusplus
} // extern "C"
#endif

#endif
