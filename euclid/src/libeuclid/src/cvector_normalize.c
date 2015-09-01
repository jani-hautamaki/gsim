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

#include "euclid/cvector.h"

// sqrt, sqrtl
#include <math.h>

void cvector_normalize(double *vdest, const double *vorg, int n) {
	double norm = cvector_norm(vorg, n);

	for (int i = 0; i < n; i++) {
		vdest[i] = vorg[i] / norm;
	}
}

void cvector_normalize_l(double *vdest, const double *vorg, int n) {
	long double norm_l = cvector_norm_l(vorg, n);

	for (int i = 0; i < n; i++) {
		vdest[i] = vorg[i] / norm_l;
	}
}
