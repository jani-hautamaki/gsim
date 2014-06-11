//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cmatrix.h"

void cmatrix_from_frame(
    double *m,
    const double *v1,
    const double *v2,
    const double *v3
) {
	// Convenience macro for matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, y, x, 3)

	for (int i = 0; i < 3; i++) {
		ELEM(m, i, 0) = v1[i];
		ELEM(m, i, 1) = v2[i];
		ELEM(m, i, 2) = v3[i];

	}

	// Undefine convenience macro
	#undef ELEM
}


void cmatrix_to_frame(
    const double *m,
    double *v1,
    double *v2,
    double *v3
) {
	// Convenience macro for matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, y, x, 3)

	for (int i = 0; i < 3; i++) {
		v1[i] = ELEM(m, i, 0);
		v2[i] = ELEM(m, i, 1);
		v3[i] = ELEM(m, i, 2);

	}

	// Undefine convenience macro
	#undef ELEM
}

