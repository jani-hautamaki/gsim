//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://code.google.com/p/gsim
//
//      Copyright (C) 2013-2014 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#ifndef GUT_PARSE_H
#define GUT_PARSE_H

#ifdef __cplusplus
extern "C" {
#endif

// The workhorses for integer types...

int gut_parse_long(const char *s, long *result);
int gut_parse_ulong(const char *s, unsigned long *result);

// The narrowing derivative methods...

int gut_parse_int(const char *s, int *result);
int gut_parse_uint(const char *s, unsigned int *result);

// The workhorses for floating-point types...

int gut_parse_double(const char *s, double *result);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
