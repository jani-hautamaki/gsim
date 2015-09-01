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

// sin, sinl, cos, cosl
#include <math.h>

// memcpy
#include <string.h>

void cmatrix_mrotx(double *mdest, const double *morg, double angle) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	double cosa = cos(angle);
	double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) = ELEM(morg,1,k);
		// Row 2
		ELEM(mdest,2,k) = cosa*ELEM(morg,2,k) - sina*ELEM(morg,3,k);
		// Row 3
		ELEM(mdest,3,k) = sina*ELEM(morg,2,k) + cosa*ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}

void cmatrix_mrotx_l(
    double *mdest,
    const double *morg,
    long double angle
) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	long double cosa = cos(angle);
	long double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) = ELEM(morg,1,k);
		// Row 2
		ELEM(mdest,2,k) = cosa*ELEM(morg,2,k) - sina*ELEM(morg,3,k);
		// Row 3
		ELEM(mdest,3,k) = sina*ELEM(morg,2,k) + cosa*ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}



void cmatrix_mroty(double *mdest, const double *morg, double angle) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	double cosa = cos(angle);
	double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Unrolled:
	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) =  cosa*ELEM(morg,1,k) + sina*ELEM(morg,3,k);
		// Row 2
		ELEM(mdest,2,k) = ELEM(morg,2,k);
		// Row 3
		ELEM(mdest,3,k) = -sina*ELEM(morg,1,k) + cosa*ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}

void cmatrix_mroty_l(
    double *mdest,
    const double *morg,
    long double angle
) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	long double cosa = cos(angle);
	long double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Unrolled:
	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) =  cosa*ELEM(morg,1,k) + sina*ELEM(morg,3,k);
		// Row 2
		ELEM(mdest,2,k) = ELEM(morg,2,k);
		// Row 3
		ELEM(mdest,3,k) = -sina*ELEM(morg,1,k) + cosa*ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}



void cmatrix_mrotz(double *mdest, const double *morg, double angle) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	double cosa = cos(angle);
	double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Unrolled:
	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) = cosa*ELEM(morg,1,k) - sina*ELEM(morg,2,k);
		// Row 2
		ELEM(mdest,2,k) = sina*ELEM(morg,1,k) + cosa*ELEM(morg,2,k);
		// Row 3
		ELEM(mdest,3,k) = ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}

void cmatrix_mrotz_l(
    double *mdest,
    const double *morg,
    long double angle
) {
	double temp[3*3];

	if (morg == mdest) {
		memcpy(temp, morg, sizeof(temp));
		morg = temp;
	}

	// Compute only once
	long double cosa = cos(angle);
	long double sina = sin(angle);

	// Convenience macro: 1-based addressing for 3x3 matrices.
	#define ELEM(m, y, x) CMATRIX_ELEM(m, (y)-1, (x)-1, 3)

	// Unrolled:
	for (int k = 1; k <= 3; k++) {
		// Row 1
		ELEM(mdest,1,k) = cosa*ELEM(morg,1,k) - sina*ELEM(morg,2,k);
		// Row 2
		ELEM(mdest,2,k) = sina*ELEM(morg,1,k) + cosa*ELEM(morg,2,k);
		// Row 3
		ELEM(mdest,3,k) = ELEM(morg,3,k);
	}

	// Undefine convenience macro
	#undef ELEM
}
