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

// floor
#include <math.h>

void sun_translate_utcdatetime(
    sun_datetime *dt,
    long delta_day,
    long delta_sec,
    double delta_subsec
) {
	// Compute the MJD
	long mjd = sun_date2mjulianday(dt->year, dt->month, dt->day);

	// Get the number of leap seconds effective at the date
	int leap1 = sun_delta_tai_utc(mjd);

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

	// Translate day
	mjd += delta_day;
	delta_day = 0;

	int leap2 = sun_delta_tai_utc(mjd);
	// If leap seconds differ,
	// re-compute timeofday
	timeofday -= (leap2-leap1);

	// Re-compute delta_day
	delta_day = 0;
	carry = (long) floor(timeofday / ((double) SUN_SECONDS_IN_DAY));
	delta_day += carry;

	// Re-compute the effective leap second
	int leap3 = sun_delta_tai_utc(mjd+delta_day);

	int leap = 0;
	if (leap2 != leap3) {
		// The effective leap second has changed
		// due to the estimated leap second.

		// Determine whether we are at the leap second
		// of the current day.

		if (timeofday == 86400) {
			// This is the leap second of the current day.
			leap = 1;
		}
		else {
			// We have incorrect effective leap second.
			// Re-adjust the time of day
			timeofday -= (leap3-leap2);

			// Is this the leap second of the previous day?
			if (timeofday == 0) {
				leap = 1;
			}
		}
	} // if-else: effective leap second changed

	// Apply leap second, if any, to the timeofday
	timeofday -= leap;

	delta_day = 0;
	carry = (long) floor(timeofday / ((double) SUN_SECONDS_IN_DAY));
	delta_day += carry;
	timeofday -= carry*SUN_SECONDS_IN_DAY;

	mjd += delta_day;

	// Transform MJD back to Gregorian date
	sun_mjulianday2date(mjd, &dt->year, &dt->month, &dt->day);

	// Translate time of day back to time
	sun_timeofday2time(timeofday, &dt->hour, &dt->min, &dt->sec);

	// Account for the leap second
	dt->sec += leap;
	// Subsec part is already ok.
}
