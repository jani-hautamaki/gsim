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

void sun_gpstime2datetime(
    int week,
    double tow,
    sun_datetime *dt
) {
	// TODO
	long day = (long)(tow/SUN_SECONDS_IN_DAY);

	long mjd = (long)(week*7) + day;

	sun_mjulianday2date(mjd, &dt->year, &dt->month, &dt->day);

	// The time within a day
	long timeofday = (long)(tow) - (day*SUN_SECONDS_IN_DAY);

	dt->hour = timeofday / 3600;
        dt->min = (timeofday % 3600) / 60;
        dt->sec = timeofday % 60;
	//dt->subsec = modf(tow, &tmp);
	dt->subsec = tow - (double) timeofday;
}
