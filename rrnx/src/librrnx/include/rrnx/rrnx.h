//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

/*
 *
 * Reference:
 *
 * Gurtner W, Estey L.
 * RINEX: The Received Independent Exchange Format Version 2.11.
 * 10 Dec 2007 [updated 26 Jun 2012].
 * Available from: ftp://igs.org/pub/data/format/rinex211.txt
 *
 */

#ifndef RRNX_H
#define RRNX_H

// Error codes.
#include "rrnx_error.h"

//#include "rrnx_common.h" // Included through rrnx_nav and rrnx_obs
#include "rrnx_file_nav.h"
#include "rrnx_file_obs.h"

#ifdef __cplusplus
extern "C" {
#endif


// OTHER METHODS
//===============

/**
 * Read a NAV file.
 * Returns error code.
 */
int rrnx_read_navfile(const char *filename, rrnx_file_nav **nav);

/**
 * Allocate and initialize a NAV file object.
 * Provided for convenience.
 */
rrnx_file_nav *rrnx_alloc_navfile(void);

/**
 * Free NAV file object.
 * Provided for convenience.
 */
void rrnx_free_navfile(rrnx_file_nav *nav);



/*
char rrnx_system2char();
unsigned int rrnx_char2system();
*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif

