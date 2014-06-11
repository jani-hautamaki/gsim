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

void cquat_inverse(double *qdest, const double *qorg) {
	double norm2 = cquat_norm2(qorg);
	qdest[CQUAT_X] = -qorg[CQUAT_X] / norm2;
	qdest[CQUAT_Y] = -qorg[CQUAT_Y] / norm2;
	qdest[CQUAT_Z] = -qorg[CQUAT_Z] / norm2;
	qdest[CQUAT_W] = qorg[CQUAT_W] / norm2;
}

void cquat_inverse_l(double *qdest, const double *qorg) {
	long double norm2_l = cquat_norm2_l(qorg);
	qdest[CQUAT_X] = -qorg[CQUAT_X] / norm2_l;
	qdest[CQUAT_Y] = -qorg[CQUAT_Y] / norm2_l;
	qdest[CQUAT_Z] = -qorg[CQUAT_Z] / norm2_l;
	qdest[CQUAT_W] = qorg[CQUAT_W] / norm2_l;
}
