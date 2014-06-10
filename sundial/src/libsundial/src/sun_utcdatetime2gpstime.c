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

extern void sun_utcdatetime2gpstime(
    const sun_datetime *dt,
    int *week,
    double *tow
) {

	// Compute the MJD of the Gregorian date.
	long mjd = sun_date2mjulianday(dt->year, dt->month, dt->day);

	// Get the number of leap seconds effective on that date.
	int delta = sun_delta_tai_utc(mjd) - SUN_DELTA_GPS_TAI;

	// Compute the second within the day
	long timeofday = sun_time2timeofday(dt->hour, dt->min, dt->sec);

	// Add the leap seconds, and normalize if roll-over
	timeofday = timeofday + delta;
	if (timeofday > SUN_SECONDS_IN_DAY) {
		mjd = mjd + 1;
		timeofday = timeofday - SUN_SECONDS_IN_DAY;
	}

	// Translate the origin to GPS epoch.
	long gpsday = mjd - SUN_EPOCH_GPSTIME;

	// Compute the GPS Week
	*week = gpsday / 7;

	// Compute the GPS Time of Week
	*tow = (double)((gpsday % 7)*SUN_SECONDS_IN_DAY + timeofday)
	    + dt->subsec;
}
