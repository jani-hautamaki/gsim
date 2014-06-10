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

extern void sun_translate_datetime(
    sun_datetime *dt,
    long delta_day,
    long delta_sec,
    double delta_subsec
) {

	// Normalize delta
	sun_normalize_delta(&delta_day, &delta_sec, &delta_subsec);

	long carry;

	// Translate subsec
	dt->subsec += delta_subsec;

	// Take carry into account
	carry = (long) floor(dt->subsec);
	delta_sec += carry;
	dt->subsec -= (double) carry;

	// Compute the time of day
	long timeofday = sun_time2timeofday(dt->hour, dt->min, dt->sec);

	// Translate the time of day
	timeofday += delta_sec;

	// Take carry into account
	carry = (long) floor(timeofday / ((double) SUN_SECONDS_IN_DAY));
	delta_day += carry;
	timeofday -= carry*SUN_SECONDS_IN_DAY;

	// Compute the MJD
	long mjd = sun_date2mjulianday(dt->year, dt->month, dt->day);

	// Translate day
	mjd += delta_day;

	// Transform MJD back to Gregorian date
	sun_mjulianday2date(mjd, &dt->year, &dt->month, &dt->day);

	// Translate time of day back to time
	sun_timeofday2time(timeofday, &dt->hour, &dt->min, &dt->sec);

	// Subsec part is already ok.
}
