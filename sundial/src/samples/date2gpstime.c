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

static int parse_date(const char *s, int *year, int *month, int *day) {
	int args = sscanf(s, "%2u.%2u.%5d", day, month, year);
	return args == 3;
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <date>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	int year;
	int month;
	int day;

	if (!parse_date(argv[1], &year, &month, &day)) {
		fprintf(stderr, "Cannot parse date: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int week;
	double tow;

	sun_date2gpstime(year, month, day, &week, &tow);

	printf("%02d.%02d.%04d begins on week %d tow %f s (GPS Time)\n",
	    day, month, year, week, tow);

	exit(EXIT_SUCCESS);
}
