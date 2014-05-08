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

#ifndef RRNX_FILE_COMMON_H
#define RRNX_FILE_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Date and time
 */
struct rrnx_datetime {
	/**
         * Year (two decimal digits).
         * 80-99: 1980-1999, and 00-79: 2000-2079.
         */
        int year;

	/** Month (1-12). */
        int month;

	/** Day (1-31). */
        int day;

	/** Hour (0-23). */
        int hour;

	/** Minute (0-59). */
        int min;

	/** Second (0 <= sec < 60). */
        double sec;
};

typedef struct rrnx_datetime rrnx_datetime;

/**
 * Identifies a satellite within a GNSS.
 */
struct rrnx_satellite_id {
        /**
         * GNSS identifier.
         */
        unsigned int system;

        /**
         * Space Vehicle identifier within the GNSS.
         */
        unsigned int id;
};

typedef struct rrnx_satellite_id rrnx_satellite_id;

#ifdef __cplusplus
} // extern C
#endif

#endif
