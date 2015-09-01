//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cmatrix.h"

void cmatrix_add(
    double *m,
    const double *a,
    const double *b,
    int rows,
    int cols
) {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			int index = CMATRIX_INDEX(i, j, cols);
			m[index] = a[index] + b[index];
		}
	}
}

