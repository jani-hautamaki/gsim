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

#ifndef RRNX_NAVREADER_H
#define RRNX_NAVREADER_H

#include "rrnx_string.h"
#include "rrnx_error.h"
#include "rrnx_filereader.h"
#include "rrnx_nav.h"



#ifdef __cplusplus
extern "C" {
#endif

/**
 * Default size for the line buffer.
 * Lines in RINEX files should be at most 80 chars wide.
 * This generously more than needed.
 */
#define RRNX_DEFAULT_WORKBUF_SIZE 512

/**
 * This data type should be opaque.
 */
struct rrnx_navreader {
	/**
	 * For reading lines from the underlying file.
	 */
	rrnx_filereader *fr;

	/**
	 * The target object that is being built.
	 */
	rrnx_nav *navdata;

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

	/**
	 * Buffer for the values in a broadcast orbit line.
	 */
	double v[4];

};

typedef struct rrnx_navreader rrnx_navreader;

extern rrnx_navreader *rrnx_navr_alloc(void);
extern void rrnx_navr_free(rrnx_navreader *navreader);

extern void rrnx_navr_readfile(
    rrnx_navreader *navreader,
    const char *filename
);

extern int rrnx_navr_consume(
    rrnx_navreader *navreader,
    const char *line
);


extern int rrnx_navr_errno(const rrnx_navreader *navreader);
extern const char *rrnx_navr_strerror(const rrnx_navreader *navreader);

/**
 * Transfer the ownership of the built navdata to the caller.
 */
extern rrnx_nav *rrnx_navr_pop(rrnx_navreader *navreader);


#ifdef __cplusplus
} // extern "C"
#endif

#endif
