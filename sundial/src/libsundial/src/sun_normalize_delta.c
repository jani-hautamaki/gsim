//******************************{begin:header}******************************//
//                      sundial - The Datetime Library                      //
//**************************************************************************//
//
//      Part of the GPS/INS measurement simulation system GSIM
//      https://github.com/jani-hautamaki/gsim
//
//      Copyright (C) 2013-2015 Jani Hautamaki <jani.hautamaki@hotmail.com>
//
//      Licensed under the terms of GNU General Public License v3.
//
//      You should have received a copy of the GNU General Public License v3
//      along with this program as the file LICENSE.txt; if not, please see
//      http://www.gnu.org/licenses/gpl-3.0.html
//
//********************************{end:header}******************************//

#include "sundial/sundial.h"

void sun_normalize_delta(
    long *delta_day,
    long *delta_timeofday,
    double *delta_subsec
) {
	long carry;

	// Normalize subsec
	carry = (long) *delta_subsec;
	*delta_timeofday += carry;
	*delta_subsec -= (double) carry;

	// Normalize sec
	carry = *delta_timeofday/SUN_SECONDS_IN_DAY;
	*delta_day += carry;
	*delta_timeofday -= carry*SUN_SECONDS_IN_DAY;
}
