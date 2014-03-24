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

#include "rrnx_common.h"
#include "rrnx_list.h"
#include "rrnx_node.h"

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
	 * Ionospheric parameters A0-A3 of almanac.
	 */
	double alpha[4];
};

typedef struct rrnx_ion_alpha rrnx_ion_alpha;

/**
 * Ionospheric parameters B0-B3 of almanac (ION BETA)
 * This is node is specific to GPS.
 */
struct rrnx_ion_beta {
	/**
	 * Ionospheric parameters B0-B3 of almanac.
	 */
	double beta[4];
};

typedef struct rrnx_ion_beta rrnx_ion_beta;

/**
 * Almanac parameters for UTC computation (DELTA-UTC: A0,A1,T,W)
 * This is node is specific to GPS.
 */
struct rrnx_delta_utc {
	/**
	 * a0 term of the polynomial
	 */
	double a0;

	/**
	 * a1 term of the polynomial
	 */
	double a1;

	/**
	 * Reference time for UTC data, GPS time of week
	 */
	int T;

	/**
	 * Reference time for UTC data, GPS week.
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
 * GPS Broadcast orbit, line 0 (PRN / EPOCH / SV CLK)
 */
struct rrnx_broadcast_orbit0 {
	/**
	 * GPS Satellite PRN number (I2).
	 */
	int sv_id;

	//rrnx_time Toc;

	/**
	 * Epoch (Toc): year (I2).
	 */
	int toc_year;

	/**
	 * Epoch (Toc): month (I2).
	 */
	int toc_month;

	/**
	 * Epoch (Toc): day (I2).
	 */
	int toc_day;

	/**
	 * Epoch (Toc): hour (I2).
	 */
	int toc_hour;

	/**
	 * Epoch (Toc): min (I2).
	 */
	int toc_min;

	/**
	 * Epoch (Toc): sec (F5.1).
	 */
	double toc_sec;

	/**
	 * SV clock bias [sec], (D19.12).
	 */
	double af0;

	/**
	 * SV clock drift [sec/sec], (D19.12).
	 */
	double af1;

	/**
	 * SV clock drift rate [sec/sec^2], (D29.12).
	 */
	double af2;
};

typedef struct rrnx_broadcast_orbit0 rrnx_broadcast_orbit0;

/**
 * GPS Broadcast orbit, line 1 (BROADCAST ORBIT - 1)
 */
struct rrnx_broadcast_orbit1 {
	/**
	 * Issue of data, ephemeris (D19.12)
	 */
	int IODE;

	/**
	 * Orbit radius: sine harmonic corr [m], (D19.12).
	 */
	double Crs;

	/**
	 * Mean motion difference from the computed value [rad/s], (D19.12).
	 */
	double delta_n;

	/**
	 * Mean anomaly at reference time [rad]; (D19.12).
	 */
	double M0;
};

typedef struct rrnx_broadcast_orbit1 rrnx_broadcast_orbit1;

/**
 * GPS Broadcast orbit, line 2 (BROADCAST ORBIT - 2)
 */
struct rrnx_broadcast_orbit2 {
	/**
	 * Argument of latitude: cosine harmonic corr [rad], (D19.12).
	 */
	double Cuc;

	/**
	 * Eccentricity [1], (D19.12).
	 */
	double e;

	/**
	 * Argument of latitude: sine harmonic corr [rad], (D19.12).
	 */
	double Cus;

	/**
	 * Square root of the semi-major axis [sqrt(m)], (D19.12).
	 */
	double sqrtA;
};

typedef struct rrnx_broadcast_orbit2 rrnx_broadcast_orbit2;

/**
 * GPS Broadcast orbit, line 3 (BROADCAST ORBIT - 3)
 */
struct rrnx_broadcast_orbit3 {
	/**
	 * Time of ephemeris, GPS time of week [sec], (D19.12).
	 */
	double toe;

	/**
	 * Inclination: cosine harmonic corr [rad], (D19.12).
	 */
	double Cic;

	/**
	 * Longitude of ascending node at weekly epoch [rad], (D19.12).
	 */
	double OMEGA0;

	/**
	 * Inclination: sine harmonic corr [rad], (D19.12).
	 */
	double Cis;
};

typedef struct rrnx_broadcast_orbit3 rrnx_broadcast_orbit3;

/**
 * GPS Broadcast orbit, line 4 (BROADCAST ORBIT - 4)
 */
struct rrnx_broadcast_orbit4 {
	/**
	 * Inclination at reference time [rad], (D19.12).
	 */
	double i0;

	/**
	 * Orbit radius: cosine harmonic corr [m], (D19.12).
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
};

typedef struct rrnx_broadcast_orbit4 rrnx_broadcast_orbit4;

/**
 * GPS Broadcast orbit, line 5 (BROADCAST ORBIT - 5)
 */
struct rrnx_broadcast_orbit5 {
	/**
	 * Rate of inclination [rad/s], (D19.12)
	 */
	double idot;

	/**
	 * Codes on L2 channel, (D19.12).
	 * Indicates which codes are commaneded ON for the L2 channel:
	 *   00: Reserved
	 *   01: P code ON.
	 *   10: C/A code ON.
	 *
	 * Defined in [IS-GPS-200F] subsection "20.3.3.3.1.2".
	 */
	double L2_codes;

	/**
	 * Time of ephemeris, GPS week [1], (D19.12).
	 * The week number is continuous, not modulo 1024.
	 */
	int toe_week;

	/**
	 * L2 P-code data flag (D19.12).
	 * Flag indicates the NAV data stream was commanded OFF
	 * on the P-code of the L2 channel.
	 *
	 * Defined in [IS-GPS-200F] subsection "20.3.3.1.6".
	 */
	int L2P_dataflag;
};

typedef struct rrnx_broadcast_orbit5 rrnx_broadcast_orbit5;

/**
 * GPS Broadcast orbit, line 6 (BROADCAST ORBIT - 6)
 */
struct rrnx_broadcast_orbit6 {
	/**
	 * SV accuracy [m], (D19.12).
	 * Corresponds to the 4-bit URA index.
	 *
	 * NOTE:
	 * Due to incompleteness of the earlier RINEX specifications,
	 * there are files in the circulation that may contain
	 * either URA index (dimensionless) or URA (in meters).
	 *
	 * TBC:
	 * Rename into "ura"?
	 *
	 */
	double accuracy;

	/**
	 * SV health (bits 17-22 in GPS navmsg subframe 1 word 3), (D19.12).
 	 *
	 * Ideally, this should contain all six-bits of
	 * the health indicator. Due to earlier RINEX specifications,
	 * however, the field values should be interpreted as follows:
	 *
	 *   0 = all nav data are ok,
	 *   1 = some or all nav data are bad (bits 18-22 not stored),
	 * >32 = some or all nav dat are bad (bits 18-22 stored).
	 *
	 * Defined in [IS-GPS-200F] subsection "20.3.3.3.1.4".
	 */
	double health;

	/**
	 * Group delay [s], (D19.12)
	 */
	double Tgd;

	/**
	 * Issue of data, clock, (D19.12)
	 */
	int IODC;
};

typedef struct rrnx_broadcast_orbit6 rrnx_broadcast_orbit6;

/**
 * GPS Broadcast orbit, line 7 (BROADCAST ORBIT - 7)
 */
struct rrnx_broadcast_orbit7 {
	/**
	 * Transmission time of message, SV time of week (?) [sec], (D19.12).
	 * Derived, for instance, from Z-count in HOW.
	 *
	 * NOTE:
	 * This can be shortly BEFORE the midnight Saturday/Sunday,
	 * while TOE and TOC of the message are already in the next week.
	 * As the reported week (in BROADCAST ORBIT 5) goes with TOE,
	 * the transmission time of message should be reduced by 604800
	 * (ie, will become negative) to also refer to the reported week.
	 *
	 * TBC:
	 * [IS-GPS-200F] uses "tot" for UTC reference time,
	 * therefore, it might be misleading to name this variable as "tot".
	 *
	 * TBC:
	 * Use name "tow" instead?
	 *
	 */
	double toz;

	/**
	 * Fit interval [hours], (D19.12).
	 */
	double fit_interval;

	/**
	 * Spare 1, (D19.12)
	 */
	double unused1;

	/**
	 * Spare 2, (D19.12)
	 */
	double unused2;
};

typedef struct rrnx_broadcast_orbit7 rrnx_broadcast_orbit7;

//============================================================================
// DATA TYPES: GPS NAV MESSAGE FILE
//============================================================================

/**
 * Reprents a navigation message file as a list of individual nodes.
 */
struct rrnx_nav {
	/**
	 * Node list
	 */
	rrnx_list *nodelist;
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

extern rrnx_node *rrnx_nav_alloc_node(rrnx_nav *nav, int type);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

