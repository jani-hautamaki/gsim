//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
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

#include "grease/gut_argparser.h"
#include "grease/gut_error.h"

// malloc, free, NULL
#include <stdlib.h>
// vsnprintf
#include <stdio.h>
// va_list, va_start, va_end
#include <stdarg.h>

// CONSTRUCTION & DESTRUCTION
//============================

gut_argparser *gut_argparser_create(void) {
	gut_argparser *parser = NULL;
	int complete = 0;

	do {
		parser = malloc(sizeof(gut_argparser));
		if (parser == NULL) break; // out of memory

		// initialize
		gut_argparser_init(parser);

		complete = 1;
	} while (0);

	if (!complete) {
		gut_argparser_free(parser);
		parser = NULL;
	}

	return parser;
}

void gut_argparser_free(gut_argparser *parser) {
	gut_argparser_deinit(parser);
	free(parser);
}

/**
 * Reallocates the error message buffer to the requested size.
 * Returns a pointer on success, or NULL if the reallocation fails.
 */
void *gut_argparser_realloc_errmsg(gut_argparser *parser, int size) {
	// Free previous buffer, if any.
	if (parser->errmsg != NULL) {
		free(parser->errmsg);
		parser->errmsg = NULL;
		parser->errmsg_size = 0;
	}

	// Allocate a new buffer
	do {
		if (size <= 0) {
			// Free errmsg only; disables errmsg
			break;
		}

		parser->errmsg = malloc((unsigned int)size);
		if (parser->errmsg == NULL) {
			// Error, out of memory.
			parser->err = GUT_E_NOMEM;
			break;
		}

		// Success
		parser->errmsg_size = size;
	} while (0);

	return parser->errmsg;
}

void gut_argparser_init(gut_argparser *parser) {
	parser->err = GUT_E_OK;
	parser->errmsg_size = 0;
	parser->errmsg = NULL;
	parser->state = 0;
	parser->carg = NULL;

	// Attempt to allocate memory
	int complete = 0;
	do {
		// Allocate the default errmsg buffer size
		gut_argparser_realloc_errmsg(
		    parser,
		    GUT_ARGPARSER_DEFAULT_ERRMSG_SIZE
		);
		if (parser->err) break;

		complete = 1;
	} while (0);

	if (!complete) {
		gut_argparser_deinit(parser);
	}

	//return parser->err;
}

/**
 * Deinitialize the argparser.
 *
 * NOTE: this method is required to leave error code intact.
 */
void gut_argparser_deinit(gut_argparser *parser) {
	if (parser != NULL) {
		// Deallocate errmsg
		free(parser->errmsg);
		parser->errmsg = NULL;
		parser->errmsg_size = 0;
	}
}

// OTHER METHODS
//===============

void gut_argparser_errorfmt(
    gut_argparser *parser,
    const char *fmt, ...
) {
        va_list args;

	// Set error
	parser->err = GUT_E_USER;

	// Format human-readable error message
        va_start(args, fmt);
        vsnprintf(parser->errmsg, parser->errmsg_size, fmt, args);
        va_end(args);
}

void gut_argparser_verrorfmt(
    gut_argparser *parser,
    const char *fmt,
    va_list ap
) {
	// Set error
	parser->err = GUT_E_USER;
	// Format human-readable error message
        vsnprintf(parser->errmsg, parser->errmsg_size, fmt, ap);
}


int gut_argparser_has_error(const gut_argparser *parser) {
	return parser->err != GUT_E_OK;
}

int gut_argparser_errno(const gut_argparser *parser) {
	return parser->err;
}

const char *gut_argparser_strerror(const gut_argparser *parser) {
	return parser->errmsg;
}

void gut_argparser_parse(gut_argparser *parser, int argc, char *argv[]) {
	// For each argument
	for (int i = 1; i < argc; i++) {
		parser->carg = argv[i];
		// Call consume cb
		if (parser->cycle == NULL) continue;

		parser->cycle(parser);
	}
}

const char *gut_argparser_get_arg(const gut_argparser *parser) {
	return parser->carg;
}

void *gut_argparser_get_itemptr(gut_argparser *parser) {
	return parser->itemptr;
}

