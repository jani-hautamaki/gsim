//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

#include "rrnx_strutil.h"

#include <string.h> // strlen()
#include <ctype.h> // isdigit()

int rrnx_substr(
    char *dest,
    const char *src,
    unsigned int offset,
    unsigned int len
) {
	// Translate src pointer by the offset
	while ((offset > 0) && (*src != '\0')) {
		offset--;
		src++;
	}

	// In effect, if the source is too short,
	// the pointer is already at the terminator,
	// and the destination will be made an empty string.

	int i;
	for (i = 0; i < len; i++) {
		char c = src[i];
		if (c == '\0') {
			// Halt here
			break;
		}
		// Otherwise copy
		dest[i] = c;
	}

	// Terminate
	dest[i] = '\0';

	return i;
}

int rrnx_trim_trailing(char *s) {
	int j = strlen(s);
	while ((j > 0) && (s[j-1] == ' ')) {
		j--;
	}
	s[j] = '\0';

	return j;
}

int rrnx_substr_trimmed2(
    char *dest,
    const char *src,
    int offset,
    int len
) {
	rrnx_substr(dest, src, offset, len);
	return rrnx_trim_trailing(dest);
}

int rrnx_substr_trimmed(
	char *buffer,
	const char *line,
	int offset,
	int len
) {
	// Determine the line length
	int linelen = strlen(line);

	// Line too short?
	if (linelen < offset) {
		buffer[0] = '\0';
		return 0;
	}

	// Otherwise trim leading spaces before substr
	const char *cptr = line+offset;

	while ((len > 0) && (*cptr == ' ')) {
		len--;
		cptr++;
	}

	// Copy as much as possible.
	// Halt when the length is consumed or the string terminated
	int i = 0;
	while ((len > 0) && (*cptr != '\0')) {
		buffer[i] = *cptr;
		i++;
		cptr++;
		len--;
	}

	// Trim trailing whitespaces
	while ((i > 0) && (buffer[i-1] == ' ')) {
		i--;
	}

	// Terminate string
	buffer[i] = '\0';

	return i;
}

void rrnx_replace_fortran_exponent(char *s) {
	int i = strlen(s);

	// Skip integer
	while ((i > 0) && (isdigit(s[i-1]))) {
		i--;
	}

	// Skip sign, if any
	if ((i > 0)
	    && ((s[i-1] == '+') || (s[i-1] == '-')))
	{
		i--;
	}

	// Now i-1 should point to an exponent, if there's such.
	if ((i > 0)
	    && ((s[i-1] == 'd') || (s[i-1] == 'D')))
	{
		// There's the fortran exp marker.
		// Replace it with one that is understood by libc.
		s[i-1] = 'e';
	}
}
