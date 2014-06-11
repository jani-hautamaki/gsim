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

// libgrease, gut_*
#include "grease/gut_parse.h"
#include "grease/gut_argparser.h"
// GUT_E_USER
#include "grease/gut_error.h"

// The application-specific configuration
#include "statgen_config.h"

// strcmp
#include <string.h>

// for debugging, get rid of these

// PARSER STATES
//===============

#define S_NOMINAL 0
#define S_EXPECT_MODE 1
#define S_EXPECT_NUMBERS 2
#define S_OUTFILE 3
#define S_INTERVAL 4
#define S_DURATION 5
#define S_ERROR -1
#define S_STEPLEN 7
#define S_STEPFREQ 8
#define S_AZIMUTH 11
#define S_ELEVATION 12
#define S_ROLL 13
#define S_EXPECT_NUMBER1 50
#define S_EXPECT_NUMBER2 51
#define S_EXPECT_NUMBER3 52


// INTERNAL HELPERS
//==================

static void parse_int(
    gut_argparser *parser,
    const char *carg,
    int *result
) {
	// Parse the string into int
	if (gut_parse_int(carg, result) != 0) {
		// Error, conversion failed.
		gut_argparser_errorfmt(parser, GUT_E_USER,
		    "Cannot convert to int: %s", carg);
		parser->state = S_ERROR;
	}
}

static void parse_uint(
    gut_argparser *parser,
    const char *carg,
    unsigned int *result
) {
	// Parse the string into int
	if (gut_parse_uint(carg, result) != 0) {
		// Error, conversion failed.
		gut_argparser_errorfmt(parser, GUT_E_USER,
		    "Cannot convert to unsigned int: %s", carg);
		parser->state = S_ERROR;
	}
}

static void parse_double(
    gut_argparser *parser,
    const char *carg,
    double *result
) {
	if (gut_parse_double(carg, result) != 0) {
		// Conversion failed
		gut_argparser_errorfmt(parser, GUT_E_USER,
		    "Cannot convert to double: %s", carg);
		parser->state = S_ERROR;
	}
}

/**
 * Consumes a single input argument
 */
static void consume(gut_argparser *parser) {

	// For convenience
	const char *carg = parser->carg;
	int state = parser->state;
	statgen_config *config = parser->itemptr;

	// For automatic return to nominal after an excursion
	int back_to_nominal = 0;
	if (state != S_NOMINAL) {
		back_to_nominal = 1;
	}

	switch(state) {
	case S_NOMINAL:
		// Unless a switch, expect it to be a number.
		if ((strcmp(carg, "-outfile") == 0)
		    || (strcmp(carg, "-out") == 0)
		    || (strcmp(carg, "-o") == 0))
		{
			state = S_OUTFILE;
		}
		else if (strcmp(carg, "-steplen") == 0) {
			state = S_STEPLEN;
		}
		else if (strcmp(carg, "-stepfreq") == 0) {
			state = S_STEPFREQ;
		}
		else if ((strcmp(carg, "-duration") == 0)
		    || (strcmp(carg, "-length") == 0)
		    || (strcmp(carg, "-len") == 0)
		    || (strcmp(carg, "-l") == 0))
		{
			state = S_DURATION;
		}
		else if ((strcmp(carg, "-az") == 0)
		    || (strcmp(carg, "-azimuth") == 0))
		{
			state = S_AZIMUTH;
		}
		else if ((strcmp(carg, "-el") == 0)
		    || (strcmp(carg, "-elev") == 0)
		    || (strcmp(carg, "-elevation") == 0))
		{
			state = S_ELEVATION;
		}
		else if ((strcmp(carg, "-roll") == 0)
		    || (strcmp(carg, "-bank") == 0))
		{
			state = S_ROLL;
		}
		// Actions
		else if (strcmp(carg, "geo") == 0) {
			config->action = ACTION_GENERATE_GEO;
			state = S_EXPECT_NUMBERS;
		}
		else if (strcmp(carg, "xyz") == 0) {
			config->action = ACTION_GENERATE_XYZ;
			state = S_EXPECT_NUMBERS;
		}
		else if (strcmp(carg, "xyz2geo") == 0) {
			config->action = ACTION_CONVERT_XYZ2GEO;
			state = S_EXPECT_NUMBERS;
		}
		else if (strcmp(carg, "geo2xyz") == 0) {
			config->action = ACTION_CONVERT_GEO2XYZ;
			state = S_EXPECT_NUMBERS;
		}
		else if (strcmp(carg, "testgeo") == 0) {
			config->action = ACTION_ROUNDTRIP_GEO;
			state = S_EXPECT_NUMBERS;
		}
		else if (strcmp(carg, "testxyz") == 0) {
			config->action = ACTION_ROUNDTRIP_XYZ;
			state = S_EXPECT_NUMBERS;
		}
		else {
			// Error, unexpected argument
			gut_argparser_errorfmt(parser, GUT_E_USER,
			    "Unexpected argument: %s", carg);
			state = S_ERROR;
		}
		break;

	case S_EXPECT_NUMBERS:
	case S_EXPECT_NUMBER1:
		back_to_nominal = 0;
		state = S_EXPECT_NUMBER2;
		// May set an error and override the state transition
		parse_double(parser, carg, &config->values[0]);
		break;

	case S_EXPECT_NUMBER2:
		back_to_nominal = 0;
		state = S_EXPECT_NUMBER3;
		// May set an error and override the state transition
		parse_double(parser, carg, &config->values[1]);
		break;

	case S_EXPECT_NUMBER3:
		back_to_nominal = 0;
		state = S_NOMINAL;
		// May set an error and override the state transition
		parse_double(parser, carg, &config->values[2]);
		break;

	case S_OUTFILE:
		config->outfile = carg;
		break;

	case S_STEPLEN:
		// Step length [s]
		// (numerator=param, denominator=1)
		parse_uint(parser, carg, &config->steplen_numer);
		config->steplen_denom = 1;
		break;

	case S_STEPFREQ:
		// Step frequency [Hz].
		// (numerator=1, denominator=param)
		parse_uint(parser, carg, &config->steplen_denom);
		config->steplen_numer = 1;
		break;

	case S_DURATION:
		// Simulation duration [s]
		parse_uint(parser, carg, &config->duration);
		break;

	case S_AZIMUTH:
		// Vehicle azimuth [deg]
		parse_double(parser, carg, &config->azimuth);
		break;

	case S_ELEVATION:
		// Vehicle elevation [deg]
		parse_double(parser, carg, &config->elevation);
		break;

	case S_ROLL:
		// Vehicle roll [deg]
		parse_double(parser, carg, &config->roll);
		break;

	case S_ERROR:
		// Finishing state; hold it.
		break;

	default:
		// Error
		break;
	} // switch

	if (back_to_nominal) {
		state = S_NOMINAL;
	}

	// Store the updated state
	parser->state = state;
}

/**
 * TODO:
 */
static void gut_argparser_eof(gut_argparser *parser) {
	// inspect the ending state
}


int argparser_parse(
    gut_argparser *parser,
    int argc,
    char *argv[],
    statgen_config *config
) {

	// Configure the application-depend aspects of the argparser.
	parser->cycle = consume;
	parser->itemptr = config;

	// Parse command-line arguments to "config".
	gut_argparser_parse(parser, argc, argv);

	// Inspect the ending state?
	if ((parser->err == 0) && (parser->state != S_NOMINAL)) {
		gut_argparser_errorfmt(parser, GUT_E_OTHER,
		    "Unexpected end of command-line");
	}
	// If err!=0 the state should be S_ERROR


	// Returns the error code for the parser
	return parser->err;
}
