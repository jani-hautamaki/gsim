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

#include "parse_double.h"
#include "parse_long.h"

int main(int argc, char *argv[]) {

	if (argc != 5) {
		fprintf(stderr, "Usage: %s <week2> <tow2> <week1> <tow1>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Parse arguments
	int week2;
	double tow2;
	int week1;
	double tow1;

	long tmp;

	if (!parse_long(argv[1], &tmp)) {
		fprintf(stderr, "Cannot parse week2: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}
	week2 = (int) tmp;

	if (!parse_double(argv[2], &tow2)) {
		fprintf(stderr, "Cannot parse tow2: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if (!parse_long(argv[3], &tmp)) {
		fprintf(stderr, "Cannot parse week1: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}
	week1 = (int) tmp;

	if (!parse_double(argv[4], &tow1)) {
		fprintf(stderr, "Cannot parse tow1: %s\n", argv[4]);
		exit(EXIT_FAILURE);
	}


	double delta;
	delta = sun_delta_gpstime(week2, tow2, week1, tow1);

	long delta_day;
	long delta_timeofday;
	double delta_subsec;

	sun_subsec2delta(delta, &delta_day, &delta_timeofday, &delta_subsec);

	int delta_hour;
	int delta_min;
	int delta_sec;

	sun_timeofday2time(delta_timeofday, &delta_hour, &delta_min, &delta_sec);

	printf("Week %d tow %f  - \n", week2, tow2);
	printf("Week %d tow %f  =  %f s   ", week1, tow1, delta);

	printf("(= %ld days %d h %02d min %06.3f s)\n",
	    delta_day, delta_hour, delta_min, delta_sec+delta_subsec);

	exit(EXIT_SUCCESS);
}
