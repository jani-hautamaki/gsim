//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
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

#ifndef GUT_ARGPARSER_H
#define GUT_ARGPARSER_H

// va_list
#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Default error message buffer size
 */
#define GUT_ARGPARSER_DEFAULT_ERRMSG_SIZE 256

/**
 * General-purpose argument parser.
 */
struct gut_argparser {

	// ERROR MANAGEMENT
	//==================

        /**
         * Computer-readable error state
         */
        int err;

        /**
         * Error message buffer size
         */
        int errmsg_size;

        /**
         * Human-readable error message
         */
        char *errmsg;


	// STATE MANAGEMENT
	//==================

        /**
         * Current state
         */
        int state;

	// CALLBACK MANAGEMENT
	//=====================

        /**
         * Current argument
         */
        const char *carg;

	// TBD: Are null-transitions needed?
	// int eps;

	/**
	 * Arbitrary pointer used by the callbacks.
 	 */
	void *itemptr;

	/**
	 * The state transition callback.
	 *
	 * User-defined method which processes
	 * the input data until it is consumed.
	 *
	 */
	void (*cycle)(struct gut_argparser *parser);
};

typedef struct gut_argparser gut_argparser;


// CONSTRUCTION & DESTRUCTION
//============================

gut_argparser *gut_argparser_create(void);
void gut_argparser_free(gut_argparser *parser);

void gut_argparser_init(gut_argparser *parser);
void gut_argparser_deinit(gut_argparser *parser);

// OTHER METHODS
//===============

void gut_argparser_errorfmt(
    gut_argparser *parser,
     const char *fmt, ...
);

void gut_argparser_verrorfmt(
    gut_argparser *parser,
    const char *fmt,
    va_list ap
);

int gut_argparser_has_error(const gut_argparser *parser);
const char *gut_argparser_strerror(const gut_argparser *parser);

void gut_argparser_set_consume_callback(
    gut_argparser *parser,
    void (*consume)(gut_argparser *parser, const char *carg)
);

void gut_argparser_parse(gut_argparser *parser, int argc, char *argv[]);

const char *gut_argparser_get_arg(const gut_argparser *parser);
void *gut_argparser_get_itemptr(gut_argparser *parser);

#ifdef __cplusplus
} // extern "C"
#endif

#endif
