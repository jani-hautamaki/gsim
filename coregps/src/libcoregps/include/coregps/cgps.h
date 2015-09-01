//******************************{begin:header}******************************//
//         coregps - Fundamental GPS data structures and algorithms
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

#ifndef CGPS_H
#define CGPS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * GPS orbital elements (LNAV).
 *
 * These parameters are needed to compute satellite's
 * position and velocity as functions of GPS system time.
 */
struct cgps_ephemeris {

	/**
	 * Issue of data, ephemeris.
	 */
	int IODE;

	// KEPLERIAN ELEMENTS
	//====================

	/**
	 * Reference time of ephemeris, GPS time of week [sec].
	 */
	double toe;

	/**
	 * Eccentricity of the orbit [1].
	 */
	double e;

	/**
	 * Square root of the orbit's semi-major axis [m^(1/2)].
	 */
	double sqrtA;

	/**
	 * Mean anomaly at the reference time [rad].
	 */
	double M0;

	/**
	 * Longitude of ascending node at weekly epoch [rad].
	 */
	double OMEGA0;

	/**
	 * Inclination at the reference time [rad].
	 */
	double i0;

	/**
	 * Argument of perigee [rad].
	 */
	double w;

	// SECULAR PERTURBATIONS
	//=======================

	/**
	 * Mean motion difference from the computed value [rad/s].
	 */
	double delta_n;

	/**
	 * Rate of right ascension [rad/s].
	 */
	double OMEGADOT;

	/**
	 * Rate of inclination [rad/s].
	 */
	double idot;

	// PERIODIC PERTURBATIONS
	//========================

	/**
	 * Orbit radius: cosine harmonic corr [m].
	 */
	double Crc;

	/**
	 * Orbit radius: sine harmonic corr [m].
	 */
	double Crs;

	/**
	 * Argument of latitude: cosine harmonic corr [rad].
	 */
	double Cuc;

	/**
	 * Argument of latitude: sine harmonic corr [rad].
	 */
	double Cus;

	/**
	 * Inclination: cosine harmonic corr [rad].
	 */
	double Cic;

	/**
	 * Inclination: sine harmonic corr [rad].
	 */
	double Cis;

};

typedef struct cgps_ephemeris cgps_ephemeris;

/**
 * GPS satellite's clock parameters (LNAV).
 */
struct cgps_clock {
	// Includes all clock correction parameters

	/**
	 * Issue of data, clock.
	 */
	int IODC;

	/**
	 * Reference time of clock, GPS time of week [sec].
	 */
	double toc;

	/**
	 * SV clock bias [sec].
	 */
	double af0;

	/**
	 * SV clock drift [sec/sec].
	 */
	double af1;

	/**
	 * SV clock drift rate [sec/sec^2].
	 */
	double af2;
};

typedef struct cgps_clock cgps_clock;


/**
 * GPS navigation message without almanac. (LNAV)
 */
struct cgps_navmsg {

	/**
	 * GPS satellite PRN number.
	 */
	int sat_id;

	/**
	 * Transmission time, GPS time of week [s].
	 */
	double tow;

	// SUBFRAME 1
	//============

	/**
	 * (Maps to RINEX how?) Transmission week number (mod 1024?).
	 * Location: subframe 1 / word 3.
	 */
	int week;

	/**
	 * Codes on L2 channel, 2-bit value.
	 * Location: subframe 1 / word 3.
	 */
	int L2_codes;

	/**
	 * SV accuracy (URA index), 4-bit value.
	 * Location: subframe 1 / word 3.
	 */
	int ura_index;

	/**
	 * SV health, 6-bit value.
	 * Location: subframe 1 / word 3.
	 */
	int health;

	/**
	 * Data flag for L2 P-code.
	 * Location: subframe 1 / word 4.
	 */
	int L2P_dataflag;

	/**
	 * Estimated group delay differential [s].
	 * Location: subframe 1 / word 7.
	 */
	double tgd;

	// SUBFRAME 2
	//============

	/**
	 * Curve fit interval, 1-bit value.
	 * Location: subframe 2 / word 10.
	*
	 * Fit interval is 4 hours (0) or greater than 4 hours (1).
	 *
	 * [IS-GPS-200G] The relationship of the curve-fit interval
	 * to transmission time and the timing of the curve-fit
	 * intervals is covered in section 20.3.4.
	 */
	int fit_interval;

	// CLOCK CORRECTION PARAMETERS
	//=============================

	/**
	 * SV clock correction.
	 * Location: subframe 1 / words 8, 9, 10.
	 */
	cgps_clock clock;

	// EPHEMERIS PARAMETERS
	//======================

	/**
	 * SV ephemeris parameters.
	 * Location: subframes 2, 3.
	 */
	cgps_ephemeris eph;

};

typedef struct cgps_navmsg cgps_navmsg;



/**
 * Ionospheric parameters of almanac (LNAV).
 */
struct cgps_iono {

	/**
	 * Ionospheric parameters A0-A3 of almanac.
	 * Location: subframe 4 / page 18 / words 3-4.
	 */
	double alpha[4];

	/**
	 * Ionospheric parameters B0-B3 of almanac.
	 * Location: subframe 4 / page 18 / words 4-5.
	 */
	double beta[4];
};

typedef struct cgps_iono cgps_iono;

/**
 * UTC parameters of almanac (LNAV).
 *
 * More concisely, almanac contains two sets of parameters
 * 1) UTC correction parameters
 * 2) Effectivity time of future or recent past value of
 *    the delta time due to leap seconds.
 *
 * However, the effecitivity time is not found from
 * RINEX v2.11 GPS navigation message files.
 *
 */
struct cgps_utc {

	/**
	 * Reference time for the UTC parameters, GPS time of week.
	 */
	int tot;

	/**
	 * Reference time for the UTC parameters, GPS week number.
	 */
	int week;

	/**
	 * a0 term of the polynomial.
	 */
	double a0;

	/**
	 * a1 term of the polynomial.
	 */
	double a1;

	// TODO: Should the number of leap seconds belong here?
	int delta_leapsec;
};

typedef struct cgps_utc cgps_utc;

struct cgps_almanac {

	/**
	 * Ionospheric correction parameters.
	 */
	cgps_iono iono;

	/**
	 * GPS/UTC transformation parameters.
	 */
	cgps_utc utc;

};

typedef struct cgps_almanac cgps_almanac;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
