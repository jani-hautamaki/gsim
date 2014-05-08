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

#ifndef RRNX_OBS_H
#define RRNX_OBS_H

#ifdef __cplusplus
extern "C" {
#endif

//============================================================================
// DATA TYPES: GNSS OBS DATA FILE
//============================================================================

/**
 * Measurement type
 */
struct rrnx_observ_type {
	/**
 	 * Satellite system
	 */
	unsigned int system;

	/**
	 * Frequency/Band
 	 */
	unsigned int band;

	/**
	 * Observable
	 */
	unsigned int observable;
};

typedef struct rrnx_observ_type rrnx_observ_type;




//============================================================================
// PAYOAD STRUCTS: GNSS OBS DATA FILE - HEADER
//============================================================================

/**
 * Antenna marker name (MARKER NAME)
 */
struct rrnx_marker_name {
	/**
	 * Name of antenna marker (A60).
	 */
	char *marker_name;
};

typedef struct rrnx_marker_name rrnx_marker_name;

/**
 * Antenna marker number (MARKER NUMBER)
 */
struct rrnx_marker_number {
	/**
	 * Number of antenna marker (A60).
	 */
	char *marker_number;
};

typedef struct rrnx_marker_number rrnx_marker_number;

/**
 * Observer information (OBSERVER / AGENCY)
 */
struct rrnx_observer_info {
	/**
	 * Observer name (A20).
	 */
	char *observer;

	/**
	 * Observer agency (A40).
	 */
	char *agency;
};

typedef struct rrnx_observer_info rrnx_observer_info;

/**
 * Receiver information (REC # / TYPE / VERS).
 */
struct rrnx_receiver_info {
	/**
	 * Receiver number (A20).
	 */
	char *number;

	/**
	 * Receiver type (A20).
	 */
	char *type;

	/**
	 * Receiver version (A20).
	 * Example: internal software version.
	 */
	char *version;
};

typedef struct rrnx_receiver_info rrnx_receiver_info;

/**
 * Antenna information (ANT # / TYPE).
 */
struct rrnx_antenna_info {
	/**
	 * Antenna number (A20).
	 */
	char *number;

	/**
	 * Antenna type (A20).
	 */
	char *type;
};

typedef struct rrnx_antenna_info rrnx_antenna_info;

/**
 * Approximate marker position in WGS84 (APPROX POSITION XYZ).
 */
struct rrnx_marker_xyz {
	/**
	 * X-coordinate [m], (F14.4)
	 */
	double x;

	/**
	 * Y-coordinate [m], (F14.4)
	 */
	double y;

	/**
	 * Z-coordinate [m], (F14.4)
	 */
	double z;
};

typedef struct rrnx_marker_xyz rrnx_marker_xyz;

/**
 * Antenna delta height/east/north (ANTENNA: DELTA H/E/N).
 */
struct rrnx_antenna_delta {
	/**
	 * Antenna height [m], (F14.4).
	 * Height of bottom surface of antenna above marker.
	 */
	double height;

	/**
	 * Antenna center east eccentricity [m], (F14.4).
	 */
	double east;

	/**
	 * Antenna center north eccentricity [m], (F14.4).
	 */
	double north;
};

typedef struct rrnx_antenna_delta rrnx_antenna_delta;


/**
 * Wavelength fact (WAVELENGTH FACT L1/2).
 */
struct rrnx_wavelength_fact {
	/**
	 * Wavelength factor for L1, GPS only (I6).
	 */
	unsigned int L1_wavelen;

	/**
	 * Wavelength factor for L2, GPS only (I6).
	 */
	unsigned int L2_wavelen;

	/**
	 * Number of satellites in the list (I6).
	 * The list contains the satellites for which
	 * these factors are valid.
	 */
	unsigned int size;

	// TODO
};

typedef struct rrnx_wavelength_fact rrnx_wavelength_fact;

/**
 * Measurement data layout (# / TYPES OF OBSERV).
 */
//struct rrnx_observ_layout {
struct rrnx_meas_layout {

	/**
	 * Number of different observation types (I6).
	 */
	unsigned int size;

	/**
	 * Observation data layout (per line: 9x (A1, A1)).
	 */
	rrnx_observ_type *item;
};

typedef struct rrnx_meas_layout rrnx_meas_layout;

/**
 * Observation interval (INTERVAL).
 */
struct rrnx_meas_interval {
	/**
	 * Observation interval [s] (F10.3).
	 */
	double interval;
};

typedef struct rrnx_meas_interval rrnx_meas_interval;

/**
 * Time of the last observation record (TIME OF FIRST OBS).
 */
struct rrnx_first_meas {
	// -Time of first observation 5I6, F13.7 (ymd hhmmss)
	// -Time system (A3)
};

typedef struct rrnx_first_meas rrnx_first_meas;

struct rrnx_last_meas {
	// -Time of first observation 5I6, F13.7 (ymd hhmmss)
	// -Time system (A3)
};

typedef struct rrnx_last_meas rrnx_last_meas;

/**
 * Measurement correction info (RCV CLOCK OFFS APPL).
 * Whether epoch, code, and phase are corrected by
 * applyting the realtime-derived receiver clock offset.
 * Record is required if clock offsets are reported
 * in the EPOCH/SAT records.
 */
struct rrnx_recv_clock_offset {
	/**
	 * Clock offset correction applied (I6).
	 * 1: true, 0: false.
	 */
	unsigned int is_applied;
};

typedef struct rrnx_recv_clock_offset rrnx_recv_clock_offset;

/**
 * Number of leap seconds since 6 Jan 1980 (LEAP SECONDS).
 * TODO: This is the same as in the GPS NAV file.
 */
struct rrnx_leap_seconds2 {
	/**
	 * Number of leap seconds since 6 Jan 1980 (I6).
	 * (The definition matches with GPS NAV file's).
	 */
	int leap_seconds;
};

typedef struct rrnx_leap_seconds2 rrnx_leap_seconds2;

/**
 * Satellite count (# OF SATELLITES).
 */
struct rrnx_satellite_count {
	/**
	 * Number of different satellites present in the data (I6).
	 */
	int sat_count;
};

typedef struct rrnx_satellite_count rrnx_satellite_count;

/**
 * Observation summary for a satellite (PRN / # OF OBS).
 */
struct rrnx_meas_summary {
	/**
	 * Satellite id
	 */
	rrnx_satellite_id sat;

	/**
	 * Measurement count for each meas type (per line: 9I6).
	 */
	int *meas_count;
};

//============================================================================
// PAYOAD STRUCTS: GNSS OBS DATA FILE - DATA
//============================================================================

struct rrnx_observ {

	int valid_value;
	int valid_flags;
	int valid_signal;

	/**
	 * Observation as a scalar value.
	 * Phase: units in whole cycles of carrier.
	 * Code: Units in meters.
	 * Missing observations are written as 0.0 or blanks.
	 */
	double value;

	/**
	 * Loss of lock indicator (LLI).
	 */
	int flags;

	/**
	 * Signal strength projected into interval 1-9.
	 */
	int signal;
};

/**
 * Measurements for a single satellite.
 */
struct rrnx_satellite_meas {
	/**
	 * Array of individual observations.
	 * The observations conform to the meas_layout.
	 */
	rrnx_observ *observ;
}

typedef struct rrnx_satellite_meas rrnx_satellite_meas;

/**
 * Measurement epoch (EPOCH/SAT).
 */
struct rrnx_meas_epoch {
	/**
	 * Epoch time: year, month, day, hour, min, sec (I2.2, 4I2, F11.7).
	 */
	rrnx_datetime t;

	/**
	 * Epoch flag (I1), or alternatively, event flag (I1).
	 * If the value is >1, it is an event flag.
	 * In that case the "size" field is interpreted as the number of
	 * special records to follow.
	 */
	unsigned int event;

	/**
	 * Number of satellites in current epoch (I3), or alternatively,
	 * number of special records to follow (I3).
	 */
	unsigned int size;

	/**
	 * List of satellites in current epoch (per line: 12(A1, I2)).
	 */
	rrnx_satellite_id *sat;

	/**
	 * Receiver clock offset [s], optional (F12.9).
	 * If not specified, NaN.
	 */
	double clock_offset;

	/**
	 * Measurements for each listed satellite.
	 */
	rrnx_satellite_meas *meas;
};

typedef struct rrnx_meas_epoch rrnx_meas_epoch;


//============================================================================
// METHODS:
//============================================================================


/*--- obs file types -------------------------------------------------------*/

/**
 * Represents a single observation epoch.
 * An observation epoch includes simultaneous
 * measurements from multiple satellites.
 */
struct rrnx_obs_epoch {
        /**
         * Epoch time
         */
	rrnx_datetime t;

        /**
         * Number of satellites
         */
	unsigned int size;

        /**
         * Array of satellite identifiers
         */
	rrnx_satellite_id *sat;

	/**
	 * A vector of the measurements
 	 */
	rrnx_meas *meas;
};

typedef struct rrnx_obs_epoch rrnx_obs_epoch;



/* OBS file */

struct rrnx_obs_header {

};
typedef struct rrnx_obs_header rrnx_obs_header;

struct rrnx_obs {
	rrnx_obs_header header;
	rrnx_list records;
};

typedef struct rrnx_obs rrnx_obs;


#ifdef __cplusplus
} // extern "C"
#endif

#endif

