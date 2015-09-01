//******************************{begin:header}******************************//
//                euclid - The C Library for Linear Algebra                 //
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

#include "euclid/cvector.h"

// sqrt, sqrtl
#include <math.h>

double cvector_norm2(const double *a, int n) {
	double sum = 0.0;
	for (int i = 0; i < n; i++) {
		sum += a[i] * a[i];
	}
	return sum;
}

double cvector_norm(const double *a, int n) {
	return sqrt(cvector_norm2(a, n));
}



long double cvector_norm2_l(const double *a, int n) {
	long double sum = 0.0;
	for (int i = 0; i < n; i++) {
		sum += a[i] * a[i];
	}
	return sum;
}

long double cvector_norm_l(const double *a, int n) {
	return sqrtl(cvector_norm2_l(a, n));
}

