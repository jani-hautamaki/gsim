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

// For explanation, see
// http://en.wikipedia.org/wiki/Julian_day#Converting_Julian_or_Gregorian_calendar_date_to_Julian_Day_Number

long sun_date2mjulianday(
    int year,
    int month,
    int day
) {
	// Some auxiliary variables
	int y, m, a;

	// 1 for Jan/Feb, 0 for other months.
	a = (14 - month)/12;

	// Number of years since March 1 -4800 (4081 BC)
	y = year + 4800 - a;

	// Number of months since the last March 1st.
	m = month + 12*a - 3;

	long jd
	    = day
	    + ((153*m) + 2)/5 // days passed the march 1st
	    + (y/4) - (y/100) + (y/400) // accounts leap years
	    + 365*y
	    - 32045; // constant bias

	// "jd" is the day number of the date at noon.
	// Consequently, the day number at the 0h is jd-1.

	return jd - 2400001;
	//return jd;
}



