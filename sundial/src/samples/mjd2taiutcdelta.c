//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
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

// fprintf, printf, stderr
#include <stdio.h>

#include "parse_long.h"

int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "Usage: %s <daynum>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// Arguments
	long daynum;

	if (!parse_long(argv[1], &daynum)) {
		fprintf(stderr, "Not an integer: %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	int delta = sun_delta_tai_utc(daynum);

	printf("On Modified Julian Day %ld the difference TAI-UTC = %d s\n",
	    daynum, delta);

	exit(EXIT_SUCCESS);
}
