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

int main(int argc, char *argv[]) {

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <week> <tow>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	int week;
	double tow;

	if (sscanf(argv[1], "%d", &week) != 1) {
		fprintf(stderr, "Cannot parse week: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	if (sscanf(argv[2], "%lf", &tow) != 1) {
		fprintf(stderr, "Cannot parse tow: %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	sun_datetime dt;

	sun_gpstime2utcdatetime(week, tow, &dt);

	printf("Week %d tow %.2f (GPS Time) corresponds to %02d.%02d.%04d %02d:%02d:%02d (UTC)\n",
	    week, tow,
	    dt.day, dt.month, dt.year,
	    dt.hour, dt.min, dt.sec
	);

	exit(EXIT_SUCCESS);
}
