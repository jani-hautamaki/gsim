//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
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

#include "sundial/sundial.h"

// exit, EXIT_SUCCESS, EXIT_FAILURE
#include <stdlib.h>

// fprintf, printf, stderr, sscanf
#include <stdio.h>

// floor
#include <math.h>

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <delta>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	double delta;
	if (sscanf(argv[1], "%lf", &delta) != 1) {
		fprintf(stderr, "Cannot parse delta: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	long delta_day;
	long delta_timeofday;
	double delta_subsec;

	delta_timeofday = 0;
	delta_day = 0;
	delta_subsec = delta;

	sun_normalize_delta(&delta_day, &delta_timeofday, &delta_subsec);

	int delta_hour;
	int delta_min;
	int delta_sec;

	sun_timeofday2time(delta_timeofday, &delta_hour, &delta_min, &delta_sec);

	printf("%f seconds is %ld days %d hours %d minutes %d seconds and %.3f sub-seconds\n",
	    delta,
	    delta_day,
	    delta_hour, delta_min, delta_sec,
	    delta_subsec
	);


	exit(EXIT_SUCCESS);
}
