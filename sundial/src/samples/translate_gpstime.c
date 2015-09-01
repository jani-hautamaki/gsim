//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
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

#include "sundial/sundial.h"

// exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// fprintf, printf, stderr
#include <stdio.h>

#include "parse_long.h"
#include "parse_double.h"

int main(int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <week> <tow> <delta>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	int week1;
	double tow1;
	double delta;

	long tmp;
	if (!parse_long(argv[1], &tmp)) {
		fprintf(stderr, "Cannot parse week: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	week1 = (int) tmp;

	if (!parse_double(argv[2], &tow1)) {
		fprintf(stderr, "Cannot parse tow: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if (!parse_double(argv[3], &delta)) {
		fprintf(stderr, "Cannot parse delta: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	int week2 = week1;
	double tow2 = tow1;


	sun_translate_gpstime(&week2, &tow2, 0, 0, delta);

	printf("Week %d tow %.3f   +   %.3f s    =\n", week1, tow1, delta);
	printf("Week %d tow %.3f\n", week2, tow2);

	exit(EXIT_SUCCESS);
}
