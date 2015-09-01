//******************************{begin:header}******************************//
//               gsim - GPS/INS measurement simulation system               //
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

#ifndef STATGEN_CONFIG_H
#define STATGEN_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define ACTION_INVALID 0
#define ACTION_GENERATE_GEO 1
#define ACTION_GENERATE_XYZ 2
#define ACTION_CONVERT_XYZ2GEO 3
#define ACTION_CONVERT_GEO2XYZ 4
#define ACTION_ROUNDTRIP_XYZ 5
#define ACTION_ROUNDTRIP_GEO 6

/**
 * Application specific configuration
 */
struct statgen_config {
	/**
	 * Action to execute
	 */
	int action;

	/**
	 * Input coordinates.
	 * The interpretation depends on the mode.
	 */
	double values[3];

	/**
	 * The output file name, or NULL if using stdout.
	 * Used only when generating a trajectory.
	 */
	const char *outfile;

	/**
	 * Simulation step length numerator
	 */
	unsigned int steplen_numer;

	/**
	 * Simulation step length denominator
	 */
	unsigned int steplen_denom;

	/**
	 * Simulation duration [s].
	 */
	unsigned int duration;

	/**
	 * Vehicle azimuth [deg]
	 */
	double azimuth;

	/**
	 * Vehicle elevation [deg]
	 */
	double elevation;

	/**
	 * Vehicle roll [deg]
	 */
	double roll;

};

typedef struct statgen_config statgen_config;

#ifdef __cplusplus
} // extern "C"
#endif

#endif
