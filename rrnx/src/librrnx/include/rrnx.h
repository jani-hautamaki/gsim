//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://code.google.com/p/gsim
//
//      Copyright (C) 2013 Jani Hautamaki <jani.hautamaki@hotmail.com>
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

//#include "rrnx_common.h" // Included through rrnx_nav and rrnx_obs
#include "rrnx_nav.h"
#include "rrnx_obs.h"

#ifdef __cplusplus
extern "C" {
#endif



//============================================================================
// NAVIGATION MESSAGE FILE: GPS
//============================================================================

/**
 * Klobuchar ionospheric model parameters.
 */
struct rrnx_iono {
	double alpha[4];
	double beta[4];
};
typedef struct rrnx_iono rrnx_iono;

/**
 * UTC parameters
 */
struct rrnx_utc {
	double A0;
	double A1;

	int t;
	int week; // TODO: t_week?

	int leap;
};
typedef struct rrnx_utc rrnx_utc;

//============================================================================
// NAVIGATION MESSAGE: GPS
//============================================================================

/**
 * Clock correction parameters, GPS.
 */
struct rrnx_clock {
	/**
	 * Issue of Data (clock).
	 */
	int IODC;

	/**
	 * Reference time (clock), GPS time of week.
	 */
	double toc;

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

typedef struct rrnx_clock rrnx_clock;

/**
 * GPS ephemeris parameters
 */
struct rrnx_ephemeris {

	// QUASI-KEPLERIEN ORBITAL ELEMENTS
	//==================================

	/**
	 * Issue of Data (Ephemeris).
	 */
	int IODE;

	/**
	 * Reference time (Ephemeris), GPS time of week.
	 */
	double toe;

	/**
	 * Mean anomaly at reference time [rad].
	 */
	double M0;

	/**
	 * Mean motion difference from computed value [rad/s].
	 */
	double delta_n;

	/**
	 * Eccentricity [1].
	 */
	double e;

	/**
	 * Square root of the semi-major axis [m^(1/2)].
	 */
	double sqrtA;

	/**
	 * Longitude of ascending node at weekly epoch [rad].
	 *
	 * NOTE:
	 * The weekly epoch is the beginning of the GPS week.
	 */
	double OMEGA0;

	/**
	 * Inclination angle at reference time [rad].
	 */
	double i0;

	/**
	 * Argument of perigee [rad].
	 */
	double w;

	/**
	 * Rate of right ascension (of the ascending node?) [rad/s].
	 */
	double OMEGADOT;

	/**
	 * Rate of inclination angle [rad/s].
	 */
	double idot;

	// HARMONIC CORRECTION TERMS
	//===========================

	/**
	 * Argument of perigee: (cos) harmonic corr term [rad].
	 */
	double Cuc;

	/**
	 * Argument of perigee: (sin) harmonic corr term [rad].
	 */
	double Cus;

	/**
	 * Orbit radius: (cos) harmonic corr term [m].
	 */
	double Crc;

	/**
	 * Orbit radius: (sin) harmonic corr term [m].
	 */
	double Crs;

	/**
	 * Inclination angle: (cos) harmonic corr term [rad].
	 */
	double Cic;

	/**
	 * Inclination angle: (sin) harmonic corr term [rad].
	 */
	double Cis;

};

typedef struct rrnx_ephemeris rrnx_ephemeris;

// struct rrnx_broadcast_orbit_gps {
struct rrnx_navmsg {

	/** GPS Satellite PRN number. */
	int sv_id;

	// TODO: toc ymd hhmmss. Relation to toc??
	// TODO: Time of transmission (derived from Z count) ??
	// TODO: GPS week of tot, toc, toe.

	// CLOCK AND EPHEMERIS
	//=====================

	/** Clock correction parameters */
	rrnx_clock clock;

	/** Ephemeris parameters */
	rrnx_ephemeris ephemeris;

	// MISCELLANEOUS
	//===============

	// Subframe 1

	int week; // reference time GPS week for both toe and toc

	int L2_codes;

	int ura;

	int health;

	int L2P_dataflag;

	// Estimated group delay
	double tgd;

	// Subframe 2

	int fit_interval;


	/** Time of transmission */
	double toz;

	// Subframe 3

	// Subframe 3

	// Subframe 5, page 25
	// toa, WNa,

	// Subframe 4, page 18
	// ionospheric alpha, beta
	// UTC: A0, A1, Tot, WNt, delta_LS,DN, delta_LSF
};

typedef struct rrnx_navmsg rrnx_navmsg;


//============================================================================
// METHODS:
//============================================================================

/*
extern char rrnx_system2char();
extern unsigned int rrnx_char2system();
*/

#ifdef __cplusplus
} // extern "C"
#endif

#endif

