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

#ifndef RRNX_STRING_H
#define RRNX_STRING_H

#include <stddef.h> // size_t
#include <stdarg.h> // va_list

#ifdef __cplusplus
extern "C" {
#endif

/** Default memory size for a string. */
#define RRNX_DEFAULT_STRING_SIZE 0x200

/**
 * Simple string object
 */
struct rrnx_string {
	/**
	 * The text content
	 */
	char *text;

	/**
	 * Memory size allocated for the text.
	 */
	size_t size;
};

typedef struct rrnx_string rrnx_string;

//============================================================================
// METHODS
//============================================================================

extern rrnx_string *rrnx_str_alloc(void);
extern rrnx_string *rrnx_str_alloc_size(size_t size);
extern rrnx_string *rrnx_str_clone(rrnx_string *src);
extern void rrnx_str_free(rrnx_string *s);

extern void rrnx_str_format(rrnx_string *s, const char *fmt, ...);
extern void rrnx_str_vformat(rrnx_string *s, const char *fmt, va_list args);

extern void rrnx_str_concat(rrnx_string *s, const rrnx_string *q);
extern void rrnx_str_concat_vformat(rrnx_string *s, const char *fmt, va_list args);

extern void rrnx_str_reset(rrnx_string *s);

extern int rrnx_str_resize(rrnx_string *s, size_t size);

extern void rrnx_str_strcpy(rrnx_string *s, const char *cstr);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
