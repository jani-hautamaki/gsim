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

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <daynum>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	long daynum;

	if (sscanf(argv[1], "%ld", &daynum) != 1) {
		fprintf(stderr, "Cannot parse daynum: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int year;
	int month;
	int day;

	sun_mjulianday2date(daynum, &year, &month, &day);

	printf("%ld (Modified Julian Day) is %02d.%02d.%04d (dd.mm.yyyy)\n",
	    daynum, day, month, year);

	exit(EXIT_SUCCESS);
}
