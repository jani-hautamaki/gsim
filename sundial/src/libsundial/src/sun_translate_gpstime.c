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

// floor
#include <math.h>

void sun_translate_gpstime(
    int *week,
    double *tow,
    long delta_day,
    long delta_sec,
    double delta_subsec
) {
	// Normalize delta
	sun_normalize_delta(&delta_day, &delta_sec, &delta_subsec);

	// Normalize delta further into delta_week
	long carry;
	int delta_week = 0;

	carry = delta_day/7;
	delta_week += carry;
	delta_day -= carry*7; // delta_dayofweek now

	// Translate tow
	*tow += (double)(delta_day*SUN_SECONDS_IN_DAY + delta_sec) + delta_subsec;

	// Normalize tow
	carry = (long) floor(*tow/SUN_SECONDS_IN_WEEK);
	*tow -= (double)(carry*SUN_SECONDS_IN_WEEK);
	delta_week += carry;

	// Translate week
	*week += delta_week;
}
