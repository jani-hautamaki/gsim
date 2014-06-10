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

// malloc, free, NULL
#include <stdlib.h>

#include "sundial/sundial.h"


extern void sun_datetime_set_date(
    sun_datetime *dt,
    int year,
    int month,
    int day
) {
	dt->year = year;
	dt->month = month;
	dt->day = day;
}

extern void sun_datetime_set_time(
    sun_datetime *dt,
    int hour,
    int min,
    int sec,
    double subsec
) {
	dt->hour = hour;
	dt->min = min;
	dt->sec = sec;
	dt->subsec = subsec;
}

/* MJD 0 epoch's Julian Day Number */
#define MJD0_JD           2400001


/* TODO:
 * Cite some reference
 */
extern int sun_date_to_mjulianday(
    int year,
    int month,
    int day
) {
	int a, y, m;
	int mjulianday;

	a = (14 - month)/12;
	y = year + 4800 - a;
	m = month + (12*a) - 3;

	mjulianday = (day + (((153*m)+2)/5) + (y*365) + (y/4) - (y/100) + (y/400) - 32045) - MJD0_JD;
	return mjulianday;
}

void sun_mjulianday_to_date(
   unsigned int mjd,
    int* year,
    int* month,
    int* day
) {
	int a, b, c;
	int d, e, m;

	a = (mjd + MJD0_JD) + 32044;
	b = ((4*a) + 3) / 146097;
	c = a - ((b*146097) / 4);

	d = ((4*c)+3) / 1461;
	e = c - (1461*d) / 4;
	m = ((5*e) + 2) / 153;

	*day = e - ((153*m+2)/5) + 1;
	*month = m + 3 - (12 * (m / 10));
	*year = (b * 100) + d - 4800 + (m/10);
}


