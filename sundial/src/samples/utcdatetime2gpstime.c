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

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <date> <time>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	sun_datetime dt;

	if (!parse_date(argv[1], &dt)) {
		fprintf(stderr, "Cannot parse date: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (!parse_time(argv[2], &dt)) {
		fprintf(stderr, "Cannot parse time: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	int week;
	double tow;

	sun_utcdatetime2gpstime(&dt, &week, &tow);

	printf("%02d.%02d.%04d %02d:%02d:%02d (UTC) corresponds to week %d tow %.2f (GPS time)\n",
	    dt.day, dt.month, dt.year,
	    dt.hour, dt.min, dt.sec,
	    week, tow
	);
	//printf("Leap seconds effective on that date: %f s\n", );
	exit(EXIT_SUCCESS);
}
