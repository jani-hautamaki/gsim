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

extern void sun_date2gpstime(
    int year,
    int month,
    int day,
    int *week,
    double *tow
){
	long mjd = sun_date2mjulianday(year, month, day);

	// GPS time epoch is 0h on January 6, 1980.
	long gpsdaynum = mjd - SUN_EPOCH_GPSTIME;

	// Divide day number into weeks and time of week
	*week = gpsdaynum / 7;
	*tow = (double) ((gpsdaynum % 7)*SUN_SECONDS_IN_DAY);
}
