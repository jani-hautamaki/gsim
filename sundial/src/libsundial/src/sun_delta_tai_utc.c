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

static const long TAI_UTC_DELTA[] = {
        41317, 10,
        41499, 11,
        41683, 12,
        42048, 13,
        42413, 14,
        42778, 15,
        43144, 16,
        43509, 17,
        43874, 18,
        44239, 19,
        44786, 20,
        45151, 21,
        45516, 22,
        46247, 23,
        47161, 24,
        47892, 25,
        48257, 26,
        48804, 27,
        49169, 28,
        49534, 29,
        50083, 30,
        50630, 31,
        51179, 32,
        53736, 33,
        54832, 34,
        56109, 35
};

int sun_delta_tai_utc(long mjd) {
	// bisect search
	int len = (sizeof(TAI_UTC_DELTA) / sizeof(TAI_UTC_DELTA[0])) / 2;

	int top, bottom;

	bottom = 0;
	top = len-1;

	int mid = bottom;

	do {
		// Select mid-point
		mid = (top+bottom)/2;

		long daynum = TAI_UTC_DELTA[mid*2+0];

		if (mjd < daynum) {
			if (mid == 0) {
				// mid is already at the end of range
				break;
			}
			top = mid-1;
			continue;
		}
		else if (mjd == daynum) {
			// Match found
			break;
		} else {
			// mjd > daynum
			if (mid == len-1) {
				// mid is already at the end of range
				break;
			}
			if (mjd < TAI_UTC_DELTA[mid*2+2]) {
				// Match found
				break;
			} else {
				bottom = mid+1;
				continue;
			}
		}
	} while (1);

	return TAI_UTC_DELTA[mid*2+1];
}

