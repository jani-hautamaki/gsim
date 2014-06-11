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

#include "euclid/cquat.h"

// sqrt, sqrtl
#include <math.h>


double cquat_norm2(const double *q) {
	double sum
	    = (q[0]*q[0]) + (q[1]*q[1]) + (q[2]*q[2]) + (q[3]*q[3]);
	return sum;
}

double cquat_norm(const double *q) {
	return sqrt(cquat_norm2(q));
}

long double cquat_norm2_l(const double *q) {
	long double sum
	    = (q[0]*q[0]) + (q[1]*q[1]) + (q[2]*q[2]) + (q[3]*q[3]);
	return sum;
}

long double cquat_norm_l(const double *q) {
	return sqrtl(cquat_norm2_l(q));
}

