//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
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

/**
 * parse_long() - a helper function for parsing strings into longs
 */

#ifndef PARSE_LONG_H
#define PARSE_LONG_H

// errno
#include <errno.h>

// isspace
#include <ctype.h>

// strtod
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

static int parse_long(const char *s, long *val) {
	char *end;

	errno = 0;
	*val = strtol(s, &end, 10);

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
