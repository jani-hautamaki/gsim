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
 * Header (RINEX VERSION / TYPE).
 */
struct rrnx_header {
        /**
         * Format version (2.11) (F9.2)
         */
        char *format_version;

        /**
         * File type, enumerated type (A1)
         */
        char file_type;

	/**
	 * Satellite system (A1).
	 *
	 * Used in obs data file.
	 * Not used in nav msg file.
	 */
	unsigned int system;
};

typedef struct rrnx_header rrnx_header;

/**
 * Program, agency, date (PGM / RUN BY / DATE).
 */
struct rrnx_creation_info {
        /**
         * Name of the program that created the current file (A20).
         */
        char *program;

        /**
         * Name of the agency that created the current file (A20).
         */
        char *agency;

        /**
         * Date when the file was created (A20).
         */
        char *date;
};

typedef struct rrnx_creation_info rrnx_creation_info;

/**
 * Comment line (COMMENT).
 */
struct rrnx_comment {
        /**
         * Comment (A60).
         */
        char *text;
};

typedef struct rrnx_comment rrnx_comment;



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
// SUPPORTING STRUCTS
//============================================================================

struct rrnx_node {
        /**
         * Node type
         */
        unsigned int type;

	/**
	 * Node payload size (chars).
	 */
	unsigned int size;

        /**
         * Node payload as a flexible array.
         */
        char data[];
};

typedef struct rrnx_node rrnx_node;






struct rrnx_list_entry {
        /** Pointer to the next item, or NULL. */
        struct rrnx_list_entry *next;

        /** Pointer to the previous item, or NULL. */
        struct rrnx_list_entry *prev;

        /** Payload data or NULL. */
        void *data;
};

typedef struct rrnx_list_entry rrnx_list_entry;

/**
 * Generic linked-list data structure
 */
struct rrnx_list {
        /** Pointer to the first (head) item, or NULL. */
        rrnx_list_entry *first;
        /** Pointer to the last (tail) item, or NULL. */
        rrnx_list_entry *last;
};

typedef struct rrnx_list rrnx_list;



#ifdef __cplusplus
} // extern C
#endif

#endif
