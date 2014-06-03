//******************************{begin:header}******************************//
//              grease - The Grease Utilities for Applications              //
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

// gut_parse_int()
#include "grease/gut_parse.h"
// gut error codes
#include "grease/gut_error.h"

// exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>
// fprintf, stderr
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <number>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int val;
	int err;

	err = gut_parse_int(argv[1], &val);

	if (err) {
		fprintf(stderr, "Error: conversion failed (%d)\n", err);
		if (err == GUT_E_RANGE) {
			fprintf(stderr, "(value returned: %d)\n", val);
		}
		exit(EXIT_FAILURE);
	}

	printf("gut_parse_int() returned %d\n", val);

	exit(EXIT_SUCCESS);
}
