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

void sun_subsec2delta(
    double subsec,
    long *delta_day,
    long *delta_sec,
    double *delta_subsec
) {
	*delta_day = 0;
	*delta_sec = 0;
	*delta_subsec = subsec;

	sun_normalize_delta(delta_day, delta_sec, delta_subsec);
}
