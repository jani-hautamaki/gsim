//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

#ifndef STRUTIL_H
#define STRUTIL_H

#ifdef __cplusplus
extern "C" {
#endif

int rrnx_substr(
    char *dest,
    const char *src,
    unsigned int offset,
    unsigned int len
);

int rrnx_trim_trailing(char *s);

int rrnx_substr_trimmed2(
    char *dest,
    const char *src,
    int offset,
    int len
);

int rrnx_substr_trimmed(
        char *buffer,
        const char *line,
        int offset,
        int len
);

void rrnx_replace_fortran_exponent(char *s);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
