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

#ifndef RRNX_NAVREADER_H
#define RRNX_NAVREADER_H

#include "rrnx_string.h" // rrnx_string
#include "rrnx_list.h" // rrnx_list
#include "rrnx_error.h"
#include "rrnx_basetypes_nav.h" // rrnx_navmsg
#include "rrnx_filereader.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// CONSTANTS
//============================================================================

/**
 * Default size for the line buffer.
 * Lines in RINEX files should be at most 80 chars wide.
 * This generously more than needed.
 */
#define RRNX_DEFAULT_WORKBUF_SIZE 512

//============================================================================
// DATA TYPES
//============================================================================

/**
 * This data type should be opaque.
 */
struct rrnx_navreader {
	/**
	 * For reading lines from the underlying file.
	 */
	rrnx_filereader *fr;

        /**
         * Parsed nodes as a linked-list.
         */
        rrnx_list *nodelist;

	/**
	 * Current navmsg that is being built or NULL.
	 */
	rrnx_navmsg *cur_navmsg;

	/**
	 * Human-readable error message.
	 */
	rrnx_string *errmsg;

	/**
	 * Error code
	 */
	//rrnx_errno err;
	int err;

	/**
	 * Parser state
	 */
	int state;

	/**
	 * Null-transition indicator
	 */
	int eps;

	/**
	 * An internal buffer for miscellaneous purposes.
	 */
	char *workbuf;

	/**
	 * Size of the internal buffer.
	 */
	unsigned int workbuf_size;

};

typedef struct rrnx_navreader rrnx_navreader;

//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

rrnx_navreader *rrnx_navr_alloc(void);
void rrnx_navr_free(rrnx_navreader *navreader);

//============================================================================
// METHODS: OTHER METHODS
//============================================================================

void rrnx_navr_readfile(
    rrnx_navreader *navreader,
    const char *filename
);

int rrnx_navr_consume(
    rrnx_navreader *navreader,
    const char *line
);

/**
 * Transfer the ownership of the built navdata to the caller.
 */
rrnx_list *rrnx_navr_release_nodelist(rrnx_navreader *navreader);

//============================================================================
// METHODS: ERROR MANAGEMENT
//============================================================================

int rrnx_navr_errno(const rrnx_navreader *navreader);
const char *rrnx_navr_strerror(const rrnx_navreader *navreader);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
