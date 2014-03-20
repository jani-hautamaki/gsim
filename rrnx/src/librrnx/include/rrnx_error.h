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

#ifndef RRNX_ERROR_H
#define RRNX_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// TYPES (TODO?)
//============================================================================

/**
 * Explicit typename to indicate that the integer result
 * is an error code.
 */
typedef int rrnx_errno;

//============================================================================
// CONSTANTS
//============================================================================

/** Default size for errmsg buffer. */
#define RRNX_DEFAULT_ERRMSG_SIZE 0x200


/** Default size for input/output or read/write buffer. */
/** TODO: This does not belong to here. */
#define RRNX_DEFAULT_IOBUFFER_SIZE 0x10000

//============================================================================
// ERROR CODES
//============================================================================

/** No error. */
#define RRNX_E_OK              0

/** System call failed. */
#define RRNX_E_SYSCALL        -1

/** File already opened. */
#define RRNX_E_ALREADY_OPEN   -2

/** End-of-file reached. */
#define RRNX_E_EOF            -3

/** Character buffer overflow; string cropped. */
#define RRNX_E_OVERFLOW       -4


//============================================================================
// METHODS
//============================================================================



#ifdef __cplusplus
} // extern "C"
#endif

#endif
