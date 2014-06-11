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
// http://en.wikipedia.org/wiki/Julian_day#Gregorian_calendar_from_Julian_day_number

void sun_mjulianday2date(
    long daynum,
    int *year,
    int *month,
    int *day
) {
	// Some auxiliary variables
	int f, e, g, h;

	// Offset the daynum to jd
	daynum = daynum + 2400001;

	f = daynum + 1401 + (((4*daynum+274277)/146097)*3)/4 - 38;
	e = 4*f + 3;
	g = (e % 1461) / 4;
	h = 5*g+2;
	*day = (h % 153)/5 + 1;
	*month = (((h / 153)+2) % 12) + 1;
	*year = (e / 1461) - 4716 + ((14-*month)/12);

}



