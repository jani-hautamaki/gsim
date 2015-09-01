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

#ifndef RRNX_FILE_NAV_H
#define RRNX_FILE_NAV_H

// rrnx_navmsg
#include "rrnx_basetypes_nav.h"

// rrnx_list
#include "rrnx_list.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// DATA TYPES: GPS NAV MESSAGE FILE
//============================================================================

struct rrnx_iono {
	/**
	 * Ionospheric correction parameters A0-A3 of almanac, 4(D12.4).
	 */
	double alpha[4];

	/**
	 * Ionospheric correction parameters B0-B3 of almanac, 4(D12.4).
	 */
	double beta[4];
};

typedef struct rrnx_iono rrnx_iono;

struct rrnx_utc {

	/**
	 * a0 term of the polynomial, (D19.12).
	 */
	double a0;

	/**
	 * a1 term of the polynomial, (D19.12).
	 */
	double a1;

	/**
	 * Reference time for UTC data, GPS time of week [s], (I9).
	 */
	int tot;

	/**
	 * Reference time for UTC data, GPS week, (I9).
	 * Continuous week number, not modulo 1024.
	 */
	int tot_week;

	/**
	 * Delta time due to leap seconds [s], (I6).
	 * Number of leap seconds since 6-Jan-1980.
	 */
	int delta_ls;
};

typedef struct rrnx_utc rrnx_utc;

/**
 * Reprents a navigation message file as a list of individual nodes.
 */
struct rrnx_file_nav {

	// ADMINISTRATIVE DATA
	//=====================

	//int err;

	int has_iono_alpha;
	int has_iono_beta;
	int has_delta_utc;
	int has_leap_seconds;

	// PAYLOAD DATA
	//==============

	/**
	 * File format declaration
	 */
	rrnx_file_format format;

	/**
	 * File origin / provenance info.
	 */
	rrnx_file_info info;

	/**
	 * Ionospheric parameters of almanac
	 */
	rrnx_iono iono;

	/**
	 * UTC parameters of almanac.
	 */
	rrnx_utc utc;

	/**
	 * Navigation message records.
	 */
	rrnx_list *navmsg_list;
};

typedef struct rrnx_file_nav rrnx_file_nav;

// CONSTRUCTION & DESTRUCTION
//============================

rrnx_file_nav *rrnx_fnav_alloc(void);
void rrnx_fnav_free(rrnx_file_nav *nav);

// OTHER METHODS
//===============

rrnx_file_nav *rrnx_fnav_deserialize(const rrnx_list *nodelist);
//extern rrnx_list *rrnx_fnav_serialize(rrnx_file_nav *nav);

#ifdef __cplusplus
} // extern "C"
#endif

#endif

