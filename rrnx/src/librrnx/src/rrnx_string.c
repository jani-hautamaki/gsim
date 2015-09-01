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

#include "rrnx/rrnx_string.h"

#include <stdlib.h> // malloc, free
#include <stdio.h> // vsnprintf
#include <stdarg.h> // va_list, va_start, va_end
#include <string.h> // memcpy


rrnx_string *rrnx_str_alloc(void) {
	return rrnx_str_alloc_size(RRNX_DEFAULT_STRING_SIZE);
}

rrnx_string *rrnx_str_alloc_size(size_t size) {
	int incomplete = 1;

	rrnx_string *s = malloc(sizeof(rrnx_string));
	if (s != NULL) do {
		// Initialie members
		s->text = NULL;
		s->size = 0;

		// Attempt further allocations

		s->text = malloc(size);
		if (s->text == NULL) {
			// Abort
			break;
		}
		s->size = size;

		// Empty string by default
		rrnx_str_reset(s);

		incomplete = 0;
	} while (0);

	if (incomplete) {
		rrnx_str_free(s);
		s = NULL;
	}

	return s;
}

rrnx_string *rrnx_str_clone(rrnx_string *src) {
	rrnx_string *s = NULL;
	if (src != NULL) do {
		// Attempt to allocate with same size
		s = rrnx_str_alloc_size(src->size);
		if (s == NULL) {
			// Allocation failed. Abort
			break;
		}
		// Copy contents
		memcpy(s->text, src->text, src->size);
	} while(0);

	return s;
}

void rrnx_str_free(rrnx_string *s) {
	if (s == NULL) {
		// Already freed
		return;
	}
	// Free text, if any.
	free(s->text);
	s->text = NULL;
	s->size = 0;

	// Free the object itself.
	free(s);
}

void rrnx_str_format(rrnx_string *s, const char *fmt, ...) {
	va_list ap;

	va_start(ap, fmt);
	// Write at most "s->size" bytes
	// (including the terminating null byte).
	vsnprintf(s->text, s->size, fmt, ap);
        va_end(ap);
}

// for vargargs
void rrnx_str_vformat(rrnx_string *s, const char *fmt, va_list args) {
	vsnprintf(s->text, s->size, fmt, args);
}

int rrnx_str_length(const rrnx_string *s) {
	if (s == NULL) {
		return -1;
	}
	// For convenience
	int max_length = s->size - 1;
	const char* text = s->text;

	int i = 0;
	while ((i < max_length) && (text[i] != '\0')) {
		i++;
	}

	return i;
}

void rrnx_str_concat(rrnx_string *s, const rrnx_string *q) {
	if ((s == NULL) || (q == NULL)) {
		return;
	}

	int len = rrnx_str_length(s);
	int length_left = s->size-1 - len;
	if (length_left <= 0) {
		// No space left.
		return;
	}

	// Copy all, and terminate
	memcpy(&s->text[len], q->text, length_left);

	// Terminate in any case.
	s->text[s->size-1] = '\0';
}

void rrnx_str_concat_vformat(rrnx_string *s, const char *fmt, va_list args) {
	int len = rrnx_str_length(s);

	// See how many bytes left in the string
	int size_left = s->size - len;
	// vsnprintf includes the string terminator
	vsnprintf(&s->text[len], size_left, fmt, args);
}


void rrnx_str_reset(rrnx_string *s) {
	s->text[0] = '\0';
}

int rrnx_str_resize(rrnx_string *s, size_t size) {
	void *newptr = realloc(s->text, size);

	if (newptr != NULL) {
		s->text = realloc(s->text, size);
		s->size = size;
		return 1;
	}

	return 0;
}

void rrnx_str_strcpy(rrnx_string *s, const char *cstr) {
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

