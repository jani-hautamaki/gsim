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

#include "rrnx_string.h"

#include <stdlib.h> // malloc, free
#include <stdio.h> // vsnprintf
#include <stdarg.h> // va_list, va_start, va_end


extern rrnx_string *rrnx_str_alloc(void) {
	return rrnx_str_alloc_size(RRNX_DEFAULT_STRING_SIZE);
}

extern rrnx_string *rrnx_str_alloc_size(size_t size) {
	rrnx_string *s = malloc(sizeof(rrnx_string));
	s->text = malloc(size);
	s->size = size;
	// Empty string by default
	rrnx_str_reset(s);
	return s;
}

extern void rrnx_str_free(rrnx_string *s) {
	free(s->text);
	s->text = NULL;
	s->size = 0;
	free(s);
}

extern void rrnx_str_printf(rrnx_string *s, const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	// Write at most "s->size" bytes
	// (including the terminating null byte).
	vsnprintf(s->text, s->size, fmt, ap);
        va_end(ap);
}

extern void rrnx_str_reset(rrnx_string *s) {
	s->text[0] = '\0';
}

extern void rrnx_str_resize(rrnx_string *s, size_t size) {
	s->text = realloc(s->text, size);
	s->size = size;
}

extern void rrnx_str_strcpy(rrnx_string *s, const char *cstr) {
	int max_size = s->size - 1;
	int i = 0;
	for (i = 0; i < max_size; i++) {
		char c = cstr[i];
		if (c == '\0') {
			// Stop here
			break;
		}
		// Otherwise, copy the char
		s->text[i] = c;
	}
	// Terminate the string
	s->text[i] = '\0';
}

