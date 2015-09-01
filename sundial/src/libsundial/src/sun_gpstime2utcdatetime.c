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

/**
 * The function generates the correct behaviour:
 *
 * $ ./build/samples/gpstime2utcdatetime 181 432001
 * Week 181 tow 432001.00 (GPS Time) corresponds to 30.06.1983 23:59:59 (UTC)
 * $ ./build/samples/gpstime2utcdatetime 181 432002
 * Week 181 tow 432002.00 (GPS Time) corresponds to 30.06.1983 23:59:60 (UTC)
 * $ ./build/samples/gpstime2utcdatetime 181 432003
 * Week 181 tow 432003.00 (GPS Time) corresponds to 01.07.1983 00:00:00 (UTC)
 *
 */

#include "sundial/sundial.h"

void sun_gpstime2utcdatetime(
    int week,
    double tow,
    sun_datetime *dt
) {

	// integer division truncates the result
	long dayofweek = (long)(tow/SUN_SECONDS_IN_DAY);

	// Compute the MJD of the GPS day.
	long mjd = (long)(week*7) + dayofweek + SUN_EPOCH_GPSTIME;

	// Compute the second within the day.
	long timeofday = (long)(tow) - (dayofweek*SUN_SECONDS_IN_DAY);

	// *estimate* the number of leap seconds affecting
	// the GPS time. The actual value might be off by 1.
	int delta0 = sun_delta_tai_utc(mjd) - SUN_DELTA_GPS_TAI;
	int delta1 = sun_delta_tai_utc(mjd-1) - SUN_DELTA_GPS_TAI;
	int leap = 0;

	// Translate the timeofday by delta0
	timeofday = timeofday - delta0;
	if ((timeofday < 0) && (delta0 != delta1)) {
		if (timeofday == -1) {
			// This is the leap second
			leap = 1;
		} else {
			timeofday = timeofday + delta0-delta1;
		}
	}

	// If day roll-over, normalize the result.
	if (timeofday < 0) {
		mjd = mjd - 1;
		timeofday = timeofday + SUN_SECONDS_IN_DAY;
	}

	// MJD to Gregorian date
	sun_mjulianday2date(mjd, &dt->year, &dt->month, &dt->day);

	// Time of day to Time.
	sun_timeofday2time(timeofday, &dt->hour, &dt->min, &dt->sec);

	// If this is the leap second
	dt->sec += leap;

	//dt->subsec = modf(tow, &tmp);
	dt->subsec = tow - (double) timeofday;
}
