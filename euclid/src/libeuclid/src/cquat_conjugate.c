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

extern void cquat_conjugate(double *qdest, const double *qorg) {
	qdest[CQUAT_X] = -qorg[CQUAT_X];
	qdest[CQUAT_Y] = -qorg[CQUAT_Y];
	qdest[CQUAT_Z] = -qorg[CQUAT_Z];
	qdest[CQUAT_W] = qorg[CQUAT_W];
}