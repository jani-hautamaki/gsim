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

// fprintf, printf, stderr, sscanf
#include <stdio.h>

// floor
#include <math.h>


static int parse_date(const char *s, sun_datetime *dt) {
	int args = sscanf(s, "%2u.%2u.%5d", &dt->day, &dt->month, &dt->year);
	return args == 3;
}

static int parse_time(const char *s, sun_datetime *dt) {
	int args = sscanf(s, "%2u:%2u:%lf", &dt->hour, &dt->min, &dt->subsec);
	dt->sec = (int) dt->subsec;
	dt->subsec -= dt->sec;
	return args == 3;
}

int main(int argc, char *argv[]) {

	if (argc != 5) {
		fprintf(stderr, "Usage: %s <date2> <time2> <date1> <time1>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Parse arguments
	sun_datetime dt2;
	sun_datetime dt1;

	if (!parse_date(argv[1], &dt2)) {
		fprintf(stderr, "Cannot parse date2: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (!parse_time(argv[2], &dt2)) {
		fprintf(stderr, "Cannot parse time2: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	if (!parse_date(argv[3], &dt1)) {
		fprintf(stderr, "Cannot parse date1: %s\n", argv[3]);
		exit(EXIT_FAILURE);
	}

	if (!parse_time(argv[4], &dt1)) {
		fprintf(stderr, "Cannot parse time1: %s\n", argv[4]);
		exit(EXIT_FAILURE);
	}


	double delta;
	delta = sun_delta_utcdatetime(&dt2, &dt1);

	long delta_day;
	long delta_timeofday;
	double delta_subsec;

	sun_subsec2delta(delta, &delta_day, &delta_timeofday, &delta_subsec);

	int delta_hour;
	int delta_min;
	int delta_sec;

	sun_timeofday2time(delta_timeofday, &delta_hour, &delta_min, &delta_sec);

	printf("%02d.%02d.%04d %02d:%02d:%06.3f  - \n",
	    dt2.day, dt2.month, dt2.year,
	    dt2.hour, dt2.min, dt2.sec + dt2.subsec
	);

	printf("%02d.%02d.%04d %02d:%02d:%06.3f  =  %.3f s  ",
	    dt1.day, dt1.month, dt1.year,
	    dt1.hour, dt1.min, dt1.sec + dt1.subsec,
	    delta
	);

	printf("(= %ld days %d h %02d min %06.3f s)\n",
	    delta_day, delta_hour, delta_min, delta_sec+delta_subsec);

	exit(EXIT_SUCCESS);
}
