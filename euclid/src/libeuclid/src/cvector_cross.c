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

void cvector_cross(double *v, const double *a, const double *b) {

	#define X 0
	#define Y 1
	#define Z 2

	v[X] =   (a[Y] * b[Z]) - (a[Z] * b[Y]);
	v[Y] = -((a[X] * b[Z]) - (a[Z] * b[X]));
	v[Z] =   (a[X] * b[Y]) - (a[Y] * b[X]);

	#undef X
	#undef Y
	#undef Z
}

