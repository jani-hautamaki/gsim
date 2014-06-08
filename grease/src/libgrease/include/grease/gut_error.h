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

#ifndef GUT_ERROR_H
#define GUT_ERROR_H

/** No error */
#define GUT_E_OK                0

/** Syntax error while parsing a number */
#define GUT_E_SYNTAX            -1

/** The underlying library call resulted in an unhandled error. */
#define GUT_E_OTHER             -2

/** Either an overflow or an underflow occurred. */
#define GUT_E_RANGE             -3

/** Application-specific error. */
#define GUT_E_USER              -4

// gut_datafile
//==============

/** malloc() failed; out of memory. */
#define GUT_E_NOMEM             -5

/** Syscall failed; consult errno. */
#define GUT_E_SYSCALL           -6

/** Unexpected end-of-file. */
#define GUT_E_EOF               -7

/**
 * Human-readable error message for the error code.
 */
//extern const char *gut_strerror(int err);

#endif
