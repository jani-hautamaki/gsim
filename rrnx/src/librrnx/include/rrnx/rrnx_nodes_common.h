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

#ifndef RRNX_NODES_COMMON_H
#define RRNX_NODES_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

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
