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

#ifndef RRNX_NAV_H
#define RRNX_NAV_H

#include "rrnx_common.h"

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// PAYLOAD STRUCTS: GPS NAV MESSAGE FILE
//============================================================================

/**
 * Ionosphere parameters A0-A3 of almanac (ION ALPHA).
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
	 * Reference time for UTC data
	 */
	int T;

	/**
	 * UTC reference week number,
	 * not mod(1024).
	 */
	unsigned int W;
};

typedef struct rrnx_delta_utc rrnx_delta_utc;

/**
 * Leap seconds information (LEAP SECONDS).
 */
struct rrnx_leap_seconds {
	/**
	 * Delta time due to leap seconds
	 */
	int delta_ls;
};

typedef struct rrnx_leap_seconds rrnx_leap_seconds;


/**
 * Broadcast orbit, line 0 (PRN / EPOCH / SV CLK)
 */
struct rrnx_broadcast_orbit0 {
	/**
	 * Satellite PRN number (I2).
	 */
	int sv_id;

	/**
	 * Epoch (Toc): year (I2).
	 */
	int Toc_year;

	/**
	 * Epoch (Toc): month (I2).
	 */
	int Toc_month;

	/**
	 * Epoch (Toc): day (I2).
	 */
	int Toc_day;

	/**
	 * Epoch (Toc): hour (I2).
	 */
	int Toc_hour;

	/**
	 * Epoch (Toc): min (I2).
	 */
	int Toc_min;

	/**
	 * Epoch (Toc): sec (F5.1).
	 */
	double Toc_sec;

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
 * Broadcast orbit, line 1 (BROADCAST ORBIT - 1)
 */
struct rrnx_broadcast_orbit1 {
	/**
	 * Issue of data, Ephemeris (D19.12)
	 */
	double IODE;

	/**
	 * Orbit radius: sine harmonic corr [m], (D19.12).
	 */
	double Crs;

	/**
	 * Mean motion difference [rad/s], (D19.12).
	 */
	double delta_n;

	/**
	 * Mean anomaly at reference time [rad]; (D19.12).
	 */
	double M0;
};

typedef struct rrnx_broadcast_orbit1 rrnx_broardcast_orbit1;

/**
 * Broadcast orbit, line 2 (BROADCAST ORBIT - 2)
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
	 * Square root of semi-major axis [sqrt(m)], (D19.12).
	 */
	double sqrtA;
};

typedef struct rrnx_broadcast_orbit2 rrnx_broardcast_orbit2;

/**
 * Broadcast orbit, line 3 (BROADCAST ORBIT - 3)
 */
struct rrnx_broadcast_orbit3 {
	/**
	 * Time of Ephemeris, GPS system tim [sec], (D19.12).
	 */
	double Toe;

	/**
	 * Inclination: cosine harmonic corr [rad], (D19.12).
	 */
	double Cic;

	/**
	 * [rad], (D19.12)
	 */
	double OMEGA;

	/**
	 * Inclination: sine harmonic corr [rad], (D19.12).
	 */
	double Cis;
};

typedef struct rrnx_broadcast_orbit3 rrnx_broardcast_orbit3;

/**
 * Broadcast orbit, line 4 (BROADCAST ORBIT - 4)
 */
struct rrnx_broadcast_orbit4 {
	/**
	 * Inclination at reference time [rad], (D19.12).
	 */
	double i0;

	/**
	 * Orbit radius: corsine harmonic corr [m], (D19.12).
	 */
	double Crc;

	/**
	 * [rad], (D19.12).
	 */
	double omega;

	/**
	 * [rad/s], (D19.12).
	 */
	double OMEGADOT;
};

typedef struct rrnx_broadcast_orbit4 rrnx_broardcast_orbit4;

/**
 * Broadcast orbit, line 5 (BROADCAST ORBIT - 5)
 */
struct rrnx_broadcast_orbit5 {
	/**
	 * [rad/s], (D19.12)
	 */
	double IDOT;

	/**
	 * Codes on L2 channel, (D19.12)
	 */
	double L2_codes;

	/**
	 * Toe: week number, GPS system time [1], (D19.12).
	 */
	double Toe_week;

	/**
	 * L2 P data flag (D19.12)
	 */
	double L2_P_flag;
};

typedef struct rrnx_broadcast_orbit5 rrnx_broardcast_orbit5;

/**
 * Broadcast orbit, line 6 (BROADCAST ORBIT - 6)
 */
struct rrnx_broadcast_orbit6 {
	/**
	 * SV accuracy [m], (D19.12).
	 */
	double accuracy;

	/**
	 * SV health (from GPS navmsg subframe 1), (D19.12).
	 */
	double health;

	/**
	 * Group delay [s], (D19.12)
	 */
	double Tgd;

	/**
	 * Issue of Data, clock, (D19.12)
	 */
	double IODC;
};

typedef struct rrnx_broadcast_orbit6 rrnx_broardcast_orbit6;

/**
 * Broadcast orbit, line 7 (BROADCAST ORBIT - 7)
 */
struct rrnx_broadcast_orbit7 {
	/**
	 * Transmission time of message, GPS system time [sec], (D19.12).
	 * Derived, for instance, from Z-count in HOW.
	 */
	double Tsend;

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

typedef struct rrnx_broadcast_orbit7 rrnx_broardcast_orbit7;


















//============================================================================
// DATA TYPES: GPS NAV MESSAGE FILE
//============================================================================

/**
 * Klobuchar ionospheric model parameters
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
	int w;

	int leap;
};
typedef struct rrnx_utc rrnx_utc;

/**
 * GPS ephemeris parameters as they appear in NAV files.
 */
struct rrnx_ephemeris_params {
	/**
	 * Reference time (sec).
	 */
	// TODO: should include week
	double Toe;

	/**
	 * Mean anomaly at reference time
	 */
	double M0;

	/**
	 * Mean motion difference from computed value
	 */
	double delta_n;

	/**
	 * Eccentricity
	 */
	double e;

	/**
	 * Square-root of the semi-major axis
	 */
	double sqrtA;

	/**
	 * Longitude of Ascending Node (LAN)
	 * of orbit plane at weekly epoch.
	 */
	double OMEGA0;

	/**
	 * Inclination at reference time
	 */
	double i0;

	/**
	 * Argument of perigee
	 */
	double w;

	/**
	 * Rate of right ascension
	 */
	double OMEGADOT;

	/**
	 * Rate of inclination angle
	 */
	double IDOT;

	/*-------------------------------------*/
	/* HARMONIC CORRECTION TERM AMPLITUDES */
	/*-------------------------------------*/

	/**
	 * Argument of latitude,
	 * cosine harmonic correction term amplitude
	 */
	double Cuc;

	/**
	 * Argument of latitude,
	 * sine harmonic correction term amplitude
	 */
	double Cus;

	/**
	 * Orbit radius,
	 * cosine harmonic correction term amplitude
	 */
	double Crc;

	/**
	 * Orbit radius,
	 * sine harmonic correction term amplitude
	 */
	double Crs;

	/**
	 * Inclination angle,
	 * cosine harmonic correction term amplitude
	 */
	double Cic;

	/**
	 * Inclination angle,
	 * sine harmonic correction term amplitude
	 */
	double Cis;

};
typedef struct rrnx_ephemeris_params rrnx_ephemeris_params;

/**
 * GPS clock correction parameters as they appear in NAV files.
 * Member attribute names follow the interface specification.
 */
struct rrnx_clock_params {
	/**
	 * Reference time (sec).
	 */
	double Toc;

        /**
         * SV clock bias (sec).
         */
	double af0;

        /**
         * SV clock drift (sec/sec).
         */
	double af1;

	/**
	 * SV clock drift rate (sec/sec^2).
	 */
	double af2;
};

typedef struct rrnx_clock_params rrnx_clock_params;


/**
 * Navigation message for GPS as it appears in NAV files.
 */
struct rrnx_navmsg_gps {
	/**
	 * Satellite system/vehicle identifier
	 * from which this navigation message was received.
	 */
	rrnx_satellite_id sat;

	/**
	 * Transmission time of message,
	 * GPS system time, seconds only.
	 */
	double Tot;

	/**
	 * Fit interval
	 */
	double fit_interval;

	/**
	 * Epoch ??
	 */
	rrnx_time Toc;

	// Subframe 1 params
	unsigned int L2_code;
	unsigned int L2_P_flag;

	unsigned int health;
	unsigned int accuracy;

	double Tgd;

	/**
	 * Clock correction parameters
	 */
	rrnx_clock_params clock;

	/**
	 * Issue of data,
	 * clock correction parameters
	 */
	unsigned int IODC;

	/**
	 * Ephemeris parameters
	 */
	rrnx_ephemeris_params orbit;

	/**
	 * Issue of data,
	 * ephemeris parameters.
	 */
	unsigned int IODE;

};

struct rrnx_nav {
	// TODO
};

typedef struct rrnx_nav rrnx_nav;

#ifdef __cplusplus
} // extern "C"
#endif

#endif

