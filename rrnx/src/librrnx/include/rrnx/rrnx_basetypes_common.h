//******************************{begin:header}******************************//
//                      rrnx - The Robust RINEX Library
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

/**
 * Base data types common for both NAV and OBS files.
 *
 * These data types are the primitive data types that propagate untouched
 * through the stages of parsing and serialization.
 */

#ifndef RRNX_BASETYPES_COMMON_H
#define RRNX_BASETYPES_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Date and time as it appears in both NAV and OBS files.
 * The field widths are specified as (NAV / OBS).
 */
struct rrnx_datetime {
	/**
	 * Year, Either (I2.2 / I6).
	 *
	 * a) Year (two decimal digits): 80-99: 1980-1999, and 00-79: 2000-2079.
	 * b) Year (four decimal digits: 0000-9999.
	 *
	 */
	int year;

	/**
	 * Month (1-12), (I2 / I6).
	 */
	int month;

	/**
	 * Day (1-31), (I2 / I6).
	 */
	int day;

	/**
	 * Hour (0-23), (I2 / I6).
	 **/
	int hour;

	/**
	 * Minute (0-59), (I2 / I6).
	 */
	int min;

	/**
	 * Second (0 <= sec < 60), (F5.1 / F13.7)
	 */
	double sec;
};

typedef struct rrnx_datetime rrnx_datetime;

/**
 * Satellite identifier.
 * Identifies a satellite and within a GNSS.
 */
struct rrnx_satellite_id {
	/**
	 * GNSS identifier
	 */
	int system;

	/**
	 * Space Vehicle identifier within the GNSS.
	 */
	int id;

};

typedef struct rrnx_satellite_id rrnx_satellite_id;

/**
 * Satellite system identifiers
 */

//      Symbolic name                   Identifier
#define RRNX_SYSTEM_GPS                 'G'
#define RRNX_SYSTEM_GLONASS             'R'
#define RRNX_SYSTEM_GALILEO             'E'
#define RRNX_SYSTEM_BEIDOU              'B'
#define RRNX_SYSTEM_QZSS                'Q'

/**
 * File format declaration
 */
struct rrnx_file_format {

	/**
	 * Format version ("2.11" expected), (F9.2).
	 */
	char version[10];

	/**
	 * file type (A1).
	 */
	char type;

	/**
	 * Satellite system (A1).
	 *
	 * Used only in OBS data file.
	 * Not used in NAV msg file.
	 */
	int system;

};

typedef struct rrnx_file_format rrnx_file_format;

/**
 * File creation information (program, agency, date).
 */
struct rrnx_file_info {
	/**
	 * Name of the program that created the current file (A20).
	 */
	char program[21];

	/**
	 * Name of the agency that created the current file (A20).
	 */
	char agency[21];

	/**
	 * Date when the file was created (A20).
	 */
	char date[21];
};

typedef struct rrnx_file_info rrnx_file_info;



#ifdef __cplusplus
} // extern "C"
#endif

#endif
