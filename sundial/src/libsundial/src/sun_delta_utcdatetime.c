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

extern double sun_delta_utcdatetime(
    const sun_datetime *dt2,
    const sun_datetime *dt1
) {

	// Compute MJD for both dates
	long mjd2 = sun_date2mjulianday(dt2->year, dt2->month, dt2->day);
	long mjd1 = sun_date2mjulianday(dt1->year, dt1->month, dt1->day);

	// Compute delta days
	long delta_day = mjd2 - mjd1;

	// Get leap seconds effective on both dates
	int leap2 = sun_delta_tai_utc(mjd2);
	int leap1 = sun_delta_tai_utc(mjd1);

	// Compute delta leap sec
	long delta_leapsec = (long)(leap2 - leap1);

	// Compute timeofday for both dates
	long timeofday2 = sun_time2timeofday(dt2->hour, dt2->min, dt2->sec);
	long timeofday1 = sun_time2timeofday(dt1->hour, dt1->min, dt1->sec);

	// Compute delta secs
	long delta_sec = timeofday2 - timeofday1;

	// Compute delta subsec
	double delta_subsec = dt2->subsec - dt1->subsec;


	// Add deltas together
	return (double)(delta_day*SUN_SECONDS_IN_DAY + delta_sec) + delta_subsec + delta_leapsec;
}
