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

#ifndef RRNX_BASETYPES_NAV_H
#define RRNX_BASETYPES_NAV_H

// rrnx_datetime
#include "rrnx_basetypes_common.h"

#ifdef __cplusplus
extern "C" {
#endif

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

typedef struct rrnx_navmsg rrnx_navmsg;

//============================================================================
// METHODS: CONSTRUCTION & DESTRUCTION
//============================================================================

#ifdef __cplusplus
} // extern "C"
#endif

#endif

