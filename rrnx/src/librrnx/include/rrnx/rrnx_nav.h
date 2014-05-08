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

/*
 * TBC:
 * Some of the broadcast orbit fields are defined as integers,
 * even though the file allows them to be floating-points.
 * These fields should probably be doubles, and leave it
 * to the caller to decide whether to convert them into ints,
 * and what to do when the value is not an integer.
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
 * indpenedent of each other. This suggests that these
 * structs should be put into a single big struct that
 * represents the set of repeating 8 lines (epoch+broadcast orbits).
 *
 */

#ifndef RRNX_NAV_H
#define RRNX_NAV_H

#include "rrnx_nodes_nav.h"

// rrnx_datetime
#include "rrnx_common.h"

#include "rrnx_list.h"
#include "rrnx_node.h"




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
 * Broadcast clock and orbit, almanac excluded.
 * This is the aggregation of the repeating fields of GPS nav file.
 */
struct rrnx_navmsg {

	// ADMINISTRATIVE DATA
	//=====================

	/**
	 * Indicates whether af2 is defined.
	 */
	int valid_af2;

	/**
	 * Indicates whether spare1 is defined.
	 */
	int valid_spare1;

	/**
	 * Indicates whether spare2 is defined.
	 */
	int valid_spare2;


	// PAYLOAD DATA
	//==============

	// PRN / EPOCH / SV CLOCK
	//========================

	/**
	 * GPS Satellite PRN number (I2).
	 */
	int sv_id;

	/**
	 * Reference time, clock.
	 */
	rrnx_datetime toc;

	/**
	 * SV clock bias [sec], (D19.12).
	 */
	double af0;

	/**
	 * SV clock drift [sec/sec], (D19.12).
	 */
	double af1;

	/**
	 * SV clock drift rate [sec/sec^2], optional, (D19.12).
	 */
	double af2;

	// BROADCAST ORBIT N
	//===================

	/**
	 * Issue of data, ephemeris, (D19.12).
	 */
	double IODE;

	/**
	 * Orbit radius: harmonic corr sine [m], (D19.12).
	 */
	double Crs;

	/**
	 * Mean motion difference from the computed value [rad/s], (D19.12).
	 */
	double delta_n;

	/**
	 * Mean anomaly at the reference time [rad], (D19.12).
	 */
	double M0;

	/**
	 * Argument of latitude: harmonic corr cosine [rad], (D19.12).
	 */
	double Cuc;

	/**
	 * Eccentricity [1], (D19.12).
	 */
	double e;

	/**
	 * Argument of latitude: harmonic corr sine [rad], (D19.12).
	 */
	double Cus;

	/**
	 * Square-root of the semi-major axis [m^(1/2)], (D19.12).
	 */
	double sqrtA;

	/**
	 * Time of ephemeris, GPS time of week [s], (D19.12).
	 */
	double toe;

	/**
	 * Inclination: harmonic corr cosine [rad], (D19.12).
	 */
	double Cic;

	/**
	 * Longitude of the ascending node at weekly epoch [rad], (D19.12).
	 */
	double OMEGA0;

	/**
	 * Inclination: harmonic corr sine [rad], (D19.12).
	 */
	double Cis;

	/**
	 * Inclination at the refernece time [rad], (D19.12).
	 */
	double i0;

	/**
	 * Orbit radius: harmonic corr cosine [m], (D19.12).
	 */
	double Crc;

	/**
	 * Argument of perigee [rad], (D19.12).
	 */
	double w;

	/**
	 * Rate of right ascension [rad/s], (D19.12).
	 */
	double OMEGADOT;

	/**
	 * Rate of incination [rad/s], (D19.12).
	 */
	double idot;

	/**
	 * Codes on L2 channel, (D19.12).
	 */
	double L2_codes;

	/**
	 * Time of ephemeris, GPS week [1], (D19.12).
	 * Continuous week number, not modulo 1024.
	 */
	double toe_week;

	/**
	 * L2 P-code data flag (D19.12).
	 */
	double L2P_dataflag;

	/**
	 * SV accuracy [m], (D19.12).
	 * WARNING: may contain URA index number or URA in meters.
	 */
	double accuracy;

	/**
	 * SV health, (D19.12).
	 */
	double health;

	/**
	 * Group delay [s], (D19.12).
	 */
	double Tgd;

	/**
	 * Issue of data, clock, (D19.12).
	 */
	double IODC;

	/**
	 * Message transmission time, GPS time of week [sec], (D19.12).
	 */
	double tow;

	/**
	 * Fit interval [hours], (D19.12).
	 */
	double fit_interval;

	/**
	 * Spare 1, (D19.12).
	 */
	double spare1;

	/**
	 * Spare 2, (D19.12).
	 */
	double spare2;

};

/**
 * Reprents a navigation message file as a list of individual nodes.
 */
struct rrnx_nav {
	/**
	 * Node list
	 */
	rrnx_list *nodelist;


	// ADMINISTRATIVE DATA
	//=====================

	int valid_iono_alpha;
	int valid_iono_beta;
	int valid_delta_utc;
	int valid_leap_seconds;

	// PAYLOAD DATA
	//==============

	/**
	 * Format version, (F9.2).
	 */
	double format_version;

	/**
	 * File type ('N' for navigation data), (A1).
	 */
	char file_type;

	/**
	 * Name of program creating current file, (A20).
	 */
	char program[21];

	/**
	 * Name of agency creating current file, (A20).
	 */
	char agency[21];

	/**
	 * Date of file creation, (A20).
	 */
	char date[21];

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

typedef struct rrnx_nav rrnx_nav;


//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

extern rrnx_nav *rrnx_nav_alloc();
extern void rrnx_nav_free(rrnx_nav *nav);

//============================================================================
// METHODS: OTHER
//============================================================================

extern int rrnx_nav_is_node_type_valid(int type);
extern rrnx_node *rrnx_nav_alloc_node(rrnx_nav *nav, int type);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

