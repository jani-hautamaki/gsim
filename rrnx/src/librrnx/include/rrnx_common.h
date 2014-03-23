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

#ifndef RRNX_COMMON_H
#define RRNX_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif


//============================================================================
// DATA TYPES: COMMON
//============================================================================

/**
 * Date and time
 */
struct rrnx_time {
        unsigned int year;
        unsigned int month;
        unsigned int day;

        unsigned int hour;
        unsigned int min;
        double sec;
};

typedef struct rrnx_time rrnx_time;

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

//============================================================================
// PAYLOAD STRUCTS: COMMON CONSTANTS
//============================================================================

/**
 * These identify the satellite system.
 *
 */

#define RRNX_SYSTEM_GPS                 'G'
#define RRNX_SYSTEM_GLONASS             'R'
#define RRNX_SYSTEM_GALILEO             'E'
#define RRNX_SYSTEM_BEIDOU              'B' ??
#define RRNX_SYSTEM_QZSS                'Q' ??

//============================================================================
// PAYLOAD STRUCTS: COMMON
//============================================================================

/**
 * RINEX declaration (RINEX VERSION / TYPE).
 */
struct rrnx_format_decl {
        /**
         * Format version (2.11) (F9.2)
         */
        char version[10];

        /**
         * File type, enumerated type (A1)
         */
        char type;

	/**
	 * Satellite system (A1).
	 *
	 * Used in obs data file.
	 * Not used in nav msg file.
	 */
	unsigned int system;
};

typedef struct rrnx_format_decl rrnx_format_decl;

/**
 * Program, agency, date (PGM / RUN BY / DATE).
 */
struct rrnx_creation_info {
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

typedef struct rrnx_creation_info rrnx_creation_info;

/**
 * Comment line (COMMENT).
 */
struct rrnx_comment {
        /**
         * Comment (A60).
         */
        char text[61];
};

typedef struct rrnx_comment rrnx_comment;


/*
struct rrnx_end_header {
};

typedef struct rrnx_end_header rrnx_end_header;
*/





#ifdef __cplusplus
} // extern C
#endif

#endif
