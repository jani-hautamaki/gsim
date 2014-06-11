//******************************{begin:header}******************************//
//               gsim - GPS/INS measurement simulation system               //
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

// EXIT_SUCCESS, EXIT_FAILURE, strtol
#include <stdlib.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <math.h>

// libgrease, gut_*
#include "grease/gut_parse.h"
#include "grease/gut_argparser.h"
#include "grease/gut_datafile.h"

// liblatitude, lat_*
#include "latitude/lat_ellipsoid.h"

// libmaid, MAID_*
#include "maid/maid_angleconv.h"

// libeuclid
#include "euclid/cvector.h"
#include "euclid/cmatrix.h"
#include "euclid/cquat.h"


// libgsim, data exchange structures
#include "gsim/gsim_messages.h"
#include "gsim/gsim_datafile.h"

// The application-specific configuration
#include "statgen_config.h"
// The command-line parser
#include "argparser.h"


/**
 * List of references:
 *
 * [WGS84]
 *     World Geodetic System 1984.
 *     http://earth-info.nga.mil/GandG/publications/tr8350.2/wgs84fin.pdf
 *
 */

/**
 * Semi-major axis [m]
 * (WGS84, p 3-2, eq 3-1)
 */
#define WGS84_A 6378137.0

/**
 * Flattening [1]
 * (WGS84, p 3-2, eq 3-2)
 */
#define WGS84_F (1.0 / 298.257223563)

/*

*/

/**
 * Reference ellipsoid used for calculations.
 * Defaults to WGS 84 ellipsoid.
 */
static lat_ellipsoid g_ellipsoid;



static void convert_geo_deg2rad(double geo[]) {
	geo[0] = MAID_DEG2RAD_L(geo[0]);
	geo[1] = MAID_DEG2RAD_L(geo[1]);
}

static void convert_geo_rad2deg(double geo[]) {
	geo[0] = MAID_RAD2DEG_L(geo[0]);
	geo[1] = MAID_RAD2DEG_L(geo[1]);
}

static void convert_xyz2geo(double xyz[]) {

	// Convert coordinates using liblatitude
	double geo[3];

	// Display result
	printf("Input (XYZ):\n");
	printf("    x:        %.6f\n", xyz[0]);
	printf("    y:        %.6f\n", xyz[1]);
	printf("    z:        %.6f\n", xyz[2]);

	lat_ellipsoid_xyz2geo(&g_ellipsoid, xyz, geo);
	convert_geo_rad2deg(geo);

	printf("Output (GEO):\n");
	printf("    lon:      %.9f\n", geo[0]);
	printf("    lat:      %.9f\n", geo[1]);
	printf("    height:   %.9f\n", geo[2]);
}

static void convert_geo2xyz(double geo[]) {

	// Convert coordinates using liblatitude
	double xyz[3];

	// Display result
	printf("Input (GEO):\n");
	printf("    lon:      %.9f\n", geo[0]);
	printf("    lat:      %.9f\n", geo[1]);
	printf("    height:   %.9f\n", geo[2]);

	convert_geo_deg2rad(geo);
	lat_ellipsoid_geo2xyz(&g_ellipsoid, geo, xyz);

	printf("Output (XYZ):\n");
	printf("    x:        %.6f\n", xyz[0]);
	printf("    y:        %.6f\n", xyz[1]);
	printf("    z:        %.6f\n", xyz[2]);
}

static void roundtrip_geo(double geo1[]) {

	double xyz[3];
	double geo2[3];

	// Roundtrip coordinates using liblatitude
	convert_geo_deg2rad(geo1);
	lat_ellipsoid_geo2xyz(&g_ellipsoid, geo1, xyz);
	lat_ellipsoid_xyz2geo(&g_ellipsoid, xyz, geo2);

	convert_geo_rad2deg(geo1);
	convert_geo_rad2deg(geo2);

	// Display result
	printf("Roundtrip (GEO):\n");
	printf("    lat:        (in)   %.9f\n", geo1[0]);
	printf("                (out)  %.9f\n", geo2[0]);
	printf("\n");
	printf("    lon:        (in)   %.9f\n", geo1[1]);
	printf("                (out)  %.9f\n", geo2[1]);
	printf("\n");
	printf("    height:     (in)   %.9f\n", geo1[2]);
	printf("                (out)  %.9f\n", geo2[2]);
	printf("\n");

}

static void roundtrip_xyz(double xyz1[]) {

	double geo[3];
	double xyz2[3];

	// Roundtrip coordinates using liblatitude
	lat_ellipsoid_xyz2geo(&g_ellipsoid, xyz1, geo);
	lat_ellipsoid_geo2xyz(&g_ellipsoid, geo, xyz2);

	// Display result
	printf("Roundtrip (XYZ):\n");
	printf("    x:          (in)   %.6f\n", xyz1[0]);
	printf("                (out)  %.6f\n", xyz2[0]);
	printf("\n");
	printf("    y:          (in)   %.6f\n", xyz1[1]);
	printf("                (out)  %.6f\n", xyz2[1]);
	printf("\n");
	printf("    z:          (in)   %.6f\n", xyz1[2]);
	printf("                (out)  %.6f\n", xyz2[2]);
	printf("\n");
}

static void compute_attitude(
    const double *local,
    const double *body,
    double *azimuth,
    double *elevation,
    double *roll
) {
	// Given a local and body, determine body orientation
	double temp[3*3];
	double dcm[3*3];
	double r[3], f[3], u[3];
	cmatrix_transpose(temp, local, 3, 3);
	cmatrix_mul(dcm, temp, body, 3, 3, 3);

	cmatrix_to_frame(dcm, r, f, u);

        double fx = f[0];
        double fy = f[1];
        double fz = f[2];

        // Azimuth and elevation are easy

        *azimuth = atan2(fx, fy);
        *elevation = atan2(fz, sqrt(fx*fx+fy*fy));

        // Roll needs some calculation...

        // Unit vectors for zero-roll right and up.
        double zr[3], zu[3];

        // zero-roll right; zr = cvector_cross(f, z)
        cvector_xyz(zr, fy, -fx, 0.0);
        // zero-roll up; zu
        cvector_cross(zu, zr, f);

        // Determine roll
        *roll = atan2(cvector_dot(zr, u, 3), cvector_dot(zu, u, 3));

}


static void generate_trajectory(statgen_config *config) {

	// For convenience
	int steplen_numer = config->steplen_numer;
	int steplen_denom = config->steplen_denom;
	int duration = config->duration;
	double xyz[3];
	xyz[0] = config->values[0];
	xyz[1] = config->values[1];
	xyz[2] = config->values[2];

	// Approximation of the step length
	double steplen = (double)(steplen_numer) / (double)(steplen_denom);

	// Truncates the result, ie. floor of the result.
	int steps = duration*steplen_denom / steplen_numer;

	printf("Step:           %d / %d s\n", steplen_numer, steplen_denom);
	printf("Step frequency: %f Hz\n", 1.0/steplen);
	printf("Step period:    %f s\n", steplen);
	printf("Max duration:   %d s\n", duration);
	printf("Steps:          %d\n", steps);
	printf("steps*len       %f\n", steps * steplen);
	printf("\n");
	printf("Location (XYZ):\n");
	printf("    x:          %.6f\n", xyz[0]);
	printf("    y:          %.6f\n", xyz[1]);
	printf("    z:          %.6f\n", xyz[2]);

	// For maintaining event time tag
	gsim_simulation_time tag;
	// For maintaining receiver's state
	gsim_motion_state state;

	double x[3];
	double v[3];
	double a[3];

	double q[4];
	double w[3];

	// These are with respect to reference frame
	double body[3*3]; // body frame [right forward up]

	// Set linear motion state
	cvector_memcpy(x, xyz, 3);
	cvector_zero(v, 3);
	cvector_zero(a, 3);

	// Express body frame wrt local frame.

	double roll = MAID_DEG2RAD_L(config->roll);
	double elevation = MAID_DEG2RAD_L(config->elevation);
	double azimuth = MAID_DEG2RAD_L(config->azimuth);

	cmatrix_eye(body, 3);

	cmatrix_mroty(body, body, roll);
	cmatrix_mrotx(body, body, elevation);
	cmatrix_mrotz(body, body, -azimuth);
	// Now body expresses the body frame wrt local frame.

	// Express local frame wrt reference frame.
	double geo[3];
	double local[3*3]; // local frame [east north up]
	lat_ellipsoid_xyz2geo(&g_ellipsoid, x, geo);
	lat_ellipsoid_localframe_at(&g_ellipsoid, geo, local);

	// Express body frame wrt reference frame.
	double dcm[3*3];
	cmatrix_mul(dcm, local, body, 3, 3, 3);

	// Convert body frame orientation into quaternion
	cquat_from_matrix(q, dcm);

	/*
	// Compute attitude; works as planned.
	compute_attitude(local, dcm, &azimuth, &elevation, &roll);
	printf("azimuth: %f, elevation: %f, roll: %f\n",
	    MAID_RAD2DEG(azimuth),
	    MAID_RAD2DEG(elevation),
	    MAID_RAD2DEG(roll)
	);
	*/

	// Attempt to open the outfile, if any
	gut_datafile *outdf = NULL;
	if (config->outfile != NULL) do {
		outdf = gut_datafile_create();
		if (outdf == NULL) {
			fprintf(stderr, "gut_datafile_create: out of memory\n");
			break;
		}
		gut_datafile_open(outdf, config->outfile, "wb");
		if (outdf->err) {
			fprintf(stderr, "gut_datafile_open: %s\n", strerror(errno));
			gut_datafile_free(outdf);
			outdf = NULL;
			break;
		}
		printf("Recording to file %s\n", config->outfile);

	} while (0);


	// TODO:
	// deduce az, el and roll from dcm and local

	int t_sec;
	double t_subsec;
	unsigned int count = 0;
	for (int i = 0; i <= steps; i++) {
		count = i*steplen_numer;
		t_sec = count / steplen_denom;
		t_subsec = (count % steplen_denom) / (double)(steplen_denom);
		printf("i=%d    t=%d  subsec=%f\n", i, t_sec, t_subsec);

		// Initialize gsim_simulation_time with proper values.
		tag.event_number = i;
		tag.cycles = count;
		tag.freq = steplen_denom;

		// Derivative values
		tag.sec = t_sec;
		tag.subsec = t_subsec;

		// Output time tag

		// Initialize trajectory data
		memcpy(&state.t, &tag, sizeof(tag));
		memcpy(state.x, x, sizeof(state.x));
		memcpy(state.v, v, sizeof(state.v));
		memcpy(state.a, a, sizeof(state.a));
		memcpy(state.q, q, sizeof(state.q));
		memcpy(state.w, w, sizeof(state.w));

		// Output motion state

		if (outdf == NULL) {
			continue; // skip output
		}

		gsim_write_tagcode(outdf, GSIM_TAG_SIMULATION_TIME);
		gsim_write_simulation_time(outdf, &tag);

		gsim_write_tagcode(outdf, GSIM_TAG_MOTION_STATE);
		gsim_write_motion_state(outdf, &state);
	}

	if (outdf != NULL) {
		gut_datafile_flush(outdf);
		gut_datafile_close(outdf);
		gut_datafile_free(outdf);
		outdf = NULL;
	}
}

static void generate_xyz(statgen_config *config) {
	generate_trajectory(config);
}

static void generate_geo(statgen_config *config) {
	// Transform into generate_xyz.
	double xyz[3];
	lat_ellipsoid_geo2xyz(&g_ellipsoid, config->values, xyz);

	// Copy the result back to config
	config->values[0] = xyz[0];
	config->values[1] = xyz[1];
	config->values[2] = xyz[2];

	// Update the action
	config->action = ACTION_GENERATE_XYZ;

	// Create the trajectory
	generate_trajectory(config);
}

int main(int argc, char *argv[]) {
	printf("argc: %d\n", argc);

	// Configuration object
	statgen_config config;

	// TODO: initialize configuration object properly.
	config.action = ACTION_INVALID;
	// Default step length: 1 sec
	config.steplen_numer = 1;
	config.steplen_denom = 1;
	// Default duration: 1 hour
	//config.duration = 60*60*1.0;
	// Default duration: 5 min
	config.duration = 5*60*1.0;
	// No output file
	config.outfile = NULL;

	// Default orientation coincidences with the local frame
	config.azimuth = 0.0;
	config.elevation = 0.0;
	config.roll = 0.0;

	// Set reference ellipsoid initially to WGS 84 ellipsoid.
	lat_ellipsoid_set_af(&g_ellipsoid, WGS84_A, WGS84_F);

	gut_argparser *parser = NULL;

	// Instantiate a parser
	parser = gut_argparser_create();
	if (parser == NULL) {
		printf("gut_argparser_create() failed: out of memory\n");
		return EXIT_FAILURE;
	}

	/*
	// Configure the application-depend aspects of the argparser.
	parser->cycle = consume_arg;
	parser->itemptr = &config;

	// Parse command-line arguments into the configuration object.
	gut_argparser_parse(parser, argc, argv);

	if (parser->err) {
		printf("gut_argparser_parse() resulted in error: %d\n", parser->err);
	}
	if (parser->state != S_NOMINAL) {
		printf("gut_argparser_parse() finished with an unexpected state\n");
	}
	*/
	argparser_parse(parser, argc, argv, &config);

	if (parser->err) {
		printf("%s\n", parser->errmsg);
	}

	// Deallocate the parser
	gut_argparser_free(parser);

	// Execute program
	if (config.action == ACTION_INVALID) {
		printf("No action specified. Halting.\n");
		exit(EXIT_FAILURE);
	}

	// Validate input data
	if ((config.action == ACTION_CONVERT_GEO2XYZ)
	    || (config.action == ACTION_ROUNDTRIP_GEO)
	    || (config.action == ACTION_GENERATE_GEO))
	{
		double lon = config.values[0];
		double lat = config.values[1];
		if ((lon < -180.0) || (lon > 180.0)) {
			printf("Error: longitude must be within -180..180 degrees\n");
			exit(EXIT_FAILURE);
		}
		if ((lat < -90.0) || (lat > 90.0)) {
			printf("Error: latitude must be within -90..90 degrees\n");
			exit(EXIT_FAILURE);
		}
	}

	// Execute according to action
	switch(config.action) {
	case ACTION_CONVERT_XYZ2GEO:
		convert_xyz2geo(config.values);
		break;
	case ACTION_CONVERT_GEO2XYZ:
		convert_geo2xyz(config.values);
		break;

	case ACTION_ROUNDTRIP_GEO:
		roundtrip_geo(config.values);
		break;
	case ACTION_ROUNDTRIP_XYZ:
		roundtrip_xyz(config.values);
		break;

	case ACTION_GENERATE_GEO:
		generate_geo(&config);
		break;

	case ACTION_GENERATE_XYZ:
		generate_xyz(&config);
		break;

	default:
		break;
	}

	exit(EXIT_SUCCESS);

}
