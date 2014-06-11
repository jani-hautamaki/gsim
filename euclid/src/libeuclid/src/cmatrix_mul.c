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

void cmatrix_mul(
    double *m,
    const double *a,
    const double *b,
    int arows,
    int nboth,
    int bcols
) {
	// For convenience
	#define ELEM(m, y, x, cols) CMATRIX_ELEM(m, y, x, cols)

	for (int y = 0; y < arows; y++) {
	for (int x = 0; x < bcols; x++) {
		double sum = 0.0;
		for (int i = 0; i < nboth; i++) {
			sum += ELEM(a, y, i, nboth) * ELEM(b, i, x, bcols);
		}
		ELEM(m, y, x, bcols) = sum;
	} // x
	} // y

	// Undefine convenience macro
	#undef ELEM
}

void cmatrix_mul_t(
    double *m,
    const double *a,
    const double *b,
    int arows,
    int nboth,
    int bcols
) {
	// For convenience
	#define ELEM(m, y, x, cols) CMATRIX_ELEM(m, y, x, cols)

	for (int y = 0; y < arows; y++) {
	for (int x = 0; x < bcols; x++) {
		long double sum = 0.0;
		for (int i = 0; i < nboth; i++) {
			sum += ELEM(a, y, i, nboth) * ELEM(b, i, x, bcols);
		}
		ELEM(m, y, x, bcols) = sum;
	} // x
	} // y

	// Undefine convenience macro
	#undef ELEM
}

