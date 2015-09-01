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
 * Nodes for GPS navigation message files.
 *
 */

/*
 * TODO #1:
 * Some of the broadcast orbit fields are defined as integers,
 * even though the file allows them to be floating-points.
 *
 * These fields should probably be doubles, and leave it
 * to the caller to decide whether to convert them into ints,
 * and what to do when the value is not an integer.
 *
 *
 * TBC:
 * Different GNSS have different kind of broadcast orbit data.
 * The data struct names should reflect this.
 * That is, the naming convention for system specific nodes
 * should be rrnx_<sysname>_<func>, where <sysname> is
 * a three-letter abbreviation for the system.
 *
 * TBC:
 * It is not possible for broadcast_orbitN structs
 * to appear in a random order. In other words, they are not
 * independent of each other. This suggests that these
 * structs should be put into a single big struct that
 * represents the set of repeating 8 lines (epoch+broadcast orbits).
 *
 */

#ifndef RRNX_NODES_NAV_H
#define RRNX_NODES_NAV_H

// rrnx_navmsg
#include "rrnx_basetypes_nav.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// PAYLOAD STRUCTS: GPS NAV MESSAGE FILE
//============================================================================

/**
 * Ionosphere parameters A0-A3 of almanac (ION ALPHA).
 * This is node is specific to GPS.
 */
struct rrnx_ion_alpha {
	/**
	 * Ionospheric parameters A0-A3 of almanac, 4(D12.4).
	 */
	double alpha[4];
};

typedef struct rrnx_ion_alpha rrnx_ion_alpha;

/**
 * Ionospheric parameters B0-B3 of almanac (ION BETA).
 * This is node is specific to GPS.
 */
struct rrnx_ion_beta {
	/**
	 * Ionospheric parameters B0-B3 of almanac, 4(D12.4).
	 */
	double beta[4];
};

typedef struct rrnx_ion_beta rrnx_ion_beta;

/**
 * UTC parameters of almanac (DELTA-UTC: A0,A1,T,W).
 * This is node is specific to GPS.
 */
struct rrnx_delta_utc {
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
	int T;

	/**
	 * Reference time for UTC data, GPS week, (I9).
	 * The week number is continuous, not modulo 1024.
	 */
	int T_week;
};

typedef struct rrnx_delta_utc rrnx_delta_utc;

/**
 * Leap seconds information (LEAP SECONDS).
 *
 * TBD:
 * Is this header record (with this interpretation) specific to GPS?
 *
 */
struct rrnx_leap_seconds {
	/**
	 * Delta time due to leap seconds (I6).
	 * Number of leap secods since 6-Jan-1980.
	 */
	int leap_seconds;
};

typedef struct rrnx_leap_seconds rrnx_leap_seconds;

/**
 * NAV msg observation record
 */
struct rrnx_datarecord_nav {
	rrnx_navmsg navmsg;
};

typedef struct rrnx_datarecord_nav rrnx_datarecord_nav;


#ifdef __cplusplus
} // extern "C"
#endif

#endif

