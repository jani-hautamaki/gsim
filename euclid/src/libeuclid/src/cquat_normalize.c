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

#include "euclid/cquat.h"

// sqrt, sqrtl
#include <math.h>

void cquat_normalize(double *qdest, const double *qorg) {
	double norm = cquat_norm(qorg);
	qdest[0] = qorg[0] / norm;
	qdest[1] = qorg[1] / norm;
	qdest[2] = qorg[2] / norm;
	qdest[3] = qorg[3] / norm;
}

void cquat_normalize_l(double *qdest, const double *qorg) {
	long double norm_l = cquat_norm_l(qorg);
	qdest[0] = qorg[0] / norm_l;
	qdest[1] = qorg[1] / norm_l;
	qdest[2] = qorg[2] / norm_l;
	qdest[3] = qorg[3] / norm_l;
}
