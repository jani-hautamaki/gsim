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

double sun_delta_gpstime(
    int week2,
    double tow2,
    int week1,
    double tow1
) {
	int delta_week = week2-week1;
	double delta_tow = tow2-tow1;

	return (double)(delta_week*SUN_SECONDS_IN_WEEK) + delta_tow;
}
